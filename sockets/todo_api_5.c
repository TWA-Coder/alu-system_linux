#include "todo_api_5.h"

/**
 * setup_server - Creates, binds and listens on a socket
 * @port: Port to listen on
 *
 * Return: The server socket file descriptor, or -1 on failure
 */
int setup_server(int port)
{
	int sockfd;
	struct sockaddr_in addr;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket");
		return (-1);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("bind");
		return (-1);
	}
	if (listen(sockfd, 10) == -1)
	{
		perror("listen");
		return (-1);
	}
	return (sockfd);
}

/**
 * route_post_todos - handles POST /todos
 * @client_fd: socket fd
 * @client_ip: caller IP
 * @buffer: the raw HTTP buffer
 */
void route_post_todos(int client_fd, const char *client_ip, const char *buffer)
{
	char *body, *cl;
	char title[256] = {0}, desc[256] = {0}, json[1024];
	todo_t *t;

	cl = get_header(buffer, "Content-Length");
	if (!cl)
	{
		send_response(client_fd, 411, "Length Required", NULL, client_ip, "POST", "/todos");
		return;
	}
	body = strstr(buffer, "\r\n\r\n");
	if (body && !parse_body_params(body + 4, title, desc))
		send_response(client_fd, 422, "Unprocessable Entity", NULL, client_ip, "POST", "/todos");
	else if (body)
	{
		t = create_todo(title, desc);
		sprintf(json, "{\"id\":%lu,\"title\":\"%s\",\"description\":\"%s\"}",
			(unsigned long)t->id, t->title, t->description);
		send_response(client_fd, 201, "Created", json, client_ip, "POST", "/todos");
	}
	else
		close(client_fd);
}

/**
 * route_get_todos - handles GET /todos
 * @client_fd: socket fd
 * @client_ip: caller IP
 */
void route_get_todos(int client_fd, const char *client_ip)
{
	char *json = serialize_todos();

	if (json)
	{
		send_response(client_fd, 200, "OK", json, client_ip, "GET", "/todos");
		free(json);
	}
	else
		close(client_fd);
}

/**
 * handle_client - Receives and parses HTTP request, sends valid response
 * @client_fd: the connected client socket
 * @client_ip: caller client IP structure
 */
void handle_client(int client_fd, const char *client_ip)
{
	char buffer[4096], method[16], path[1024];

	memset(buffer, 0, sizeof(buffer));
	if (recv(client_fd, buffer, sizeof(buffer) - 1, 0) <= 0)
	{
		close(client_fd);
		return;
	}
	if (sscanf(buffer, "%15s %1023s", method, path) != 2)
	{
		close(client_fd);
		return;
	}
	if (strcmp(path, "/todos") == 0)
	{
		if (strcmp(method, "POST") == 0)
			route_post_todos(client_fd, client_ip, buffer);
		else if (strcmp(method, "GET") == 0)
			route_get_todos(client_fd, client_ip);
		else
			send_response(client_fd, 404, "Not Found", NULL, client_ip, method, path);
	}
	else
		send_response(client_fd, 404, "Not Found", NULL, client_ip, method, path);
}

/**
 * main - Opens an IPv4/TCP socket, listens, and serves REST API
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	int sockfd, client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	struct sockaddr *client_addr_ptr = (struct sockaddr *)&client_addr;

	setbuf(stdout, NULL);
	sockfd = setup_server(8080);
	if (sockfd == -1)
		return (EXIT_FAILURE);
	printf("Server listening on port 8080\n");
	while (1)
	{
		client_fd = accept(sockfd, client_addr_ptr, &client_len);
		if (client_fd == -1)
			continue;
		handle_client(client_fd, inet_ntoa(client_addr.sin_addr));
	}
	close(sockfd);
	return (EXIT_SUCCESS);
}
