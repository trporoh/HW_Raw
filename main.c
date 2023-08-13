#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 8888
#define PATH "/tmp/stream_serv"
#define ADDR "127.0.0.5"
#include <linux/udp.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

char SMAC[6] = { 0x28, 0xd0, 0xea, 0x8c, 0xb7, 0x8e };

int
main(int argc, char** argv)
{
    struct sockaddr_ll serv, client;

    socklen_t addrlen = sizeof(serv);

    int fd;
    char* msg = (char*)malloc(sizeof(struct udphdr) + 20);
    char* buf = (char*)malloc(sizeof(struct udphdr) + 42);

    memset(&client, '\0', sizeof(struct sockaddr_in));

    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv.sll_family = AF_INET;
    serv.sll_halen = 6;
    serv.sll_ifindex = 2;
    strcpy(serv.sll_addr, SMAC);
    
    bind(fd, (struct sockaddr*)&serv, addrlen);

    while (1)
    {
        if (recvfrom(fd, (void*)buf, sizeof(struct udphdr) + 42, 0,
            (struct sockaddr*)&client, &addrlen)
            == -1)
        {
            perror("recv");
            exit(EXIT_FAILURE);
        };
        char* bufp = buf + 20;

        printf("src-port = %d\ndst-port = %d\nlenght = %d\n check = "
            "%d\nmessage:%s\n\n",
            htons(((struct udphdr*)bufp)->source),
            htons(((struct udphdr*)bufp)->dest),
            htons(((struct udphdr*)bufp)->len),
            htons(((struct udphdr*)bufp)->check),
            (bufp + sizeof(struct udphdr)));
    }
    
    printf("program finished!!!\n");
    getchar();
    close(fd);

    exit(EXIT_SUCCESS);
}