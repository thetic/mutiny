#ifndef IEEE754EXCEPTIONSPLUGIN_H
#define IEEE754EXCEPTIONSPLUGIN_H

#ifdef __cplusplus
extern "C"
{
#endif

  void set_divisionbyzero_c(void);
  void set_overflow_c(void);
  void set_underflow_c(void);
  void set_inexact_c(void);
  void set_nothing_c(void);
  void set_everything_c(void);

#ifdef __cplusplus
}
#endif

#endif /* IEEE754EXCEPTIONSPLUGIN_H */
