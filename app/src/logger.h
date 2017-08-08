#pragma once

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#define BUFFER_SIZE 256

static const char* SOCKNAME = "/tmp/logger.sock";

static int logger_initialized = 0;
static int sfd;

enum log_level {
	DEBUG,
	INFO,
	WARN,
	ERROR
};

struct log_entry {
	time_t time;
	enum log_level level;
	size_t length;
} __attribute__((packed));

/* Prototypes */
static void connect_to_logger();
static void logging(const enum log_level level, const char* message);
void debug(const char* message);

static void connect_to_logger()
{
	//create the unix socket
	struct sockaddr_un addr;
	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sfd == -1)
		err(1,NULL);
	printf("Socket created\n");

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKNAME, sizeof(addr.sun_path) -1);
	int ret = connect(sfd, (const struct sockaddr *) &addr,
		sizeof(struct sockaddr_un));
	if (ret == -1)
		err(1,NULL);
	logger_initialized = 1;
	printf("Connected to logger\n");
}

static void logging(const enum log_level level, const char* message)
{
	if (!logger_initialized)
		connect_to_logger();
	struct log_entry log;
	log.time = time(NULL);
	log.level = level;
	log.length = strlen(message);
	size_t length = sizeof(log) + log.length;
	char* buffer = (char*) malloc(length);
	memset(buffer, 0, length);
	memcpy(buffer, &log, sizeof(log));
	memcpy(buffer+sizeof(log), message, log.length);
	ssize_t ret = write(sfd, buffer, length);
	printf("Bytes written: %ld\n", ret);
	if (ret < (ssize_t) (log.length + sizeof(log)))
		printf("Cannot write to socket\n");
	else
		printf("Bytes written: %ld, message: %s, message length: %ld\n", ret, message, log.length);
	free(buffer);
	printf("Log entry sent\n");
}

void debug(const char* message)
{
	logging(DEBUG, message);
}
