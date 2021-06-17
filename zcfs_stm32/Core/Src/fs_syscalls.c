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

//void* __attribute((__section__(".user_data"))) user_data;
//extern int __user_data_start__, __user_data_end__;
//#define _VZCFS_DISK_START (uint32_t)&__user_data_start__
//#define _VZCFS_DISK_SIZE 524288
//uint32_t write_ptr = _VZCFS_DISK_START;


#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2


//__attribute__((weak)) int _read(int file, char *ptr, int len)
//{
//	int DataIdx;
//
//	for (DataIdx = 0; DataIdx < len; DataIdx++)
//	{
//		*ptr++ = __io_getchar();
//	}
//
//return len;
//}




//void inode_write(char* data, uint32_t data_len);
//void data_write(char* data, uint32_t data_len);



int _write(int fd, char* ptr, int len) {
  HAL_StatusTypeDef hstatus;

  if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
    hstatus = HAL_UART_Transmit(gHuart, (uint8_t *) ptr, len, HAL_MAX_DELAY);
    if (hstatus == HAL_OK)
      return len;
    else
      return EIO;
  }

  if (fd != STDOUT_FILENO && fd != STDERR_FILENO && fd != STDIN_FILENO){
	  return fs_write(fd, ptr, len);
  }

  errno = EBADF;
  return -1;
}
