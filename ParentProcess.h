#ifndef PARENTPROCESS_H
#define PARENTPROCESS_H

#include <sys/types.h>

// Macro definitions
#define NUM_CHILDREN 10
#define SLEEP_TIME 1
#define CHILD_NAME_SIZE 15
#define LOG_FILE_PATH "/tmp/ParantProcessStatus"

// Prototypes
void parent_process(void);
void fork_child(int index, int log_fd, pid_t pids[]);
void wait_for_children(int log_fd, pid_t pids[]);
const char* get_timestamp(void);
void log_child_start(int index, int log_fd);
void log_child_end(int index, int log_fd);

#endif
