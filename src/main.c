/*
 *  @Author : Rahul Mahadev (hybridNeo)
 *  Starting point for tee-os
 */
#include "tee.h"
#include "ta.h"
#include "client_controller.h"
#include "helper.h"

/* constants */
#define APP_PRIORITY seL4_MaxPrio
#define APP_IMAGE_NAME "hello-4-app"
#define HELLO_TA_INCREMENT 1


/*
 *                  boot   -----   process
 *                 /                    \
 *              RICH OS               RICH-syscall interface       
 *                                         |
 *                                         TA   
 *
 */

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

    //start client os
    vspace_t client_vspace;
    error = get_new_vspace(&client_vspace,&vka,&simple);
    start_rich_os(&vka,&vspace);

    //
    //start ta
    vspace_t ta_vspace;
    error = get_new_vspace(&ta_vspace,&vka,&simple);
    trusted_app_t new_app;
    init_ta(&new_app,&vka,&vspace,APP_IMAGE_NAME);
    start_ta(&new_app,&vka,&ta_vspace,APP_IMAGE_NAME);
    char arr[51] = "This is a sample text 0\0";
    call_function(&new_app,6,HELLO_TA_INCREMENT,arr,sizeof(char) * 50);
    //Only single function call works
    //call_function(&new_app,5,HELLO_TA_INCREMENT);
  
    return 0;
}

