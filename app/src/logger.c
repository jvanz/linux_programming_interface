#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>

#include "common.h"
#include "logger.h"

static int sfd; //socket file descriptor
static int epoll; // epoll instance
/** File descriptor where the log should be written*/
static int file;

static const char* LOG_FILE = "/tmp/logger.log";
static const char* SOCKNAME = "/tmp/logger.sock";

/* Prototypes */
static void init(void);
static void cleanup(void);
static void write_message(struct log_entry);
static const char* get_level_str(enum log_level);

static void write_message(struct log_entry log)
{
	char* time = ctime(&log.time);
	time[24] = '\0';
	dprintf(file, "%s (%d) %s: %s\n", time, getpid(),
		get_level_str(log.level), log.message);
}

static const char* get_level_str(enum log_level level)
{
	if (level == DEBUG)
		return "DEBUG";
	if (level == INFO)
		return "INFO";
	if (level == WARN)
		return "WARN";
	if (level == ERROR)
		return "ERROR";
	return "UNKNOWN";
}

/**
 * Function to initialize all resources pessary to the logger work properly
 */
static void init(void)
{
	//make sure the socket are closed before try to create it again
	cleanup();

	//open file to write the log messages
	file = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND,  S_IRUSR | S_IWUSR
		| S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
	if (file == -1)
		exit_with_error();

	//create the unix socket
	struct sockaddr_un addr;
	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sfd == -1)
		exit_with_error();
	debug("Socket created");

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKNAME, sizeof(addr.sun_path) -1);

	if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
		exit_with_error();
	debug("Socket binded");

	//create the epoll instance to monitor the fds used by the logger
	epoll = epoll_create1(0);
	if (epoll < 0)
		exit_with_error();
	debug("epoll created");

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = sfd;
	if (epoll_ctl(epoll, EPOLL_CTL_ADD, sfd, &ev) == -1)
		exit_with_error();
	debug("epoll setup");
}

/**
 * Function to cleanup all resources used by logger daemon
 */
static void cleanup(void)
{
	//remove the socket
	if (unlink(SOCKNAME) == -1)
		debug(strerror(errno));
	// close log file descriptor
	close(file);
}

int main(void)
{
	init();
	struct log_entry log;
	log.time = time(NULL);
	log.level = DEBUG;
	log.message = "Testing...";
	write_message(log);
	cleanup();
}
