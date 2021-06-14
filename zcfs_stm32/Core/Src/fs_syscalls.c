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
#include <../Inc/fs_syscalls.h>
#include "fs_util.h"
#include <stdint.h>
#include <stdio.h>

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2


UART_HandleTypeDef *gHuart;

void RetargetInit(UART_HandleTypeDef *huart) {
  gHuart = huart;

  /* Disable I/O buffering for STDOUT stream, so that
   * chars are sent out as soon as they are printed. */
  setvbuf(stdout, NULL, _IONBF, 0);
}


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



void flash_write(uint32_t address, char* data, uint32_t data_len){
    HAL_FLASH_Unlock();

//    FLASH_Erase_Sector(FLASH_SECTOR_4,VOLTAGE_RANGE_1);

//    FLASH_Erase_Sector(FLASH_SECTOR_6, FLASH_VOLTAGE_RANGE_1);
//    FLASH_Erase_Sector(FLASH_SECTOR_7,VOLTAGE_RANGE_1);
//    FLASH_Erase_Sector(FLASH_SECTOR_8,VOLTAGE_RANGE_1);
//    FLASH_Erase_Sector(FLASH_SECTOR_9,VOLTAGE_RANGE_1);

	for(int DataIdx = 0; DataIdx < data_len; DataIdx++)
	{
//		__io_putchar(*ptr++);
//		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *data++);

		// CORRECT
		//		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address++, *data++);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address++, *data);
	}
    HAL_FLASH_Lock();
}


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
	  char *data = 0x0;
	  flash_write( /*ADDRESS*/ _DOWN_MEMORY_LIMIT, data, _UP_MEMORY_LIMIT - _DOWN_MEMORY_LIMIT);
  }

  errno = EBADF;
  return -1;
}
