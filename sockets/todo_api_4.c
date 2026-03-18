#include "todo_api_4.h"

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
