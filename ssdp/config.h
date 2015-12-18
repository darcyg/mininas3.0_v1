#ifndef __SSDP_CONFIG_H_
#define __SSDP_CONFIG_H_

#define SSDP_PORT                   (1900)
#define SSDP_MULTICAST_ADDR         ("239.255.255.250")
#define UUID_LEN					(40)
#define SIXTY_LEN                   64

#define THREAD_CONCURRENCE_COUNT    (10)
#define THREAD_QUEUE_SIZE           (1024)
#define RECV_BUF_LEN				(1024)

typedef struct _getInforSystem{
     char version[SIXTY_LEN];
     char mac_addr[SIXTY_LEN];
	 char ip_addr[SIXTY_LEN];
}getInforSystem;

#endif /* #ifndef __SSDP_CONFIG_H_ */

