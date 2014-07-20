/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef JAMS_DEBUG_ASSERT_H
#define JAMS_DEBUG_ASSERT_H


#include "std.h"


#define ASSERT(_expr) \
 	((_expr) ? (void)0 : \
 		assert_fail((PSTR(__FILE__) + 4), __LINE__))

#define ASSERT_EX(_expr) \
 	((_expr) ? (void)0 : \
 		assert_fail_ex(PSTR(#_expr), (PSTR(__FILE__) + 4), __LINE__))


noreturn void assert_fail(const char *file, uint16_t line);
noreturn void assert_fail_ex(const char *expr, const char *file, uint16_t line);


#endif
