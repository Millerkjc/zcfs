/*
 * fs_core.c
 *
 *  Created on: Jun 17, 2021
 *      Author: m3jc
 */

#include "fs_core.h"
#include "stm32f407xx.h"

void* __attribute((__section__(".user_data"))) user_data;
extern int __user_data_start__, __user_data_end__;
#define _VZCFS_DISK_START (uint32_t)&__user_data_start__
#define _VZCFS_DISK_SIZE 524288
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
	char* s = "ciao mammaaaaa";
	char* s2 = "hello mammaaaaa";


//	 uint32_t SectorError;
//	 FLASH_EraseInitTypeDef EraseInitStruct;
//	 EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
//	 EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
//	 EraseInitStruct.Sector = FLASH_SECTOR_6; //Specify sector number
//	 EraseInitStruct.NbSectors = 1; //This is also important!
//	 if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
//		 //Erase error!
//			 sprintf(s, "bbbb");
//	 }
//
	 FLASH_Erase_Sector(FLASH_SECTOR_6,VOLTAGE_RANGE_4);
	 FLASH_Erase_Sector(FLASH_SECTOR_7,VOLTAGE_RANGE_4);
	 FLASH_Erase_Sector(FLASH_SECTOR_8,VOLTAGE_RANGE_4);
	 FLASH_Erase_Sector(FLASH_SECTOR_9,VOLTAGE_RANGE_1);
	 FLASH_Erase_Sector(FLASH_SECTOR_10,VOLTAGE_RANGE_1);
	 FLASH_Erase_Sector(FLASH_SECTOR_11,VOLTAGE_RANGE_1);
//
//
//	 EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
//	 EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
//	 EraseInitStruct.Sector = FLASH_SECTOR_7; //Specify sector number
//	 EraseInitStruct.NbSectors = 1; //This is also important!
//	 if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
//		 //Erase error!
//			 sprintf(s, "bbbb");
//	 }




	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR| FLASH_FLAG_PGSERR);

	flash_write(superblock_pointer_t, (uint32_t)s, strlen(s));
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR| FLASH_FLAG_PGSERR);

	flash_write(0x0805fffc, (uint32_t)s, strlen(s));
//	flash_write(0x0805fffc, (uint32_t)s2, strlen(s2));
//	flash_write(0x0805fffc, (uint32_t)s, strlen(s));

	flash_write(0x08060000, (uint32_t)s, strlen(s));
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR| FLASH_FLAG_PGSERR);

	flash_write(0x08041000, (uint32_t)s, strlen(s));

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR| FLASH_FLAG_PGSERR);

//	char msg[30] = "hello";


//	TODO Uncomment when released?
//	uint32_t address = (uint32_t)_VZCFS_DISK_START;
//	for(int DataIdx = 0; DataIdx < (uint32_t)_VZCFS_DISK_SIZE; DataIdx++)
//	{
//	    HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address++, 0xF);
//	}

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
