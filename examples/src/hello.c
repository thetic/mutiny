#include "hello.h"

#include <stdio.h>

void
printHelloWorld(void)
{
  PrintFormated("Hello World!\n");
}

int (*PrintFormated)(const char*, ...) = printf;
