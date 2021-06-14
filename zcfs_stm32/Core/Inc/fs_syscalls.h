/*
 * fs_syscalls.h
 *
 *  Created on: Jun 14, 2021
 *      Author: m3jc
 */

#ifndef INC_FS_SYSCALLS_H_
#define INC_FS_SYSCALLS_H_

#include "stm32f4xx_hal.h"
#include <sys/stat.h>

int _isatty(int fd);
int _write(int fd, char* ptr, int len);
int _close(int fd);
int _lseek(int fd, int ptr, int dir);
int _read(int fd, char* ptr, int len);
int _fstat(int fd, struct stat* st);

#endif /* INC_FS_SYSCALLS_H_ */
