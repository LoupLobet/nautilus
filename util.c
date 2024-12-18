#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

#define PROGNAME ""

void
error(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	(void)fprintf(stderr, "%s, error: ", PROGNAME);
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputc('\n', stderr);
	exit(1);
}

void
warning(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	(void)fprintf(stderr, "%s: warning: ", PROGNAME);
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputc('\n', stderr);
}
