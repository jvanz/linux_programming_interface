#pragma once

enum log_level {
	DEBUG,
	INFO,
	WARN,
	ERROR
};

struct log_entry {
	enum log_level level;
	char* message;
};
