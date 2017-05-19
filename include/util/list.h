#pragma once

#include <stddef.h>

typedef struct list_node {
	struct list_node *next;
	struct list_node *prev;
	void *data;
} list_node_t;

typedef struct {
	list_node_t *head;
	list_node_t *tail;
	size_t length;
} list_t;

typedef int (*list_compare_t)(const list_node_t *left, const list_node_t *right);

void list_append(list_t *list, list_node_t *node);
void list_insert_after(list_t *list, list_node_t *after, list_node_t *node);
void list_remove(list_t *list, list_node_t *node);
