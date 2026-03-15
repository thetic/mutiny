#ifndef INCLUDED_HELLO_H
#define INCLUDED_HELLO_H

#ifdef __cplusplus
extern "C"
{
#endif

  void print_hello_world(void);

  extern int (*print_formated)(const char*, ...);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDED_HELLO_H*/
