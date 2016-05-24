/*
 *	@Author : Rahul Mahadev (hybridNeo)
 *  Implementation file for defining structures and functions related to trusted application
 */

#include "ta.h"
#define APP_PRIORITY seL4_MaxPrio

int generate_ep(char *app_name){
	int len = strlen(app_name);
	int i,count=0;
	for(i=0;i < len;++i){
		count++;
	}
	return count * len;
}
void init_ta(trusted_app_t *ta,vka_t *vka,vspace_t *vspace,char *app_name){
	UNUSED int error;
	(*ta).ep_id = 0x61 ;//generate_ep(app_name);
	error = sel4utils_configure_process(&((*ta).proc_obj),vka,vspace,APP_PRIORITY,app_name);
	assert(error == 0);
	name_thread((*ta).proc_obj.thread.tcb.cptr,app_name);
    // (*ta).ep_object = {0};
    error = vka_alloc_endpoint(vka, &((*ta).ep_object));
    assert(error == 0);
	printf("In ta.c code blah %d \n",(*ta).ep_id);
}