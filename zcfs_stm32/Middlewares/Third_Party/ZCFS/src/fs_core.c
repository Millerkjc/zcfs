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
uint32_t address_to_write = _VZCFS_DISK_START;


/*
 * Private methods
 */

void get_header(char *header, uint32_t type){

	switch(type){
		case 0:
			strcpy(header, SERIAL_PKT_HEADER);
			break;
		case 1:
			strcpy(header, WRITE_PKT_HEADER);
			break;
		case 2:
			strcpy(header, READ_PKT_HEADER);
			break;
		default:
			strcpy(header, ERROR_PKT_HEADER);
			break;
	}
}

void RetargetInit(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx){
  gHuart = huart;
  ghdma_usart2_tx = hdma_usart_tx;
  ghdma_usart2_rx = hdma_usart_rx;
  // Enable UART in DMA mode
  gHuart->Instance->CR3 |= USART_CR3_DMAT;


  /* Disable I/O buffering for STDOUT stream, so that
   * chars are sent out as soon as they are printed. */
  setvbuf(stdout, NULL, _IONBF, 0);
}

/*
 * Packet format: HEADER @ RAW_DATA(address:data) @ INODE_INFO(address:data)
 * 		      Es: "zcfs_write_@FLASH#RAW_DATA@INODE#ADDRESS"
 */
//HAL_StatusTypeDef create_packet(uint32_t type, uint32_t inode_info, uint32_t inode_address, uint32_t data_buffer, uint32_t data_len, uint32_t address_buffer){
//	// TODO Create timer - if no full buffer
//
//	char *header, packet;
//
//	header = malloc(HEADER_SIZE);
//	get_header(header, type);
//
//	packet = malloc(HEADER_SIZE + data_len + DELIMITER_SIZE);
//
//	memcpy(packet, header, HEADER_SIZE);
//
//	memcpy(packet + sizeof(packet), CHUNK_DELIMITER, 1);
//	memcpy(packet + sizeof(packet), address_buffer, sizeof(address_buffer));
//	memcpy(packet + sizeof(packet), DATA_DELIMITER, 1);
//	memcpy(packet + sizeof(packet), data_buffer, data_len);
//
//	memcpy(packet + sizeof(packet), CHUNK_DELIMITER, 1);
//	memcpy(packet + sizeof(packet), inode_info, sizeof(inode_info));
//	memcpy(packet + sizeof(packet), DATA_DELIMITER, 1);
//	memcpy(packet + sizeof(packet), inode_address, sizeof(inode_address));
//
//
//	HAL_DMA_Start_IT(&ghdma_usart2_tx, (uint32_t)packet, (uint32_t)&gHuart.Instance->DR, strlen(msg));
//}


uint32_t create_packet(char* pkt, uint32_t type, uint32_t* address_buffer, uint32_t data_buffer, uint32_t data_len){
	char *header, *eol;
	uint32_t size_pkt = 0;

	header = malloc(HEADER_SIZE);
	get_header(header, type);

	eol = malloc(1);
	strcpy(eol, EOL_PKT);

	memcpy(pkt, header, HEADER_SIZE);
	size_pkt += HEADER_SIZE;
	memcpy(pkt + size_pkt, &address_buffer, sizeof(uint32_t));
	size_pkt += sizeof(uint32_t);
	memcpy(pkt + size_pkt, (uint32_t *)data_buffer, data_len);
	size_pkt += data_len;
	memcpy(pkt + size_pkt, eol, strlen(EOL_PKT));
	size_pkt += strlen(EOL_PKT);

	return size_pkt;
}




HAL_StatusTypeDef virtual_flash_write(uint32_t* address, uint32_t data, uint32_t data_len){
	char *pkt = malloc(HEADER_SIZE + sizeof(address) + data_len);
	uint32_t size_pkt = create_packet(pkt, WRITE_PKT, address, data, data_len);
	HAL_DMA_Start_IT(ghdma_usart2_tx, (uint32_t)pkt, (uint32_t)&gHuart->Instance->DR, size_pkt);

	address_to_write += data_len;
	superblock.ptr_data_address = address_to_write;

	// This must be here! USART writes too fast
	HAL_Delay(25);

	return HAL_OK;
}


HAL_StatusTypeDef virtual_terminal_write(uint32_t data, uint32_t data_len){
//	return create_packet(SERIAL_PKT, 0x0, data, data_len);
	return HAL_OK;
}


/*
 * Public methods
 */

void fs_init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx){

	RetargetInit(huart, hdma_usart_tx, hdma_usart_rx);
	mbuf = buffer_init(mbuf);
	pbuffi = pending_buffer_init();

	int x = 3;
	int y = 4;
	int z = 5;
	linked_list_append(pbuffi->last_inode_data, &x);
	linked_list_append(pbuffi->last_inode_data, &y);
	linked_list_append(pbuffi->last_inode_data, &z);

	/*
	 * 0 - Input
	 * 1 - Output
	 */
	next_fd = 2;

	for(int p=0; p<_INODE_LIST_LIMIT; p++){
		superblock.inode_list[p] = NULL;
	}
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
	superblock.ptr_data_address = _VZCFS_DISK_START;
	superblock.ptr_inode_address = superblock_pointer_t;


	// Only for tests
	superblock.ptr_data_address = 0x00000101;
	address_to_write = 0x00000101;

	char *s = "helooooo";
//	virtual_flash_write((uint32_t *)0x00012345, (uint32_t)s, strlen(s)+1);
//	virtual_flash_write((uint32_t *)0x00000101, (uint32_t)s, strlen(s)+1);

	virtual_flash_write((uint32_t *)address_to_write, (uint32_t)s, strlen(s)+1);
	virtual_flash_write((uint32_t *)address_to_write, (uint32_t)s, strlen(s)+1);
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
