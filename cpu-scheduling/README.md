# CPU Scheduling

## Project Overview

This application simulates a simple CPU scheduler, which simulates different scheduling algorithms given a process trace. This scheduler does not currently handle I/O, system-calls, or memory-related interrupts. This scheduler only deals with the actual process execution schedule, not with context-switching, process creation, or actual execution. The processes to be scheduled will be defined in terms of submission time, id and CPU time of each process.

## Project Details 

- Developed in C
- Utilization of custom queue implementation

## Getting Started

- To Compile 
- std=c99 is a safety precaution for virtual environments

```
$ gcc -Wall -std=c99 ./scheduler.c ./my_queue.c -o scheduler
```

- To Run 

```
$ ./scheduler [FCFS | RR] [optional quantum]
```

## Standard Input 

```
$ <pid> <arrival_time> <cpu_time> (all integers, enter: 0 0 0 to begin simulation)
```
## Program Files

- scheduler.c | main method
- my_queue.c | implementation of custom queue
- my_queue.h | header file for custom my_queue
- README.txt | application description file

## Program Notes 

- First Come First Serve (FCFS) Scheduler: Operational with full utilization of custom queue implementation

- Round Robin (RR) Scheduler: Operational 

