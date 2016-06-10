/*
 *	@Author : Rahul Mahadev (hybridNeo)
 *  
 */
#include "tee.h"

 // void make_ta_listener(vka_t *vka){
 // 	UNUSED int error = 0;
 // 	vka_object_t tcb_object = {0};
 // 	error = vka_alloc_tcb(&vka,&tcb_object);
 // 	assert(error == 0);
 // 	error = seL4_TCB_Configure(tcb_object.cptr,seL4_CapNull,seL4_MaxPrio,cspace_cap,seL4_NilData)
 // }

 int get_new_vspace(vspace_t *vspace, vka_t *vka, simple_t *simple){
 	UNUSED int error;
    UNUSED static sel4utils_alloc_data_t ta_vs_data;
    void *existing_frames = {NULL};
    error = sel4utils_bootstrap_vspace(vspace,&ta_vs_data,simple_get_pd(simple),vka,NULL,NULL,existing_frames);
    return error;
 }