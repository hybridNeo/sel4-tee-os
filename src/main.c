/*
 *  @Author : Rahul Mahadev (hybridNeo)
 *  Starting point for tee-os
 */

#include <autoconf.h>

#include <stdio.h>
#include <assert.h>

#include <sel4/sel4.h>

#include <simple/simple.h>
#include <simple-default/simple-default.h>

#include <vka/object.h>

#include <allocman/allocman.h>
#include <allocman/bootstrap.h>
#include <allocman/vka.h>

#include <vspace/vspace.h>

#include <sel4utils/vspace.h>
#include <sel4utils/mapping.h>
#include <sel4utils/process.h>

#include "ta.h"

/* constants */
#define EP_BADGE 0x61 
#define MSG_DATA 0x6161 

#define APP_PRIORITY seL4_MaxPrio
#define APP_IMAGE_NAME "hello-4-app"

/* global environment variables */
seL4_BootInfo *info;

#define ALLOCATOR_STATIC_POOL_SIZE ((1 << seL4_PageBits) * 10)
UNUSED static char allocator_mem_pool[ALLOCATOR_STATIC_POOL_SIZE];

#define ALLOCATOR_VIRTUAL_POOL_SIZE ((1 << seL4_PageBits) * 100)

UNUSED static sel4utils_alloc_data_t data;

#define THREAD_2_STACK_SIZE 4096
UNUSED static int thread_2_stack[THREAD_2_STACK_SIZE];


//functions defined here
extern void name_thread(seL4_CPtr tcb, char *name);
void init_tee(simple_t *simple){
    printf("--- Initializing TEE container --- \n");
    name_thread(seL4_CapInitThreadTCB, "tee-container");
    info = seL4_GetBootInfo();
    simple_default_init_bootinfo(simple, info);
    simple_print(simple);
}


int main(void)
{
    UNUSED int error;
    simple_t simple;
    vka_t vka;
    allocman_t *allocman;
    vspace_t vspace;
    
    init_tee(&simple);

    allocman = bootstrap_use_current_simple(&simple, ALLOCATOR_STATIC_POOL_SIZE,
        allocator_mem_pool);
    assert(allocman);
    allocman_make_vka(&vka, allocman);
    
    error = sel4utils_bootstrap_vspace_with_bootinfo_leaky(&vspace,&data,simple_get_pd(&simple),&vka,info);
    void *vaddr;
    UNUSED reservation_t virtual_reservation;
    virtual_reservation = vspace_reserve_range(&vspace,
        ALLOCATOR_VIRTUAL_POOL_SIZE, seL4_AllRights, 1, &vaddr);
    assert(virtual_reservation.res);
    bootstrap_configure_virtual_pool(allocman, vaddr,
        ALLOCATOR_VIRTUAL_POOL_SIZE, simple_get_pd(&simple));

    //
    trusted_app_t new_app;
    init_ta(&new_app,&vka,&vspace,APP_IMAGE_NAME);
    start_ta(&new_app,&vka,&vspace,APP_IMAGE_NAME);
    call_function(&new_app,&vka,&vspace);
  
    return 0;
}

