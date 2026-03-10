/******************************************************************************
 *
 * PlatformSpecificFunctions_c.H
 *
 * Provides an interface for when working with pure C
 *
 *******************************************************************************/

#ifndef PLATFORMSPECIFICFUNCTIONS_C_H_
#define PLATFORMSPECIFICFUNCTIONS_C_H_

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

  /* Jumping operations. They manage their own jump buffers */
  extern int (*PlatformSpecificSetJmp)(void (*function)(void*), void* data);
  extern void (*PlatformSpecificLongJmp)(void);
  extern void (*PlatformSpecificRestoreJumpBuffer)(void);

  /* Time operations */
  extern unsigned long (*GetPlatformSpecificTimeInMillis)(void);
  extern const char* (*GetPlatformSpecificTimeString)(void);

  /* String operations */
  extern int (*PlatformSpecificVSNprintf)(char* str,
      size_t size,
      const char* format,
      va_list va_args_list);

  /* Misc */
  extern double (*PlatformSpecificFabs)(double d);
  extern int (*PlatformSpecificIsNan)(double d);
  extern int (*PlatformSpecificIsInf)(double d);
  extern int (*PlatformSpecificAtExit)(void (*func)(void));

  /* IO operations */
  typedef void* PlatformSpecificFile;

  extern PlatformSpecificFile PlatformSpecificStdOut;

  extern PlatformSpecificFile (
      *PlatformSpecificFOpen)(const char* filename, const char* flag);
  extern void (
      *PlatformSpecificFPuts)(const char* str, PlatformSpecificFile file);
  extern void (*PlatformSpecificFClose)(PlatformSpecificFile file);

  extern void (*PlatformSpecificFlush)(void);

  /* Random operations */
  extern void (*PlatformSpecificSrand)(unsigned int);
  extern int (*PlatformSpecificRand)(void);

  /* Dynamic Memory operations */
  extern void* (*PlatformSpecificMalloc)(size_t size);
  extern void* (*PlatformSpecificRealloc)(void* memory, size_t size);
  extern void (*PlatformSpecificFree)(void* memory);
  extern void* (*PlatformSpecificMemCpy)(void* s1, const void* s2, size_t size);
  extern void* (*PlatformSpecificMemset)(void* mem, int c, size_t size);

  extern void (*PlatformSpecificAbort)(void);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORMSPECIFICFUNCTIONS_C_H_ */
