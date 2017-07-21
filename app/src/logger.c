#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/** File descriptor where the log should be written*/
static int file;

/* Prototypes */
static int open_file(const char* path);
static void write_message();

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


int main(void)
{
	if (!open_file("/tmp/vanz.log"))
		exit(-1);
	write_message();
	close(file);
}
