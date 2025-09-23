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
 * Main implementation of the interposer library
 *
 ****************************************************************************/

#include <assert.h>
#include <dlfcn.h>
#include <glib.h>

#include "glib-log-interposer.h"

gboolean get_interposer_disabled()
{
    char *env_var = getenv("GLIB_LOG_INTERPOSER_DISABLE");

    return (env_var && strcmp(env_var, "1") == 0);
}

typedef void (* g_logv_func)(const gchar    *log_domain,
                             GLogLevelFlags  log_level,
                             const gchar    *format,
                             va_list         args) G_GNUC_PRINTF(3, 0);

// TODO: remove or rename
G_GNUC_PRINTF(3, 0) void g_logv(const gchar    *log_domain,
            GLogLevelFlags  log_level,
            const gchar    *format,
            va_list         args)
{
    //g_log_func func = (g_log_func)dlsym(RTLD_NEXT, "g_log");
    //assert(func != NULL);

    dbgprint("log_domain=\"%s\"", log_domain);
}

// TODO: remove
typedef void (* g_log_structured_standard_func)(const gchar    *log_domain,
                                                GLogLevelFlags  log_level,
                                                const gchar    *file,
                                                const gchar    *line,
                                                const gchar    *func,
                                                const gchar    *message_format,
                                                ...) G_GNUC_PRINTF (6, 7);

// TODO
G_GNUC_PRINTF (6, 7) void g_log_structured_standard (const gchar    *log_domain,
                                GLogLevelFlags  log_level,
                                const gchar    *file,
                                const gchar    *line,
                                const gchar    *func,
                                const gchar    *message_format,
                                ...)
{
    va_list args;

    // TODO: remove
    // TODO: only load once
    // TODO: unload again?
    /*void *handle = dlopen("libglib-2.0.so.0", RTLD_LOCAL | RTLD_NOW);
    assert (handle != NULL);
    g_log_structured_standard_func orig_func =
            (g_log_structured_standard_func)dlsym(handle, "g_log_structured_standard");
    assert(orig_func != NULL);*/

    va_start(args, message_format);

    dbgvprint(message_format, args, "log_domain=\"%s\"", log_domain);

    // TODO: add comment that calling the original function here isn't possible

    if (get_interposer_disabled())
    {
        g_logv(log_domain, log_level, message_format, args);
    }

    va_end(args);
}

// TODO: remove
typedef void (* g_log_func)(const gchar* log_domain,
                            GLogLevelFlags log_level,
                            const gchar* format,
                            ...);

void g_log(const gchar* log_domain, GLogLevelFlags log_level, const gchar* format, ...)
{
    va_list args;
    //char buffer[1024]; // TODO remove

    // TODO: remove
    //g_log_func func = (g_log_func)dlsym(RTLD_NEXT, "g_log");
    //assert(func != NULL);

    // TODO remove
    /*va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    va_end(args);

    printf("%s\n", buffer);*/

    va_start(args, format);

    dbgvprint(format, args, "log_domain=\"%s\"", log_domain);

    // TODO: add comment that calling the original function here isn't possible

    if (get_interposer_disabled())
    {
        g_logv(log_domain, log_level, format, args);
    }

    va_end(args);
}

typedef gboolean (* g_source_remove_func)(guint tag);

gboolean g_source_remove(guint tag)
{
    GSource *source;

    // TODO: only load once
    // TODO: unload again?
    void *handle = dlopen("libglib-2.0.so.0", RTLD_LOCAL | RTLD_NOW);
    assert(handle != NULL);
    g_source_remove_func func = (g_source_remove_func)dlsym(handle, "g_source_remove");
    assert(func != NULL);

    dbgprint("tag=%u", tag);

    source = g_main_context_find_source_by_id(NULL, tag);

    dbgprint("source=%p", source);

    if (!source)
        return FALSE;

    return func(tag);
}

// TODO: remove
void g_assertion_message(const char     *domain,
                         const char     *file,
                         int             line,
                         const char     *func,
                         const char     *message)
{
    dbgprint("%s", message);
}

// TODO: remove
void g_assertion_message_expr(const char     *domain,
                              const char     *file,
                              int             line,
                              const char     *func,
                              const char     *expr)
{
    dbgprint("%s", expr);
}

// TODO: remove
void g_assertion_message_error(const char     *domain,
                               const char     *file,
                               int             line,
                               const char     *func,
                               const char     *expr,
                               const GError   *error,
                               GQuark          error_domain,
                               int             error_code)
{
    dbgprint("%s", expr);
}

typedef void (* g_return_if_fail_warning_func)(const char *log_domain,
                                               const char *pretty_function,
                                               const char *expression);

void g_return_if_fail_warning(const char *log_domain,
                              const char *pretty_function,
                              const char *expression)
{
    g_return_if_fail_warning_func func = (g_return_if_fail_warning_func)dlsym(RTLD_NEXT, "g_return_if_fail_warning");
    assert(func != NULL);

    dbgprint("%s", expression);

    if (get_interposer_disabled())
    {
        func(log_domain, pretty_function, expression);
    }
}
