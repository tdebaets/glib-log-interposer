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
target_filename="${HOME}/${so_filename}"

echo "${so_filename}"

pwd

if [ -f "$target_filename" ]; then
    rm "$target_filename"
    return_code=$?

    if [ $return_code -ne 0 ]; then
        exit $return_code
    fi
fi

cp "${so_filename}" ${HOME}
return_code=$?

if [ $return_code -ne 0 ]; then
    exit $return_code
fi
