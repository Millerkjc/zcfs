/*
 * fs_struct.h
 *
 *  Created on: Jun 7, 2021
 *      Author: m3jc
 */

#ifndef THIRD_PARTY_ZCFS_INCLUDE_FS_STRUCT_H_
#define THIRD_PARTY_ZCFS_INCLUDE_FS_STRUCT_H_

#endif /* THIRD_PARTY_ZCFS_INCLUDE_FS_STRUCT_H_ */

#define _INODE_LIST_LIMIT 1000
#define _BUFFER_SIZE 32000 // 4/16/32 Kbyte - To be parameterized

/*
 * Superblock and file inodes
 */

/* Singleton Superblock */
uint32_t superblock_t[_INODE_LIST_LIMIT];

/* block per file: 4 + 16 + 4 + 4 + 4 = 32 byte */
typedef struct __attribute__((__packed__)) inode_file{
	uint32_t id;			/* file identifier 			 */
	char name[16]; 		 	/* 15 + '\0' 				 */
	uint32_t *inode_ptr;	/* pointer to the inode file */
	uint32_t time;			/* time of the last edit     */
	uint32_t size;			/* size of the file 	 	 */
} ifile_t;


/*
 * Buffers
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
 * TODO
 * All methods for buffering data
 */

