/*
 * buffer.c
 *
 *  Created on: Jun 9, 2021
 *      Author: m3jc
 */

#include "fs_buffer.h"
#include <string.h>

/*
 * TODO
 * All methods for buffering data, etc.
 */

#define _BUFFER_SIZE _BUFFER_SIZE_16K
#define _FBUFFER_INIT_SIZE 12

/*
 * fbuffer initialization
 * id	: id of the file
 * bfill: how many byte are in the buffer
 * size : the current size of the buffer
 */
void fbuffer_init(file_buffer_t* fbuf, uint32_t id){
	fbuf->id = id;
	fbuf->bfill = 0;
	fbuf->size = _FBUFFER_INIT_SIZE;
	fbuf->file_buffer = malloc(sizeof(char)*_FBUFFER_INIT_SIZE);
	memset(fbuf->file_buffer, 0, sizeof(char)*_FBUFFER_INIT_SIZE);
}


/*
 * fbuffer insertion
 * fbuf: the specific file buffer
 * data: the data that will be inserted
 *
 * Inserimento di dati nel buffer di uno specifico file
 */
void fbuffer_insert(file_buffer_t* fbuf, char *data){
	if(fbuf->bfill + sizeof(data) >= fbuf->size){
		/*
		 * Resize "size" and realloc memory
		 */
		fbuf->size = fbuf->size + sizeof(data) + _FBUFFER_INIT_SIZE;
		fbuf->file_buffer = (char *)realloc(fbuf->file_buffer, fbuf->size);
		memset(fbuf->file_buffer+fbuf->bfill, 0, sizeof(data) + _FBUFFER_INIT_SIZE);
	}
	/*
	 * Insert data and adjust "bfill"
	 */
	strcpy(fbuf->file_buffer+fbuf->bfill, data);
	fbuf->bfill += sizeof(data);
}

/*
 * fbuffer flush
 * fbuf: the specific file buffer
 *
 * the data will be write in memory and the buffer cleared
 */
void fbuffer_flush(file_buffer_t* fbuf){
	// TODO write in mem

	free(&fbuf->file_buffer);
	fbuf->bfill = 0;
	fbuf->size = 0;
}


/*
 * mbuffer initialization
 * mbuf: the main buffer
 */
void buffer_init(main_buffer_t* mbuf){
	mbuf->size = 0;
	mbuf->files = 0;
	memset(mbuf->list, 0, _INODE_LIST_LIMIT*sizeof(file_buffer_t*));
}

/*
 * mbuffer initialization
 * mbuf: the main buffer
 */
void buffer_flush(main_buffer_t* mbuf){
	// TODO write in mem

	for(int b=0; b < mbuf->files; b++){
		fbuffer_flush(mbuf->list[b]);
		free(mbuf->list[b]);
	}
}

/*
 * mbuffer initialization
 * mbuf : the main buffer
 * id	: id of the file
 * data	: the data that will be inserted
 */
void buffer_insert(main_buffer_t* mbuf, uint32_t id, char *data){
	if(mbuf->size + sizeof(data) >= _BUFFER_SIZE){
		// Flush buffer
		// TODO Flush buffer + write on mem
		buffer_flush(mbuf);
		buffer_init(mbuf);
	}

	/*
	 * 	looking for fbuf into the list
	 */
	if (! mbuf->list[id]){ // or if mbuf->files <= id
		mbuf->files += 1;
		mbuf->list[id] = malloc(sizeof(struct buffer_file));
		fbuffer_init(mbuf->list[id], id);
	}

	/*
	 * Insert the data in the buffer of the specific file
	 */
	fbuffer_insert(mbuf->list[id], data);
	mbuf->size += sizeof(data);
}


