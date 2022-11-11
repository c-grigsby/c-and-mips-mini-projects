/********************************************************
 * war_sockets.c - program to simulate a card game of war
 * Author: Christopher Grigsby
 */

// @packages
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>   
#include <time.h>

#define DECK 52
#define SOCKET_PATH "war_path"

// client_side: handles client-side functionality for the application 
void* client_side();
// server_side: handles server-side functionality for the application
void server_side(int total_rounds);
// create_client_sockets: creates client UNIX socket, connects, returns listening socket descriptor 
int create_client_socket();
// create_server_sockets: creates server UNIX socket, returns listening socket descriptor 
int create_server_socket();
// parent_tell_child_draw: tells child to draw game card
int parent_tell_child_draw(int client_connection);
// parent_stop_game: sends a message to the child to stop playing
int parent_stop_game (int client_connection);
// generate_random_card: returns a random integer between 0 and 52
int generate_random_card(int **card_arr);
// displayCard: displays a card to the console from the provided integer
void display_card(int card);
// card_test_suite: tests core functionality of card components 
int card_test_suite();
// check_end_game: terminates the game if current round equals the user provided number of rounds
int check_end_game (int *current_round, int total_rounds, int client_connection);
// child_send_parent_card: sends a game card to the parent
int child_send_parent_card(int **card_arr, int server_socket_fd);
// check_game_winner: checks for a game winner
int check_game_winner(int *current_round, int total_game_rounds, int player1_round_wins, int player2_round_wins);
// check_round_winner: checks & displays round winner, updates total player wins 
int check_round_winner(int *playerOne_round_score, int *playerTwo_round_score, int *playerOne_round_wins, int *playerTwo_round_wins);
// parent_wait_child_card: waits for game card from child & displays results
int parent_wait_child_card(int client_connection, int total_game_rounds, int current_round, int *playerOne_round_score, int *playerTwo_round_score, int *current_iteration);


int main(int argc, char *argv[]) {
  pthread_t client_thread1, client_thread2;
  int total_rounds;

  if (argc != 2) {
    printf("usage: ./war_pipes <num_rounds>\n");
    exit(EXIT_FAILURE);
  } else {
    total_rounds = atoi(argv[1]);
  }

  if (pthread_create(&client_thread1, NULL, client_side, NULL) != 0) {
    printf("error creating client_thread1\n");
    exit(EXIT_FAILURE);
  }
  if (pthread_create(&client_thread2, NULL, client_side, NULL) != 0) {
    printf("error creating client_thread2\n");
    exit(EXIT_FAILURE);
  }
  printf("\nChild PID: %ld\n\n", client_thread1);
  printf("Child PID: %ld\n\n", client_thread2);
  
  server_side(total_rounds);

  exit(EXIT_SUCCESS);
}


// client_side: handles client-side functionality for the application 
void* client_side() {
  char message[5], stop_message[5];
  // anticipated end game msg
  strcpy(stop_message, "stop");
  int cards[DECK];
  // @init deck to false
  for (int i = 0; i < DECK; ++i) {
    cards[i] = 0;
  }
  int* cards_ptr = cards;
  // create the client socket
  int server_socket_fd = create_client_socket();

  while (read(server_socket_fd, message, sizeof(message)) != -1) {
    if (strcmp(message, stop_message) == 0) { break; }
    child_send_parent_card(&cards_ptr, server_socket_fd);
  }
  close(server_socket_fd);
  pthread_exit(NULL);
}


// server_side: handles server-side functionality for the application
void server_side(int total_rounds) {
  int current_round = 1;
  int total_game_rounds = total_rounds;
  int current_iteration = 1;
  int playerOne_round_score = 0; 
  int playerOne_round_wins = 0;
  int playerTwo_round_score = 0;
  int playerTwo_round_wins = 0;
  int client_connection1, client_connection2;
  // create the server socket
  int server_socket_fd = create_server_socket();
  // accept connections
  client_connection1 = accept(server_socket_fd, NULL, NULL);
  if (client_connection1 < -1) {
    perror("server accept error...");
    exit(EXIT_FAILURE);
  }   
  client_connection2 = accept(server_socket_fd, NULL, NULL);
  if (client_connection2 < -1) {
    perror("server accept error...");
    exit(EXIT_FAILURE);
  }   

  while (1) {
    parent_tell_child_draw(client_connection1);
    parent_wait_child_card(client_connection1, total_game_rounds, current_round, &playerOne_round_score, &playerTwo_round_score, &current_iteration);
    sleep(1);
    parent_tell_child_draw(client_connection2);
    parent_wait_child_card(client_connection2, total_game_rounds, current_round, &playerOne_round_score, &playerTwo_round_score, &current_iteration);
    check_round_winner(&playerOne_round_score, &playerTwo_round_score, &playerOne_round_wins, &playerTwo_round_wins);
    check_game_winner(&current_round, total_game_rounds, playerOne_round_wins, playerTwo_round_wins);
    check_end_game (&current_round, total_game_rounds, client_connection1);
    sleep(1);
  }
  close(server_socket_fd);
}


// create_client_sockets: implements client UNIX socket, connects, returns listening socket descriptor 
int create_client_socket() {
  int server_socket_fd, connection_check;
  //create socket
  server_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if(server_socket_fd < 0) {
    perror("client socket error...\n");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_un s_addr;
  //set up socker address
  s_addr.sun_family = AF_UNIX;
  strcpy(s_addr.sun_path, SOCKET_PATH);
  //connect to server
  for (int i = 0; i < 5; i++) {
    connection_check = connect(server_socket_fd, (struct sockaddr *) &s_addr, sizeof(s_addr));
    if (connection_check < 0) { 
      usleep(1000);
    } else { break; }
  }
  if (connection_check == -1) {
    perror("client unable to connect to parent...\n");
    exit(EXIT_FAILURE);
  }
  return server_socket_fd;
}


// create_server_sockets: implements server UNIX socket, returns listening socket descriptor 
int create_server_socket() {
  int server_socket_fd;
  // create socket
  server_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (server_socket_fd< 0) {
     perror("server socket error...\n");
     exit(EXIT_FAILURE);
  }
  struct sockaddr_un serv_addr;
  // setup socket address
  serv_addr.sun_family = AF_UNIX;
  strcpy(serv_addr.sun_path, SOCKET_PATH);
  // clear out the socket
  unlink(SOCKET_PATH);
  // bind to server socket
  int size = sizeof(serv_addr);
  if (bind(server_socket_fd, (struct sockaddr *) &serv_addr, size) < 0) {
    perror("server bind error...\n");
    exit(EXIT_FAILURE);
  }
  // listen for connection
  if (listen(server_socket_fd, 1) < 0) {
    perror("server listen error...\n");
    exit(EXIT_FAILURE);
  }
  return server_socket_fd;
}


// parent_tell_child_draw: tells child to draw game card
int parent_tell_child_draw(int client_connection) {
  char message[5];
  strcpy(message, "draw");
  if (write(client_connection, message, sizeof(message)) < 0) {
    printf("Error sending message to child\n"); 
    exit(EXIT_FAILURE);
  } 
  return 1;
}


// parent_stop_game: sends a message to the child to stop playing
int parent_stop_game (int client_connection) {
  char message[5];
  strcpy(message, "stop");
  int result = write(client_connection, message, sizeof(message)); 
  if (result < 0) {
    printf("Error sending message to child\n"); 
    exit(EXIT_FAILURE);
  } 
  return 1;
}


// child_send_parent_card: sends a game card to the parent
int child_send_parent_card(int **card_arr, int server_socket_fd) {
  char randCard1_str[20];
  int randCard1_num = generate_random_card(card_arr);
  sprintf(randCard1_str, "%d", randCard1_num);

  if (write(server_socket_fd, randCard1_str, (strlen(randCard1_str)+1)) < 1) {
    printf("Error from child sending msg to parent\n"); 
    exit(EXIT_FAILURE);
  }
  return 1;
}


// generate_random_card: returns a random integer between 0 and 52
int generate_random_card(int **card_arr) {
  int randomCard; 
  srand((unsigned)time(NULL)); 

  for (int j = 0; j < DECK; j++) {
    while(1) {
      randomCard = rand() % DECK; 
      // if the card has not been drawn    
      if ((*card_arr)[randomCard] == 0) { 
        // then mark the card as drawn
        (*card_arr)[randomCard] = 1;  
        return randomCard;                   
      }
    }                   
  }
  return -1;
}


// displayCard: displays a card to the console from the provided integer
void display_card(int card) {
  char *cardSuit[4] = {"Clubs", "Diamonds", "Hearts", "Spades"};
  // determine if number is a face card 
  switch(card%13) {
    case 0:
      printf("%s","Ace");
      break;
    case 10:
      printf("%s","Jack");
      break;
    case 11:
      printf("%s","Queen");
      break;
    case 12:
      printf("%s","King");
      break;
    default:
      printf("%d",card%13+1);
    }
  printf(" of %s\n\n",cardSuit[card/13]);
}


// card_test_suite: tests core functionality of card components 
int card_test_suite() {
  int cards[DECK];
  // @init the deck to false
  for (int i = 0; i < DECK; ++i) {
    cards[i] = 0;
  } 
  int* cards_ptr = cards;
  // @test random card (int) generator
  int card1 = generate_random_card(&cards_ptr);
  int card2 = generate_random_card(&cards_ptr);
  int card3 = generate_random_card(&cards_ptr);
  printf("...testing number generator\n");
  printf("num1: %d, num2: %d, num3: %d\n", card1, card2, card3);
  // @test displayCard
  printf("...testing display_card\n"); 
  display_card(card1); 
  display_card(card2); 
  display_card(card3); 
  return 0;
}


// parent_wait_child_card: waits for game card from child & displays results
int parent_wait_child_card(int client_connection, int total_game_rounds, int current_round, int *playerOne_round_score, int *playerTwo_round_score, int *current_iteration) {
  int card;
  char randCard1_str[20];
  int result = read(client_connection, &randCard1_str, sizeof(randCard1_str));
  if (result < 0) {
    printf("Error retrieving message from child\n"); 
    exit(EXIT_FAILURE);
  }
  card = atoi(randCard1_str);

  if ((*current_iteration) == 1) {
    printf("Beginning %d Rounds…\n\n", total_game_rounds);
    printf("Fight!\n\n");
    printf("----------------------\n\n");
  }
  
  if ((*current_iteration) % 2 != 0) {
    printf("Round %d:\n\n", current_round);
    printf("Player One draws ");
    display_card(card);
    *playerOne_round_score = card;
  } else { 
    printf("Player Two draws ");
    display_card(card);
    *playerTwo_round_score = card;
  } 
  ++*current_iteration;
  return 1;
}

// check_round_winner: checks & displays round winner, updates total player wins 
int check_round_winner(int *playerOne_round_score, int *playerTwo_round_score, int *playerOne_round_wins, int *playerTwo_round_wins) {
  int playerOne_score = (*playerOne_round_score)%13;
  int playerTwo_score = (*playerTwo_round_score)%13;
  // handle Aces
  if (playerOne_score == 0 || playerTwo_score == 0) 
  {
    if (playerOne_score == 0 && playerTwo_score == 0) {
      // then check card suit to resolve ties
      if ((*playerOne_round_score)/13 > (*playerTwo_round_score)/13) {
        printf("Child 1 Wins!\n");
        ++*playerOne_round_wins;
      } else {
        printf("Child 2 Wins!\n");
        ++*playerTwo_round_wins;
      }
    } else if (playerOne_score == 0) {
      printf("Child 1 Wins!\n");
      ++*playerOne_round_wins; 
    } else {
      printf("Child Two Wins!\n");
      ++*playerTwo_round_wins; 
    }
    printf("----------------------\n\n");
    return 1; 
  }

  if (playerOne_score > playerTwo_score) {
      printf("Child 1 Wins!\n");
      ++*playerOne_round_wins; 
  } else if (playerOne_score == playerTwo_score) {
      // then check card suit to resolve ties
      if ((*playerOne_round_score)/13 > (*playerTwo_round_score)/13) {
        printf("Child 1 Wins!\n");
        ++*playerOne_round_wins;
      } else {
        printf("Child 2 Wins!\n");
        ++*playerTwo_round_wins;
      }
  } else {
      printf("Child 2 Wins!\n");
      ++*playerTwo_round_wins;
  } 

  *playerOne_round_score = 0; 
  *playerTwo_round_score = 0;
  printf("----------------------\n\n");
  return 1;
}

// check_game_winner: checks for a game winner
int check_game_winner(int *current_round, int total_game_rounds, int player1_round_wins, int player2_round_wins) {
  if ((*current_round) == total_game_rounds) {
    printf("------Total Wins------\n\n");
    printf("Child 1: %d\n\n", player1_round_wins);
    printf("Child 2: %d\n\n", player2_round_wins);
    if (player1_round_wins > player2_round_wins) {
      printf("Child 1 Wins Game!\n\n");
    } else if (player1_round_wins == player2_round_wins) {
      printf("Tie Game! Play Again!\n\n");
      --*current_round;
    } else {
      printf("Child 2 Wins Game!\n\n");
    }
  }
  return 1;
}


// check_end_game: terminates the game if current round equals the user provided number of rounds
int check_end_game(int *current_round, int total_rounds, int client_connection) {
  if ((*current_round) == total_rounds) {
     parent_stop_game(client_connection);
  } else {
    ++*current_round;
  }
  return 1;
}