#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 512
#define PORT 8088
#define TRUE 1

int main() {
	int socketfd;
	char buffer[BUF_SIZE];
	struct sockaddr_in serveraddr, clientaddr;

	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0 )) < 0) {
		perror("Cannot create the socket!");
		exit(EXIT_FAILURE);
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	memset(&clientaddr, 0, sizeof(clientaddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(PORT);

	if (bind(socketfd, (const struct sockaddr *)&serveraddr,
		sizeof(serveraddr)) < 0){
		perror("Cannot bind the socket");
		exit(EXIT_FAILURE);
	}

	while (TRUE) {
		int n;
		socklen_t len;
		n = recvfrom(socketfd, (char *)buffer, BUF_SIZE, MSG_WAITALL,
			(struct sockaddr *)&clientaddr, &len);
		buffer[n] = '\0';
		printf("Client (%d): %s\n", n, buffer);
	}
}
