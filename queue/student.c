/**************************************************************
 * student.c - program to utilize queue to read student records
 * Author: Christopher Grigsby
 * Email: cwgrigsby@coastal.edu
 * Course: CSCI 356
 */

// @packages
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// @scripts
#include "my_queue.h"
#include "student.h"

// newStudent: returns a pointer to a new student
student newStudent() {
    struct student* stu = malloc(sizeof(*stu));
    strcpy(stu[0].name, " ");
    stu[0].data = 0;
    return stu; 
}

// testStudentAndQueue: displays the functionality of the queue and student struct the console
void testStudentAndQueue() {
    //test student
    student stu = newStudent(); 
    strcpy(stu[0].name, "Chris");
    stu[0].data = 15;
    printf("...Testing student name and data: %s %d\n\n", stu[0].name, stu[0].data);
    //test queue
    queue q = newqueue();
    printf("...Testing for empty queue: %d\n\n", isempty(q));
    //test enqueue
    enqueue(q, stu);
    student stu2 = newStudent(); 
    strcpy(stu2[0].name, "Josh");
    stu2[0].data = 10;
    enqueue(q, stu2); 
    //test peek 
    student stu_peek = peek(q);
    printf("...Testing for successful peek: %s %d\n\n", stu_peek[0].name, stu_peek[0].data); 
    //test dequeue
    student s1 = dequeue(q);
    student s2 = dequeue(q); 
    printf("...Testing for successful dequeue: %s %d\n\n", s1[0].name, s1[0].data); 
    printf("...Testing for successful dequeue: %s %d\n\n", s2[0].name, s2[0].data); 
}

// getStudentInformation: a loop to collect user input, returns a queue of the data collected
queue getStudentInformation(int *count) {
    queue q = newqueue();
    char student_name[50]; 
    int student_data; 
    char buffer[100];

    printf("Please enter student first name and data on the same line below.\n");
    printf("(Enter an empty line to stop once complete): \n");
    while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // break the loop if the buffer is empty
        if (buffer[0] == '\n') { 
          break;
        }
        // use the buffer to collect the input
        sscanf(buffer, "%s %d", student_name, &student_data);
        student stu = newStudent();
        strcpy(stu[0].name, student_name);
        stu[0].data = student_data;
        ++*count;
        enqueue(q, stu);
    }
    return q;
}

// distanceFromMax: provides the difference from the userScore and the maxScore
int distanceFromMax (int maxScore, int userScore) {
    return (maxScore - userScore); 
}

// driver
int main(int argc, char *argv[]) {
    int count = 0;
    int max_value = 0;  
    int sum = 0; 
    float average = 0; 

    queue q = getStudentInformation(&count); 
    printf("\nPlease enter the max value for the data entered: ");
    scanf("%d", &max_value); 
    printf("...Processing your request\n"); 

    printf("\nName\t\t Data\t\t Distance\n");
    for(int i = 0; i < count; ++i) {
        student stu = dequeue(q); 
        int distance = distanceFromMax(max_value, stu[0].data);
        printf("%s:\t\t %-10d\t %d\n", stu[0].name, stu[0].data, distance);
        sum = sum + stu[0].data;
    }
    average = sum/count; 
    printf("\nAverage:\t\t\t %.1f\n", average); 
    return 0;
}