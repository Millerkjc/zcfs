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

//// DMA CALLBACK
//void DMATransferComplete(DMA_HandleTypeDef *hdma){
//  if(hdma->Instance == DMA1_Stream6){
//	  // Disable UART DMA mode
//	  gHuart->Instance->CR3 &= ~USART_CR3_DMAT;
//	  // Turn Red Led On
//	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
//	  HAL_Delay(500);
//  }
//}

void RetargetInit(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx, DMA_HandleTypeDef *hdma_memtomem){
  gHuart = huart;
  ghdma_usart2_tx = hdma_usart_tx;
  ghdma_usart2_rx = hdma_usart_rx;
  ghdma_memtomem_dma2_stream0 = hdma_memtomem;


  /* Disable I/O buffering for STDOUT stream, so that
   * chars are sent out as soon as they are printed. */
  setvbuf(stdout, NULL, _IONBF, 0);
}


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




//void fs_init(UART_HandleTypeDef *huart){
void fs_init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx, DMA_HandleTypeDef *hdma_memtomem){

	RetargetInit(huart, hdma_usart_tx, hdma_usart_rx, hdma_memtomem);
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
