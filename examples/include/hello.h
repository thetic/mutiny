#ifndef HELLO_H_
#define HELLO_H_

#ifdef __cplusplus
extern "C"
{
#endif

  void printHelloWorld(void);

  extern int (*PrintFormated)(const char*, ...);

#ifdef __cplusplus
}
#endif

#endif /*HELLO_H_*/
