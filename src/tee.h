/*
 *	@Author : Rahul Mahadev (hybridNeo)
 *  
 */
#include <stdio.h>
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
#define EP_BADGE 0x61 

int get_new_vspace(allocman_t *allocman,vspace_t *cur_vspace,vspace_t *vspace,vka_t *vka,simple_t *simple);
#define ALLOCATOR_VIRTUAL_POOL_SIZE ((1 << seL4_PageBits) * 100)

#define ALLOCATOR_STATIC_POOL_SIZE ((1 << seL4_PageBits) * 10)