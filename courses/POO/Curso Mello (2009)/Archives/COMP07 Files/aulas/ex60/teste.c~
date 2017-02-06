#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

	FILE *fp = fopen("/tmp/tmp2.txt", "r+");

	if (fp == NULL) {
		switch (errno) {
			case 1: break;
			case 2: printf("codigo de erro = %d\n", errno);
				break;
		}
	}

	return 0;
}
