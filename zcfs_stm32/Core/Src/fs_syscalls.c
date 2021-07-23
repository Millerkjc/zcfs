/*
 * fs_syscalls.c
 *
 *  Created on: Jun 14, 2021
 *      Author: m3jc
 */


#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include "fs_syscalls.h"


int _myinit(UART_HandleTypeDef* huart, DMA_HandleTypeDef* hdma_usart_tx, DMA_HandleTypeDef* hdma_usart_rx){
	fs_init(huart, hdma_usart_tx, hdma_usart_rx);

	return 1;
}


uint32_t _read(uint32_t fd, char* data, int start, int stop)
{
	return fs_read(fd, data, start, stop);
}

uint32_t _write(int fd, char* ptr, int len) {
	return fs_write(fd, ptr, len);
}

uint32_t _open(char* file_name)
{
	return fs_open(file_name);
}

int _close(uint32_t fd)
{
	return (int)fs_close(fd);
}
