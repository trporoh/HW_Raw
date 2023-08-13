#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 7777
#define SPORT 8888
#define SADDR "127.0.0.2"
#define DADDR "127.0.0.5"
#include <linux/udp.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

char SMAC[6] = {0x08, 0x00, 0x27, 0x93, 0x07, 0xba};
char DMAC[6] = {0x28, 0xd0, 0xea, 0x8c, 0xb7, 0x8e};

int
main(int argc, char* argv[])
{
    char* msg = (char*)malloc(sizeof(struct udphdr) + 28);
    char* ip = (char*)malloc(sizeof(struct iphdr) + sizeof(struct udphdr) + 28);
    char* link = (char*)malloc(sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + 28);

    strncpy(((struct ethhdr*)link)->h_source, SMAC, 6);
    strncpy(((struct ethhdr*)link)->h_dest, DMAC, 6);
    ((struct ethhdr*)link)->h_proto = htons(0x08);

    ((struct iphdr*)ip)->version = 0b0100;
    ((struct iphdr*)ip)->ihl = 0b0101;
    ((struct iphdr*)ip)->tos = 0;
    ((struct iphdr*)ip)->tot_len = 40;
    ((struct iphdr*)ip)->id = htons(15);
    ((struct iphdr*)ip)->frag_off = htons(0);
    ((struct iphdr*)ip)->ttl = 4;
    ((struct iphdr*)ip)->protocol = IPPROTO_UDP;
    ((struct iphdr*)ip)->check = 0;
    ((struct iphdr*)ip)->saddr = inet_addr(SADDR);
    ((struct iphdr*)ip)->daddr = inet_addr(DADDR);


    ((struct udphdr*)msg)->source = htons(PORT);
    ((struct udphdr*)msg)->dest = htons(SPORT);
    ((struct udphdr*)msg)->len = htons(sizeof(struct udphdr) + 20);
    ((struct udphdr*)msg)->check = htons(0);


    strcpy(ip + sizeof(struct iphdr), msg);
    strcpy(ip + sizeof(struct iphdr) + sizeof(struct udphdr), "HELLO!?");
    strcpy(link + sizeof(struct ethhdr), ip);
    printf("%s\n",link);

    int fd;
    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_ll serv;
    serv.sll_family = AF_PACKET;
    serv.sll_halen = 6;
    serv.sll_ifindex = 2;
    strcpy(serv.sll_addr, DMAC);

    
    socklen_t addrlen = sizeof(serv);

    if (sendto(fd, (void*)ip, sizeof(struct udphdr) + sizeof(struct iphdr) + 20, 0, (struct sockaddr*)&serv, addrlen)
        == -1)
    {
        perror("send");
        exit(EXIT_FAILURE);
    }
    close(fd);

    exit(EXIT_SUCCESS);
}
