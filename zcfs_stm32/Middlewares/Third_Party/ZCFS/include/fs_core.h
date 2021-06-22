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

#define write _write

// Functions
void fs_init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx);
HAL_StatusTypeDef fs_write(int fd, char* ptr, int len);
//HAL_StatusTypeDef fs_read(int fd, char* ptr, int len);
HAL_StatusTypeDef flash_write(uint32_t address, uint32_t data, uint32_t data_len);

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
