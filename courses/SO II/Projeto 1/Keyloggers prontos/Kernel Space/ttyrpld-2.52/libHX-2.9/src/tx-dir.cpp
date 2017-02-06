#ifndef __cplusplus
#	include <stdio.h>
#	include <stdlib.h>
#else
#	include <cstdio>
#	include <cstdlib>
#endif
#include <libHX/misc.h>

int main(void)
{
	void *d = HXdir_open("/tmp");
	const char *n;

	printf("Available files in /tmp:\n");
	while ((n = HXdir_read(d)) != NULL)
		printf("\t" "%s\n", n);

	HXdir_close(d);
	return EXIT_SUCCESS;
}
