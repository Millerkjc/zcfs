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

#define HEADER_SIZE 9
#define EOL_SIZE 4
#define CONSOLE_PKT 0
#define CONSOLE_PKT_HEADER "zcfs_cns_"
#define WRITE_PKT 1
#define WRITE_PKT_HEADER "zcfs_wrt_"
#define READ_PKT 2
#define READ_PKT_HEADER "zcfs_rdx_"
#define ERROR_PKT_HEADER "zcfs_err_"
#define EOL_PKT "_end"

// FS public functions
void fs_init(UART_HandleTypeDef* huart, DMA_HandleTypeDef* hdma_usart_tx, DMA_HandleTypeDef* hdma_usart_rx);
uint32_t fs_open(char* file_name);
HAL_StatusTypeDef fs_close(uint32_t fd);
uint32_t fs_read(uint32_t fd, char *data, int start, int stop);
uint32_t fs_write(uint32_t fd, char* ptr, uint32_t len);
uint32_t fs_error(char* error_msg);

/*
 * Dinode function (used in fs_buffer)
 */
HAL_StatusTypeDef dinode_write(uint32_t fd, char* data, uint32_t data_len);


// Variables
UART_HandleTypeDef *gHuart;
main_buffer_t* mbuf;
pending_buffer_t* pbuffi;
DMA_HandleTypeDef *ghdma_usart2_tx;
DMA_HandleTypeDef *ghdma_usart2_rx;
mutex_t *glock;
uint32_t start_time;


/*
 * Superblock
 */
/* Singleton Superblock */
typedef struct __attribute__((__packed__)) block{
	uint32_t ptr_data_address;   // last written address for data
	uint32_t ptr_dinode_address; // last written address for inode
	uint32_t next_fd;			 // next fd to write
	ifile_t inode_list[_INODE_LIST_LIMIT];
}superblock_t;
superblock_t superblock;


#endif /* INCLUDE_FS_CORE_H_ */
