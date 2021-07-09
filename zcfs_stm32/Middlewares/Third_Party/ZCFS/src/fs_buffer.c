/*
 * buffer.c
 *
 *  Created on: Jun 9, 2021
 *      Author: m3jc
 */

#include <string.h>
#include "fs_buffer.h"
#include "fs_core.h"


/*
 * TODO
 * All methods for buffering data, etc.
 */


/*
 * fbuffer reset
 * fbuf: the specific file buffer
 */
void fbuffer_reset(file_buffer_t* fbuf){
	fbuf->bfill = 0;
	fbuf->size = _FBUFFER_INIT_SIZE;
	fbuf->file_buffer = malloc(sizeof(char)*_FBUFFER_INIT_SIZE);
	memset(fbuf->file_buffer, 0, sizeof(char)*_FBUFFER_INIT_SIZE);
}

/*
 * fbuffer initialization
 * id	: id of the file
 * bfill: how many byte are in the buffer
 * size : the current size of the buffer
 */
file_buffer_t* fbuffer_init(file_buffer_t* fbuf, uint32_t fd){
	fbuf = malloc(sizeof(struct buffer_file));
	fbuf->fd = fd;
	fbuffer_reset(fbuf);
	return fbuf;
}


/*
 * fbuffer insertion
 * fbuf: the specific file buffer
 * data: the data that will be inserted
 *
 * Inserimento di dati nel buffer di uno specifico file
 */
HAL_StatusTypeDef fbuffer_insert(file_buffer_t* fbuf, char *data, uint32_t len){
	if(fbuf->bfill + len >= fbuf->size){
		/*
		 * Resize "size" and realloc memory
		 */
		fbuf->size = fbuf->size + len + _FBUFFER_INIT_SIZE;
		fbuf->file_buffer = (char *)realloc(fbuf->file_buffer, fbuf->size);
		memset(fbuf->file_buffer+fbuf->bfill, 0, len + _FBUFFER_INIT_SIZE);
	}
	/*
	 * Insert data and adjust "bfill"
	 */
	strcpy(fbuf->file_buffer+fbuf->bfill, data);
	fbuf->bfill += len;

	return HAL_OK;
}

/*
 * fbuffer flush
 * fbuf: the specific file buffer
 *
 * the data will be write in memory and the buffer cleared
 */
void fbuffer_flush(file_buffer_t* fbuf){

	if(fbuf){
		// TODO write in mem
		// TODO dinode_write

		char* end = "\0";

		strcat(fbuf->file_buffer, end);
		dinode_write(fbuf->fd, fbuf->file_buffer, fbuf->bfill + 1);

		free(fbuf->file_buffer);
//		fbuf->file_buffer = NULL;
//		fbuffer_reset(fbuf);
//		free(fbuf);
//		fbuf = NULL;
	}
}



/*
 * buffer reset
 * mbuf: the main buffer
 */
void buffer_reset(main_buffer_t* mbuf){
	mbuf->size = 0;
	mbuf->files = 0;
	memset(mbuf->list, 0, _INODE_LIST_LIMIT*sizeof(file_buffer_t*));
}


/*
 * mbuffer initialization
 * mbuf: the main buffer
 */
main_buffer_t* buffer_init(main_buffer_t* mbuf){
	mbuf = malloc(sizeof(main_buffer_t));
	buffer_reset(mbuf);
	return mbuf;
}

/*
 * mbuffer flush
 * mbuf: the main buffer
 */
void buffer_flush(main_buffer_t* mbuf){
	for(int b=2; b < _INODE_LIST_LIMIT; b++){
		fbuffer_flush(mbuf->list[b]);
		if(mbuf->list[b]){
			free((uint32_t *)mbuf->list[b]);
			mbuf->list[b] = NULL;
		}
	}

	mbuf->files=0;
	mbuf->size=0;
}

/*
 * mbuffer insertion
 * mbuf : the main buffer
 * id	: id of the file
 * data	: the data that will be inserted
 */
HAL_StatusTypeDef buffer_insert(main_buffer_t* mbuf, uint32_t fd, char *data, uint32_t len){
	if(mbuf->size + len >= _BUFFER_SIZE){
		// Flush buffer
		buffer_flush(mbuf);
	}

	/*
	 * 	looking for fbuf into the list
	 */
	if (! mbuf->list[fd]){ // or if mbuf->files <= id
		mbuf->files += 1;
		mbuf->list[fd] = fbuffer_init(mbuf->list[fd], fd);
	}

	/*
	 * Insert the data in the buffer of the specific file
	 */
	fbuffer_insert(mbuf->list[fd], data, len);
	mbuf->size += len;

	return HAL_OK;
}


pending_buffer_t* pending_buffer_init(){
	pending_buffer_t* pbuffi = (pending_buffer_t *)malloc(sizeof(pending_buffer_t));
//	pbuffi->last_dinode = (linked_list_t *)malloc(sizeof(linked_list_t));
	//	linked_list_init(pbuffi->last_dinode);

	pbuffi->list_new_fd = (linked_list_t *)malloc(sizeof(linked_list_t));
	linked_list_init(pbuffi->list_new_fd);

	return pbuffi;
}

HAL_StatusTypeDef pending_dinode_insert(ifile_t* list_new_inode, uint32_t* last_inode_data){
	/*
	 * se c'è fd e last_dinode NULL -> aggiungere next_dinode all'ifile (INODE)
	 */

	linked_list_append(pbuffi->list_new_fd, &list_new_inode->id);
//	linked_list_append(pbuffi->last_dinode, &last_inode_data);

	return HAL_OK;
}

/*
 * return -1 quando file non è stato trovato
 */
uint32_t pending_fd_find(uint32_t fd){
	return linked_list_find_fd(pbuffi->list_new_fd, fd);
}

///*
// * return NULL quando file non è stato trovato
// */
//uint32_t* pending_dinode_get(uint32_t idx){
//	if(idx != -1){
//		return (uint32_t *)linked_list_get(pbuffi->last_dinode, idx);
//	}
//
//	return NULL;
//}


//uint32_t* pending_dinode_get(uint32_t idx){
//	uint32_t idx = linked_list_find_fd(pbuffi->list_new_fd, fd);
//
//	if(idx != -1){
//		return (uint32_t *)linked_list_get(pbuffi->last_dinode, idx);
//	}
//
//	return NULL;
//}













int linked_list_find_fd(linked_list_t *list, int fd){
	list_item_t *current = list->head;
	uint32_t pos = 0;

	while(current != NULL && *(uint32_t*)current->data != fd){
		pos+=1;
		current = current->next;
	}

	if(current == NULL){
		pos = -1;
	}

	return pos;
}





