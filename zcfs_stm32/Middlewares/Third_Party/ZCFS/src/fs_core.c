/*
 * fs_core.c
 *
 *  Created on: Jun 17, 2021
 *      Author: m3jc
 */

#include "fs_core.h"
#include "stm32f407xx.h"

#define _VZCFS_DISK_START (uint32_t)0x0
#define _VZCFS_DISK_SIZE 524288 // 5 MB
uint32_t write_ptr = _VZCFS_DISK_START;
#define WORD_SIZE 4
uint32_t superblock_pointer_t = SUPERBLOCK_ADDRESS((sizeof(superblock_t)/sizeof(ifile_t*)));


/*
 * Private methods
 */

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




/*
 * Public methods
 */

void fs_init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx){

	RetargetInit(huart, hdma_usart_tx, hdma_usart_rx);
	mbuf = buffer_init(mbuf);
	// TODO write superblock in FLASH
//	flash_write(, )

//	superblock_t[0] = malloc(sizeof(ifile_t));
//	superblock_t[0]->data_ptr = NULL;
//	superblock_t[0]->id = 123;
//	strcpy(superblock_t[0]->name,"hello");
//	superblock_t[0]->next_dinode = NULL;
//	superblock_t[0]->size = 345;
//	superblock_t[0]->time = 456;
//
//	superblock_t[1] = malloc(sizeof(ifile_t));
//	superblock_t[1]->data_ptr = NULL;
//	superblock_t[1]->id = 321;
//	strcpy(superblock_t[1]->name, "hello2");
//	superblock_t[1]->next_dinode = NULL;
//	superblock_t[1]->size = 543;
//	superblock_t[1]->time = 654;
//
//	flash_write(superblock_pointer_t, (uint32_t)superblock_t[0], sizeof(ifile_t));

//


}



uint32_t fs_open(uint32_t fd, int flags, ...){

	// TODO allocate buffer, create superblock entry, inode, etc.
	return -1;
}


HAL_StatusTypeDef fs_write(uint32_t fd, char* ptr, uint32_t len){

	HAL_StatusTypeDef hstatus = buffer_insert(mbuf, fd, ptr, len);

	if (hstatus != HAL_OK)
	  return EIO;

	return len;

}
