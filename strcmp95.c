/* strcmp95 extension for PHP */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_strcmp95.h"
#include "strcmp95_arginfo.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/**
 * Helper function to find the maximum of two integers
 */
static inline int max(int a, int b) {
	return (a > b) ? a : b;
}

/**
 * Helper function to find the minimum of two integers
 */
static inline int min(int a, int b) {
	return (a < b) ? a : b;
}

/**
 * Helper function for the Jaro-Winkler algorithm to determine if
 * two characters are considered matching
 */
static inline int is_match(char s1_char, char s2_char, int pos1, int pos2, int range) {
	return s1_char == s2_char && abs(pos1 - pos2) <= range;
}

/**
 * Calculate the Jaro similarity score between two strings
 */
static double jaro_similarity(const char *s1, const char *s2, int len1, int len2) {
	// If both strings are empty, they're identical
	if (len1 == 0 && len2 == 0) {
		return 1.0;
	}

	// If one string is empty, no similarity
	if (len1 == 0 || len2 == 0) {
		return 0.0;
	}

	// Calculate the matching range (half the length of the longer string)
	int match_range = max(len1, len2) / 2 - 1;
	if (match_range < 0) match_range = 0;

	// Allocate arrays to track matches
	char *s1_matches = ecalloc(len1, sizeof(char));
	char *s2_matches = ecalloc(len2, sizeof(char));

	// Count matches
	int matches = 0;
	for (int i = 0; i < len1; i++) {
		// Calculate the lower and upper bounds of the matching window
		int start = max(0, i - match_range);
		int end = min(i + match_range + 1, len2);

		for (int j = start; j < end; j++) {
			// If s2[j] has not been matched yet and characters match
			if (!s2_matches[j] && s1[i] == s2[j]) {
				s1_matches[i] = 1;
				s2_matches[j] = 1;
				matches++;
				break;
			}
		}
	}

	// If no matches were found, the strings are completely different
	if (matches == 0) {
		efree(s1_matches);
		efree(s2_matches);
		return 0.0;
	}

	// Count transpositions
	int transpositions = 0;
	int k = 0;

	for (int i = 0; i < len1; i++) {
		if (s1_matches[i]) {
			int j;
			for (j = k; j < len2; j++) {
				if (s2_matches[j]) {
					k = j + 1;
					break;
				}
			}
			if (s1[i] != s2[j]) {
				transpositions++;
			}
		}
	}

	// Calculate Jaro similarity
	double jaro_similarity = (
		(double)matches / len1 +
		(double)matches / len2 +
		(double)(matches - transpositions / 2.0) / matches
	) / 3.0;

	efree(s1_matches);
	efree(s2_matches);

	return jaro_similarity;
}

/**
 * Calculate the Jaro-Winkler similarity (strcmp95) between two strings
 *
 * The Jaro-Winkler similarity is an extension of the Jaro similarity,
 * giving more weight to common prefixes.
 */
static double jaro_winkler_similarity(const char *s1, const char *s2, int len1, int len2, double prefix_weight) {
	// Calculate the basic Jaro similarity
	double jaro_sim = jaro_similarity(s1, s2, len1, len2);

	// Calculate the length of the common prefix (up to 4 characters)
	int prefix_len = 0;
	int max_prefix = min(min(4, len1), len2);

	for (int i = 0; i < max_prefix; i++) {
		if (s1[i] == s2[i]) {
			prefix_len++;
		} else {
			break;
		}
	}

	// Calculate Jaro-Winkler similarity
	return jaro_sim + (prefix_len * prefix_weight * (1.0 - jaro_sim));
}

/**
 * PHP strcmp95 function implementation
 *
 * Calculates the Jaro-Winkler similarity between two strings
 */
PHP_FUNCTION(strcmp95)
{
	char *str1, *str2;
	size_t str1_len, str2_len;
	double prefix_weight = 0.1;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_STRING(str1, str1_len)
		Z_PARAM_STRING(str2, str2_len)
		Z_PARAM_OPTIONAL
		Z_PARAM_DOUBLE(prefix_weight)
	ZEND_PARSE_PARAMETERS_END();

	// Validate the prefix weight parameter
	if (prefix_weight < 0.0 || prefix_weight > 0.25) {
		zend_argument_value_error(3, "must be between 0.0 and 0.25");
		RETURN_THROWS();
	}

	// Calculate the similarity
	double similarity = jaro_winkler_similarity(str1, str2, (int)str1_len, (int)str2_len, prefix_weight);
	
	// Return the result
	RETURN_DOUBLE(similarity);
}

PHP_RINIT_FUNCTION(strcmp95)
{
#if defined(ZTS) && defined(COMPILE_DL_STRCMP95)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}

PHP_MINFO_FUNCTION(strcmp95)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "strcmp95 support", "enabled");
	php_info_print_table_row(2, "Version", PHP_STRCMP95_VERSION);
	php_info_print_table_row(2, "Algorithm", "Jaro-Winkler similarity (strcmp95)");
	php_info_print_table_end();
}

zend_module_entry strcmp95_module_entry = {
	STANDARD_MODULE_HEADER,
	"strcmp95",					/* Extension name */
	ext_functions,				/* zend_function_entry */
	NULL,						/* PHP_MINIT - Module initialization */
	NULL,						/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(strcmp95),		/* PHP_RINIT - Request initialization */
	NULL,						/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(strcmp95),		/* PHP_MINFO - Module info */
	PHP_STRCMP95_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_STRCMP95
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(strcmp95)
#endif
