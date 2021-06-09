/*
 * fs_util.h
 *
 *  Created on: Jun 7, 2021
 *      Author: m3jc
 */

#ifndef THIRD_PARTY_ZCFS_INCLUDE_FS_UTIL_H_
#define THIRD_PARTY_ZCFS_INCLUDE_FS_UTIL_H_

#endif /* THIRD_PARTY_ZCFS_INCLUDE_FS_UTIL_H_ */

/*
 * Struct size
 */
#define _INODE_LIST_LIMIT 1024

// 4/16/32 Kbyte - To be parameterized
#define _BUFFER_SIZE_4K  0x1000 // 4094
#define _BUFFER_SIZE_16K 0x4000 // 16384
#define _BUFFER_SIZE_32K 0x8000 // 32768

/*
 * Memory location and limits
 *
 * External Device [0xA0000000 - 0xDFFFFFFF]
 * External RAM	   [0x9FFFFFFF - 0x60000000]
 */
#define _UP_MEMORY_LIMIT 0x9FFFFFFF
#define _DOWN_MEMORY_LIMIT 0x60000000

typedef struct list_item {
    void *data;
    struct list_item *next;
} list_item_t;

typedef struct linked_list {
    list_item_t *head;
    int size;
} linked_list_t;

void linked_list_init(linked_list_t *list);
void linked_list_append(linked_list_t *list, void *item);
void linked_list_add(linked_list_t *list, int idx, void *data);
void* linked_list_get(linked_list_t *list, int idx);
void linked_list_replace(linked_list_t *list, int idx, void *data);
void linked_list_remove(linked_list_t *list, int idx);
void linked_list_clear(linked_list_t *list);










