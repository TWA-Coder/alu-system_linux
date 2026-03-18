#include "todo_api_5.h"

todo_t *todos_head = NULL;
size_t todos_count = 0;

/**
 * create_todo - Creates a new todo and adds it to the list
 * @title: Todo title
 * @description: Todo description
 * Return: pointer to created todo
 */
todo_t *create_todo(const char *title, const char *description)
{
	todo_t *t = malloc(sizeof(todo_t));
	todo_t *cur;

	if (!t)
		return (NULL);

	t->id = todos_count++;
	t->title = strdup(title);
	t->description = strdup(description);
	t->next = NULL;

	if (!todos_head)
	{
		todos_head = t;
	}
	else
	{
		cur = todos_head;
		while (cur->next)
			cur = cur->next;
		cur->next = t;
	}
	return (t);
}

/**
 * serialize_todos - Convert todo list to JSON array
 * Return: A newly allocated string containing the JSON
 */
char *serialize_todos(void)
{
	todo_t *cur = todos_head;
	char *json = malloc(8192);
	char temp[512];

	if (!json)
		return (NULL);
	strcpy(json, "[");
	while (cur)
	{
		sprintf(temp, "{\"id\":%lu,\"title\":\"%s\",\"description\":\"%s\"}",
			(unsigned long)cur->id, cur->title, cur->description);
		strcat(json, temp);
		if (cur->next)
			strcat(json, ",");
		cur = cur->next;
	}
	strcat(json, "]");
	return (json);
}
