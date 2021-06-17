/*
 * fs_core.h
 *
 *  Created on: Jun 17, 2021
 *      Author: m3jc
 */


#include <stdio.h>
#include <errno.h>
#include "fs_buffer.h"

//void* __attribute((__section__(".user_data"))) user_data;
//extern int __user_data_start__, __user_data_end__;
//#define _VZCFS_DISK_START (uint32_t)&__user_data_start__
//#define _VZCFS_DISK_SIZE 524288
//uint32_t write_ptr = _VZCFS_DISK_START;


#ifndef INCLUDE_FS_CORE_H_
#define INCLUDE_FS_CORE_H_

#define write _write

// Functions
void fs_init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef fs_write(int fd, char* ptr, int len);
//HAL_StatusTypeDef fs_read(int fd, char* ptr, int len);

// Variables
UART_HandleTypeDef *gHuart;
main_buffer_t* mbuf;

#endif /* INCLUDE_FS_CORE_H_ */
