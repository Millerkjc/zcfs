/*
 * fs_core.c
 *
 *  Created on: Jun 17, 2021
 *      Author: m3jc
 */

#include "fs_core.h"


void RetargetInit(UART_HandleTypeDef *huart) {
  gHuart = huart;

  /* Disable I/O buffering for STDOUT stream, so that
   * chars are sent out as soon as they are printed. */
  setvbuf(stdout, NULL, _IONBF, 0);
}


void fs_init(UART_HandleTypeDef *huart){
	RetargetInit(huart);
	mbuf = buffer_init(mbuf);

//	TODO Uncomment when released
//	uint32_t address = (uint32_t)_VZCFS_DISK_START;
//	for(int DataIdx = 0; DataIdx < (uint32_t)_VZCFS_DISK_SIZE; DataIdx++)
//	{
//	    HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address++, 0x0);
//	}

}


HAL_StatusTypeDef fs_write(int fd, char* ptr, int len){

	HAL_StatusTypeDef hstatus = buffer_insert(mbuf, fd, ptr, len);

	if (hstatus != HAL_OK)
	  return EIO;

	return len;

}
