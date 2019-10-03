#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 512
#define PORT 8088

int main() {
	int socketfd;
	struct sockaddr_in serveraddr;

	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0 )) < 0) {
		perror("Cannot create the socket!");
		exit(EXIT_FAILURE);
	}

	memset(&serveraddr, 0, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(PORT);

	char *msg = "Message from the client";
	sendto(socketfd, (const char*)msg, strlen(msg), MSG_CONFIRM,
		(const struct sockaddr*) &serveraddr, sizeof(serveraddr));
	printf("Message sent\n");

	sleep(35);

	close(socketfd);
	return 0;
}
