#ifndef PHP_DAMERAU_H
#define PHP_DAMERAU_H 1
#define PHP_DAMERAU_VERSION "1.4"
#define PHP_DAMERAU_EXTNAME "damerau"

#include "ext/mbstring/libmbfl/mbfl/mbfl_string.h"
#include "ext/mbstring/libmbfl/mbfl/mbfl_language.h"
#include "ext/mbstring/libmbfl/mbfl/mbfilter.h"
#include "TSRM.h"

PHP_FUNCTION(damerau_levenshtein);

extern zend_module_entry damerau_module_entry;
#define phpext_damerau_ptr &damerau_module_entry

ZEND_BEGIN_MODULE_GLOBALS(damerau)
	enum mbfl_no_language language;
	enum mbfl_no_encoding internal_encoding;
	enum mbfl_no_encoding current_internal_encoding;
ZEND_END_MODULE_GLOBALS(damerau)

#ifdef ZTS
#define MBSTRG_EX(v) TSRMG(damerau_globals_id, zend_damerau_globals *, v)
#else
#define MBSTRG_EX(v) (damerau_globals.v)
#endif

#endif
