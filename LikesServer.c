#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "LikesServer.h"

void likes_server(const char *server_name) {
    int likes;
    char log_filename[50];
    snprintf(log_filename, sizeof(log_filename), "/tmp/%s", server_name);

    int likes_log_fd = open(log_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (likes_log_fd < 0) {
        perror("Could not open like server log file");
        exit(EXIT_FAILURE);
    }

    printf("%s: Started\n", server_name);

    sleep(CHILD_LIFE_TIME);

    srand(time(NULL) ^ (getpid() << 16));

    likes = rand() % 43;

    dprintf(likes_log_fd, "%s %d %d\n", server_name, likes, 0);

    close(likes_log_fd);

    send_likes_to_server(server_name, likes);

    printf("%s: Ended\n", server_name);

    exit(0);
}

void send_likes_to_server(const char *server_name, int likes){
    int notify_interval = (rand() % 5) + 1;
    sleep(notify_interval);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;

    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    char message[50];
    snprintf(message, sizeof(message), "%s %d", server_name, likes);

    if (send(sock, message, strlen(message), 0) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    printf("%s: Sent %d likes to PrimaryLikesServer\n", server_name, likes);
    fflush(stdout);

    char buffer[1024] = {0};
    if (read(sock, buffer, 1024) < 0) {
        perror("Read failed");
        exit(EXIT_FAILURE);
    }

    if (strcmp(buffer, "ACK") == 0) {
        likes = 0;
    } else {
        printf("%s: Did not receive acknowledgment. Likes counter did not reset.\n", server_name);
    }

    close(sock);
}
