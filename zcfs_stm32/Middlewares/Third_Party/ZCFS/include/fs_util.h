/*
 * fs_util.h
 *
 *  Created on: Jun 7, 2021
 *      Author: m3jc
 */

#ifndef THIRD_PARTY_ZCFS_INCLUDE_FS_UTIL_H_
#define THIRD_PARTY_ZCFS_INCLUDE_FS_UTIL_H_

#endif /* THIRD_PARTY_ZCFS_INCLUDE_FS_UTIL_H_ */

/*
 * Struct size
 */
#define _INODE_LIST_LIMIT 1000

// 4/16/32 Kbyte - To be parameterized
#define _BUFFER_SIZE_4K  4000
#define _BUFFER_SIZE_16K 16000
#define _BUFFER_SIZE_32K 32000

/*
 * Memory location and limits
 *
 * External Device [0xA0000000 - 0xDFFFFFFF]
 * External RAM	   [0x9FFFFFFF - 0x60000000]
 */
#define _UP_MEMORY_LIMIT 0x9FFFFFFF
#define _DOWN_MEMORY_LIMIT 0x60000000

