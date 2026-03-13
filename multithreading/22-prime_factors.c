#include "multithreading.h"
#include <stdlib.h>

/**
 * create_task - creates a task
 * @entry: entry function
 * @param: parameter
 *
 * Return: pointer to created task
 */
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task = calloc(1, sizeof(task_t));

	if (!task)
		return (NULL);

	task->entry = entry;
	task->param = param;
	task->status = PENDING;
	task->result = NULL;
	pthread_mutex_init(&task->lock, NULL);

	return (task);
}

/**
 * destroy_task - destroys a task
 * @task: pointer to the task to destroy
 */
void destroy_task(task_t *task)
{
	if (task)
	{
		if (task->result)
		{
			list_destroy((list_t *)task->result, free);
			free(task->result);
		}
		pthread_mutex_destroy(&task->lock);
		free(task);
	}
}

/**
 * exec_tasks - executes a list of tasks
 * @tasks: list of tasks to be executed
 *
 * Return: NULL
 */
void *exec_tasks(list_t const *tasks)
{
	node_t *node;
	task_t *task;
	int count = 0;

	if (!tasks)
		return (NULL);

	for (node = tasks->head; node; node = node->next, count++)
	{
		task = (task_t *)node->content;
		pthread_mutex_lock(&task->lock);
		if (task->status == PENDING)
		{
			task->status = STARTED;
			tprintf("[%02d] Started\n", count);
			pthread_mutex_unlock(&task->lock);

			task->result = task->entry(task->param);

			pthread_mutex_lock(&task->lock);
			if (task->result)
			{
				task->status = SUCCESS;
				tprintf("[%02d] Success\n", count);
			}
			else
			{
				task->status = FAILURE;
				tprintf("[%02d] Failure\n", count);
			}
		}
		pthread_mutex_unlock(&task->lock);
	}
	return (NULL);
}
