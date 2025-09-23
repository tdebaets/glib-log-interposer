/****************************************************************************
 *
 *            glib-log-interposer
 *
 *            Copyright (c) 2025 Tim De Baets
 *
 ****************************************************************************
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * This Source Code Form is "Incompatible With Secondary Licenses", as
 * defined by the Mozilla Public License, v. 2.0.
 *
 ****************************************************************************
 *
 * Main header file for the interposer library
 *
 ****************************************************************************/

#ifndef GLIB_LOG_INTERPOSER_H_
#define GLIB_LOG_INTERPOSER_H_

#include <stdio.h>

#ifdef DEBUG

#define dbgprint(szFormat, ...) \
    printf("[%s:%d] " szFormat "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define dbgvprint(szFormat1, args, szFormat2, ...)                              \
({                                                                              \
    va_list args_copy;                                                          \
    va_copy(args_copy, args);                                                   \
    printf("[%s:%d] " szFormat2 " ", __FUNCTION__, __LINE__, ##__VA_ARGS__);    \
    vprintf(szFormat1, args_copy);                                              \
    printf("\n");                                                               \
})

#else
#define dbgprint(szFormat, ...) ;;
#define dbgvprint(szFormat1, args, szFormat2, ...) ;;
#endif

#endif /* GLIB_LOG_INTERPOSER_H_ */
