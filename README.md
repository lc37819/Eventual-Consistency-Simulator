## Eventual Consistency Simulator
## Overview: 
This projects simulates a consistency model called Eventual Consistency.

## parent_process()
Opens the /tmp/ParentProcessStatus log file with write access, creating it if it does not exist and truncating it to zero length if it does.
Logs the start of the parent process, then enters a loop to create 10 child LikesServer using fork_child() and waits one second before creating 
the next one. After child creation, it waits for all child processes to finish using wait_for_children(). Lastly, logs the parent process completion
and closes the log file.

## likes_server()
Creates a log file /tmp/LikesServer to store the number of likes accumulated by the server. Each child process (LikesServer) live for five minutes.
In that time each child process generates a random number of likes between 0 and 42 using the rand() function. Once the five minutes limit expire the
number of likes are written to the log file

## fork_child()
Uses the fork() system call to create a new process. The child process executes likes_server() and then exit. The parent process logs the start of the
child process and stores the child's PID in the pids array.

## wait_for_children()
Waits for all child processes to finish using the wait() system call. Logs the end of each child process. If a child's PID is not recognized,
it logs an error message.

## get_timestamp
Returns the current time

## log_child_start()
Logs the start of each child process to the /tmp/ParentProcessStatus log file, including a timestamp.

## log_child_end()
Logs the end of each child process to the /tmp/ParentProcessStatus log file, including a timestamp.