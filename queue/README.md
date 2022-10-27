Program Specification
Implement the functions prototyped in my_queue.h in a file named my_queue.c. Write a program (student.c) that uses your queue to read student records (this is where your main function will be). In addition, you will have a header file for your student.c where you will place a struct that represents a Student (name and grade) along with any protoypes for functions you create/use in your implementation file in a header file named, student.h. You are encouraged to create additional functions (think divide and conquer). Your program is expected to take user input from standard in and stops when a blank line is entered. While the user is entering student data, you should determine the max value. After the user has finished getting user input, your program should then calculate and output in order each student’s name, data and distance from the max, and finally display the average (the average should be calculated in the phase where the records are being ‘dequeued’ from the queue NOT while data is being input (I will look at your code).

For example,
```
> Students
John     15
Tim      15
Sarah    20
Chris    10
```
 
```
Name   Data     Distance
John:   15         5
Tim:    15         5
Sarah:  20         0
Chris:  10         10

average:           15
```