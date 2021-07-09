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

// Functions
void fs_init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx);
uint32_t fs_open(char* file_name);
uint32_t fs_write(uint32_t fd, char* ptr, uint32_t len);
uint32_t fs_error(char* error_msg);
//HAL_StatusTypeDef fs_read(int fd, char* ptr, int len);




uint32_t get_inode_addr_to_wrt(uint32_t);
uint32_t get_dinode_addr_to_wrt();

void get_header(char *, uint32_t);
HAL_StatusTypeDef virtual_flash_write(uint32_t* address, uint32_t data, uint32_t data_len);
HAL_StatusTypeDef virtual_flash_read(uint32_t* address, uint32_t* data, uint32_t data_len);

uint32_t create_packet(char*, uint32_t, uint32_t*, uint32_t, uint32_t);
uint32_t create_packet_read(char* pkt, uint32_t* address_buffer, uint32_t data_len);

HAL_StatusTypeDef inode_write(uint32_t id, char* name, uint32_t time, uint32_t size, uint8_t is_open, idfile_t* last_dinode, idfile_t* next_dinode);
HAL_StatusTypeDef dinode_write(uint32_t fd, char* data, uint32_t data_len);
HAL_StatusTypeDef data_write(char* data, uint32_t data_len);

//ifile_t* inode_read(uint32_t fd);
void dinode_read(uint32_t* address, uint32_t* dinode);
void data_read(uint32_t* address, char* data, uint32_t data_len);









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
	uint32_t ptr_data_address;   // last written address for data
	uint32_t ptr_dinode_address; // last written address for inode
	uint32_t next_fd;			 // next fd to write
	ifile_t inode_list[_INODE_LIST_LIMIT];
}superblock_t;
superblock_t superblock;


#endif /* INCLUDE_FS_CORE_H_ */
