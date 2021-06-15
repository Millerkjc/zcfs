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
#include <stdint.h>
#include <stdio.h>


#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

void* __attribute((__section__(".user_data"))) user_data;
extern int __user_data_start__, __user_data_end__;

#define _VZCFS_DISK_START (uint32_t)&__user_data_start__
#define _VZCFS_DISK_SIZE 524288

uint32_t write_ptr = _VZCFS_DISK_START;

UART_HandleTypeDef *gHuart;
main_buffer_t* mbuf;

void RetargetInit(UART_HandleTypeDef *huart, main_buffer_t* buf) {
  gHuart = huart;
  mbuf = buf;

  /* Disable I/O buffering for STDOUT stream, so that
   * chars are sent out as soon as they are printed. */
  setvbuf(stdout, NULL, _IONBF, 0);

  // TODO Uncomment when released
//  uint32_t address = (uint32_t)_VZCFS_DISK_START;
//  for(int DataIdx = 0; DataIdx < (uint32_t)_VZCFS_DISK_SIZE; DataIdx++)
//  {
//  	HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address++, 0x0);
//  }
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



HAL_StatusTypeDef flash_write(uint32_t address, char* data, uint32_t data_len){
    HAL_FLASH_Unlock();

//    FLASH_Erase_Sector(FLASH_SECTOR_4,VOLTAGE_RANGE_1);

//    uint32_t x = (uint32_t)&__user_data_start__;
//    uint32_t y = (uint32_t)_VZCFS_DISK_START + (uint32_t)_VZCFS_DISK_SIZE;



    // TODO WRITE WITH DMA?





//    FLASH_Erase_Sector(FLASH_SECTOR_6, FLASH_VOLTAGE_RANGE_1);
//    FLASH_Erase_Sector(FLASH_SECTOR_7,VOLTAGE_RANGE_1);
//    FLASH_Erase_Sector(FLASH_SECTOR_8,VOLTAGE_RANGE_1);
//    FLASH_Erase_Sector(FLASH_SECTOR_9,VOLTAGE_RANGE_1);

//	for(int DataIdx = 0; DataIdx < data_len; DataIdx++)
//	{
////		__io_putchar(*ptr++);
////		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *data++);
//
//		// CORRECT
//		//		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address++, *data++);
//		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address++, *data);
//	}
    HAL_FLASH_Lock();

    return HAL_OK;
}

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

//		  char *data = 0x0;
	  //char *test = *_End_section;
	  //flash_write( /*ADDRESS*/ *_Start_section, data, _End_section - _Start_section);

	  // TODO ADD
	  hstatus = buffer_insert(mbuf, fd, ptr, len);

	  hstatus = HAL_OK;

	  if (hstatus == HAL_OK)
		  return len;
	  else
		  return EIO;
  }

  errno = EBADF;
  return -1;
}
