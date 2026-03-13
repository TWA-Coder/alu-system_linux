#ifndef LIST_H
#define LIST_H

#include <stddef.h>

/**
 * struct node_s - singly linked list node
 * @content: Content
 * @next: Next node
 * @prev: Previous node
 */
typedef struct node_s
{
	void *content;
	struct node_s *next;
	struct node_s *prev;
} node_t;

/**
 * struct list_s - singly linked list
 * @head: Head of the list
 * @tail: Tail of the list
 * @size: Size of the list
 */
typedef struct list_s
{
	node_t *head;
	node_t *tail;
	size_t size;
} list_t;

typedef void (*node_func_t)(void *);

list_t *list_init(list_t *list);
void list_add(list_t *list, void *content);
void list_each(list_t *list, node_func_t func);
void list_destroy(list_t *list, node_func_t node_free);

#endif /* LIST_H */
