#ifndef TODO_API_H
#define TODO_API_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * struct todo - Represents a todo item
 * @id: positive integer identifier
 * @title: title of the todo
 * @description: description of the todo
 * @next: pointer to next todo item in list
 */
typedef struct todo
{
	size_t id;
	char *title;
	char *description;
	struct todo *next;
} todo_t;

extern todo_t *todos_head;
extern size_t todos_count;

int setup_server(int port);
void handle_client(int client_fd, const char *client_ip);
char *get_header(const char *buffer, const char *header_name);
int parse_body_params(char *body, char *title, char *desc);
void send_response(int fd, int code, const char *msg,
		   const char *json, const char *ip,
		   const char *meth, const char *path);
todo_t *create_todo(const char *title, const char *description);
char *serialize_todos(void);
void route_post_todos(int client_fd, const char *client_ip, const char *buffer);
void route_get_todos(int client_fd, const char *client_ip);

#endif /* TODO_API_H */
