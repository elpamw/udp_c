#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "utils.h"

int main(){
    int sock=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
    while(1){
        char buf[4096]; struct sockaddr_in src; socklen_t len=sizeof(src);
        int n=recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr*)&src,&len);
        if(n<0) continue;
        ip_hdr *iph=(ip_hdr*)buf;
        udp_hdr *udph=(udp_hdr*)(buf+sizeof(ip_hdr));
        char *data=buf+sizeof(ip_hdr)+sizeof(udp_hdr);
        int dlen=ntohs(udph->len)-sizeof(udp_hdr);
        printf("Received from %s:%d -> %.*s\n",inet_ntoa(src.sin_addr),ntohs(udph->source),dlen,data);
    }
    close(sock);
    return 0;
}

