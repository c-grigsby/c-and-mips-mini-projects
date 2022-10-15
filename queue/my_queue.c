/*****************************************
 * my_queue.c - implementation for a queue
 * Author: Christopher Grigsby
 * Email: cwgrigsby@coastal.edu
 * Course: CSCI 356
 */

// @packages
#include <stdlib.h>
#include <stdio.h>
// @scripts
#include "my_queue.h"

// newqueue: initializes a new queue
queue newqueue() {
  struct queueS* q = malloc(sizeof(*q));
  if (!q) {
    printf("Error: malloc failed initializing queue\n\n");
    exit(1);
  }
  q->front = NULL;
  return q;
}

// isempty: returns a positive integer if the queue is empty
int isempty (const queue q) {
  if (q->front == NULL) return 1;
  return -1;
}

// enqueue: adds an item to the end of the queue
void enqueue (queue q, void* item) {
  struct q_elementS *q_element = malloc(sizeof(struct q_elementS));
  q_element->contents = item;
  q_element->next = NULL; 

  if(isempty(q) > 0) { 
    q->front = q_element;
  } else { 
    q->front->next = q_element;
  }
}

// dequeue: removes an item from the front of a queue
void* dequeue (queue q) {
  if(isempty(q) > 0) {
    printf("Error: queue is empty, aborting dequeue\n\n");
    exit(1);
  }
  struct q_elementS *tmp_qElement; 
  void* data = q->front->contents; 
  tmp_qElement = q->front;
  q->front = q->front->next;
  free(tmp_qElement);
  return data; 
}; 

// peek: returns the contents of the first item of the queue without a dequeue
void* peek (queue q){
  if(isempty(q) > 0) {
    printf("Error: queue is empty, aborting dequeue\n\n");
    exit(1);
  }
  void* data = q->front->contents;
  return data;
}