#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "utils.h"

int main(){
    int sock=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
    int one=1; setsockopt(sock,IPPROTO_IP,IP_HDRINCL,&one,sizeof(one));

    char packet[4096]; memset(packet,0,sizeof(packet));
    char *data=packet+sizeof(ip_hdr)+sizeof(udp_hdr); strcpy(data,"Hello Raw UDP!");
    int dlen=strlen(data);

    ip_hdr *iph=(ip_hdr*)packet;
    iph->ihl=5; iph->ver=4; iph->tot_len=htons(sizeof(ip_hdr)+sizeof(udp_hdr)+dlen);
    iph->id=htons(1234); iph->frag_off=0; iph->ttl=64; iph->proto=17;
    iph->saddr=inet_addr("127.0.0.1"); iph->daddr=inet_addr("127.0.0.1");
    iph->checksum=ip_checksum(iph,sizeof(ip_hdr));

    udp_hdr *udph=(udp_hdr*)(packet+sizeof(ip_hdr));
    udph->source=htons(12345); udph->dest=htons(54321);
    udph->len=htons(sizeof(udp_hdr)+dlen);
    udph->checksum=udp_checksum(iph,udph,data,dlen);

    struct sockaddr_in dest={.sin_family=AF_INET,.sin_port=udph->dest,.sin_addr.s_addr=iph->daddr};
    sendto(sock,packet,ntohs(iph->tot_len),0,(struct sockaddr*)&dest,sizeof(dest));
    printf("Packet sent!\n"); close(sock);
    return 0;
}

