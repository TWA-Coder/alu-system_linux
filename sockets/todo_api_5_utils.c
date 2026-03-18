#include "todo_api_5.h"

/**
 * get_header - extracts header value
 * @buffer: request buffer
 * @header_name: header to find
 * Return: parsed value or NULL
 */
char *get_header(const char *buffer, const char *header_name)
{
	char search[128];
	char *pos;
	static char val[1024];
	int i = 0;

	sprintf(search, "\n%s:", header_name);
	pos = strstr(buffer, search);
	if (!pos)
		return (NULL);
	pos += strlen(search);
	while (*pos == ' ')
		pos++;
	while (pos[i] != '\r' && pos[i] != '\n' && pos[i] != '\0' && i < 1023)
	{
		val[i] = pos[i];
		i++;
	}
	val[i] = '\0';
	return (val);
}

/**
 * parse_body_params - extract body params
 * @body: body string
 * @title: out title
 * @desc: out description
 * Return: 1 if both title and desc are present, 0 otherwise
 */
int parse_body_params(char *body, char *title, char *desc)
{
	char *pair, *key, *val;
	int has_title = 0, has_desc = 0;

	if (!body)
		return (0);
	pair = strtok(body, "&");
	while (pair)
	{
		key = pair;
		val = strchr(pair, '=');
		if (val)
		{
			*val = '\0';
			val++;
			if (strcmp(key, "title") == 0)
			{
				strcpy(title, val);
				has_title = 1;
			}
			else if (strcmp(key, "description") == 0)
			{
				strcpy(desc, val);
				has_desc = 1;
			}
		}
		pair = strtok(NULL, "&");
	}
	return (has_title && has_desc);
}

/**
 * send_response - sends the HTTP response
 * @fd: socket fd
 * @code: HTTP status code
 * @msg: HTTP status message
 * @json: Optional JSON body
 * @ip: client IP
 * @meth: HTTP Method
 * @path: URI path
 */
void send_response(int fd, int code, const char *msg, const char *json,
		   const char *ip, const char *meth, const char *path)
{
	char resp[8192];

	printf("%s %s %s -> %d %s\n", ip, meth, path, code, msg);
	if (json)
		sprintf(resp, "HTTP/1.1 %d %s\r\nContent-Length: %lu\r\n"
		      "Content-Type: application/json\r\n\r\n%s",
		      code, msg, (unsigned long)strlen(json), json);
	else
		sprintf(resp, "HTTP/1.1 %d %s\r\n\r\n", code, msg);

	send(fd, resp, strlen(resp), 0);
	close(fd);
}
