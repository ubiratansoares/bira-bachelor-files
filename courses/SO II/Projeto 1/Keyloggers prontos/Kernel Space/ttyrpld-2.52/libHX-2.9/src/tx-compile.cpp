/* This file is for testing the cumulative include */
#ifndef __cplusplus
#	include <stdlib.h>
#else
#	include <cstdlib>
#endif
#include <libHX.h>
#include <libHX/clist.h>
#include <libHX/defs.h>
#include <libHX/list.h>
#include <libHX/ctype_helper.h>

#define ZZ 64

int main(void)
{
	printf(HX_STRINGIFY(1234+2 +2) "," HX_STRINGIFY(ZZ) "\n");
	return EXIT_SUCCESS;
}
