/*
 * fs_util.h
 *
 *  Created on: Jun 7, 2021
 *      Author: m3jc
 */

#include <stdlib.h>
#include "stm32f4xx_hal.h"


#ifndef THIRD_PARTY_ZCFS_INCLUDE_FS_UTIL_H_
#define THIRD_PARTY_ZCFS_INCLUDE_FS_UTIL_H_

/*
 * Struct size
 */
#define _INODE_LIST_LIMIT ((uint32_t)1024)

// 4/16/32 Kbyte - To be parameterized
#define _BUFFER_SIZE_256B  ((uint16_t)0x100)
//#define _BUFFER_SIZE_512B  ((uint16_t)0x200)
//#define _BUFFER_SIZE_1K ((uint16_t)0x400)
//#define _BUFFER_SIZE_2K ((uint16_t)0x800)


//typedef struct mutex {
//	int Lock;
//} mutex_t;


typedef struct list_item {
    void *data;
    void *next;
} list_item_t;

typedef struct linked_list {
    list_item_t *head;
    list_item_t *tail;
    int size;
} linked_list_t;

void linked_list_init(linked_list_t *list);
void linked_list_append(linked_list_t *list, void *item);
void linked_list_add(linked_list_t *list, int idx, void *data);
void* linked_list_get(linked_list_t *list, int idx);
void linked_list_replace(linked_list_t *list, int idx, void *data);
void linked_list_remove(linked_list_t *list, int idx);
void linked_list_clear(linked_list_t *list);
void linked_list_print(linked_list_t *list);


#endif /* THIRD_PARTY_ZCFS_INCLUDE_FS_UTIL_H_ */
