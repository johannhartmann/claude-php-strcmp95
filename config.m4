dnl config.m4 for the strcmp95 extension

PHP_ARG_ENABLE([strcmp95],
  [whether to enable strcmp95 support],
  [AS_HELP_STRING([--enable-strcmp95],
    [Enable strcmp95 (Jaro-Winkler similarity) support])],
  [no])

if test "$PHP_STRCMP95" != "no"; then
  dnl Define a preprocessor macro to indicate that this PHP extension is available
  AC_DEFINE([HAVE_STRCMP95], [1],
    [Define to 1 if the PHP extension 'strcmp95' is available.])

  dnl Configure extension sources
  PHP_NEW_EXTENSION([strcmp95],
    [strcmp95.c],
    [$ext_shared],
    [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1])
fi
