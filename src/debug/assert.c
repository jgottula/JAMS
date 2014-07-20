/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "debug/assert.h"
#include "debug/fatal.h"


noreturn void assert_fail(const char *file, uint16_t line) {
	char str[192];
	sprintf_P(str, PSTR("Assertion failure\nat %S:%" PRIu16),
		file, line);
	
	fatal_S(str);
}

noreturn void assert_fail_ex(const char *expr, const char *file,
	uint16_t line) {
	char str[192];
	sprintf_P(str, PSTR("Assertion failure\n%S\nat %S:%" PRIu16),
		expr, file, line);
	
	fatal_S(str);
}
