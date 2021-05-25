#include <stdio.h>
#include <string.h>
#include "conio.h"
#include <errno.h>
#include <limits.h>

char *gets(char* str)
{
    con_init();
    if(con_gets(str, STDIO_MAX_MEM)==NULL){
        errno = EIO;
        return NULL;
    }
    str[strlen(str)-1]='\0';
    return str;
}