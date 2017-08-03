#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>

static int sfd; //socket file descriptor
static int epoll;

/** File descriptor where the log should be written*/
static int file;


/* Prototypes */
static int open_file(const char* path);
static void write_message(void);
static void init(void);

/**
 * Open the file used to write the log message
 */
static int open_file(const char* path)
{
	file = open(path, O_WRONLY | O_CREAT | O_APPEND,  S_IRUSR | S_IWUSR
		| S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
	return file >= 0;
}

static void write_message()
{
	dprintf(file, "Date time %s MSG...", "INFO");
}

/**
 * Function to initialize all resources pessary to the logger work properly
 */
static void init(void)
{
	//create the epoll instance to monitor the fds used by the logger
	epoll = epoll_create1(1);
	if (epoll < 0)
		exit(errno);

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = sfd;
	if (epoll_ctl(epoll, EPOLL_CTL_ADD, sfd, &ev) == -1)
		exit(errno);
}


int main(void)
{
	if (!open_file("/tmp/vanz.log"))
		exit(-1);
	init();
	write_message();
	close(file);
}
