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

 int get_new_vspace(allocman_t *allocman,vspace_t *cur_vspace,vspace_t *vspace, vka_t *vka, simple_t *simple){
 	UNUSED int error;
    UNUSED static sel4utils_alloc_data_t ta_vs_data;
    // void *existing_frames = {NULL};
    error = sel4utils_get_vspace(cur_vspace,vspace,&ta_vs_data,vka,simple_get_pd(simple),NULL,NULL);
 
    void *vaddr;
    UNUSED reservation_t virtual_reservation;
    virtual_reservation = vspace_reserve_range(vspace,
        ALLOCATOR_VIRTUAL_POOL_SIZE, seL4_AllRights, 1, &vaddr);
    assert(virtual_reservation.res);
    bootstrap_configure_virtual_pool(allocman, vaddr,
        ALLOCATOR_VIRTUAL_POOL_SIZE, simple_get_pd(simple));
    // error = sel4utils_bootstrap_vspace(vspace,&ta_vs_data,simple_get_pd(simple),vka,NULL,NULL,existing_frames);
    printf("Error is %d \n",error );
    return error;
 }