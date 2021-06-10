/*
 * buffer.c
 *
 *  Created on: Jun 9, 2021
 *      Author: m3jc
 */

#include "fs_struct.h"

/*
 * TODO
 * All methods for buffering data, etc.
 */

#define _BUFFER_SIZE _BUFFER_SIZE_16K

/*
 * Alloca un buffer di X per il file con file-descriptor "id"
 * Dove la X iniziale è 256 bytes
 * TODO malloc
 */
void fbuffer_init(file_buffer_t* fbuf, uint32_t id){
	// TODO
}


/*
 * Inserimento di dati nel buffer di uno specifico file
 * TODO eventuale realloc
 */
void fbuffer_insert(file_buffer_t* fbuf, uint32_t id, char *data){
	if(fbuf->bfill + sizeof(data) >= fbuf->size){
		// TODO
		// Realloc
	}
	// insert data
}


/*
 * Main buffer initialization
 */
void buffer_init(main_buffer_t* mbuf){
	mbuf->size = 0;
}

void buffer_insert(main_buffer_t* mbuf, uint32_t id, char *data){
	if(mbuf->size + sizeof(data) >= _BUFFER_SIZE){
		// Flush buffer
		// TODO Flush buffer + write on mem
		mbuf->size = 0;
	}

	/*
	 * 	looking for fbuf into the list
	 */
	file_buffer_t* fbuf = mbuf->list[id]; //TODO
	fbuffer_insert(fbuf, id, data);
	mbuf->size += sizeof(data);
}


