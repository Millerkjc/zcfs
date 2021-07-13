/*
 * fs_core.c
 *
 *  Created on: Jun 17, 2021
 *      Author: m3jc
 */

#include "fs_core.h"
#include "stm32f407xx.h"
#include "fs_syscalls.h"


#define _VZCFS_DISK_START (uint32_t)0x0
#define _VZCFS_DISK_SIZE 5242880 // 5 MB
uint32_t write_ptr = _VZCFS_DISK_START;
#define WORD_SIZE 4

#define STDIN 0
#define STDOUT 1

uint32_t superblock_pointer_t = _VZCFS_DISK_SIZE  - sizeof(superblock_t);

/*
 * Private functions
 */

// Pkt functions
void get_header(char *, uint32_t);
uint32_t create_packet(char*, uint32_t, uint32_t*, uint32_t, uint32_t);
uint32_t create_packet_read(char* pkt, uint32_t* address_buffer, uint32_t data_len);

// Read + Write functions
HAL_StatusTypeDef virtual_flash_write(uint32_t* address, uint32_t data, uint32_t data_len);
HAL_StatusTypeDef virtual_flash_read(uint32_t* address, uint32_t* data, uint32_t data_len);

// Superblock
void update_superblock_metadata();

// Inode functions
uint32_t inode_write(char* name, uint32_t time, uint32_t size, uint8_t is_open, idfile_t* last_dinode, idfile_t* next_dinode);
HAL_StatusTypeDef data_write(char* data, uint32_t data_len);
uint32_t get_inode_addr_to_wrt(uint32_t);

// Dinode functions
void dinode_read(uint32_t* address, uint32_t* dinode);
void data_read(uint32_t* address, char* data, uint32_t data_len);
uint32_t get_dinode_addr_to_wrt();



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


void update_superblock_metadata(){
		virtual_flash_write((uint32_t *)superblock_pointer_t, (uint32_t)&superblock, sizeof(uint32_t)*3);
}

uint32_t get_inode_addr_to_wrt(uint32_t fd){
	return superblock_pointer_t + sizeof(uint32_t)*2 + fd*sizeof(ifile_t);
}

uint32_t get_dinode_addr_to_wrt(){
	return superblock.ptr_dinode_address - sizeof(idfile_t);
}


/*
 * Packet format: HEADER @ RAW_DATA(address:data) @ INODE_INFO(address:data)
 * 		      Es: "zcfs_write_@FLASH#RAW_DATA@INODE#ADDRESS_end"
 */
uint32_t create_packet(char* pkt, uint32_t type, uint32_t* address_buffer, uint32_t data_buffer, uint32_t data_len){
	char *header, *eol;
	uint32_t size_pkt = 0;

	header = malloc(HEADER_SIZE);
	get_header(header, type);

	eol = malloc(strlen(EOL_PKT));
	strcpy(eol, EOL_PKT);

	memcpy(pkt, header, HEADER_SIZE);
	size_pkt += HEADER_SIZE;
	memcpy(pkt + size_pkt, &address_buffer, sizeof(uint32_t));
	size_pkt += sizeof(uint32_t);
	memcpy(pkt + size_pkt, (uint32_t *)data_buffer, data_len);
	size_pkt += data_len;
	memcpy(pkt + size_pkt, eol, strlen(EOL_PKT));
	size_pkt += strlen(EOL_PKT);

	free(header);
	free(eol);

	return size_pkt;
}


uint32_t create_packet_read(char* pkt, uint32_t* address_buffer, uint32_t data_len){
	char *header, *eol;
	uint32_t size_pkt = 0;

	header = malloc(HEADER_SIZE);
	get_header(header, READ_PKT);

	eol = malloc(strlen(EOL_PKT));
	strcpy(eol, EOL_PKT);

	memcpy(pkt, header, HEADER_SIZE);
	size_pkt += HEADER_SIZE;
	memcpy(pkt + size_pkt, &address_buffer, sizeof(uint32_t));
	size_pkt += sizeof(uint32_t);
	memcpy(pkt + size_pkt, &data_len, sizeof(uint32_t));
	size_pkt += sizeof(uint32_t);
	memcpy(pkt + size_pkt, eol, strlen(EOL_PKT));
	size_pkt += strlen(EOL_PKT);

	free(header);
	free(eol);

	return size_pkt;
}



/*
 * PRIVATE
 * Write data/inode on the disk
 */
HAL_StatusTypeDef virtual_flash_write(uint32_t* address, uint32_t data, uint32_t data_len){
	int pkt_tmp_size = HEADER_SIZE + sizeof(address) + data_len;
	char pkt[pkt_tmp_size];

	memset(pkt, 0, HEADER_SIZE + sizeof(address) + data_len);

	uint32_t size_pkt = create_packet(pkt, WRITE_PKT, address, data, data_len);

	while(HAL_UART_Transmit_DMA(gHuart, (uint8_t*)pkt, size_pkt) != HAL_OK){};


	/*
	 *
	 * RACE CONDITION pkt[pkt_tmp_size] su USART troppo lenta rispetto al trigger della funzione
	 * No HAL_Delay a causa di HAL_UART_Transmit_DMA
	 *
	 */

	// This must be here! USART writes too fast // 500000/100000!!!
	for(int i=0; i<100000; i++);

	return HAL_OK;
}



HAL_StatusTypeDef virtual_flash_read(uint32_t* address, uint32_t* data, uint32_t data_len){
	int pkt_tmp_size = HEADER_SIZE + sizeof(address) + data_len;
	char pkt[pkt_tmp_size];


	memset(pkt, 0, HEADER_SIZE + sizeof(address) + data_len);

	uint32_t size_pkt = create_packet_read(pkt, address, data_len);

	while(HAL_UART_Transmit_DMA(gHuart, (uint8_t*)pkt, size_pkt) != HAL_OK){};
	while(HAL_UART_Receive(gHuart, (uint8_t*)data, data_len, HAL_MAX_DELAY)!= HAL_OK){};

	return HAL_OK;

}


void dinode_read(uint32_t* address, uint32_t* dinode){
	virtual_flash_read(address, dinode, sizeof(idfile_t));
}

void data_read(uint32_t* address, char* data, uint32_t data_len){
	virtual_flash_read(address, (uint32_t*)data, data_len);
}






/*
 * INODE
 */

void set_ifile(ifile_t* ifile, uint32_t fd, char* name, uint32_t time, uint32_t size, uint8_t is_open, idfile_t* last_dinode, idfile_t* next_dinode){
	ifile->fd = fd;
	strncpy(ifile->name, name, FNAME_LENGTH);
	ifile->time = time;
	ifile->size = size;
	ifile->is_open = is_open;
	ifile->last_dinode = last_dinode;
	ifile->next_dinode = next_dinode;
}


uint32_t inode_write(char* name, uint32_t time, uint32_t size, uint8_t is_open, idfile_t* last_dinode, idfile_t* next_dinode){
	uint32_t fd_inode = superblock.next_fd;

	// Write on RAM
	set_ifile(&superblock.inode_list[fd_inode], fd_inode, name, time, size, is_open, last_dinode, next_dinode);

	// Write on disk
	virtual_flash_write((uint32_t*)get_inode_addr_to_wrt(fd_inode), (uint32_t)&superblock.inode_list[fd_inode], sizeof(ifile_t));

	superblock.next_fd += 1;
	update_superblock_metadata();

	if(fd_inode >= 2){
		pending_dinode_insert(&superblock.inode_list[fd_inode]);
	}

	return fd_inode;
}



HAL_StatusTypeDef dinode_write(uint32_t fd, char* data, uint32_t data_len){

	uint32_t idx_fd_file = pending_fd_find(fd);

	if(idx_fd_file != -1){
		uint32_t* old_dinode_addr = (uint32_t*)superblock.inode_list[fd].last_dinode;

		idfile_t dinode_file;
		dinode_file.data_ptr = superblock.ptr_data_address;
		dinode_file.data_len = data_len;
		dinode_file.next_dinode = 0x0;

		data_write(data, data_len);

		uint32_t* dinode_addr = (uint32_t *)(superblock.ptr_dinode_address - sizeof(idfile_t));
		virtual_flash_write(dinode_addr, (uint32_t)&dinode_file, sizeof(idfile_t));

		if (old_dinode_addr == NULL){
			/*
			 * First dinode! -> write address direct to inode
			 */
			superblock.inode_list[fd].next_dinode = (idfile_t *)dinode_addr;
			superblock.inode_list[fd].last_dinode = (idfile_t *)dinode_addr;
			superblock.inode_list[fd].size = data_len;
			superblock.inode_list[fd].time = HAL_GetTick();
			virtual_flash_write((uint32_t*)get_inode_addr_to_wrt(fd), (uint32_t)&superblock.inode_list[fd], sizeof(ifile_t));

		}else{

			/*
			 * UPDATE OLD DINODE
			 */
			idfile_t old_dinode;
			dinode_read((uint32_t*)superblock.inode_list[fd].last_dinode, (uint32_t*)&old_dinode);
			old_dinode.next_dinode = (uint32_t)dinode_addr;
			virtual_flash_write((uint32_t*)superblock.inode_list[fd].last_dinode, (uint32_t)&old_dinode, sizeof(idfile_t));

			/*
			 * UPDATE INODE - LAST DINODE ADDRESS
			 */
			superblock.inode_list[fd].last_dinode = (idfile_t *)dinode_addr;
			superblock.inode_list[fd].size += data_len;
			superblock.inode_list[fd].time = HAL_GetTick();
			virtual_flash_write((uint32_t*)get_inode_addr_to_wrt(fd), (uint32_t)&superblock.inode_list[fd], sizeof(ifile_t));

		}

		superblock.ptr_dinode_address -= sizeof(idfile_t);
		update_superblock_metadata();
	}

	return HAL_OK;
}

HAL_StatusTypeDef data_write(char* data, uint32_t data_len){

	virtual_flash_write((uint32_t *)superblock.ptr_data_address, (uint32_t)data, data_len);

	superblock.ptr_data_address += data_len;
	update_superblock_metadata();

	return HAL_OK;
}



/*
 * Initialize superblock
 */
void initialize_superblock(){
	memset(superblock.inode_list, 0, sizeof(ifile_t)*_INODE_LIST_LIMIT);
	superblock.next_fd = 0;
	superblock.ptr_data_address = _VZCFS_DISK_START;
	superblock.ptr_dinode_address = superblock_pointer_t;

	virtual_flash_write((uint32_t *)superblock_pointer_t, (uint32_t)&superblock, sizeof(uint32_t)*16);

	inode_write("STDIN", 0, 0, 1, NULL, NULL);
	inode_write("STDOUT", 0, 0, 1, NULL, NULL);




	uint32_t fd_test = fs_open("wrt0");

	uint32_t fd_test_2 = fs_open("test0");

	uint32_t fd_test_3 = fs_open("file3");

	char *s = "AAAAAAABBBBB";
	char *s2 = "hello world";
    char *s3 = "secondo file RULES";
    char *s4 = "TRIIIIIIIII";

    fs_write(fd_test, s, strlen(s));
    fs_write(fd_test, s2, strlen(s2));
    fs_write(fd_test_2, s2, strlen(s2));
    buffer_flush(mbuf);
    fs_write(fd_test_2, s3, strlen(s3));
    fs_write(fd_test_3, s4, strlen(s4));
    buffer_flush(mbuf);
    fs_write(fd_test, s, strlen(s));
    fs_write(fd_test_2, s3, strlen(s3));
    fs_write(fd_test_3, s4, strlen(s4));
    buffer_flush(mbuf);


//    pending_fd_remove(fd_test_3);
//    linked_list_print(pbuffi->list_new_fd);

    // TODO virtual terminal
    fs_close(fd_test_3);
//    linked_list_print(pbuffi->list_new_fd);



    /*
     * TODO
     * - read (sia su fs che su python)
     * - timer
     * - tests
     *
     * - relazione
     * - virtual flash write on terminal
     * - ...
     */






//	char s3[sizeof(uint32_t)*16];
//	virtual_flash_read((uint32_t *)superblock_pointer_t, (uint32_t*)s3, sizeof(uint32_t)*16);







//	virtual_flash_write((uint32_t *)superblock_pointer_t, (uint32_t)&superblock, sizeof(superblock_t), 50);

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



// TODO
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
	initialize_superblock();
}



uint32_t fs_open(char* file_name){
	uint32_t ptr = 2;

	/*
	 * check if file exists
	 * if it exists and it is closed -> set is_open to 1 and update superblock
	 * return fd
	 */
	while(ptr<superblock.next_fd){
		ifile_t* inode = &superblock.inode_list[ptr];

		if(!strncmp(inode->name, file_name, strlen(inode->name))){

			// TODO - NEED TESTS

			if(!inode->is_open){

				/*
				 * UPDATE INODE on disk
				 */
				inode->is_open = 1;
				virtual_flash_write((uint32_t*)get_inode_addr_to_wrt(inode->fd), (uint32_t)inode, sizeof(ifile_t));

				/*
				 * insert file in pending list
				 */
				pending_dinode_insert(inode);
			}

			return inode->fd;
		}

		ptr++;
	}


	return inode_write(file_name, 0, 0, 1, NULL, NULL);
}



HAL_StatusTypeDef fs_close(uint32_t fd){
	// TODO remove fd from pending buffer
	// TODO

	if (fd <= STDOUT || fd >= superblock.next_fd){
		char* msg_error = "";
		sprintf(msg_error, "Cannot close the following file descriptor: %lu\r\n", fd);
		fs_error(msg_error);
		return HAL_ERROR;
	}


	/*
	 * If file not in the pending list
	 */
	uint32_t idx_fd_file = pending_fd_find(fd);
	if(idx_fd_file == -1){
		char* msg = "The current file is just closed\r\n";
		fs_write(STDOUT, msg, strlen(msg));
		return HAL_OK;
	}

	ifile_t* inode = &superblock.inode_list[fd];

	/*
	 * UPDATE INODE on disk
	 */
	inode->is_open = 0;
	virtual_flash_write((uint32_t*)get_inode_addr_to_wrt(inode->fd), (uint32_t)&superblock.inode_list[fd], sizeof(ifile_t));

	pending_fd_remove(inode->fd);

	return HAL_OK;
}


uint32_t fs_read(uint32_t fd, uint32_t start, uint32_t stop){
	if (fd <= STDOUT || fd >= superblock.next_fd){
		char* msg_error = "";
		sprintf(msg_error, "Cannot read the following file descriptor: %lu\r\n", fd);
		fs_error(msg_error);
		return HAL_ERROR;
	}


	/*
	 * If file not in the pending list
	 */
	uint32_t idx_fd_file = pending_fd_find(fd);
	if(idx_fd_file == -1){
		char* msg = "The current file is closed\r\n";
		fs_write(STDOUT, msg, strlen(msg));
		return HAL_OK;
	}


//	ifile_t* inode = &superblock.inode_list[fd];
	// TODO TO IMPLEMENT

	return 1;
}


/*
 * User writes data into fs
 */
uint32_t fs_write(uint32_t fd, char* ptr, uint32_t len){

	if(fd == STDIN){
		return fs_error("Cannot write on file descriptor STDIN\r\n");
	}

	if (fd == STDOUT){
		virtual_terminal_write((uint32_t)ptr, len);
		return len;
	}

	uint32_t idx_fd_file = pending_fd_find(fd);

	if(idx_fd_file != -1){


		if(len > _BUFFER_SIZE){
			return fs_error("buffer: write too big\r\n");
		}

		HAL_StatusTypeDef hstatus = buffer_insert(mbuf, fd, ptr, len);

		if (hstatus != HAL_OK)
		  return EIO;

		return len;
	}

	return -1;
}


uint32_t fs_error(char* error_msg){
	return fs_write(STDOUT, error_msg, strlen(error_msg));
}


