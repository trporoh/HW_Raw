#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXB 100
#define PORT 8888
#define PATH "/tmp/stream_serv"
#include <linux/udp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int
main(int argc, char* argv[])
{
    struct sockaddr_in serv, client;

    socklen_t addrlen = sizeof(serv);

    int fd;
    char* msg = (char*)malloc(sizeof(struct udphdr) + 20);
    char* buf = (char*)malloc(sizeof(struct udphdr) + 42);
    char hi[MAXB];

    memset(&client, '\0', sizeof(struct sockaddr_in));

    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    
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
        char* bufp = buf + 28;
        printf("[!]Packet received: %s\n", bufp);
    }
    
    printf("program finished!!!\n");
    getchar();
    close(fd);

    exit(EXIT_SUCCESS);
}