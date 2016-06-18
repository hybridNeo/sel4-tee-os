/*
 *  @Author : Rahul Mahadev (hybridNeo)
 *  
 */

#include <string.h>	
#include "ta.h"
#define CLIENT_NAME "client-os"
#define CLIENT_OS_EP 0x77
#define APP_PRIORITY seL4_MaxPrio
#define HELLO_TA_INCREMENT 1
#define START_TA_CMD 1001
#define CALL_FUNC_CMD 1002
int start_rich_os(sel4utils_process_t *client_proc,vka_t *vka,vspace_t *vspace,cspacepath_t *ep_cap_path);
int send_msg(sel4utils_process_t *client_proc,cspacepath_t *ep_cap_path);
void listener(cspacepath_t *ep_cap_path,vka_t *vka,vspace_t *vspace,sel4utils_process_t *client_proc);