/*
 * fs_struct.h
 *
 *  Created on: Jun 7, 2021
 *      Author: m3jc
 */

#ifndef THIRD_PARTY_ZCFS_INCLUDE_FS_STRUCT_H_
#define THIRD_PARTY_ZCFS_INCLUDE_FS_STRUCT_H_

#endif /* THIRD_PARTY_ZCFS_INCLUDE_FS_STRUCT_H_ */

#include "fs_util.h"


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
	uint32_t id;		/* file identifier    */
	char *file_buffer;	/* buffer of the file */
} bfile_t;

// TODO change from list to another struct (es. tree, ...)
typedef struct __attribute__((__packed__)) buffer{
	bfile_t list[_INODE_LIST_LIMIT];	/* list of all files */
	uint32_t size;						/* size of the files */
} main_buffer_t;


/*
 * Memory location
 */

uint32_t superblock_pointer_t = _UP_MEMORY_LIMIT - size(superblock_t);

