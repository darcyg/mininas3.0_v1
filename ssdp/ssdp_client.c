#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {

	printf("FILE: %s,LINE: %d,%s\n", __FILE__, __LINE__, "SSDP Server start!");
	
	// Socket
    int ssdp_sock = ssdp_socket_init();
    if (ssdp_sock < 0) {
        printf("ssdp socket init fail\n");
        return -1;
    }


    char result[1024] = {0};
	client_receive_packet(ssdp_sock, 0, result);
    ssdp_socket_deinit(ssdp_sock);

 
	printf("FILE: %s,LINE: %d,%s\n",__FILE__, __LINE__, "SSDP Server stop!");
	
}

