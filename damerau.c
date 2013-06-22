#ifdef HAVE_CONFIG_H

#include "config.h"

#endif

#include "php.h"
#include "zend_API.h"
#include "TSRM.h"
#include "php_ini.h"
#include "php_damerau.h"
#include "ext/mbstring/mbstring.h"
#include "ext/mbstring/libmbfl/mbfl/mbfl_string.h"
#include "ext/mbstring/libmbfl/mbfl/mbfl_language.h"
#include "ext/mbstring/libmbfl/mbfl/mbfilter.h"

ZEND_BEGIN_ARG_INFO(arginfo_damerau_levenshtein, 0)
    ZEND_ARG_INFO(0, str1)
    ZEND_ARG_INFO(0, str2)
    ZEND_ARG_INFO(0, cost_ins)
    ZEND_ARG_INFO(0, cost_sub)
    ZEND_ARG_INFO(0, cost_del)
    ZEND_ARG_INFO(0, cost_tran)
ZEND_END_ARG_INFO()

static zend_function_entry damerau_levenshtein_functions[] = {

  PHP_FE(damerau_levenshtein, arginfo_damerau_levenshtein)

  {NULL, NULL, NULL}

};

/* {{{ damerau ini */



ZEND_DECLARE_MODULE_GLOBALS(damerau)

static PHP_GINIT_FUNCTION(damerau);
static PHP_RINIT_FUNCTION(damerau);

zend_module_entry damerau_module_entry = {
    STANDARD_MODULE_HEADER,
	PHP_DAMERAU_EXTNAME,
	damerau_levenshtein_functions,
	NULL, // PHP_MINIT(damerau),
	NULL, // PHP_MSHUTDOWN(damerau),
	PHP_RINIT(damerau),
	NULL, // PHP_RSHUTDOWN(damerau),
	NULL, // PHP_MINFO(damerau),
    NO_VERSION_YET,
    PHP_MODULE_GLOBALS(damerau),
    PHP_GINIT(damerau),
    NULL, // PHP_GSHUTDOWN(damerau),
    NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_DAMERAU

ZEND_GET_MODULE(damerau)

#endif


int _php_mb_ini_mbstring_internal_encoding_set(const char *new_value, uint new_value_length TSRMLS_DC)
{
    enum mbfl_no_encoding no_encoding;
  	const char *enc_name = NULL;
  	uint enc_name_len = 0;
   
  	no_encoding = new_value ? mbfl_name2no_encoding(new_value):
  				mbfl_no_encoding_invalid;
	if (no_encoding != mbfl_no_encoding_invalid) {
  		enc_name = new_value;
  		enc_name_len = new_value_length;
  	} else {
  		switch (MBSTRG_EX(language)) {
  			case mbfl_no_language_uni:
  				enc_name = "UTF-8";
  				enc_name_len = sizeof("UTF-8") - 1;
  				break;
  			case mbfl_no_language_japanese:
  				enc_name = "EUC-JP";
  				enc_name_len = sizeof("EUC-JP") - 1;
  				break;
  			case mbfl_no_language_korean:
  				enc_name = "EUC-KR";
  				enc_name_len = sizeof("EUC-KR") - 1;
  				break;
  			case mbfl_no_language_simplified_chinese:
  				enc_name = "EUC-CN";
  				enc_name_len = sizeof("EUC-CN") - 1;
  				break;
  			case mbfl_no_language_traditional_chinese:
  				enc_name = "EUC-TW";
  				enc_name_len = sizeof("EUC-TW") - 1;
  				break;
  			case mbfl_no_language_russian:
  				enc_name = "KOI8-R";
  				enc_name_len = sizeof("KOI8-R") - 1;
  				break;
  			case mbfl_no_language_german:
  				enc_name = "ISO-8859-15";
  				enc_name_len = sizeof("ISO-8859-15") - 1;
  				break;
  			case mbfl_no_language_armenian:
  				enc_name = "ArmSCII-8";
  				enc_name_len = sizeof("ArmSCII-8") - 1;
  				break;
  			case mbfl_no_language_turkish:
  				enc_name = "ISO-8859-9";
  				enc_name_len = sizeof("ISO-8859-9") - 1;
  				break;
  			default:
  				enc_name = "ISO-8859-1";
  				enc_name_len = sizeof("ISO-8859-1") - 1;
  				break;
  		}
  		no_encoding = mbfl_name2no_encoding(enc_name);
  	}
	MBSTRG_EX(internal_encoding) = no_encoding;
	MBSTRG_EX(current_internal_encoding) = no_encoding;
#if HAVE_MBREGEX
	{
		const char *enc_name = new_value;
		if (FAILURE == php_mb_regex_set_default_mbctype(enc_name TSRMLS_CC)) {
			/* falls back to EUC-JP if an unknown encoding name is given */
			enc_name = "EUC-JP";
			php_mb_regex_set_default_mbctype(enc_name TSRMLS_CC);
		}
		php_mb_regex_set_mbctype(new_value TSRMLS_CC);
	}
#endif
	return SUCCESS;
}

static PHP_INI_MH(OnUpdate_damerau_language)
{
    enum mbfl_no_language no_language;

	no_language = mbfl_name2no_language(new_value);
	if (no_language == mbfl_no_language_invalid) {
		MBSTRG_EX(language) = mbfl_no_language_neutral;
		return FAILURE;
	}
	MBSTRG_EX(language) = no_language;
    
    return SUCCESS;
}

static PHP_INI_MH(OnUpdate_damerau_internal_encoding)
{
    if (stage == PHP_INI_STAGE_STARTUP || stage == PHP_INI_STAGE_SHUTDOWN
			|| stage == PHP_INI_STAGE_RUNTIME) {
        return _php_mb_ini_mbstring_internal_encoding_set(new_value, new_value_length TSRMLS_CC);
	} else {
		return SUCCESS;
	}
}

PHP_INI_BEGIN()
	PHP_INI_ENTRY("mbstring.language", "neutral", PHP_INI_ALL, OnUpdate_damerau_language)
	PHP_INI_ENTRY("mbstring.internal_encoding", NULL, PHP_INI_ALL, OnUpdate_damerau_internal_encoding)
PHP_INI_END()

static PHP_GINIT_FUNCTION(damerau)
{
	damerau_globals->language = mbfl_no_language_uni;
	damerau_globals->internal_encoding = mbfl_no_encoding_utf8;
	damerau_globals->current_internal_encoding = damerau_globals->internal_encoding;
}

static PHP_RINIT_FUNCTION(damerau)
{
    MBSTRG_EX(current_internal_encoding) = MBSTRG_EX(internal_encoding);
	return SUCCESS;
}
/* }}} */

#define LEVENSHTEIN_MAX_LENGTH 255

/* {{{ reference_damerau_levenshtein
 * reference implementation, only optimized for memory usage and speed */
static int reference_mb_damerau_levenshtein(
        const char *f, 
        int fl, 
        const char *s, 
        int sl, 
        int cost_ins, int cost_sub, int cost_del, int cost_tran
        TSRMLS_DC
){
    
    int *currentRow, *previousRow, *transpositionRow, *tempRow;
    int i, ii, j, t, from, to, cost = 0, value;
    mbfl_string first, second;
    
    int firstLength, secondLength;
    mbfl_string_init(&first);
    first.no_language = MBSTRG_EX(language);
	first.no_encoding = MBSTRG_EX(current_internal_encoding);
    first.val = (unsigned char *)f;
    first.len = fl;

    mbfl_string_init(&second);
    second.no_language = MBSTRG_EX(language);
    second.no_encoding = MBSTRG_EX(current_internal_encoding);
    second.val = (unsigned char *)s;
    second.len = sl;

	if(first.len > second.len){
        char *tmps = first.val;
        int tmpl = first.len;
        first.len = second.len;
        first.val = second.val;
        second.len = tmpl;
        second.val = tmps;
    }

    firstLength = mbfl_strlen(&first);
    secondLength = mbfl_strlen(&second);
    
    if (firstLength == 0) {
        return secondLength * cost_ins;
    }
    if (secondLength == 0) {
        return firstLength * cost_del;
    }
    if ((firstLength > LEVENSHTEIN_MAX_LENGTH) || (secondLength > LEVENSHTEIN_MAX_LENGTH)) {
        return -1;
    }
    
    currentRow = safe_emalloc((firstLength + 1), sizeof(int), 0);
    previousRow = safe_emalloc((firstLength + 1), sizeof(int), 0);
    transpositionRow = safe_emalloc((firstLength + 1), sizeof(int), 0);

    for (i=0; i<=firstLength; i++){
        previousRow[i] = i * cost_ins;
        currentRow[i] = 0;
        transpositionRow[i] = 0;
    }
    
    mbfl_string lastFirstCh;
    mbfl_string lastSecondCh;
    mbfl_string firstCh, *retFirstCh = (mbfl_string *)NULL;
    mbfl_string secondCh, *retSecondCh = (mbfl_string *)NULL;
    
    mbfl_string_init(&firstCh);
    mbfl_string_init(&secondCh);
    mbfl_string_init(&lastFirstCh);
    mbfl_string_init(&lastSecondCh);
    firstCh.no_language = MBSTRG_EX(language);
    firstCh.no_encoding = MBSTRG_EX(current_internal_encoding);
    secondCh.no_language = MBSTRG_EX(language);
    secondCh.no_encoding = MBSTRG_EX(current_internal_encoding);
    lastFirstCh.no_language = MBSTRG_EX(language);
    lastFirstCh.no_encoding = MBSTRG_EX(current_internal_encoding);
    lastSecondCh.no_language = MBSTRG_EX(language);
    lastSecondCh.no_encoding = MBSTRG_EX(current_internal_encoding);
    
    for(i = 1; i <= secondLength; i++){
        retSecondCh = mbfl_substr(&second, &secondCh, i-1, 1);
        currentRow[0] = i;
        
        from = i-secondLength - 1;
        if(from < 1) from = 1;
        to = i+secondLength + 1;
        if(to > firstLength) to = firstLength;
        
        for(j=from; j <= to; j++){
            retFirstCh = mbfl_substr(&first, &firstCh, j-1, 1);
            cost = strcmp(retFirstCh->val,retSecondCh->val) == 0? 0 : 1;
            value = currentRow[j-1] + cost_ins; // insertion
            t = previousRow[j] + cost_del; // deletion
            if(value > t) value = t;
            t = previousRow[j-1] + (cost?cost_sub:0); // substitution
            if(value > t) value = t;
            if( j>from && i > 1)
            {
                if(        (strcmp(lastSecondCh.val,retFirstCh->val) == 0) 
                        && (strcmp(lastFirstCh.val,retSecondCh->val) == 0)
                ){
                    t = transpositionRow[j-2] + cost_tran; // transposition
                    if(value > t) value = t; 
                }
            }
            
            currentRow[j] = value;
            mbfl_string_clear(&lastFirstCh);
            lastFirstCh.val = retFirstCh->val;
            lastFirstCh.len = retFirstCh->len;
        }

        mbfl_string_clear(&lastSecondCh);
        lastSecondCh.val = retSecondCh->val;
        lastSecondCh.len = retSecondCh->len;
        
        tempRow = transpositionRow;
        transpositionRow = previousRow;
        previousRow = currentRow;
        currentRow = tempRow;
        for (ii=0; ii<=firstLength; ii++){
            currentRow[ii] = 0;
        }
    }
    
    mbfl_string_clear(&lastFirstCh);
    mbfl_string_clear(&lastSecondCh);
    
    cost = previousRow[firstLength];

    efree(currentRow);
    efree(previousRow);
    efree(transpositionRow);
    
    return cost;
    
}

/* }}} */

/* {{{ proto int levenshtein(string str1, string str2[, int cost_ins, int cost_sub, int cost_del, int cost_tran])
 Calculate Damerau - Levenshtein distance between two strings */
PHP_FUNCTION(damerau_levenshtein)
{
    int argc = ZEND_NUM_ARGS();
    char *str1, *str2;
    int str1_len, str2_len;
    long cost_ins, cost_sub, cost_del, cost_tran;
    int distance = -1;
    
    switch (argc) {
        case 2: /* just two strings: use maximum performance version */
            if (zend_parse_parameters(2 TSRMLS_CC, "ss", &str1, &str1_len, &str2, &str2_len) == FAILURE) {
                return;
            }
            distance = reference_mb_damerau_levenshtein(str1, str1_len, str2, str2_len, 1, 1, 1, 1 TSRMLS_CC);
            break;
            
        case 6: /* more general version: calc cost by ins/rep/del weights */
            if (zend_parse_parameters(6 TSRMLS_CC, "ssllll", &str1, &str1_len, &str2, &str2_len, &cost_ins, &cost_sub, &cost_del, &cost_tran) == FAILURE) {
                return;
            }
            distance = reference_mb_damerau_levenshtein(str1, str1_len, str2, str2_len, cost_ins, cost_sub, cost_del, cost_tran TSRMLS_CC);
            break;
            
        default:
            WRONG_PARAM_COUNT;
    }
    
    if (distance < 0 && /* TODO */ ZEND_NUM_ARGS() != 3) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Argument string(s) too long");
    }
    
    RETURN_LONG(distance);
}
/* }}} */

