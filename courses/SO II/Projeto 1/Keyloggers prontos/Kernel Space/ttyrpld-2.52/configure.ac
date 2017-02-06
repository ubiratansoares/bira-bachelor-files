
AC_INIT([ttyrpld], [2.52])
AC_CONFIG_HEADERS([config.h])
AC_PROG_INSTALL
AM_INIT_AUTOMAKE
AC_PROG_CC
AM_PROG_CC_C_O

AC_HAVE_HEADERS([alloca.h])
AC_CHECK_LIB([rt], [nanosleep], [ttyreplay_LIBS="$ttyreplay_LIBS -lrt"])
AC_CHECK_LIB([socket], [accept], [rpld_LIBS="$rpld_LIBS -lsocket -lnsl"])
AC_CHECK_LIB([intl], [gettext], [regular_LIBS="$regular_LIBS -lintl"])
PKG_CHECK_MODULES([libHX], [libHX >= 1.25])

AC_PATH_PROG([MSGFMT], [gmsgfmt])
if [[ -z "$MSGFMT" ]]; then
	AC_PATH_PROG([MSGFMT], [msgfmt])
fi;
AC_PATH_PROG([PERL], [perl])
if [[ -z "$PERL" ]]; then
	AC_PATH_PROG([PERL], [perl5])
fi;
AC_PATH_PROG([PHP], [php5])
if [[ -z "$PHP" ]]; then
	AC_PATH_PROG([PHP], [php])
fi;
AC_PATH_PROG([W3M], [w3m])

AM_CONDITIONAL([GOT_TOOLS_FOR_DOC],
	[$am_make -v | grep -q GNU && test -n "$PERL" -a -n "$PHP" -a -n "$W3M"])
AM_CONDITIONAL([GOT_TOOLS_FOR_LOCALE],
	[$am_make -v | grep -q GNU && test -n "$MSGFMT"])

m4_sinclude([m4lib/getpwuid.m4])

regular_CFLAGS="-D_LARGEFILE_SOURCE=1 -D_LARGE_FILES -D_FILE_OFFSET_BITS=64 \
	-D_REENTRANT -Wall -Waggregate-return -Wmissing-declarations \
	-Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes \
	-Wformat=2 -pipe"
AC_SUBST([regular_CFLAGS])
AC_SUBST([regular_LIBS])
AC_SUBST([ttyreplay_LIBS])
AC_SUBST([rpld_LIBS])
AC_OUTPUT([Makefile etc/Makefile etc/rpld user/Makefile
	doc/GNUmakefile locale/GNUmakefile])
