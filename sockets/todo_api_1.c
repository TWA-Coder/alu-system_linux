#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * parse_query - Parses and prints query parameters
 * @query: The query string (after '?')
 */
void parse_query(char *query)
{
	char *key, *val, *pair;

	if (!query)
		return;
	pair = strtok(query, "&");
	while (pair != NULL)
	{
		key = pair;
		val = strchr(pair, '=');
		if (val != NULL)
		{
			*val = '\0';
			val++;
			printf("Query: \"%s\" -> \"%s\"\n", key, val);
		}
		pair = strtok(NULL, "&");
	}
}

/**
 * handle_client - Receives and parses HTTP request, sends 200 OK
 * @client_fd: the connected client socket
 */
void handle_client(int client_fd)
{
	char buffer[4096], path[1024];
	char *query;
	ssize_t bytes_read;
	const char *resp = "HTTP/1.1 200 OK\r\n\r\n";

	memset(buffer, 0, sizeof(buffer));
	bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_read > 0)
	{
		printf("Raw request: \"%s\"\n", buffer);
		if (sscanf(buffer, "%*s %1023s", path) == 1)
		{
			query = strchr(path, '?');
			if (query)
			{
				*query = '\0';
				query++;
			}
			printf("Path: %s\n", path);
			parse_query(query);
		}
	}
	send(client_fd, resp, strlen(resp), 0);
	close(client_fd);
}

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
 * main - Opens an IPv4/TCP socket, listens, and serves HTTP 200 responses
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
		printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));
		handle_client(client_fd);
	}
	close(sockfd);
	return (EXIT_SUCCESS);
}
