#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#include "LikesServer.h"

#define CHILD_LIFE_TIME 300

void likes_server(const char *server_name) {
    int likes;
    char log_filename[50];
    snprintf(log_filename, sizeof(log_filename), "/tmp/%s", server_name);

    int likes_log_fd = open(log_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (likes_log_fd < 0) {
        perror("Could not open like server log file");
        exit(1);
    }

    srand(time(NULL));

    likes = rand() % 43;

    printf("%s: Started\n", server_name);

    sleep(CHILD_LIFE_TIME);

    dprintf(likes_log_fd, "%s %d %d\n", server_name, likes, 0);

    close(likes_log_fd);

    printf("%s: Ended\n", server_name);

    exit(0);
}