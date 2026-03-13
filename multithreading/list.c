#include "list.h"
#include <stdlib.h>

/**
 * list_init - initializes a list
 * @list: list to initialize
 *
 * Return: pointer to the initialized list
 */
list_t *list_init(list_t *list)
{
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}

/**
 * list_add - adds a node to the end of a list
 * @list: pointer to the list
 * @content: content to add to the new node
 */
void list_add(list_t *list, void *content)
{
	node_t *new_node;

	if (!list)
		return;
	new_node = calloc(1, sizeof(node_t));
	if (!new_node)
		return;
	new_node->content = content;
	if (!list->head)
	{
		list->head = new_node;
		list->tail = new_node;
	}
	else
	{
		new_node->prev = list->tail;
		list->tail->next = new_node;
		list->tail = new_node;
	}
	list->size++;
}

/**
 * list_each - executes a function on each node of a list
 * @list: pointer to the list
 * @func: function to execute
 */
void list_each(list_t *list, node_func_t func)
{
	node_t *node;

	if (!list || !func)
		return;
	for (node = list->head; node; node = node->next)
		func(node->content);
}

/**
 * list_destroy - destroys a list
 * @list: pointer to the list
 * @node_free: function to free the content of a node
 */
void list_destroy(list_t *list, node_func_t node_free)
{
	node_t *node, *next;

	if (!list)
		return;
	node = list->head;
	while (node)
	{
		next = node->next;
		if (node_free && node->content)
			node_free(node->content);
		free(node);
		node = next;
	}
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}
