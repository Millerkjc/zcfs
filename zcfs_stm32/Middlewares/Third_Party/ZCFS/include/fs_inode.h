/*
 * fs_inode.h
 *
 *  Created on: Jun 22, 2021
 *      Author: m3jc
 */

#include "fs_util.h"

#ifndef INCLUDE_FS_INODE_H_
#define INCLUDE_FS_INODE_H_

/*
 * File nodes and data inodes
 */

#define FNAME_LENGTH 16

/*
 * Data inode
 */
//typedef struct __attribute__((__packed__)) inode_file_data{
typedef struct inode_file_data{
	uint32_t data_ptr;	  // ptr to the file data
	uint32_t data_len;	  // length of the pointed data
	uint32_t next_dinode; // next inode
} idfile_t;

/*
 * File inode
 */
/* block per file: 4 + 16 + 4 + 4 + 1 + 4 = 33 byte */
typedef struct __attribute__((__packed__)) inode_file{
	uint32_t fd;			 // file descriptor
	char name[FNAME_LENGTH]; // 15 + '\0'

	uint32_t time;			 // time of the last edit
	uint32_t size;			 // size of the file
	uint8_t is_open:1;	     // file opened = 1 bit

	idfile_t* last_dinode;   // last dinode written for the specific file
	idfile_t* next_dinode;   // next(first) inode written for the specific file
} ifile_t;





/*
 * open(f1) -> Create INODE(pending list) -> point to INODE data(NULL) -> WRITE INODE
 * write(1) -> Create&Write INODE data -> Update INODE pointer to next data
 *
 */

#endif /* INCLUDE_FS_INODE_H_ */
