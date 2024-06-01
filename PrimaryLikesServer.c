#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdbool.h>

#define PORT 54321
#define BACKLOG 10
#define BUFFER_SIZE 1024
#define LOG_FILE_PATH "/tmp/PrimaryLikesLog"

bool validate_data(const char *buffer, int *likes);

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *ack = "ACK";
    int total_likes = 0;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    FILE *log_file = fopen(LOG_FILE_PATH, "w");
    if (log_file == NULL) {
        perror("Failed to open log file");
    }

    printf("PrimaryLikesServer started, waiting for a connection...\n");

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        memset(buffer, 0, BUFFER_SIZE);
        valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread < 0) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }

        int likes;
        if (!validate_data(buffer, &likes)) {
            fprintf(log_file, "Invalid data received: %s\n", buffer);
            fflush(log_file);
            close(new_socket);
            continue;
        }

        total_likes += likes;
        fprintf(log_file, "%s\nTotal %d\n", buffer, total_likes);
        fflush(log_file);

        if (send(new_socket, ack, strlen(ack), 0) != strlen(ack)) {
            perror("send failed");
            exit(EXIT_FAILURE);
        }

        close(new_socket);
    }

    fclose(log_file);
    close(server_fd);

    return 0;
}

bool validate_data(const char *buffer, int *likes) {
    char server_name[15];
    int scan_count = sscanf(buffer, "%s %d", server_name, likes);

    if (scan_count != 2 || strncmp(server_name, "LikesServer", 11) != 0 || *likes < 0 || *likes > 42) {
        return false;
    }

    return true;
}