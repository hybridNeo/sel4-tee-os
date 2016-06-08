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

/*
 *	Syscall param-interface for calling TA
 * 	reg 1 - integer parameter
 * 	reg 2 -	function name
 *	reg 3 - length of buffer
 * 	reg 4-maxLength - buffer
 *	
 */
void call_function(trusted_app_t *ta,int param,int function){
    //sample structure to send
    int data[5] = {23,41,56,723,21};
    size_t obj_size = sizeof(int) * 5;
    printf("size is %d  %d \n",obj_size,data[1]); 
    int length = obj_size/(double)sizeof(seL4_Word);
    printf("%d length\n", length);
    if((length+3) > seL4_MsgMaxLength){
    	printf("Params too large. operation failed\n");
    }else{
    	seL4_MessageInfo_t tag;
	    seL4_Word msg;
	    tag = seL4_MessageInfo_new(0, 0, 0, 3+length);
	   	seL4_Word* blockptr = (seL4_Word*)data;
	    seL4_SetMR(0, param);
	    seL4_SetMR(1,function);
	    seL4_SetMR(2,length);
	    for(int i =0 ; i < length;++i){
	    	seL4_SetMR(i+3,*blockptr);
	    	blockptr++;
	    }

	    // seL4_SetMR(length+3,MSG_END);
	    seL4_Call((*ta).ep_cap_path.capPtr,tag);
		printf("%d \n",seL4_MsgMaxLength * sizeof(int) );
		// assert(seL4_MessageInfo_get_length(tag) == 1);
	    msg = seL4_GetMR(0);
	    printf("returned value is %d \n",msg );	
	}

    

}

