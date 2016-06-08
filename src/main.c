/*
 *  @Author : Rahul Mahadev (hybridNeo)
 *  Starting point for tee-os
 */
#include "tee.h"
#include "ta.h"
#include "helper.h"

/* constants */


#define APP_PRIORITY seL4_MaxPrio
#define APP_IMAGE_NAME "hello-4-app"
#define HELLO_TA_INCREMENT 1




int main(void)
{
    UNUSED int error;
    simple_t simple;
    vka_t vka;
    allocman_t *allocman = NULL;
    vspace_t vspace;
    seL4_BootInfo *info = seL4_GetBootInfo();
    init_tee(&simple,info);

    get_allocators(allocman,&vspace,&vka,&simple,info);
    //new vspace
    vspace_t ta_vspace;
    UNUSED static sel4utils_alloc_data_t ta_vs_data;
    void *existing_frames = {NULL};

    error = sel4utils_bootstrap_vspace(&ta_vspace,&ta_vs_data,simple_get_pd(&simple),&vka,NULL,NULL,existing_frames);
    
    //
    trusted_app_t new_app;
    init_ta(&new_app,&vka,&vspace,APP_IMAGE_NAME);
    start_ta(&new_app,&vka,&ta_vspace,APP_IMAGE_NAME);
    call_function(&new_app,6,HELLO_TA_INCREMENT);
    //Only single function call works
    //call_function(&new_app,5,HELLO_TA_INCREMENT);
  
    return 0;
}

