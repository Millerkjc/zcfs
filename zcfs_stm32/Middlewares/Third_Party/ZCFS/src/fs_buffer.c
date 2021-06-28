/*
 * buffer.c
 *
 *  Created on: Jun 9, 2021
 *      Author: m3jc
 */

#include <string.h>
#include "fs_buffer.h"


/*
 * TODO
 * All methods for buffering data, etc.
 */

#define _BUFFER_SIZE _BUFFER_SIZE_4K
#define _FBUFFER_INIT_SIZE 12

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
file_buffer_t* fbuffer_init(file_buffer_t* fbuf, uint32_t id){
	fbuf = malloc(sizeof(struct buffer_file));
	fbuf->id = id;
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
	// TODO write in mem
	// TODO inode_write

	free(&fbuf->file_buffer);
	fbuf->file_buffer = NULL;
	fbuffer_reset(fbuf);
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
	for(int b=0; b < mbuf->files; b++){
		fbuffer_flush(mbuf->list[b]);
		free(mbuf->list[b]);
	}
	buffer_reset(mbuf);
}

/*
 * mbuffer insertion
 * mbuf : the main buffer
 * id	: id of the file
 * data	: the data that will be inserted
 */
HAL_StatusTypeDef buffer_insert(main_buffer_t* mbuf, uint32_t id, char *data, uint32_t len){
	if(mbuf->size + len >= _BUFFER_SIZE){
		// Flush buffer
		// TODO Flush buffer + write on mem
		buffer_flush(mbuf);
	}

	/*
	 * 	looking for fbuf into the list
	 */
	if (! mbuf->list[id]){ // or if mbuf->files <= id
		mbuf->files += 1;
		mbuf->list[id] = fbuffer_init(mbuf->list[id], id);
	}

	/*
	 * Insert the data in the buffer of the specific file
	 */
	fbuffer_insert(mbuf->list[id], data, len);
	mbuf->size += len;

	return HAL_OK;
}

HAL_StatusTypeDef pending_inode_insert(uint32_t fd){
	return HAL_OK;
}



