#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * main - Opens an IPv4/TCP socket, and listens to traffic on port 12345
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	int sockfd;
	struct sockaddr_in addr;

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
		close(sockfd);
		return (EXIT_FAILURE);
	}

	if (listen(sockfd, 10) == -1)
	{
		perror("listen");
		close(sockfd);
		return (EXIT_FAILURE);
	}

	printf("Server listening on port 12345\n");

	while (1)
	{
		sleep(1);
	}

	close(sockfd);
	return (EXIT_SUCCESS);
}
