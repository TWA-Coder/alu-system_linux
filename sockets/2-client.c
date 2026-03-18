#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/**
 * main - connects to a listening server
 * @argc: number of command-line arguments
 * @argv: array of command-line arguments
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if (argc != 3)
	{
		printf("Usage: %s <host> <port>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	server = gethostbyname(argv[1]);
	if (server == NULL)
	{
		fprintf(stderr, "No such host\n");
		return (EXIT_FAILURE);
	}
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons((unsigned short)atoi(argv[2]));
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("connect");
		return (EXIT_FAILURE);
	}
	printf("Connected to %s:%d\n", argv[1], atoi(argv[2]));
	close(sockfd);
	return (EXIT_SUCCESS);
}
