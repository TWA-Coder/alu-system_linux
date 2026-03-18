#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * setup_server - Creates, binds and listens on a socket
 *
 * Return: The server socket file descriptor, or -1 on failure
 */
int setup_server(void)
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
	addr.sin_port = htons(12345);
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
 * main - Opens an IPv4/TCP socket, listens, accepts connection, and receives msg
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	int sockfd, client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	char buffer[1024];
	ssize_t bytes_read;

	setbuf(stdout, NULL);
	sockfd = setup_server();
	if (sockfd == -1)
		return (EXIT_FAILURE);
	printf("Server listening on port 12345\n");
	client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd == -1)
	{
		perror("accept");
		close(sockfd);
		return (EXIT_FAILURE);
	}
	printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));
	memset(buffer, 0, sizeof(buffer));
	bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_read > 0)
		printf("Message received: \"%s\"\n", buffer);
	close(client_fd);
	close(sockfd);
	return (EXIT_SUCCESS);
}
