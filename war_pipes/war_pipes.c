/******************************************************
 * war_pipes.c - program to simulate a card game of war
 * Author: Christopher Grigsby
 */

// @packages
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>   
#include <time.h>
#include <string.h>
// @global vars
#define DECK 52
// @init file descriptors
static int fd1[2], fd2[2]; 

// game_welcome_msg: displays game welcome message to the console
int check_welcome_msg(int current_round, int total_rounds, pid_t pid_child1, pid_t pid_child2) {
  if (current_round == 1) {
    printf("\nChild 1 PID: %d\n\n", pid_child1);
    printf("Child 2 PID: 114 %d\n\n", pid_child2);
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
int parent_wait_child_card(int fileDescriptor, int current_round, int *playerOne_round_score, int *playerTwo_round_score) {
  int card;
  char randCard1_str[20];
  int result = read(fileDescriptor, &randCard1_str, sizeof(randCard1_str));
  if (result < 0) {
    printf("Error retrieving message from child\n"); 
    exit(EXIT_FAILURE);
  }
  card = atoi(randCard1_str);
  
  if (fileDescriptor == fd1[0]) {
    if (current_round == 1) {
      printf("Fight!\n\n");
      printf("----------------------\n\n");
    }
    printf("Round %d:\n\n", current_round);
    printf("Player One draws ");
    display_card(card);
    *playerOne_round_score = card;
  } else if (fileDescriptor == fd2[0]) { 
    printf("Player Two draws ");
    display_card(card);
    *playerTwo_round_score += card;
  } else {
    printf("File descriptor from unknown source\n");
  }
  return 1;
}

// parent_tell_child_draw: pipe to tell child to draw game card, displays a welcome message if needed
int parent_tell_child_draw(int fileDescriptor) {
  int result = write(fileDescriptor, "draw", 4);
  if (result < 0) {
    printf("Error sending message to child one\n"); 
    exit(EXIT_FAILURE);
  } 
  return 1;
}

// child_wait_parent_draw: pipe for child to await request for game card
int child_wait_parent_draw(int fileDescriptor) {
  char draw[4];
  int result = read(fileDescriptor, &draw, sizeof(draw));
  if (result < 0) {
    printf("Error from child receving msg from parent"); 
    exit(EXIT_FAILURE);
  }
  return 1;
}

// child1_send_parent_card: pipe to send a game card to the parent
int child_send_parent_card(int **card_arr, int fileDescriptor){
  char randCard1_str[20];
  int randCard1_num = generate_random_card(card_arr);
  sprintf(randCard1_str, "%d", randCard1_num);
  int result = write(fileDescriptor, randCard1_str, (strlen(randCard1_str)+1));
  if (result < 1) {
    printf("Error from child sending msg to parent\n"); 
    exit(EXIT_FAILURE);
  }
  return 1;
}

// driver
int main(int argc, char *argv[]) {
  int playerOne_round_wins = 0; 
  int playerTwo_round_wins = 0;
  int playerOne_round_score = 0; 
  int playerTwo_round_score = 0;
  int current_round = 1;
  int total_rounds = 1;
  char *user_input;
  int cards[DECK];
  // @init the deck to false
  for (int i = 0; i < DECK; ++i) {
    cards[i] = 0;
  }
  int* cards_ptr = cards;
  // obtain rounds
  if (argc != 2) {
    printf("usage: ./war_pipes <num_rounds>\n");
    exit(EXIT_FAILURE);
  } else {
    total_rounds = atoi(argv[1]);
  }
  // @init process identification
  pid_t pid_child1;
  pid_t pid_child2;
  // @init pipes and children
  if (pipe(fd1) < 0) { 
    printf("Error creating pipe\n");
    exit(EXIT_FAILURE); 
  }
  pid_child1 = fork();
  if (pid_child1 < 0) { 
    printf("Error creating child process\n");
    exit(EXIT_FAILURE); 
  } 
  if (pipe(fd2) < 0) { 
    printf("Error creating pipe\n");
    exit(EXIT_FAILURE); 
  }
  pid_child2 = fork();
  if (pid_child2 < 0) { 
    printf("Error creating child process\n");
    exit(EXIT_FAILURE); 
  } 

  if(pid_child1 != 0 && pid_child2 != 0){
    check_welcome_msg(current_round, total_rounds, pid_child1, pid_child2);
  }

  if (pid_child1 != 0 && pid_child2 != 0) {
    while(1) {
      sleep(rand()%2+1);  
       // child one behaviors   
       parent_tell_child_draw(fd1[1]);
       sleep(1);
       parent_wait_child_card(fd1[0], current_round, &playerOne_round_score, &playerTwo_round_score);
       // child two behaviors
       parent_tell_child_draw(fd2[1]);
       sleep(1);
       parent_wait_child_card(fd2[0], current_round, &playerOne_round_score, &playerTwo_round_score);
       check_round_winner(&playerOne_round_score, &playerTwo_round_score, &playerOne_round_wins, &playerTwo_round_wins);
       check_game_winner(&current_round, total_rounds, playerOne_round_wins, playerTwo_round_wins);
       sleep(1);
       check_end_game(&current_round, total_rounds, pid_child1, pid_child2); 
    }
  } else {
    while(1){
      sleep(rand()%2+1);
      // child one behaviors
      child_wait_parent_draw(fd1[0]);
      sleep(1);
      child_send_parent_card(&cards_ptr, fd1[1]);
      // child two behaviors
      child_wait_parent_draw(fd2[0]);
      sleep(1);
      child_send_parent_card(&cards_ptr, fd2[1]);
    }
  }
  return EXIT_SUCCESS;
}