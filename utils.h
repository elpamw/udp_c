#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

// UDP and IP

typedef struct { uint16_t source, dest, len, checksum; } udp_hdr;

typedef struct {
#if __BYTE_ORDER==__LITTLE_ENDIAN
    uint8_t ihl:4, ver:4;
#else
    uint8_t ver:4, ihl:4;
#endif
    uint8_t tos;
    uint16_t tot_len, id, frag_off;
    uint8_t ttl, proto;
    uint16_t checksum;
    uint32_t saddr, daddr;
} ip_hdr;

// Simple IP checksum
static inline uint16_t ip_checksum(void *vdata, size_t len){
    char *data=vdata; uint32_t sum=0;
    for(size_t i=0;i<len;i+=2){
        uint16_t w=(data[i]<<8)+(i+1<len?data[i+1]:0);
        sum+=w;
    }
    while(sum>>16) sum=(sum&0xFFFF)+(sum>>16);
    return (uint16_t)(~sum);
}

// UDP checksum
typedef struct { uint32_t saddr,daddr; uint8_t zero,proto; uint16_t udp_len; } pseudo_hdr;
static inline uint16_t chk(uint16_t *buf,int n){ uint32_t s=0; for(int i=0;i<n;i++) s+=ntohs(buf[i]); while(s>>16) s=(s&0xFFFF)+(s>>16); return htons(~s&0xFFFF);}
static inline uint16_t udp_checksum(ip_hdr *iph, udp_hdr *udph, char *data, int len){
    int size=sizeof(pseudo_hdr)+sizeof(udp_hdr)+len;
    char *buf=malloc(size);
    pseudo_hdr *ph=(pseudo_hdr*)buf;
    ph->saddr=iph->saddr; ph->daddr=iph->daddr; ph->zero=0; ph->proto=iph->proto; ph->udp_len=udph->len;
    memcpy(buf+sizeof(pseudo_hdr),udph,sizeof(udp_hdr));
    memcpy(buf+sizeof(pseudo_hdr)+sizeof(udp_hdr),data,len);
    uint16_t c=chk((uint16_t*)buf,size/2+size%2); free(buf); return c;
}

#endif

