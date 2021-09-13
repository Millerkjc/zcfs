/*
 * demo.c
 *
 *  Created on: Sep 13, 2021
 *      Author: m3jc
 */


#include <stdio.h>
#include <string.h>
#include "fs_syscalls.h"

#define STDOUT 1


void demo_0(){
	char *log_time = (char*)malloc(20);
	char *config = "This is my awesome config file!";

	sprintf(log_time, "time: %ld", HAL_GetTick());

	uint32_t fd_log = zcfs_open("log_file.txt");
    uint32_t fd_config = zcfs_open("config_file.conf");

	zcfs_write(fd_config, config, strlen(config));

    for(int i=0; i < 3; i++){
    	zcfs_write(fd_log, log_time, strlen(log_time));
    	sprintf(log_time, "time: %ld", HAL_GetTick());
    	HAL_Delay(1000);
    }

    zcfs_close(fd_log);
    zcfs_close(fd_config);
}


void demo_1(){
	char *log_time = (char*)malloc(20);

	sprintf(log_time, "time: %ld", HAL_GetTick());

	uint32_t fd_log = zcfs_open("log_file.txt");
   	zcfs_write(fd_log, log_time, strlen(log_time));
   	zcfs_close(fd_log);

   	HAL_Delay(1000);

   	sprintf(log_time, "time: %ld", HAL_GetTick());

   	fd_log = zcfs_open("log_file.txt");
   	zcfs_write(fd_log, log_time, strlen(log_time));
   	zcfs_close(fd_log);
}


void demo_2(){
	char *read_file = "Hei! This is a awesome file to read!!\x00";
	char *buffer = malloc(100);
	memset(buffer, 0, 100);

	uint32_t fd_file = zcfs_open("README.txt");
   	zcfs_write(fd_file, read_file, strlen(read_file));

   	zcfs_read(fd_file, buffer, 15, 27);
   	fs_write(STDOUT, buffer, strlen(buffer));

   	zcfs_close(fd_file);
}




//void test_write_long(){
//	uint32_t dr0n3_test = zcfs_open("dr0n3");
//	uint32_t arm_test = zcfs_open("4rm");
//	char *str = malloc(101);
//	char *str2 = malloc(101);
//
//	memcpy(str, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x00", 101);
//	memcpy(str2, "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\x00", 101);
//
//	for(int i=0; i<20; i++){
//		zcfs_write(dr0n3_test, str2, strlen(str2));
//		zcfs_write(arm_test, str, strlen(str));
//		zcfs_write(dr0n3_test, str2, strlen(str2));
//		zcfs_write(arm_test, str2, strlen(str));
//		zcfs_write(dr0n3_test, str2, strlen(str2));
//		zcfs_write(arm_test, str, strlen(str));
//		zcfs_write(dr0n3_test, str2, strlen(str2));
//		zcfs_write(arm_test, str2, strlen(str));
//		zcfs_write(dr0n3_test, str2, strlen(str2));
//	    HAL_Delay(500);
//	}
//}
