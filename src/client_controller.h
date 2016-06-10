/*
 *  @Author : Rahul Mahadev (hybridNeo)
 *  
 */
#include <stdio.h>
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
#include <string.h>
#include <sel4utils/vspace.h>
#include <sel4utils/mapping.h>
#include <sel4utils/process.h>	
#define CLIENT_NAME "client-os"
#define CLIENT_EP 0x77
#define APP_PRIORITY seL4_MaxPrio
int start_rich_os(vka_t *vka,vspace_t *vspace);
