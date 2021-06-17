/*
 * fs_buffer.h
 *
 *  Created on: Jun 11, 2021
 *      Author: m3jc
 */

#include "fs_util.h"


#ifndef INCLUDE_FS_BUFFER_H_
#define INCLUDE_FS_BUFFER_H_

/*
 * Superblock and file inodes
 */
/* block per file: 4 + 16 + 4 + 4 + 4 = 32 byte */
typedef struct __attribute__((__packed__)) inode_file{
	uint32_t id;			/* file identifier 		 */
	char name[16]; 		 	/* 15 + '\0' 			 */
	char* inode_ptr;    	/* list of chunks        */  // LINKED LIST
	uint32_t time;			/* time of the last edit */
	uint32_t size;			/* size of the file 	 */
} ifile_t;

/* Singleton Superblock */
ifile_t* superblock_t[_INODE_LIST_LIMIT];

/*
 * Buffers: main buffer and buffer of the files
 */
typedef struct __attribute__((__packed__)) buffer_file{
	uint32_t id;		/* file identifier    		*/
	char* file_buffer;	/* buffer of the file 		*/
	uint32_t bfill;		/* buffer filled 			*/
	uint32_t size;		/* size of the file buffer  */
} file_buffer_t;

// TODO change from list to another struct (es. tree, ...)
typedef struct __attribute__((__packed__)) buffer{
	file_buffer_t* list[_INODE_LIST_LIMIT];	/* list of all files */
	uint32_t files;							/* number of files	 */
	uint32_t size;							/* size of the files */
} main_buffer_t;


// main buffer
void buffer_reset(main_buffer_t* mbuf);
main_buffer_t* buffer_init(main_buffer_t* mbuf);
void buffer_flush();
HAL_StatusTypeDef buffer_insert(main_buffer_t* mbuf, uint32_t id, char *data, uint32_t len);
// file buffers
void fbuffer_reset(file_buffer_t* fbuf);
file_buffer_t* fbuffer_init(file_buffer_t* fbuf, uint32_t id);
HAL_StatusTypeDef fbuffer_insert(file_buffer_t* fbuf, char *data, uint32_t len);
void fbuffer_flush();


#endif /* INCLUDE_FS_BUFFER_H_ */
