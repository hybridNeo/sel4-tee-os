/*
 *	@Author : Rahul Mahadev (hybridNeo)
 *  
 */
#include "helper.h"

extern void name_thread(seL4_CPtr tcb, char *name);

UNUSED static char allocator_mem_pool[ALLOCATOR_STATIC_POOL_SIZE];



UNUSED static sel4utils_alloc_data_t data;
void init_tee(simple_t *simple,seL4_BootInfo *info){
    printf("--- Initializing TEE container --- \n");
    name_thread(seL4_CapInitThreadTCB, "tee-container");
    simple_default_init_bootinfo(simple, info);
    simple_print(simple);
}

void get_allocators(allocman_t *allocman,vspace_t *vspace,vka_t *vka,simple_t *simple,seL4_BootInfo *info){
    UNUSED int error;
    allocman = bootstrap_use_current_simple(simple, ALLOCATOR_STATIC_POOL_SIZE,
        allocator_mem_pool);
    assert(allocman);
    allocman_make_vka(vka, allocman);
    
    error = sel4utils_bootstrap_vspace_with_bootinfo_leaky(vspace,&data,simple_get_pd(simple),vka,info);
    void *vaddr;
    UNUSED reservation_t virtual_reservation;
    virtual_reservation = vspace_reserve_range(vspace,
        ALLOCATOR_VIRTUAL_POOL_SIZE, seL4_AllRights, 1, &vaddr);
    assert(virtual_reservation.res);
    bootstrap_configure_virtual_pool(allocman, vaddr,
        ALLOCATOR_VIRTUAL_POOL_SIZE, simple_get_pd(simple));

}
