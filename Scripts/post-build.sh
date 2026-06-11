#!/bin/bash

#############################################################################
#
#            glib-log-interposer
#
#            Copyright (c) 2026 Tim De Baets
#
#############################################################################
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# This Source Code Form is "Incompatible With Secondary Licenses", as
# defined by the Mozilla Public License, v. 2.0.
#
#############################################################################
#
# Shell script with post-build actions
#
#############################################################################

if [ -z "$1" ]; then
    echo "Usage: $(basename "$0") <shared library file>"
    exit 1
fi

return_code=
so_filename=$1
so_filetype=
deploy_dir=$GLIB_LOG_INTERPOSER_DEPLOY_DIR
arch_subdir_32="lib/i386-linux-gnu"
arch_subdir_64="lib/x86_64-linux-gnu"
arch_subdir=
target_dir=
target_filename=

echo "${so_filename}"

pwd

if [ ! -z "${deploy_dir}" ]; then

    # `file` always prints to stdout, so no point in redirecting stderr
    so_filetype=$(file "$so_filename")
    return_code=$?
    
    if [ $return_code -ne 0 ]; then
        echo "Failed to retrieve file type for ${so_filename}"
        exit $return_code
    fi
    
    if [[ "${so_filetype}" =~ "32-bit" ]]; then
        arch_subdir=${arch_subdir_32}
    elif [[ "${so_filetype}" =~ "64-bit" ]]; then
        arch_subdir=${arch_subdir_64}
    else
        echo "Failed to determine architecture of ${so_filename} - ${so_filetype}"
        exit 1
    fi
    
    target_filename="${deploy_dir}/${arch_subdir}/${so_filename}"

    if [ -f "${target_filename}" ]; then
        rm "${target_filename}"
        return_code=$?

        if [ $return_code -ne 0 ]; then
            exit $return_code
        fi
    fi

    cp "${so_filename}" "${target_filename}"
    return_code=$?

    if [ $return_code -ne 0 ]; then
        exit $return_code
    fi

fi
