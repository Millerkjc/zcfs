
#include "test.h"

/*
* FBuff init test
*/

void test_fbuf_init(){
	char s[30];

	for(int i=0; i<2; i++){
	  file_buffer_t *fbuf;
	  fbuf = fbuffer_init(fbuf, i);

	  uint32_t id = fbuf->id;
	  uint32_t x = fbuf->bfill;
	  uint32_t y = fbuf->size;
	  fbuf->file_buffer = "hello\0";

	  sprintf(s, "%d : %s", i, fbuf->file_buffer);

	}

	//assert(strcmp(s, "1 : hello"));

}

void test_fbuf_insert_flush(){
	/*
	   * FBuff insert test
	   */
	  file_buffer_t *fbuf = NULL;
	  fbuf = fbuffer_init(fbuf, 1);
	  fbuffer_insert(fbuf, "test", 4);
	  char s[30];
	  sprintf(s, "%lu : %s", fbuf->id, fbuf->file_buffer);
	  fbuffer_insert(fbuf, "test", 4);
	  fbuffer_insert(fbuf, "test", 4);
	  fbuffer_insert(fbuf, "test", 4);
	  sprintf(s, "%lu : %s", fbuf->id, fbuf->file_buffer);

	  //assert(strcmp(s, "1 : testtesttest"));


	  /*
	   * FBuff flush + add
	   */
	  fbuffer_flush(fbuf);
	  fbuffer_insert(fbuf, "test", 4);

	  /*
	   * FBuff flush
	   */
	  fbuffer_flush(fbuf);
	  free(fbuf);
	  //sprintf(s, "%lu : %s", fbuf->id, fbuf->file_buffer); // questa mi va in hardware fault

	  //assert(strcmp(s, "1 : "));
}

void test_mbuf_init_insert(){

	  /*
	   * MBuff init test
	   */
//	main_buffer_t *mbuf = malloc(sizeof(main_buffer_t));
//	main_buffer_t *mbuf = NULL;
//	mbuf = buffer_init(mbuf);

	/*
	 * MBuff insert test
	 */
	buffer_insert(mbuf, 0, "test1", 5);
	buffer_insert(mbuf, 0, "test2", 5);
	buffer_insert(mbuf, 1, "test3", 5);
}






