#include <panic.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

void panic(const char *message)
{
    perror(message);
    exit(1);
}