/*
 * fs_buffer.h
 *
 *  Created on: Jun 11, 2021
 *      Author: m3jc
 */

#include "fs_util.h"
#include "fs_inode.h"


#ifndef INCLUDE_FS_BUFFER_H_
#define INCLUDE_FS_BUFFER_H_

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


/*
 * INODE STRUCT LISTS (open files)
 */
// TODO change from list to another struct (es. tree, ...)
typedef struct __attribute__((__packed__)) inode_buffer{
	linked_list_t* list_new_fd; 		// list fd - file aperti
//	linked_list_t* last_dinode;			// last dinode address on disk di uno specifico fd
} pending_buffer_t;


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
// pending inode buffer
pending_buffer_t* pending_buffer_init();
//HAL_StatusTypeDef pending_inode_insert(ifile_t* list_new_inode, idfile_t* last_inode_data);
HAL_StatusTypeDef pending_dinode_insert(ifile_t* list_new_inode, uint32_t* last_inode_data);


// To find the fd of the open file
int linked_list_find_fd(linked_list_t *list, int fd);
uint32_t pending_fd_find(uint32_t fd);
uint32_t* pending_dinode_get(uint32_t idx);



#endif /* INCLUDE_FS_BUFFER_H_ */
