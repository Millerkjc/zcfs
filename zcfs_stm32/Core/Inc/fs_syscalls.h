/*
 * fs_syscalls.h
 *
 *  Created on: Jun 14, 2021
 *      Author: m3jc
 */


#include <sys/stat.h>
#include "stm32f4xx_hal.h"
#include "fs_core.h"

#ifndef INC_FS_SYSCALLS_H_
#define INC_FS_SYSCALLS_H_

#define zcfs_write _write
#define zcfs_read _read
#define zcfs_open _open
#define zcfs_close _close
#define zcfs_init _myinit

int _myinit(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx);
int _isatty(int fd);
int _write(int fd, char* ptr, int len);
int _open(char* file_name);
int _close(int fd);
int _lseek(int fd, int ptr, int dir);
int _read(int fd, char* ptr, int len);
int _fstat(int fd, struct stat* st);

#endif /* INC_FS_SYSCALLS_H_ */
