#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct llist_node_ {
	void 				*data;
	struct llist_node_  *next;
} llist_node;

typedef struct llist_ {
	llist_node *head;
	llist_node *tail;

	int size;
	bool (*comparator)(const void *key1, const void *key2);
	void (*data_destructor)(void *data);
} llist;

void llist_init(llist *list, 
				void (*data_destructor)(void *data),
				bool (*comparator)(const void *key1, const void *key2));
void llist_destroy(llist *list);
int llist_ins_next(llist *list, llist_node *node, const void *data);
int llist_rem_next(llist *list, llist_node *node, void **data);

#define llist_size(list) ((list)->size)
#define llist_head(list) ((list)->head)
#define llist_tail(list) ((list)->tail)
#define llist_is_head(list, node) (((node) == (list->head)) ? true : false)
#define llist_is_tail(node) ((node)->next == NULL? true : false)
#define llist_data(node) ((node)->data)
#define llist_next(node) ((node)->next)