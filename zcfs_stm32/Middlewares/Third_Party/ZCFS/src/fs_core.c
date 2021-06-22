/*
 * fs_core.c
 *
 *  Created on: Jun 17, 2021
 *      Author: m3jc
 */

#include "fs_core.h"

void* __attribute((__section__(".user_data"))) user_data;
extern int __user_data_start__, __user_data_end__;
#define _VZCFS_DISK_START (uint32_t)&__user_data_start__
#define _VZCFS_DISK_SIZE 524288
uint32_t write_ptr = _VZCFS_DISK_START;
#define WORD_SIZE 4

void RetargetInit(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx){
  gHuart = huart;
  ghdma_usart2_tx = hdma_usart_tx;
  ghdma_usart2_rx = hdma_usart_rx;


  /* Disable I/O buffering for STDOUT stream, so that
   * chars are sent out as soon as they are printed. */
  setvbuf(stdout, NULL, _IONBF, 0);
}

/*
 * https://stackoverflow.com/questions/54159802/how-to-write-to-stm32-flash
 */
HAL_StatusTypeDef flash_write(uint32_t address, uint32_t data, uint32_t data_len){
    HAL_FLASH_Unlock();

    /*
     * Counts how many WORDS it has to write
     */
    uint32_t word_count = (int)data_len/WORD_SIZE;
    word_count = !data_len%WORD_SIZE ? word_count : word_count+1;

    for(int i=0; i<word_count; i++)
    	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + WORD_SIZE*i, *(uint32_t *)(data + WORD_SIZE*i));

    HAL_FLASH_Lock();

    return HAL_OK;
}



void fs_init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx){

	RetargetInit(huart, hdma_usart_tx, hdma_usart_rx);
	mbuf = buffer_init(mbuf);

//	char msg[30] = "hello";
//
//	// DMA initialization (Interrupt mode)
//	HAL_DMA_Start_IT(ghdma_usart2_tx, (uint32_t)msg, (uint32_t)gHuart->Instance->DR, strlen(msg));
//	// Enable UART in DMA mode
//	gHuart->Instance->CR3 |= USART_CR3_DMAT;




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
