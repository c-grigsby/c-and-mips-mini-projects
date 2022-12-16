/********************************************************
 * scheduler.c - program to simulate a simple cpu scheduler
 * Author: Christopher Grigsby
 * Email: cwgrigsby@coastal.edu
 * Course: CSCI 356
 */

// @packages
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <memory.h>
// @scripts
#include "my_queue.h"

struct process {
    bool eligible_to_run;
    int arrival_time;
    int cpu_time;
    int pid;
};

// testProcessAndQueue: displays the functionality of the queue and process struct to the console
void testProcessAndQueue() {
  // test process
  struct process process1;
  process1.eligible_to_run = true;
  process1.arrival_time = 2;
  process1.cpu_time = 4;
  process1.pid = 1;
  if (process1.eligible_to_run) {
    printf("...Testing process data; arrival_time: %d cpu_time: %d pid: %d\n\n", process1.arrival_time, process1.cpu_time, process1.pid);
  }
  // test queue
  queue q = newqueue();
  printf("...Testing for empty queue: %d\n\n", isempty(q));
  // test enqueue
  enqueue(q, &process1);
  struct process process2;
  process2.eligible_to_run = true;
  process2.arrival_time = 1;
  process2.cpu_time = 2;
  process2.pid = 3;
  enqueue(q, &process2);
  // test peek
  struct process *process_peek = peek(q);
  printf("...Testing process_peek data; arrival_time: %d cpu_time: %d pid: %d\n\n", process_peek->arrival_time, process_peek->cpu_time, process_peek->pid);
  // test dequeue
  struct process *p1 = dequeue(q);
  struct process *p2 = dequeue(q);
  printf("...Testing for successful dequeue; arrival_time: %d cpu_time: %d pid: %d\n\n", p1[0].arrival_time, p1[0].cpu_time, p1[0].pid);
  printf("...Testing for successful dequeue; arrival_time: %d cpu_time: %d pid: %d\n\n", p2[0].arrival_time, p2[0].cpu_time, p2[0].pid);
}

int main(int argc, char *argv[]) {
  bool idle = false;
  char type_of_scheduler[10];
  double turn_around_time = 0.0;
  int clock_time = 0;
  int cpu_times[25]; // initial cpu times
  int first_job_arrival_time = 0; 
  int head_pid = -1; 
  int i = 0;
  int idle_time = 0;
  int in_pid, in_arrival_time, in_cpu_time;
  int quantum = 0;               
  int quantum_counter = 0;      
  int response = 0; // time of first finished job
  int total_arrived_jobs = 0; 
  int total_finished_jobs = 0;
  int total_jobs = 0; 
  int wait_time = 0;
  queue q = newqueue();
  struct process incoming_processes[25]; 
  struct process temp_jobs_arr[25];   

  // ensure proper usage
  if (argc < 2) {
    printf("usage: ./scheduler [FCFS | RR] [optional quantum]\n");
    exit(EXIT_FAILURE);
  } 
  if (strcmp(argv[1], "RR") == 0 && argc != 3) {
    printf("usage: ./scheduler [FCFS | RR] [optional quantum]\n");
    exit(EXIT_FAILURE);
  }

  // store the type of scheduler
  strcpy(type_of_scheduler, argv[1]);
  // store the quantum if present 
  if (strcmp(argv[1], "RR") == 0 && argc == 3) {
    char *quant_str = argv[2];
    quantum = atoi(quant_str);
  }

  // capture the standard input
  scanf("%d %d %d", &in_pid, &in_arrival_time, &in_cpu_time);
  incoming_processes[i].pid = in_pid;
  incoming_processes[i].arrival_time = in_arrival_time;
  incoming_processes[i].cpu_time = in_cpu_time;
  incoming_processes[i].eligible_to_run = false;
  cpu_times[i] = incoming_processes[i].cpu_time;

  // continue capturing data until input of: 0 0 0
  do {
    if (incoming_processes[i].pid == 0 && incoming_processes[i].arrival_time == 0 && incoming_processes[i].cpu_time == 0) { break; }
    ++i;
    ++total_jobs;
    scanf("%d %d %d", &in_pid, &in_arrival_time, &in_cpu_time);
    incoming_processes[i].pid = in_pid;
    incoming_processes[i].arrival_time = in_arrival_time;
    incoming_processes[i].cpu_time = in_cpu_time;
    incoming_processes[i].eligible_to_run = false;
    cpu_times[i] = incoming_processes[i].cpu_time;
  } while (true);

  if (total_jobs == 0) {
    printf("You have chosen for no processes to run. Goodbye\n");
    exit(EXIT_SUCCESS);
  }

  // otherwise sort incoming_processes by arrival_time
  i = 0;
  while (incoming_processes[i].pid != 0 || incoming_processes[i].arrival_time != 0 || incoming_processes[i].cpu_time != 0) {
  if (i == 0) { i++; }
    else {
      for (int j = i; j > 0; j--) {
        if (incoming_processes[j].arrival_time < incoming_processes[j-1].arrival_time) {
          struct process temp = incoming_processes[j-1];
          incoming_processes[j-1] = incoming_processes[j];
          incoming_processes[j] = temp;
        }
      }
      i++;
    }
  }

  // and move the sorted array into the queue
  for (int k = 0; k < total_jobs; k++) {
    enqueue(q, &incoming_processes[k]);
  }

  // get the the pid at the head of the queue
  struct process *process_peek = peek(q);
  head_pid = process_peek[0].pid;


  printf("Simulation starting:\n");
  if (strcmp(type_of_scheduler, "FCFS") == 0) {

    while (true) {

      for (int i = 0; i < total_jobs; i++) {
        // dequeue process
        struct process *current_process = dequeue(q);
        // interrogate arrival time
        if (current_process[0].arrival_time == clock_time) {
          printf("%d %d arriving\n", clock_time, current_process[0].pid);
          total_arrived_jobs++;
          current_process[0].eligible_to_run = true;

          if (total_arrived_jobs == 1) { first_job_arrival_time = clock_time; }
        }
        // return process to queue (current_process is a *)
        enqueue(q, current_process);
      }

     for (int i = 0; i < total_jobs; i++) {
        // dequeue the process
        struct process *current_process = dequeue(q);
        // and interrogate eligibility to run
        if (current_process[0].eligible_to_run) {
          // if the process is finished
          if (current_process[0].cpu_time == 0) {
            printf("%d %d finished\n", clock_time, current_process[0].pid);
            turn_around_time = turn_around_time + clock_time;
            total_finished_jobs++;

            if (total_finished_jobs == 1) { response = clock_time; }
            // terminate run eligibility
            current_process[0].eligible_to_run = false;
            // return process to the queue
            enqueue(q, current_process);
          } else {
            // otherwise, run the process
            printf("%d %d running\n", clock_time, current_process[0].pid);
            if (current_process[0].cpu_time == cpu_times[i]) {
              wait_time = wait_time + clock_time;
              wait_time = wait_time - current_process[0].arrival_time;
            }
            current_process[0].cpu_time = current_process[0].cpu_time - 1;
            idle = false;
            // return process to the queue
            enqueue(q, current_process);
            break;
          }
        } else {
          idle = true;
          // return process to the queue
          enqueue(q, current_process);
        }
      }
      // interrogate completed jobs to terminate
      if (total_finished_jobs == total_jobs) { break; }
      // check for idleness
      if (idle) {
        printf("%d idle\n", clock_time);
        idle_time++;
      }
      // increment clock_time
      clock_time++;
      // reset queue
      while(true) {
        struct process *peek_process = peek(q);
        if (peek_process[0].pid != head_pid) {
          struct process *reset_process = dequeue(q);
          enqueue(q, reset_process);
        } else { break; }
      }
    }
  } else if (strcmp(type_of_scheduler, "RR") == 0) {
    // transfer the queue to a temp_array
    for (int j = 0; j < total_jobs; j++) {
      struct process *move_process = dequeue(q);
      temp_jobs_arr[j].pid = move_process[0].pid;
      temp_jobs_arr[j].arrival_time = move_process[0].arrival_time;
      temp_jobs_arr[j].cpu_time = move_process[0].cpu_time;
      temp_jobs_arr[j].eligible_to_run = move_process[0].eligible_to_run;
    }
    
    while (true) {

      for (int i = 0; i < total_jobs; i++) {
        // interrogate if arriving
        if (temp_jobs_arr[i].arrival_time == clock_time) {
          printf("%d %d arriving\n", clock_time, temp_jobs_arr[i].pid);
          total_arrived_jobs++;
          temp_jobs_arr[i].eligible_to_run = true;

          if (total_arrived_jobs == 1) { first_job_arrival_time = clock_time;}
        }
      }

      for (int i = 0; i < total_jobs; i++) {
        // interrogate eligibility to run
        if (temp_jobs_arr[i].eligible_to_run == true) {
          // if the process is complete
          if (temp_jobs_arr[i].cpu_time == 0) {
            printf("%d %d finished\n", clock_time, temp_jobs_arr[i].pid);
            turn_around_time = turn_around_time + clock_time;
            total_finished_jobs++;

            if (total_finished_jobs == 1) { response = clock_time; }
            // then terminate run eligibility
            temp_jobs_arr[i].eligible_to_run = false;
          } else {
            // otherwise, run the process
            printf("%d %d running\n", clock_time, temp_jobs_arr[i].pid);
            if (cpu_times[i] == temp_jobs_arr[i].cpu_time) {
              wait_time = wait_time + clock_time;
              wait_time = wait_time - temp_jobs_arr[i].arrival_time;
            }
            temp_jobs_arr[i].cpu_time = temp_jobs_arr[i].cpu_time - 1;
            quantum_counter++;
            // if the job is not finished but quantum is up
            if (temp_jobs_arr[i].cpu_time != 0 && quantum_counter == quantum) {
              for (int j = 0; j < total_jobs - 1; j++) {
                struct process temp = temp_jobs_arr[j+1];
                temp_jobs_arr[j+1] = temp_jobs_arr[j];
                temp_jobs_arr[j] = temp;
              }
              quantum_counter = 0;
            }
            idle = false;
            break;
          }
        } else { idle = true; }
      }
      // interrogate completed jobs
      if (total_finished_jobs == total_jobs) { break; }
      // assess idle time
      if (idle) {
        printf("%d idle\n", clock_time);
        idle_time++;
      }
      // increment clock_time
      clock_time++;
    } 
  } else {
    printf("...Please enter a valid process\n");
    printf("usage: ./scheduler [FCFS | RR] [optional quantum]\n");
    exit(EXIT_FAILURE);
  }

  // @calc data
  double final_wait_time = (double)wait_time / (double)total_jobs;
  int response_time = response - first_job_arrival_time;
  turn_around_time = turn_around_time / (double)total_jobs;
  double cpu_usage = ((double)idle_time / (double)clock_time) * 100;
  cpu_usage = 100 - cpu_usage;
  // @display data
  printf("Average wait time: %.2f\n", final_wait_time);
  printf("Average response time: %d\n", response_time);
  printf("Average turnaround time: %.2f\n", turn_around_time);
  printf("Average CPU usage: %.2f%%\n", cpu_usage);

  exit(EXIT_SUCCESS);
}
