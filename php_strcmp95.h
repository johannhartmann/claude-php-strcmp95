/* strcmp95 extension for PHP */

#ifndef PHP_STRCMP95_H
# define PHP_STRCMP95_H

extern zend_module_entry strcmp95_module_entry;
# define phpext_strcmp95_ptr &strcmp95_module_entry

# define PHP_STRCMP95_VERSION "1.0.0"

# if defined(ZTS) && defined(COMPILE_DL_STRCMP95)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

PHP_FUNCTION(strcmp95);
PHP_RINIT_FUNCTION(strcmp95);
PHP_MINFO_FUNCTION(strcmp95);

#endif	/* PHP_STRCMP95_H */
