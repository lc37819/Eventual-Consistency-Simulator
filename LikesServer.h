#ifndef LIKESSERVER_H
#define LIKESSERVER_H

// Macro definitions
#define CHILD_LIFE_TIME 300
#define PORT 54321

// Prototypes
void likes_server(const char *server_name);
void send_likes_to_server(const char *server_name, int likes);

#endif
