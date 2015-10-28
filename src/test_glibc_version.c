#include <gnu/libc-version.h>

int main(int argc, char ** argv)
{
	printf("The glibc is: %s", gnu_get_libc_version());
}
