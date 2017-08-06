#pragma once

#include <time.h>

enum log_level {
	DEBUG,
	INFO,
	WARN,
	ERROR
};

struct log_entry {
	time_t time;
	enum log_level level;
	char* message;
};
