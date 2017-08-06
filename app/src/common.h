#pragma once

void debug(const char* msg)
{
	printf(msg);
	printf("\n");
}

void exit_with_error()
{
	debug(strerror(errno));
	exit(errno);
}
