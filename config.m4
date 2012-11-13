PHP_ARG_ENABLE(damerau, whether to enable damerau support,
[ --enable-damerau Enable damerau support])

if test "$PHP_DAMERAU" = "yes"; then
 AC_DEFINE(HAVE_DAMERAU, 1, [Whether you have Hello World])
 PHP_NEW_EXTENSION(damerau, damerau.c, $ext_shared)
fi
