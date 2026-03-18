#include "todo_api_4.h"

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
