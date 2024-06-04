## Eventual Consistency Simulator
This projects simulates Eventual Consistency.

Eventual consistency is a consistency model used in distributed computing to achieve high availability that informally guarantees that, if no
new updates are made to a given data item, eventually all accesses to that item will return the last updated value.

## Build and Run
1. Start the server:
    ```code
    gcc PrimaryLikesServer.c -o server

    ./server
2. Important: Open up a new terminal window and navigate to the project directory.

3. Start the ParentProcess (create 10 child LikesServers):
    ```code
    make

    ./main
4. Open a new Terminal and navigate to the /tmp directory to view the log files

## parent_process()
Opens the /tmp/ParentProcessStatus log file with write access, creating it if it does not exist and truncating it to zero length if it does.
Logs the start of the parent process, then enters a loop to create 10 child LikesServer using fork_child() and waits one second before creating 
the next one. (This is also printed out to stdout for testing/debugging purposes) After child creation, it waits for all child processes to finish using
wait_for_children(). Lastly, logs the parent process completion and closes the log file.

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

## likes_server()
Creates a log file /tmp/LikesServer to store the number of likes accumulated by the server. Each child process (LikesServer) live for five minutes i.e
300 seconds. During those five minutes the child process generates a random number of likes between 0 and 42 and send those likes to the PrimaryLikesServer
every 1 to 5 seconds. The child name and number of likes are written to a log. Example /tmp/LikesServer0 in this format “LikesServer0 14”. The file is closed
and the send_likes_to_server function is called.

## send_likes_to_server
Takes the server_name and number of likes as parameters. Creates a client socket. checks the socket creation, prepares the message, sends the message to the
PrimaryLikesServer, displays the number of likes sent to the PrimaryLikesServer to stdout for testing/debuging, if a response is sent from the PrimaryLikesServer
the likes counter is reset and the client socket is closed.

## PrimaryLikesServer
Creates and configue a TCP socket, accepts connections and reads data from each individual LikesServer. Process the received data using the validate_data helper function, and logs this data to the "/tmp/PrimaryLikesLog" file. Lastly an acknowledgment message "ACK" back to the client. If the send operation fails, an error is displayed, and the server exits. Then the log file and the server socket are closed before exiting the program.

## validate_data
Checks for invalid data and does not accept it. This is used to verify if the data that is being passed from each LikesServer to the PrimaryLikesServer is valid, before it is written to the log file.