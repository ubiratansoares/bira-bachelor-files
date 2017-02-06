
AC_DEFUN([HAVE_GETPWUID], [AC_CHECK_FUNCS([getpwuid_r], [

	AC_LANG_PUSH(C)
	AC_MSG_CHECKING([getpwuid])
	AC_COMPILE_IFELSE(
		AC_LANG_PROGRAM([
			#define _GNU_SOURCE 1
			#include <sys/types.h>
			#include <pwd.h>
			extern int getpwuid_r(uid_t,
				struct passwd *,
				char *, size_t,
				struct passwd **);
		], []),
		[
			AC_DEFINE(HAVE_GETPWUID5, 1, [Set if we have GNU getpwuid_r])
			AC_MSG_RESULT([int getpwuid_r(uid_t, struct passwd *, char *, size_t, struct passwd **)])
			cv_getpwuid_signature="getpwuid5";
		], [])

	if [[ -z "$cv_getpwuid_signature" ]]; then
	AC_COMPILE_IFELSE(
		AC_LANG_PROGRAM([
			#include <pwd.h>
			extern struct passwd *getpwuid_r(uid_t,
				struct passwd *, char *, int);
		], []),
		[
			AC_DEFINE(HAVE_GETPWUID4, 1, [Set if we have Solaris getpwuid_r])
			AC_MSG_RESULT([struct passwd *getpwuid_r(uid_t, struct passwd *, char *, int)])
			cv_getpwuid_signature="getpwuid4";
		], [])
	fi;

	if [[ -z "$cv_getpwuid_signature" ]]; then
		AC_MSG_RESULT([getpwuid(uid_t)])
	fi;

	AC_LANG_POP(C)
], [
	AC_MSG_CHECKING([getpwuid])
	AC_MSG_RESULT([getpwuid(uid_t)])
])])
HAVE_GETPWUID
