#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "config.h"
#include "daemon.h"

//return the count of netcard
static int getNetCardCount(){
    int nCount = 0;
	FILE* f = fopen("/proc/net/dev", "r");
	if (!f)
	{
			perror("Open /proc/net/dev failed!errno");
			exit(-1);
	}

	char szLine[512];
	fgets(szLine, sizeof(szLine), f);    /* eat line */
	fgets(szLine, sizeof(szLine), f);
	while(fgets(szLine, sizeof(szLine), f))
	{
		char szName[128] = {0};
		sscanf(szLine, "%s", szName);
		int nLen = strlen(szName);
		if (nLen <= 0)
			continue;
		if (szName[nLen - 1] == ':') szName[nLen - 1] = 0;
		if (!strcmp(szName, "lo") || !strncmp(szName,"sit",3) || !strncmp(szName,"ip6",3))
			continue;
		nCount++;
	}

	fclose(f);
	f = NULL;
	return nCount;
}


static int getip(char *ipaddr,int count){
	char name[128];
    char ethNum[8];
	struct hostent *hent;
	int i,sock_get_ip;  

	struct   sockaddr_in *sin;  
	struct   ifreq ifr_ip;    

	if ((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) == -1)  
	{   
			printf("socket create failse...GetLocalIp!/n");  
			return -1;  
	}   

	memset(&ifr_ip, 0, sizeof(ifr_ip));    
	sprintf(ethNum,"eth%d",count);
	strncpy(ifr_ip.ifr_name,ethNum, sizeof(ifr_ip.ifr_name)-1);    
	if( ioctl(sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0 )    
	{    
		return -1;    
	}    
	sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;    
	sprintf(ipaddr,"%s",inet_ntoa(sin->sin_addr));
	close(sock_get_ip);
	return 0;
}

//????mac??
static int getmac(char *mac_addr,int count){
	int sock_mac;
    char ethNum[8];

	struct ifreq ifr_mac;

	sock_mac = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock_mac == -1)
	{
		perror("create socket falise...mac/n");
		return -1;
	}

	memset(&ifr_mac,0,sizeof(ifr_mac));
	sprintf(ethNum,"eth%d",count);
	strncpy(ifr_mac.ifr_name,ethNum, sizeof(ifr_mac.ifr_name)-1);
	if( (ioctl( sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)
	{
			printf("mac ioctl error/n");
			return -1;
	}
	sprintf(mac_addr,"%02x:%02x:%02x:%02x:%02x:%02x",
 			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]);

	close( sock_mac );
	return 0;
}


//?????
static void getVersion(char *version){
	FILE* fp;

	if((fp = fopen("/etc/mininas/version","r")) == NULL){
			perror("fopen error");
			sprintf(version,"mininas.x");
			exit(-1); 
	}
	fread(version,sizeof(char),SIXTY_LEN,fp);
	*(version+strlen(version)-2) = '\0';
	fclose(fp);
}

static void createuuid(char *output){
	FILE* fp; 

	if((fp = fopen("/proc/sys/kernel/random/uuid","r")) == NULL){
			perror("fopen error");
			sprintf(output,"not known");
			exit(-1); 
	}   
	fread(output,sizeof(char),SIXTY_LEN,fp);
	*(output+strlen(output)-2) = '\0';
	fclose(fp);
}

//get serial number
static void getSerial(char* serial){
	if(!access("/proc/BOARD_event",0)){
		FILE* fp = fopen("/dev/mtdblock0","r");
		if(fp == NULL){
			perror("serial fopen error"),exit(-1);
		}
	
		fseek(fp,1697760,SEEK_SET);
		fread(serial,sizeof(char),11,fp);
		fclose(fp);
	}
}

static void ssdpd_response_msearch(int ssdp_sock, struct sockaddr_in* dst_addr) {
    char buf[1024] = {0};
	char url[20] = {0};
    snprintf(buf, sizeof(buf), "HTTP/1.1 200 OK\r\n"
            "ST:upnp:rootdevice\r\n"
            "CACHE-CONTROL:max-age=1800\r\n"
            "LOCATION: http://%s\r\n"
            "\r\n"
            ,url);

    int len = strlen(buf);
    ssize_t bytes = sendto(ssdp_sock, buf, len, 0, (struct sockaddr* )dst_addr, 
            sizeof(struct sockaddr_in));
    if (bytes != len) {
        printf("FILE: %s,LINE: %d,%s\n",__FILE__, __LINE__, "Send M-SEARCH response fail");
    } else {
		printf("FILE: %s,LINE: %d,Send M-SEARCH response successed: %s\n",__FILE__, __LINE__, inet_ntoa(dst_addr->sin_addr));
    }
}

static void ssdpd_send_notify(int sock,int count,getInforSystem* getInfor,char* serial) {
    char buf[1024] =  {0};
	char uuid[UUID_LEN] = {0};

	createuuid(uuid);
	
//	printf("current IP=%s, uuid = %s\n", ipaddr, uuid);
	
    snprintf(buf, sizeof(buf), 
                "NOTIFY * HTTP/1.1\r\n"
                "HOST:%s:%d\r\n"
                "CACHE-CONTROL: max-age=1800\r\n"
                "LOCATION: http://%s\r\n"
                "NT:uuid:%s::urn:schemas-upnp-org:[$device:WinSunMiniNAS:1"
                "/MiniNAS/%s/%s/%s$]\r\n"
                "NTS:ssdp:alive\r\n"
                "\r\n", SSDP_MULTICAST_ADDR, SSDP_PORT 
				,getInfor->ip_addr, uuid,getInfor->version,getInfor->mac_addr,serial);
    
    printf("buf=%s\n",buf);
    int len = strlen(buf);
    struct sockaddr_in dst_addr;
    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_port = htons(SSDP_PORT);
    dst_addr.sin_addr.s_addr = inet_addr(SSDP_MULTICAST_ADDR);


    ssize_t bytes = sendto(sock, buf, len, 0, (struct sockaddr* )&dst_addr, 
            sizeof(struct sockaddr_in));
    if (bytes != len) {
        printf("FILE: %s,LINE: %d,%s\n",__FILE__, __LINE__, "Send Notify fail");
    } else {
		printf("FILE: %s,LINE: %d,%s\n",__FILE__, __LINE__, "Send Notify successed");
    }
}


int main (int argc, char* argv[]) {
    int daemon = 1;
    int i=0,count = 0;
	int ssdp_sock[5];
	char ipaddr[24]={};
	char serial[11] = {};
	getInforSystem getInfor[5]= {};
	getSerial(serial);
//	getVersion(getInfor.version);
//	getmac(getInfor.mac_addr,0);
//	getip(getInfor.ip_addr,0);
	
	for(i=0;i<5;i++){
		ssdp_sock[i] = -1;
	}

    /* ÊØ»¤½ø³Ì */
    if (daemon) { 
       daemonize();
    } 
	
	printf("FILE: %s,LINE: %d,%s\n", __FILE__, __LINE__, "SSDP Server start!");
	
    /* Send notify */
	while(1){
		count = getNetCardCount();
		int k = count;

    	/* Socket */
		for(i=0;i<count;i++){
			getVersion(getInfor[i].version);
			getmac(getInfor[i].mac_addr,i);
			getip(getInfor[i].ip_addr,i);
    		ssdp_sock[i] = ssdp_socket_init(ipaddr);
		}

		while(1){
			count = getNetCardCount();
			if(k != count){
				int j;
				for(j=0;j<count;j++){
					ssdp_socket_deinit(ssdp_sock[i]);
					ssdp_sock[i] = -1;
				}
				break;
			}
			for(i=0;i<count;i++){
    			ssdpd_send_notify(ssdp_sock[i],i,&getInfor[i],serial);
			}
			sleep(1);
		}
	}
	ssdp_socket_deinit(ssdp_sock);

	printf("FILE: %s,LINE: %d,%s\n",__FILE__, __LINE__, "SSDP Server stop!");
	
}


