
#include "logger.h"

int main(void)
{
	for (int i = 0; i < 1000; ++i){
		debug("Dummy process log");
		sleep(1);
	}
}
