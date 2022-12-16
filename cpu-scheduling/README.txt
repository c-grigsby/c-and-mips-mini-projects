CPU Scheduling

PROJECT_DESCRIPTION: 

- This application simulates a simple CPU scheduler, which simulates different scheduling algorithms given a process trace. This scheduler will not need to deal with I/O, system-calls, or memory-related interrupts. This scheduler will only deal with the actual process execution schedule, not with context-switching, process creation, or actual execution. The processes to be scheduled will be defined in terms of submission time, id and CPU time of each process.

PROJECT_DETAILS: 

- Developed in C
- Utilization of custom queue implementation

GETTING_STARTED: 

To Compile: $ gcc -Wall -std=c99 ./scheduler.c ./my_queue.c -o scheduler

(Note: -std=c99 is believed to be unneeded but a possible safety precaution for the virtual environment within the CI Project)

To Run: $ ./scheduler [FCFS | RR] [optional quantum]

Standard Input: <pid> <arrival_time> <cpu_time>  (all integers, enter: 0 0 0 to begin simulation)

PROGRAM_FILES: 

- scheduler.c | main method
- my_queue.c | implementation of custom queue
- my_queue.h | header file for custom my_queue
- README.txt | application description file


CI_PROJECT_COMPILATION_TEST: CSCI 135-4 (Ubuntu w/ XFCE, 4 core virtual machine, 16 GiB RAM) 12/6/2022

PROGRAM NOTES: 

FCFS: Fully operational with full utilization of custom queue implementation
RR: Fully operational. Implementing this feature with full utilization of the queue was shown to be troublesome at this time. Thus, since the assignment description did not state to what extent the queue needed to be utilized the developer chose to focus on providin a working feature rather than to focus on a specific data structure. Though this does not effect the performance of the program it is felt that with more development time the organization of the application could be improved upon. 

---------------------------
Author: Christopher Grigsby
Course: CSCI*356
Date: 12/6/2022
Email: cwgrigsby@coastal.edu
