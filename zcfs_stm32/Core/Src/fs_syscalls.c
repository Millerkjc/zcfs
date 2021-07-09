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

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2


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

int _open(char* file_name)
{

	int fd = find_file(file_name);

	// File not exists
	if (fd == -1){
		/*
		 * Create file
		 * - create inode
		 * - insert into the superblock
		 * - insert inode into pending_list (open files)
		 */
		create_file(file_name);
		// TODO redefine fd -> generate fd
		// TODO call _open((uint32_t)fd, 3); // open file in write/read
								   	   	     // returns 1 success/0 fail
	}

	/*
	 * if existing file was closed
	 * TODO MUST read from disk
	 */
//	if (!superblock.inode_list[fd]->is_open){
//		superblock.inode_list[fd]->is_open = 1;

		/*
		 * reinsert into pending_list (open files)
		 */
		// Insert Inode
		// Insert last data TODO - until next_dinode = NULL
//	}

	return (uint32_t)fd;


}

int _close(int file)
{
	return -1;
}
