/*
 * fs_mem.h
 *
 *  Created on: Jun 11, 2021
 *      Author: m3jc
 */

#ifndef INCLUDE_FS_MEM_H_
#define INCLUDE_FS_MEM_H_

/*
 * Memory location
 */
//uint32_t superblock_pointer_t = ((uint32_t)_UP_MEMORY_LIMIT) - (sizeof(superblock_t)/sizeof(ifile_t*));
uint32_t superblock_pointer_t = SUPERBLOCK_ADDRESS((sizeof(superblock_t)/sizeof(ifile_t*)));

#endif /* INCLUDE_FS_MEM_H_ */
