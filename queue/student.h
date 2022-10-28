/******************************************************************
 * student.h - prototype functions and student class implementation
 */

#ifndef STUDENT_H_
#define STUDENT_H_

struct student {
  char name[50];
  int  data;
};

typedef struct student* student;

// newStudent: returns a pointer to a new student
student newStudent();

// testStudentAndQueue: displays the functionality of the queue and student struct to the console
void testStudentAndQueue();

// getStudentInformation: a loop to collect user input, returns a queue of the data collected
queue getStudentInformation(int *count);

// distanceFromMax: provides the difference from the userScore and the maxScore
int distanceFromMax(int maxScore, int userScore);

#endif /* STUDENT_H_ */