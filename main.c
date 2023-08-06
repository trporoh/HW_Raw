#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <error.h>

#define port 2223

int main(int argc, char** argv) {

	struct sockaddr_in server;

	int raw_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (-1 == raw_fd) {
		perror("create socket error");
		exit(EXIT_FAILURE);
	}
	int count = 0;

	char* received = (char*)malloc(128);

	memset(&server, '\0', sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(port);

	if (-1 == bind(raw_fd, (struct sockaddr*)&server, sizeof(struct sockaddr_in))) {
		perror("Bind error");
		close(raw_fd);
		exit(EXIT_FAILURE);
	}

	while (1) {
		recvfrom(raw_fd, received, sizeof(received), 0,  NULL, NULL);
		count++;
		printf("%d\n", count);
	}

	exit(EXIT_SUCCESS);

}