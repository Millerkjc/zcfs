/*
 * fs_core.h
 *
 *  Created on: Jun 17, 2021
 *      Author: m3jc
 */


#include <stdio.h>
#include <errno.h>
#include "fs_buffer.h"

#define write _write

#ifndef INCLUDE_FS_CORE_H_
#define INCLUDE_FS_CORE_H_

void fs_init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef fs_write(int fd, char* ptr, int len);
//HAL_StatusTypeDef fs_read(int fd, char* ptr, int len);

UART_HandleTypeDef *gHuart;
main_buffer_t* mbuf;

#endif /* INCLUDE_FS_CORE_H_ */
