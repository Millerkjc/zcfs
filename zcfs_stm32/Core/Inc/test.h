#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "assert.h"
#include <string.h>

#ifndef INCLUDE_FS_BUFFER_H_
#define INCLUDE_FS_BUFFER_H_
#include "fs_buffer.h"
#endif

#ifndef INCLUDE_FS_MEM_H_
#define INCLUDE_FS_MEM_H_
#include "fs_mem.h"
#endif

#ifndef THIRD_PARTY_ZCFS_SRC_TEST_SRC_H_
#define THIRD_PARTY_ZCFS_SRC_TEST_SRC_H_
#include "test_src.h"
#endif

#ifndef TEST_H_
#define TEST_H_

void test_fbuf_init();
void test_fbuf_insert();
void test_fbuf_insert_flush();
void test_mbuf_init_insert();

#endif
