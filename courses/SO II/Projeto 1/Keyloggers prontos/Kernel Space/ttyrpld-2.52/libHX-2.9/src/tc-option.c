/*
A=b;C="d" ; E = "F;" ;
*/
/*
 *	option parser test program
 *	written by Jan Engelhardt
 *	this program is released in the Public Domain
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <libHX/arbtree.h>
#include <libHX/option.h>
#include <libHX.h>

static void t_format(int argc)
{
	struct HXbtree *fmt = HXformat_init();
	HXformat_add(fmt, "USER", "jengelh", HXTYPE_STRING | HXFORMAT_IMMED);
	HXformat_add(fmt, "ARGC", &argc, HXTYPE_INT);
	HXformat_add(fmt, "ARGK", (const void *)(long)argc, HXTYPE_INT | HXFORMAT_IMMED);
	HXformat_add(fmt, "ZERO", "", HXTYPE_STRING | HXFORMAT_IMMED);
	HXformat_add(fmt, "ONE", "1", HXTYPE_STRING | HXFORMAT_IMMED);
	++argc;
	HXformat_fprintf(fmt, stdout, "/%(HOME)/%(lower USER)/%(ARGC).%(ARGK)\n");
	HXformat_fprintf(fmt, stdout, ">%(ZERO)<\n");
	HXformat_fprintf(fmt, stdout, ">%(before=\"-o\" ZERO)<\n\n");
	HXformat_fprintf(fmt, stdout, ">%(ifempty=\"zero is empty\" ZERO)<\n");
	HXformat_fprintf(fmt, stdout, ">%(ifnempty=\"zero is not empty\" ZERO)<\n");
	HXformat_fprintf(fmt, stdout, ">%(ifempty=\"one is empty\" ONE)<\n");
	HXformat_fprintf(fmt, stdout, ">%(ifnempty=\"one is not empty\" ONE)<\n");
	HXbtree_free(fmt);
}

static void t_shconfig(const char *file)
{
	char *A, *C, *E;
	struct HXoption opt_tab[] = {
		{.ln = "A", .type = HXTYPE_STRING, .ptr = &A},
		{.ln = "C", .type = HXTYPE_STRING, .ptr = &C},
		{.ln = "E", .type = HXTYPE_STRING, .ptr = &E},
		HXOPT_TABLEEND,
	};
	if (HX_shconfig(file, opt_tab) < 0)
		fprintf(stderr, "Read error %s: %s\n", file, strerror(errno));
	else
		printf("A => >%s<\n" "C => >%s<\n" "E => >%s<\n", A, C, E);
}

static int opt_v = 0, opt_mask = 0;
static char *opt_kstr = NULL;
static long opt_klong = 0;
static double opt_kdbl = 0;
static int opt_kflag = 0, opt_kint = 0;
static int opt_dst = 0;

static void opt_cbf(const struct HXoptcb *cbi)
{
	printf("cbf was called... with \"%s\"/'%c'\n",
	       cbi->current->ln, cbi->current->sh);
}

static const char *opt_eitheror[] = {"neither", "either", "or"};
static struct HXoption table[] = {
	{.ln = "dbl", .type = HXTYPE_DOUBLE, .cb = opt_cbf,
	 .ptr = &opt_kdbl, .help = "Callback function for doubles"},
	{.ln = "flag", .sh = 'F', .type = HXTYPE_NONE, .cb = opt_cbf,
	 .ptr = &opt_kflag, .help = "Callback function for flags"},
	{.ln = "long", .sh = 'L', .type = HXTYPE_LONG, .cb = opt_cbf,
	 .ptr = &opt_klong, .help = "Callback function for integers"},
	{.ln = "str", .sh = 'S', .type = HXTYPE_STRING, .cb = opt_cbf,
	 .ptr = &opt_kstr, .help = "Callback function for strings"},
	{.ln = "either", .type = HXTYPE_VAL, .cb = opt_cbf, .ptr = &opt_dst,
	 .val = 1, .help = "Mutually exclusive selection: either | or"},
	{.ln = "or", .type = HXTYPE_VAL, .ptr = &opt_dst, .val = 2,
	 .cb = opt_cbf, .help = "Mutually exclusive selection: either | or"},
	{.ln = "quiet", .sh = 'q', .type = HXOPT_DEC, .ptr = &opt_v,
	 .cb = opt_cbf, .help = "Decrease verbosity"},
	{.ln = "verbose", .sh = 'v', .type = HXOPT_INC, .ptr = &opt_v,
	 .cb = opt_cbf, .help = "Increase verbosity"},
	{.sh = 'A', .type = HXTYPE_INT | HXOPT_AND, .ptr = &opt_mask,
	 .cb = opt_cbf, .help = "AND mask test", .htyp = "value"},
	{.sh = 'O', .type = HXTYPE_INT | HXOPT_OR, .ptr = &opt_mask,
	 .cb = opt_cbf, .help = "OR mask test", .htyp = "value"},
	{.sh = 'X', .type = HXTYPE_INT | HXOPT_XOR, .ptr = &opt_mask,
	 .cb = opt_cbf, .help = "XOR mask test", .htyp = "value"},
	{.sh = 'G', .type = HXTYPE_NONE, .help = "Just a flag", .cb = opt_cbf},
	{.sh = 'H', .type = HXTYPE_NONE, .help = "Just a flag", .cb = opt_cbf},
	{.sh = 'I', .type = HXTYPE_NONE, .help = "Just a flag", .cb = opt_cbf},
	{.sh = 'J', .type = HXTYPE_NONE, .help = "Just a flag", .cb = opt_cbf},
	HXOPT_AUTOHELP,
	HXOPT_TABLEEND,
};

int main(int argc, const char **argv)
{
	t_format(argc);
	t_shconfig((argc >= 2) ? argv[1] : "t-option.c");

	printf("Return value of HX_getopt: %d\n",
	       HX_getopt(table, &argc, &argv, HXOPT_USAGEONERR));
	printf("Either-or is: %s\n", opt_eitheror[opt_dst]);
	printf("values: D=%lf I=%d L=%ld S=%s\n",
	       opt_kdbl, opt_kint, opt_klong, opt_kstr);
	printf("Verbosity level: %d\n", opt_v);
	printf("Mask: 0x%08X\n", opt_mask);
	return EXIT_SUCCESS;
}
