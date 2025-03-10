/*
 * This file is part of Everking. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef MANGOSSERVER_ERRORS_H
#define MANGOSSERVER_ERRORS_H

#include "Common.h"

// Normal assert.
#define WPError(CONDITION) \
if (!(CONDITION)) \
{ \
    assert(STRINGIZE(CONDITION) && 0); \
    fprintf(stderr, "Critical Error: A condition which must never be false was found to be false. \
Server was shut down to protect data integrity.\nIf this error is occurring frequently, please \
recompile the software in debug mode to get more details.\n\n%s(): %s\n", __FUNCTION__, STRINGIZE(CONDITION)); \
    std::abort(); \
}

// Just warn.
#define WPWarning(CONDITION) \
if (!(CONDITION)) \
{ \
    printf("%s:%i: Warning: Assertion in %s failed: %s",\
        __FILE__, __LINE__, __FUNCTION__, STRINGIZE(CONDITION)); \
}

#ifdef MANGOS_DEBUG
#  define MANGOS_ASSERT WPError
#else
#  define MANGOS_ASSERT WPError                             // Error even if in release mode.
#endif

#endif
