/*
 * gmtime.c
 * Original Author:	G. Haley
 *
 * Converts the calendar time pointed to by tim_p into a broken-down time
 * expressed as Greenwich Mean Time (GMT). Returns a pointer to a structure
 * containing the broken-down time, or a null pointer if GMT is not
 * available.
 */

/*
FUNCTION
<<gmtime>>---convert time to UTC traditional form

INDEX
	gmtime
INDEX
	gmtime_r

ANSI_SYNOPSIS
	#include <time.h>
	struct tm *gmtime(const time_t *<[clock]>);
	struct tm *gmtime_r(const time_t *<[clock]>, struct tm *<[res]>);

TRAD_SYNOPSIS
	#include <time.h>
	struct tm *gmtime(<[clock]>)
	const time_t *<[clock]>;
	struct tm *gmtime_r(<[clock]>, <[res]>)
	const time_t *<[clock]>;
	struct tm *<[res]>;

DESCRIPTION
<<gmtime>> takes the time at <[clock]> representing the number
of elapsed seconds since 00:00:00 on January 1, 1970, Universal
Coordinated Time (UTC, also known in some countries as GMT,
Greenwich Mean time) and converts it to a <<struct tm>>
representation.

<<gmtime>> constructs the traditional time representation in static
storage; each call to <<gmtime>> or <<localtime>> will overwrite the
information generated by previous calls to either function.

RETURNS
A pointer to the traditional time representation (<<struct tm>>).

PORTABILITY
ANSI C requires <<gmtime>>.

<<gmtime>> requires no supporting OS subroutines.
*/

#include <stdlib.h>
#include <time.h>

#define _GMT_OFFSET 0

#ifndef _REENT_ONLY

struct tm *
_DEFUN (gmtime, (tim_p),
	_CONST time_t * tim_p)
{
  _REENT_CHECK_TM(_REENT);
  return gmtime_r (tim_p, (struct tm *)_REENT_TM(_REENT));
}

#endif
