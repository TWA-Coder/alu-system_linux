#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * main - Opens an IPv4/TCP socket, listens, and serves HTTP 200 responses
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	int sockfd, client_fd;
	struct sockaddr_in addr, client_addr;
	socklen_t client_len;
	char buffer[4096], method[128], path[1024], version[128];
	ssize_t bytes_read;
	const char *response = "HTTP/1.1 200 OK\r\n\r\n";

	setbuf(stdout, NULL);
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return (EXIT_FAILURE);

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		return (EXIT_FAILURE);

	if (listen(sockfd, 10) == -1)
		return (EXIT_FAILURE);

	printf("Server listening on port 8080\n");

	while (1)
	{
		client_len = sizeof(client_addr);
		client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
		if (client_fd == -1)
			continue;

		printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));
		memset(buffer, 0, sizeof(buffer));
		bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
		if (bytes_read > 0)
		{
			printf("Raw request: \"%s\"\n", buffer);
			if (sscanf(buffer, "%127s %1023s %127s", method, path, version) == 3)
				printf("Method: %s\nPath: %s\nVersion: %s\n", method, path, version);
		}
		send(client_fd, response, strlen(response), 0);
		close(client_fd);
	}
	return (EXIT_SUCCESS);
}
