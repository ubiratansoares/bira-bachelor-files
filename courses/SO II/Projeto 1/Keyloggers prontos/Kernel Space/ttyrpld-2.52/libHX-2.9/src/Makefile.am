# -*- Makefile -*-

SUBDIRS = include

AM_CFLAGS        = ${regular_CFLAGS} ${GCC_FVISIBILITY_HIDDEN} -Iinclude
AM_CXXFLAGS      = ${regular_CXXFLAGS} ${GCC_FVISIBILITY_HIDDEN} -Iinclude

lib_LTLIBRARIES  = libHX.la
libHX_la_SOURCES = arbtree.c deque.c dir.c dl.c format.c mc.c misc.c opt.c \
                   rand.c string.c ${mingw32_extra} ${proc_sources}
libHX_la_LIBADD  = ${regular_LIBS}
libHX_la_LDFLAGS = -version-info 21:0:3

if MINGW32
mingw32_extra    = ux-file.c ux-mmap.c
endif
if B_PROC
proc_sources     = proc.c
endif

noinst_PROGRAMS    = tc-compile tx-compile
noinst_PROGRAMS   += tc-arbtree tx-arbtree
noinst_PROGRAMS   += tc-cast tx-cast
noinst_PROGRAMS   += tc-deque tx-deque
noinst_PROGRAMS   += tc-list tx-list
noinst_PROGRAMS   += tc-misc tx-misc
noinst_PROGRAMS   += tc-option tx-option
noinst_PROGRAMS   += tc-proc tx-proc
noinst_PROGRAMS   += tc-dir tx-dir
noinst_PROGRAMS   += tc-string tx-string
tc_arbtree_LDADD   = libHX.la -lm
tc_cast_LDADD      = -lm
tc_compile_LDADD   = libHX.la
tc_dir_LDADD       = libHX.la
tc_list_LDADD      = libHX.la
tc_misc_LDADD      = libHX.la
tc_option_LDADD    = libHX.la
tc_proc_LDADD      = libHX.la
tc_string_LDADD    = libHX.la
tx_arbtree_SOURCES = tx-arbtree.cpp
tx_arbtree_LDADD   = libHX.la -lm
tx_cast_SOURCES    = tx-cast.cpp
tx_cast_LDADD      = -lm
tx_compile_SOURCES = tx-compile.cpp
tx_compile_LDADD   = libHX.la
tx_deque_SOURCES   = tx-deque.cpp
tx_dir_SOURCES     = tx-dir.cpp
tx_dir_LDADD       = libHX.la
tx_list_SOURCES    = tx-list.cpp
tx_list_LDADD      = libHX.la
tx_misc_SOURCES    = tx-misc.cpp
tx_misc_LDADD      = libHX.la
tx_option_SOURCES  = tx-option.cpp
tx_proc_SOURCES    = tx-proc.cpp
tx_proc_LDADD      = libHX.la
tx_string_SOURCES  = tx-string.cpp
tx_string_LDADD    = libHX.la
