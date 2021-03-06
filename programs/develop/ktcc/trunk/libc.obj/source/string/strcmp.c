/* strcmp( const char *, const char * )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <errno.h>
#include <string.h>
#include <sys/ksys.h>

int strcmp(const char * s1, const char * s2)
{
    return __strcmp(s1, s2);
}