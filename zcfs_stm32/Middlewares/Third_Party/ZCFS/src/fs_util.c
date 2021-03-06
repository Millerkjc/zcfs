/*
 * fs_util.c
 *
 *  Created on: Jun 9, 2021
 *      Author: m3jc
 */

#include "fs_util.h"
#include <stdio.h>
#include <string.h>

/*
 * TODO: Linked list code
 */
void linked_list_init(linked_list_t *list){
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

void linked_list_append(linked_list_t *list, void* item){
	list_item_t *new_item = (list_item_t *)malloc(sizeof(list_item_t));

	if(list->tail == NULL){
		list->head = new_item;
		list->tail = new_item;
	}else{
		list->tail->next = new_item;
		list->tail = list->tail->next;
	}

	// TODO TEST
	new_item->next = NULL;
	new_item->data = item;

}

void *linked_list_get(linked_list_t *list, int idx){

	list_item_t *current = list->head;

	for(int i = 0; i < idx; i++)
		current = current->next;

	return current->data;
}

void linked_list_add(linked_list_t *list, int idx, void *data){

	list_item_t *prev = list->head;

	for(int i = 0; i < idx - 1; i++)
		prev = prev->next;

	list_item_t *new = (list_item_t *)malloc(sizeof(list_item_t));
	new->data = data;
	new->next = prev->next;
	prev->next = new;

}

void linked_list_replace(linked_list_t *list, int idx, void *data){

	list_item_t *current = list->head;

	for(int i = 0; i < idx; i++)
		current = current->next;

	current->data = data;
}

void linked_list_remove(linked_list_t *list, int idx){

	list_item_t *prev = list->head;

	for(int i = 0; i < idx - 1; i++)
		prev = prev->next;

	list_item_t *target = prev->next;
	prev->next = target->next;
	free(target);
}

void linked_list_clear(linked_list_t *list){

	list_item_t *current = list->head;
	list_item_t *next = current;

	while(current != NULL){
		next = current->next;
		free(current);
		current = next;
	}

	linked_list_init(list);
}




void linked_list_print(linked_list_t *list){
	list_item_t *new_item = list->head;
	char all[50];
	memset(all, 0, 50);

	while(new_item != NULL){
		char s[10];
		memset(s, 0, 10);
		sprintf(s, "%ld", (uint32_t)new_item->data);
		strcat(all, s);
		new_item = new_item->next;
	}
}








