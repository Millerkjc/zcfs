/*
 * write_on_file.c
 *
 *  Created on: Jul 28, 2021
 *      Author: m3jc
 */

#include <stdio.h>
#include <string.h>
#include "fs_syscalls.h"


void test_write_long(){
	uint32_t dr0n3_test = zcfs_open("dr0n3");
	uint32_t arm_test = zcfs_open("4rm");
	char *str = malloc(101);
	char *str2 = malloc(101);

	memcpy(str, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x00", 101);
	memcpy(str2, "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\x00", 101);

	for(int i=0; i<20; i++){
		zcfs_write(dr0n3_test, str2, strlen(str2));
		zcfs_write(arm_test, str, strlen(str));
		zcfs_write(dr0n3_test, str2, strlen(str2));
		zcfs_write(arm_test, str2, strlen(str));
		zcfs_write(dr0n3_test, str2, strlen(str2));
		zcfs_write(arm_test, str, strlen(str));
		zcfs_write(dr0n3_test, str2, strlen(str2));
		zcfs_write(arm_test, str2, strlen(str));
		zcfs_write(dr0n3_test, str2, strlen(str2));
	    HAL_Delay(500);
	}
}
