#include <stdlib.h>
#include <string.h>
#include "../../include/dstructs/llist.h"

void llist_init(llist *list, 
				void (*data_destructor)(void *data), 
				bool (*comparator)(const void *key1, const void *key2))
{
	list->size = 0;
	list->comparator = comparator;
	list->data_destructor = data_destructor;
	list->head = NULL;
	list->tail = NULL;
}

int llist_rem_next(llist *list, llist_node *node, void **data)
{
	llist_node *old_node;

	if (llist_size(list) == 0) return -1;

	if (!node) {
		// removal from the head

		*data = list->head->data;
		old_node = list->head;
		list->head = list->head->next;

		if (llist_size(list) == 1) {
			list->tail = NULL;
		}
	}
	else {
		if (!node->next) return -1;

		*data = node->next->data;
		old_node = node->next;
		node->next = node->next->next;

		if (!node->next) {
			list->tail = node;
		}
	}

	free(old_node);
	list->size--;

	return 0;
}

void llist_destroy(llist *list)
{
	void *data;

	while (llist_size(list) > 0) {
		if (llist_rem_next(list, NULL, &data) == 0 && list->data_destructor) {
			list->data_destructor(data);
		}
	}
	memset(list, 0, sizeof(llist));
}

int llist_ins_next(llist *list, llist_node *node, const void *data)
{
	llist_node *new_node;
	if (!(new_node = malloc(sizeof(llist_node)))) return -1;

	new_node->data = data;

	if (!node) {
		// insertion in the head

		if (llist_size(list) == 0) {
			list->tail = node;
		}
		new_node->next = list->head;
		list->head = new_node;
	}
	else {
		if (!node->next) {
			list->tail = new_node;
		}
		new_node->next = node->next;
		node->next = new_node;
	}
	list->size++;
	return 0;
}
