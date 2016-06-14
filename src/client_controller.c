/*
 *  @Author : Rahul Mahadev (hybridNeo)
 *  
 */
#include "client_controller.h"
vka_object_t ep_object;

int start_rich_os(sel4utils_process_t *client_proc,vka_t *vka,vspace_t *vspace){
	

	UNUSED int error;
	
	cspacepath_t ep_cap_path;
	error = sel4utils_configure_process(client_proc,vka,vspace,APP_PRIORITY,CLIENT_NAME);
	assert(error == 0);
	// seL4_DebugNameThread((*client_proc).thread.tcb.cptr,"RICH OS");
    error = vka_alloc_endpoint(vka, &ep_object);
    assert(error == 0);
	seL4_CPtr new_ep_cap;
    vka_cspace_make_path(vka,ep_object.cptr,&ep_cap_path);
    new_ep_cap = sel4utils_mint_cap_to_process(client_proc,ep_cap_path,seL4_AllRights,seL4_CapData_Badge_new(CLIENT_EP));
    assert(new_ep_cap != 0);
    error = sel4utils_spawn_process_v(client_proc,vka,vspace,0,NULL,1);
    return error;
}

