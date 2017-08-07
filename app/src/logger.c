#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common.h"
#include "logger.h"

static int sfd; //socket file descriptor
static int epoll; // epoll instance
/** File descriptor where the log should be written*/
static int file;

static const char* LOG_FILE = "/tmp/logger.log";
static const int BACKLOG = 32;
static const int MAX_EVENTS = 10;

/* Prototypes */
static void init(void);
static void cleanup(void);
static void write_message(struct log_entry);
static const char* get_level_str(enum log_level);
static void run(void);

static void run(void)
{
	struct epoll_event events[MAX_EVENTS];
	int ready;
	for (;;){
		ready = epoll_wait(epoll, events, MAX_EVENTS, -1);
		if (ready == -1)
			// TODO deal with the error
			continue;
		for (int i = 0; i < ready; i++){
			if (events[i].data.fd == sfd){
				// seems to be a new connection. Let's add it
				// in the epoll
				int cfd = accept(events[i].data.fd, NULL, NULL);
				if (cfd == -1){
					printf("Cannot accept connection");
				} else {
					struct epoll_event ev;
					ev.events = EPOLLIN;
					ev.data.fd = cfd;
					if (epoll_ctl(epoll, EPOLL_CTL_ADD, cfd, &ev) == -1)
						printf("epoll setup\n");
				}
			} else {
				if (events[i].events & EPOLLERR)
					// TODO handle error
					continue;
				/*printf("EPOLLERR");*/
				if (events[i].events & EPOLLHUP)
					// TODO handle hangup
					continue;
				/*printf("EPOLLHUP");*/
				if (events[i].events & (EPOLLIN | EPOLLPRI)){
					printf("EPOLLIN and EPOLLPRI");
					struct log_entry log;
					ssize_t bytes = read(events[i].data.fd, &log, sizeof(log));
					if (bytes == -1) {
						printf(strerror(errno));
					} else {
						write_message(log);
					}
				}
			}
		}
	}
}

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
	printf("Socket created");

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKNAME, sizeof(addr.sun_path) -1);

	if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
		exit_with_error();
	printf("Socket binded");

	if (listen(sfd, BACKLOG) == -1)
		exit_with_error();
	printf("Listening...\n");

	//create the epoll instance to monitor the fds used by the logger
	epoll = epoll_create1(0);
	if (epoll < 0)
		exit_with_error();
	printf("epoll created");

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = sfd;
	if (epoll_ctl(epoll, EPOLL_CTL_ADD, sfd, &ev) == -1)
		exit_with_error();
	printf("epoll setup");
}

/**
 * Function to cleanup all resources used by logger daemon
 */
static void cleanup(void)
{
	//remove the socket
	if (unlink(SOCKNAME) == -1)
		printf(strerror(errno));
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
	run();
	cleanup();
}
