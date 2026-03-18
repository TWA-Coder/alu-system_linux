#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * main - Opens an IPv4/TCP socket, listens, and accepts a connection
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	int sockfd, client_fd;
	struct sockaddr_in addr, client_addr;
	socklen_t client_len = sizeof(client_addr);

	setbuf(stdout, NULL);
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("bind");
		return (EXIT_FAILURE);
	}
	if (listen(sockfd, 10) == -1)
	{
		perror("listen");
		return (EXIT_FAILURE);
	}
	printf("Server listening on port 12345\n");
	client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd == -1)
	{
		perror("accept");
		return (EXIT_FAILURE);
	}
	printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));
	close(client_fd);
	close(sockfd);
	return (EXIT_SUCCESS);
}
