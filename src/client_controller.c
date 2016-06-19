/*
 *  @Author : Rahul Mahadev (hybridNeo)
 *  
 */
#include "client_controller.h"
vka_object_t ep_object;

int start_rich_os(sel4utils_process_t *client_proc,vka_t *vka,vspace_t *vspace,cspacepath_t *ep_cap_path){
	

	UNUSED int error;
	
	
	error = sel4utils_configure_process(client_proc,vka,vspace,APP_PRIORITY,CLIENT_NAME);
	assert(error == 0);
	// seL4_DebugNameThread((*client_proc).thread.tcb.cptr,"RICH OS");
	error = vka_alloc_endpoint(vka, &ep_object);
	assert(error == 0);
	seL4_CPtr new_ep_cap;
	vka_cspace_make_path(vka,ep_object.cptr,ep_cap_path);
	new_ep_cap = sel4utils_mint_cap_to_process(client_proc,*ep_cap_path,seL4_AllRights,seL4_CapData_Badge_new(CLIENT_OS_EP));
	assert(new_ep_cap != 0);
	error = sel4utils_spawn_process_v(client_proc,vka,vspace,0,NULL,1);
	return error;

}

void refresh_cap(sel4utils_process_t *client_proc,cspacepath_t *ep_cap_path){
	// seL4_CPtr new_ep_cap;
	// vka_cspace_make_path(vka,ep_object.cptr,ep_cap_path);
	sel4utils_mint_cap_to_process(client_proc,*ep_cap_path,seL4_AllRights,seL4_CapData_Badge_new(CLIENT_OS_EP));
}

int send_msg(sel4utils_process_t *client_proc,cspacepath_t *ep_cap_path){
	seL4_MessageInfo_t tag;
	seL4_Word msg;
	tag = seL4_MessageInfo_new(0, 0, 0, 1);
	seL4_SetMR(0, 1234);
	seL4_Call(ep_cap_path->capPtr,tag);
	msg = seL4_GetMR(0);
	printf("tee-container: got msg %d \n", msg);
	return 1;
}

void listener(cspacepath_t *ep_cap_path,vka_t *vka,vspace_t *vspace,sel4utils_process_t *client_proc){	
	trusted_app_t ta_app_list[10];
	int ta_count = 0;
	while(1){
		// refresh_cap(client_proc,ep_cap_path);
		seL4_Word sender_badge;
		seL4_MessageInfo_t tag;
		seL4_Word msg;
		tag = seL4_Recv(ep_cap_path->capPtr,&sender_badge);
		msg = seL4_GetMR(0);
		if(msg == START_TA_CMD){
			// printf("tee-container: starting ta \n");
			int arg_len = seL4_GetMR(1);
			seL4_Word dat_ptr[arg_len];
			for(int i=0;i < arg_len;++i){
				dat_ptr[i] = seL4_GetMR(i+2);
			}
			char app_name[arg_len];
			strcpy(app_name,(char *)dat_ptr);
			printf("tee-container: starting ta %s\n",app_name );
			
			init_ta(&(ta_app_list[ta_count]),vka,vspace,app_name);
			start_ta(&(ta_app_list[ta_count]),vka,vspace,app_name);
			
			// printf(" %d \n",ta_app_list[0]->ep_id ); //remove later
			// char arr[51] = "This is a sample text 0\0";
			// call_function(&new_app,6,HELLO_TA_INCREMENT,arr,sizeof(char) * 50);
			seL4_SetMR(0,ta_count);
			ta_count++;
		}else if(msg == CALL_FUNC_CMD){
			
			int ta_num = seL4_GetMR(1);
			
			if(ta_num > ta_count || ta_num < 0){
				//throw err
			}else{
				int func_id = seL4_GetMR(2);
				int simple_arg = seL4_GetMR(3);
				int length_var = seL4_GetMR(4);
				seL4_Word dat_arr[length_var];
				for (int i = 0; i < length_var; ++i){
					dat_arr[i] = seL4_GetMR(i+5);
				}
				seL4_Word res_obj[length_var];
				int result = call_function(&ta_app_list[ta_num],simple_arg,func_id,dat_arr,sizeof(seL4_Word) * length_var,res_obj);
				seL4_SetMR(0,result);
				seL4_SetMR(1,length_var);
				for (int i = 0; i < length_var; ++i){
					seL4_SetMR(i+2,res_obj[i]);
				}
			}
		}

		seL4_Reply(tag);
	
	}
	
}