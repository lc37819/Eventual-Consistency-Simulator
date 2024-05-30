/*
project: Eventual Consistency
author: Elton Gbollie
email: lc37819@umbc.edu
student id: lc37819
description: a program that simulates Eventual Consistency
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include "ParentProcess.h"
#include "LikesServer.h"

int main(void) {

    parent_process();

    return 0;
}

void parent_process(void) {
    pid_t pids[NUM_CHILDREN];
    int log_fd = open(LOG_FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (log_fd < 0) {
        perror("Could not open log file");
        exit(1);
    }

    printf("ParentProcess Started\n");
    dprintf(log_fd, "ParentProcess started at %s\n", get_timestamp());

    for (int i = 0; i < NUM_CHILDREN; i++) {
        fork_child(i, log_fd, pids);
        sleep(SLEEP_TIME);
    }

    wait_for_children(log_fd, pids);

    dprintf(log_fd, "ParentProcess ended at %s\n", get_timestamp());
    printf("ParentProcess Ended\n");

    close(log_fd);
}

void fork_child(int index, int log_fd, pid_t pids[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        char child_name[CHILD_NAME_SIZE];
        snprintf(child_name, sizeof(child_name), "LikesServer%d", index);
        likes_server(child_name);
        exit(0);
    } else {
        pids[index] = pid;
        log_child_start(index, log_fd);
    }
}

void wait_for_children(int log_fd, pid_t pids[]) {
    int status;
    int remaining_children = NUM_CHILDREN;

    while (remaining_children > 0) {
        pid_t done = wait(&status);
        if (done == -1) {
            perror("wait() failed");
            exit(1);
        } else {
            int child_index = -1;
            for (int i = 0; i < NUM_CHILDREN; i++) {
                if (pids[i] == done) {
                    child_index = i;
                    break;
                }
            }

            if (child_index != -1) {
                log_child_end(child_index, log_fd);
            } else {
                dprintf(log_fd, "Error: Unknown child PID %d ended at %s\n", done, get_timestamp());
            }

            remaining_children--;
        }
    }
}

const char* get_timestamp(void) {
    static char timestamp[100];
    time_t now = time(NULL);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %X", localtime(&now));
    return timestamp;
}

void log_child_start(int index, int log_fd) {
    dprintf(log_fd, "LikesServer%d Started at %s\n", index, get_timestamp());
}

void log_child_end(int index, int log_fd) {
    dprintf(log_fd, "LikesServer%d ended at %s\n", index, get_timestamp());
}
