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

// Functions
void fs_init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx);
uint32_t fs_open(uint32_t fd, int flags, ...);
HAL_StatusTypeDef fs_write(uint32_t fd, char* ptr, uint32_t len);
//HAL_StatusTypeDef fs_read(int fd, char* ptr, int len);


// Variables
UART_HandleTypeDef *gHuart;
main_buffer_t* mbuf;
DMA_HandleTypeDef *ghdma_usart2_tx;
DMA_HandleTypeDef *ghdma_usart2_rx;


/*
 * Superblock
 */
/* Singleton Superblock */
ifile_t* superblock_t[_INODE_LIST_LIMIT];


#endif /* INCLUDE_FS_CORE_H_ */
