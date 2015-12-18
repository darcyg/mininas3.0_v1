#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "config.h"

int ssdp_socket_init(char* ip) {
    int opt = 1, ret = -1;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        //ERROR("socket create error");//huguanjun
		printf("FILE: %s,LINE: %d,%s\n",__FILE__, __LINE__, "socket create error");
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SSDP_PORT);
    addr.sin_addr.s_addr = inet_addr(ip);

    /* Must be set before bind*/
    ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
   
	if (ret < 0) {
		printf("FILE: %s,LINE: %d,%s\n",__FILE__, __LINE__, "setsockopt(SO_REUSEADDR) fail");
	}
    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		printf("FILE: %s,LINE: %d,%s\n",__FILE__, __LINE__, "socket bind fail");
        
        goto _err;
    }

    /* Join multicast group */
    struct ip_mreq imreq;
    imreq.imr_multiaddr.s_addr = inet_addr(SSDP_MULTICAST_ADDR);
    imreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void *)&imreq, 
            sizeof(struct ip_mreq)) < 0) {
		printf("FILE: %s,LINE: %d,%s\n",__FILE__, __LINE__, "join multicast group fail");
        goto _err;
    }

    return sock;
_err:
    if (sock >= 0) close(sock);
    return -1;
}

void ssdp_socket_deinit(int socket) {
    if (socket >= 0) close(socket);
}

int set_socket_blocking(int socket, int block) {
    int flags = fcntl(socket, F_GETFL); 
    if (flags < 0) return -1;

    if (block) {
        flags &= O_NONBLOCK;
        fcntl(socket, F_SETFL, flags & ~O_NONBLOCK);
    } else {
        flags |= O_NONBLOCK;
    }
    
    if (fcntl(socket, F_SETFL, flags) < 0) return -1;

    return 0;
}


/* unit is ms */
static long diff_time(struct timeval* t1, struct timeval* t2) {
    long long t1ms = t1 ? ((t1->tv_sec)*1000 + (t1->tv_usec)/1000) : 0;
    long long t2ms = t2 ? ((t2->tv_sec)*1000 + (t2->tv_usec)/1000) : 0;
    return (t2ms - t1ms);
}

void server_receive_packet(int socket, long maxtimeout, char *result){
    fd_set readfds;
    //threadpool_t* pool = NULL;
    struct timeval timeout, start_time;

    gettimeofday(&start_time, NULL);

    if (socket < 0) return;

	int count = 0;
	/* Receive loop */
    while(1) {
        FD_ZERO(&readfds);
        FD_SET(socket, &readfds);
        struct timeval* timeout_ptr = NULL;
        if (maxtimeout > 0) {
            struct timeval cur_time;
            gettimeofday(&cur_time, NULL);
            long dt = maxtimeout - diff_time(&start_time, &cur_time);
            if (dt <= 0) break;

            timeout.tv_sec = dt/1000;
            timeout.tv_usec = (dt%1000)*1000;
            timeout_ptr = &timeout;
        }
        if (select(socket+1, &readfds, NULL, NULL, timeout_ptr) < 0) {
            if(errno != EINTR) break;
            
            continue;
        }

        /* Process ssdp socket */
        if (FD_ISSET(socket, &readfds)) {
			struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(struct sockaddr_in);
			
			char recvbuf[RECV_BUF_LEN] = {0};
			memset(result, 0, sizeof(result));

            int bytes = recvfrom(socket, recvbuf, RECV_BUF_LEN, 0, 
                    (struct sockaddr*)&client_addr, &addr_len);
			
			strncpy(result, recvbuf, bytes); //huguanjun
			printf("SSDP packet:\n%s\n", result);
        }
    }
    
}


void client_receive_packet(int socket, long maxtimeout, char *result){
    fd_set readfds;
    struct timeval timeout, start_time;

    gettimeofday(&start_time, NULL);

    if (socket < 0) return;

	int count = 0;
	/* Receive loop */
    while(1) {
        FD_ZERO(&readfds);
        FD_SET(socket, &readfds);
        struct timeval* timeout_ptr = NULL;
        if (maxtimeout > 0) {
            struct timeval cur_time;
            gettimeofday(&cur_time, NULL);
            long dt = maxtimeout - diff_time(&start_time, &cur_time);
            if (dt <= 0) break;

            timeout.tv_sec = dt/1000;
            timeout.tv_usec = (dt%1000)*1000;
            timeout_ptr = &timeout;
        }
        if (select(socket+1, &readfds, NULL, NULL, timeout_ptr) < 0) {
            if(errno != EINTR) break;
            
            continue;
        }

        /* Process ssdp socket */
        if (FD_ISSET(socket, &readfds)) {
			struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(struct sockaddr_in);
			
			char recvbuf[RECV_BUF_LEN] = {0};
			memset(result, 0, sizeof(result));

            int bytes = recvfrom(socket, recvbuf, RECV_BUF_LEN, 0, 
                    (struct sockaddr*)&client_addr, &addr_len);
			
			/*²âÊÔ°üº¬MiniNAS*/
			if(strstr(recvbuf, "MiniNAS")){
				strncpy(result, recvbuf, bytes); //huguanjun
				printf("SSDP packet:\n%s\n", result);
				break;
			}
				
			//strncpy(result, recvbuf, bytes); //huguanjun
			printf("SSDP packet:\n%s\n", result);
			
        }
    }
    
}


