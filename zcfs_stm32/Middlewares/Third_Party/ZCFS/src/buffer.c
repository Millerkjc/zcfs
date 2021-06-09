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
void fbuffer_alloc(uint32_t id){
	bfile_t b;
}


/*
 * Inserimento di dati nel buffer di uno specifico file
 * TODO eventuale realloc
 */
void fbuffer_insert(uint32_t id, char *data){
	if(main_buffer_t.size + size(data) >= _BUFFER_SIZE){
		// Flush buffer
		main_buffer_t.size = 0;
	}
}


/*
 * Main buffer initialization
 */
void buffer_init(){
	main_buffer_t.size = 0;
}

void buffer_insert(uint32_t id, char *data){
	if(main_buffer_t.size + size(data) >= _BUFFER_SIZE){
		// Flush buffer
		main_buffer_t.size = 0;
	}

	fbuffer_insert(id, data);
	main_buffer_t.size += size(data);
}



