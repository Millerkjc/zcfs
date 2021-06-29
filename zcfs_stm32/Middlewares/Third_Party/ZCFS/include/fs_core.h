/*
 * fs_core.h
 *
 *  Created on: Jun 17, 2021
 *      Author: m3jc
 */


#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "fs_buffer.h"
#include "fs_inode.h"

#ifndef INCLUDE_FS_CORE_H_
#define INCLUDE_FS_CORE_H_

//#define write _write
#define HEADER_SIZE 9
#define SERIAL_PKT 0
#define SERIAL_PKT_HEADER "zcfs_ser_"
#define WRITE_PKT 1
#define WRITE_PKT_HEADER "zcfs_wrt_"
#define READ_PKT 2
#define READ_PKT_HEADER "zcfs_rdx_"
#define ERROR_PKT_HEADER "zcfs_err_"

#define CHUNK_DELIMITER "@"
#define DATA_DELIMITER "#"
#define EOL_PKT "_end"

uint32_t next_fd;

// Functions
void fs_init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx);
uint32_t fs_open(uint32_t fd, int flags, ...);
HAL_StatusTypeDef fs_write(uint32_t fd, char* ptr, uint32_t len);
//HAL_StatusTypeDef fs_read(int fd, char* ptr, int len);


// Variables
UART_HandleTypeDef *gHuart;
main_buffer_t* mbuf;
pending_buffer_t* pbuffi;
DMA_HandleTypeDef *ghdma_usart2_tx;
DMA_HandleTypeDef *ghdma_usart2_rx;


/*
 * Superblock
 */
/* Singleton Superblock */
typedef struct __attribute__((__packed__)) block{
	uint32_t ptr_data_address;  // last written address for data
	uint32_t ptr_inode_address; // last written address for inode
	ifile_t* inode_list[_INODE_LIST_LIMIT];
}superblock_t;
superblock_t superblock;


#endif /* INCLUDE_FS_CORE_H_ */
