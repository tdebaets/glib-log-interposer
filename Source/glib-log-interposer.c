/****************************************************************************
 *
 *            glib-log-interposer
 *
 *            Copyright (c) 2026 Tim De Baets
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

// TODO: move Debug and Release directories to Output directory and update readme
// TODO: add scripts to do headless builds (see "Eclipse headless build.txt" and "Old/geteclipseprefs.sh")
//      also test with newer Eclipse versions

#include "glib-log-interposer.h"

gboolean get_interposer_disabled()
{
    char *env_var = getenv("GLIB_LOG_INTERPOSER_DISABLE");

    return (env_var && strcmp(env_var, "1") == 0);
}

void g_log_structured_standard (const gchar    *log_domain,
                                GLogLevelFlags  log_level,
                                const gchar    *file,
                                const gchar    *line,
                                const gchar    *func,
                                const gchar    *message_format,
                                ...)
{
    va_list args;

    va_start(args, message_format);

    dbgvprint(message_format, args, "log_domain=\"%s\"", log_domain);

    if (get_interposer_disabled())
    {
        /*
         * Because this is a variadic function, we can't call the function's original
         * implementation (no way to pass the additional arguments without va_start/va_end).
         */
        g_logv(log_domain, log_level, message_format, args);
    }

    va_end(args);
}

void g_log(const gchar* log_domain, GLogLevelFlags log_level, const gchar* format, ...)
{
    va_list args;

    va_start(args, format);

    dbgvprint(format, args, "log_domain=\"%s\"", log_domain);

    if (get_interposer_disabled())
    {
        /*
         * Because this is a variadic function, we can't call the function's original
         * implementation (no way to pass the additional arguments without va_start/va_end).
         */
        g_logv(log_domain, log_level, format, args);
    }

    va_end(args);
}

typedef gboolean (* g_source_remove_func)(guint tag);

/*
 * This implementation replaces the original g_source_remove() implementation in glib/gmain.c.
 * When the g_main_context_find_source_by_id() call in the original implementation returns NULL, a
 * "Source ID %u was not found when attempting to remove it" message is printed to the terminal.
 * An example of when these messages are generated is exiting the Eclipse IDE, after one (or more)
 * of the projects opened in the IDE were automatically refreshed using the Eclipse 'Zoodiac'
 * plug-in.
 * The macro used by GLib for printing these messages is g_critical(), which is a wrapper around
 * the g_log_structured_standard() function. But because this function is called internally in the
 * GLib library, it is not replaced by our g_log_structured_standard() implementation above. To
 * work around this, we replace the g_source_remove() function and only call the original
 * implementation when we're sure that it won't print the unwanted message.
 */
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
    
    if (get_interposer_disabled())
        return func(tag);

    source = g_main_context_find_source_by_id(NULL, tag);

    dbgprint("source=%p", source);

    if (!source)
        return FALSE;

    return func(tag);
}

typedef void (* g_return_if_fail_warning_func)(const char *log_domain,
                                               const char *pretty_function,
                                               const char *expression);

void g_return_if_fail_warning(const char *log_domain,
                              const char *pretty_function,
                              const char *expression)
{
    g_return_if_fail_warning_func func =
            (g_return_if_fail_warning_func)dlsym(RTLD_NEXT, "g_return_if_fail_warning");
    assert(func != NULL);

    dbgprint("%s", expression);

    if (get_interposer_disabled())
    {
        func(log_domain, pretty_function, expression);
    }
}
