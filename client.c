#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 7777
#define SPORT 8888
#define PATH "/tmp/stream_serv"
#define SADDR "127.0.0.1"
#define DADDR "127.0.0.2"
#include <linux/udp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

int
main(int argc, char* argv[])
{
    char* msg = (char*)malloc(sizeof(struct udphdr) + 28);
    char* ip = (char*)malloc(sizeof(struct iphdr) + sizeof(struct udphdr) + 28);

    ((struct iphdr*)ip)->version = 4;
    ((struct iphdr*)ip)->ihl = 5;
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

    printf("%s\n",ip);

    int fd;
    fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW); //знаю что ничего нельзя с него принять, но отправить водь можно? + автоматом выставляется IP_HDRINCL

    if (fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    //serv.sin_addr.s_addr = htonl(INADDR_LOOPBACK); Убрал, чтобы избежать повторений, так как при sendto в этой структуре так же требуется указать адрес, но в самом payload уже указан dest addr.
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