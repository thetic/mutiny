#include "hello.h"

#include <stdio.h>

void print_hello_world(void)
{
  print_formated("Hello World!\n");
}

int (*print_formated)(const char*, ...) = printf;
