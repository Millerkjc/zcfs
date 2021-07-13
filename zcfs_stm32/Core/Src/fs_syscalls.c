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

uint32_t find_file(char* file_name);
uint32_t create_file(char* file_name);


uint32_t find_file(char* file_name){
	int ptr = 2;

	while(ptr<superblock.next_fd){
		// Lettura su disco
		//if(!strncmp(superblock.inode_list[ptr]->name, file_name, FNAME_LENGTH)){
		//	return superblock.inode_list[ptr]->id;
		//}

		// READ

		ptr++;
	}

	return -1;
}


uint32_t create_file(char* file_name){
	/*
	 *
	 */
	// TODO creazione inode => ifile_t

	//superblock.inode_list[next_fd]->id;
	//next_fd+=1;

	return -1;
}



int _myinit(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_tx, DMA_HandleTypeDef *hdma_usart_rx){
	fs_init(huart, hdma_usart_tx, hdma_usart_rx);

	return 1;
}


int _read(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
//		*ptr++ = __io_getchar();
	}

return len;
}



//void inode_write(char* data, uint32_t data_len);
//void data_write(char* data, uint32_t data_len);

int _write(int fd, char* ptr, int len) {
  return fs_write(fd, ptr, len);
}

int _open(char* file_name)
{
	return fs_open(file_name);
}

int _close(int file)
{
	return -1;
}
