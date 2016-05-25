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
    error = vka_alloc_endpoint(vka, &((*ta).ep_object));
    assert(error == 0);
	// printf("In ta.c code blah %d \n",(*ta).ep_id);
	seL4_CPtr new_ep_cap;
    vka_cspace_make_path(vka,(*ta).ep_object.cptr,&((*ta).ep_cap_path));
    new_ep_cap = sel4utils_mint_cap_to_process(&((*ta).proc_obj),(*ta).ep_cap_path,seL4_AllRights,seL4_CapData_Badge_new((*ta).ep_id));
    assert(new_ep_cap != 0);
}

void start_ta(trusted_app_t *ta,vka_t *vka,vspace_t *vspace,char *app_name){
	UNUSED int error;
	error = sel4utils_spawn_process_v(&((*ta).proc_obj),vka,vspace,0,NULL,1);
    assert(error == 0);
}

void call_function(trusted_app_t *ta,vka_t *vka,vspace_t *vspace){

    seL4_Word sender_badge;
    seL4_MessageInfo_t tag;
    seL4_Word msg;
	tag = seL4_Recv((*ta).ep_cap_path.capPtr,&sender_badge);
    assert(sender_badge == (*ta).ep_id );
    assert(seL4_MessageInfo_get_length(tag) == 1);
    msg = seL4_GetMR(0);
    printf("tee-container: got message from %#x from %#x \n",msg,sender_badge );

}