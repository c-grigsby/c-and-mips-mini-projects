/*
 * student.c - program to utilize queue to read student records
 *
 * Author: Christopher Grigsby
 * Email: cwgrigsby@coastal.edu
 * Course: CSCI 356
 */
// @packages
#include <stdio.h>
#include <stdlib.h>
// @scripts
#include "my_queue.h"

int main(int argc, char *argv[])
{
    printf("Hello from student.c");
    void* q = newqueue();
    printf("Test for empty queue: %d", isempty(q));
    return 0;
}