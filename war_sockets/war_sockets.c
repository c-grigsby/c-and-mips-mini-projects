/********************************************************
 * war_sockets.c - program to simulate a card game of war
 * Author: Christopher Grigsby
 */

// @packages
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>   
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define DECK 52
#define SOCKET_PATH "WAR_CARDS_SOCKET"

typedef struct child_process_data {
  int id;
  pid_t child_pid;
  int child_server_socket;
  int* child_cards_ptr;
} child_process_data;

typedef struct parent_process_data {
  int client_socket;
  pid_t child1_pid1;
  pid_t child2_pid2;
  int current_round;
  int current_iteration;
  int playerOne_round_score;
  int playerOne_round_wins;
  int playerTwo_round_score;
  int playerTwo_round_wins;
  int total_game_rounds;
} parent_process_data;

// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_rwlock_t rwlock_id;

// game_welcome_msg: displays game welcome message to the console
int check_welcome_msg(int current_round, int total_rounds, pid_t pid_child1, pid_t pid_child2) {
  if (current_round == 1 && pid_child1 > 0 && pid_child2 > 0) {
    printf("\nChild 1 PID: %d\n\n", pid_child1);
    printf("Child 2 PID: %d\n\n", pid_child2);
    printf("Beginning %d Rounds…\n\n", total_rounds);
  } 
  return 1;
}

// check_round_winner: declares a round winner and updates total player wins 
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
int check_game_winner(int *current_round, int total_rounds, int playerOne_round_wins, int playerTwo_round_wins) {
  printf("Checking game winner!\n\n");
  if ((*current_round) == total_rounds) {
    printf("------Total Wins------\n\n");
    printf("Child 1: %d\n\n", playerOne_round_wins);
    printf("Child 2: %d\n\n", playerTwo_round_wins);
    if (playerOne_round_wins > playerTwo_round_wins) {
      printf("Child 1 Wins Game!\n\n");
    } else if (playerOne_round_wins == playerTwo_round_wins) {
      printf("Tie Game! Play Again!\n");
      --*current_round;
    } else {
      printf("Child 2 Wins Game!\n\n");
    }
  }
  return 1;
}

// check_end_game: terminates the game if current round equals the user provided number of rounds
int check_end_game(int *current_round, int total_rounds, pid_t pid_child1, pid_t pid_child2) {
  printf("Checking end game!\n");
  if ((*current_round) == total_rounds) {
    kill(pid_child1, SIGKILL);
    kill(pid_child2, SIGKILL);
    exit(EXIT_SUCCESS);
  } else {
    ++*current_round;
  }
  return 1;
}

// generate_random_card: returns a random integer between 0 and 52
int generate_random_card(int **card_arr) {
  int randomCard; 
  srand((unsigned)time(NULL)); 

  for(int j = 0; j < DECK; j++) {
    while(1) {
      randomCard = rand() % DECK; 
      // if the card has not been drawn    
      if((*card_arr)[randomCard] == 0) { 
        // then mark the card as drawn
        (*card_arr)[randomCard] = 1;  
        return randomCard;                   
      }
    }                   
  }
  return -1;
}

// displayCard: displays a card to the console based on the provided integer
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

// test_suite: tests core functionality of card components 
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

// parent_wait_child_card: pipe to wait for game card from child process & displays results
int parent_wait_child_card(int client_socket, int current_round, int *current_iteration, int *playerOne_round_score, int *playerTwo_round_score, int total_rounds, pid_t pid_child1) {
  int card;
  char randCard1_str[20];
  int result = read(client_socket, &randCard1_str, sizeof(randCard1_str));
  if (result < 0) {
    printf("Error retrieving message from child\n"); 
    exit(EXIT_FAILURE);
  }
  card = atoi(randCard1_str);

  if ((*current_iteration) == 1) {
    printf("\nChild 1 PID: %d\n\n", pid_child1);
    printf("Child 2 PID: %d\n\n", 214672);
    printf("Beginning %d Rounds…\n\n", total_rounds);
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
    *playerTwo_round_score += card;
  } 
  ++*current_iteration;
  return 1;
}

// parent_tell_child_draw: tells child to draw game card
int parent_tell_child_draw(int client_socket) {
  char message[4] = "draw";
  int result = write(client_socket, message, sizeof(message)); 
  if (result < 0) {
    printf("Error sending message to child\n"); 
    exit(EXIT_FAILURE);
  } 
  return 1;
}

// child_wait_parent_draw: pipe for child to await request for game card
int child_wait_parent_draw(int server_socket, int ret) {
  char expectedMsg[4] = "draw";
  char message[4];

  int result = read(server_socket, message, sizeof(message));
  if (result < 0) {
    printf("Error from child receving msg from parent\n"); 
    exit(EXIT_FAILURE);
  }
  if (strcmp(message, expectedMsg) != 0) {
    return -1;
  }
  return 1;
}

// child1_send_parent_card: pipe to send a game card to the parent
int child_send_parent_card(int **card_arr, int server_socket, int ret){
  char randCard1_str[20];
  int randCard1_num = generate_random_card(card_arr);
  sprintf(randCard1_str, "%d", randCard1_num);

  int result = write(server_socket, randCard1_str, (strlen(randCard1_str)+1));
  if (result < 1) {
    printf("Error from child sending msg to parent\n"); 
    exit(EXIT_FAILURE);
  }
  return 1;
}

// parent_behaviors: hosts the server thread behaviors
void* parent_behaviors(void* data_ptr) {
  struct parent_process_data* data = (struct parent_process_data*)data_ptr;
  int client_socket = data[0].client_socket;
  int current_round = data[0].current_round;
  int total_rounds = data[0].total_game_rounds;
  int current_iteration = data[0].current_iteration;
  int playerOne_round_score = data[0].playerOne_round_score;
  int playerOne_round_wins = data[0].playerOne_round_wins;
  int playerTwo_round_score = data[0].playerTwo_round_score;
  int playerTwo_round_wins = data[0].playerTwo_round_wins;
  int pid_child1 = data[0].child1_pid1;
  int pid_child2 = data[0].child2_pid2;

  parent_tell_child_draw(client_socket);
  sleep(1);
  parent_wait_child_card(client_socket, current_round, &current_iteration, &playerOne_round_score, &playerTwo_round_score, total_rounds, pid_child1);
  sleep(2);
  parent_tell_child_draw(client_socket);
  sleep(1);
  parent_wait_child_card(client_socket, current_round, &current_iteration, &playerOne_round_score, &playerTwo_round_score, total_rounds, pid_child1);
  sleep(2);
  check_round_winner(&playerOne_round_score, &playerTwo_round_score, &playerOne_round_wins, &playerTwo_round_wins);
  check_game_winner(&current_round, total_rounds, playerOne_round_wins, playerTwo_round_wins);
  check_end_game(&current_round, total_rounds, pid_child1, pid_child2); 
  return NULL;
}

// child_behaviors: hosts the child thread behaviors
void* child_behaviors(void *data_ptr) {
  struct child_process_data* data = (struct child_process_data*)data_ptr;
  int server_socket = data[0].child_server_socket;
  int* cards_ptr = data[0].child_cards_ptr;
  int ret = 0; 
  printf("Thread created!\n");

  while (1) { 
    sleep(1);
    // if parent stops the game 
    if (child_wait_parent_draw(server_socket, ret) < 0) {
      // then kill the thread 
      int pthread_cancel(pthread_t child1_thread);
    }
    sleep(1);
    child_send_parent_card(&cards_ptr, server_socket, ret);
  }
  pthread_exit(NULL);
}


// driver
int main(int argc, char *argv[]) {
  int playerOne_round_wins = 0; 
  int playerTwo_round_wins = 0;
  int playerOne_round_score = 0; 
  int playerTwo_round_score = 0;
  int current_round = 1;
  int current_iteration = 1;
  int total_rounds = 1;
  int cards[DECK];
  // @init the deck to false
  for (int i = 0; i < DECK; ++i) {
    cards[i] = 0;
  }
  int* cards_ptr = cards;
  // obtain game rounds
  if (argc != 2) {
    printf("usage: ./war_pipes <num_rounds>\n");
    exit(EXIT_FAILURE);
  } else {
    total_rounds = atoi(argv[1]);
  }
  // @init parent data for child processes
  parent_process_data parent_data = {
    .child1_pid1 = 0,
    .child2_pid2 = 0,
    .current_round = current_round,
    .total_game_rounds = total_rounds,
    .current_iteration = current_iteration,
    .playerOne_round_score = playerOne_round_score, 
    .playerOne_round_wins = playerOne_round_wins,
    .playerTwo_round_score = playerTwo_round_score,
    .playerTwo_round_wins = playerTwo_round_wins,
  };
  /* SERVER-SIDE SOCKET: UNIX */
  struct sockaddr_un serv_addr;
  int socket_fd, client_socket;
  // create socket
  socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("socket error...");
    exit(EXIT_FAILURE);
  }
  // setup socket address
  serv_addr.sun_family = AF_UNIX;
  strcpy(serv_addr.sun_path, SOCKET_PATH);
  // clear out the socket
  unlink(SOCKET_PATH);
  // bind to server socket
  int size = sizeof(serv_addr);
  if (bind(socket_fd, (struct sockaddr *) &serv_addr, size) < 0) {
    perror("bind error...");
    exit(EXIT_FAILURE);
  }
  // listening for connection
  if (listen(socket_fd, 1) < 0) {
    perror("listen error...");
    exit(EXIT_FAILURE);
  }
  // @init child process id
  pid_t pid_child1, pid_child2;
  // @init child one
  pid_child1 = fork();
  if (pid_child1 < 0) { 
      printf("error creating child process\n");
      exit(EXIT_FAILURE); 
  } else if (pid_child1 != 0) {
      parent_data.child1_pid1 = pid_child1;
  }

  if (pid_child1 != 0) {
    // accept connections 
    while(1) {
        client_socket = accept(socket_fd, NULL, NULL);
        if (client_socket < -1) {
        perror("accept error..."); 
        exit(EXIT_FAILURE);
        }
        parent_data.client_socket = client_socket;
        parent_behaviors(&parent_data);
        close(socket_fd);
    }
  } else {
        /* CREATE CHILDREN SERVER: UNIX */
        struct sockaddr_un server_address;
        int socket_fd, connect_check;
        // create socket
        socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (socket_fd < 0) {
          perror("socket error...");
          exit(EXIT_FAILURE);
        }
        // setup socket address
        server_address.sun_family = AF_UNIX;
        strcpy(server_address.sun_path, SOCKET_PATH);
        // connect to server
        for (int i = 0; i < 5; i++) {
          connect_check = connect(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address));
          if(connect_check < 0) {
              perror("do_child: connection failed, trying again...");
              usleep(1000);
            } else {
                break; 
            }
        }

        // check if connection success
        if (connect_check == -1) {
            perror("do_child: unable to connect to parent...");
            exit(EXIT_FAILURE);
        }
        // @init data for thread
        child_process_data data = {
          .child_pid = pid_child1,
          .child_server_socket = socket_fd,
          .child_cards_ptr = cards_ptr, 
        };
        // @init threads
        pthread_t child1_thread;
        pthread_t child2_thread;
    
        if (current_round == 1) {
          // spawn thread one and handle behavior
          pthread_create(&child1_thread, NULL, child_behaviors, &data);
        }

        // create child two process
        pid_child2 = fork();
        if (pid_child2 < 0) { 
          printf("Error creating child process\n");
          exit(EXIT_FAILURE); 
        } else if (pid_child2 != 0) {
          parent_data.child2_pid2 = pid_child2;
        }
        
        if (pid_child2 == 0 && current_round == 1) {
          // spawn thread two and handle behavior
          pthread_create(&child2_thread, NULL, child_behaviors, &data);
        }
        // ensure the threads finishes running 
        pthread_join(child1_thread, NULL);
        pthread_join(child2_thread, NULL);
        pthread_cancel(child1_thread);
        pthread_cancel(child2_thread);
        // handle children
        wait(NULL);
    }
    exit(EXIT_SUCCESS);
}