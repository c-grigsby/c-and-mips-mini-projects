/*
 * my_queue.c - implementation for a queue
 *
 * Author: Christopher Grigsby
 * Email: cwgrigsby@coastal.edu
 * Course: CSCI 356
 */

// @packages
#include <stdlib.h>
// @scripts
#include "my_queue.h"

queue newqueue() {
  queue q; 
  return (&q);
}

int isempty (const queue q) {
  if (q->front == NULL) {
    return 1;
  }
}

