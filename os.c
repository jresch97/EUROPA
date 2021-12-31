/**
 *
 * Author: Jared B. Resch
 *
 * This file is part of EUROPA.
 *
 * EUROPA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EUROPA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EUROPA. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "os.h"

#include "platform.h"

#ifdef PLATFORM_LINUX
#include <sys/utsname.h>
#endif

#ifdef PLATFORM_LINUX
static struct utsname *g_utsname = NULL;
#endif

int osinit()
{
#ifdef PLATFORM_LINUX
        if (!g_utsname) {
                g_utsname = malloc(sizeof(*g_utsname));
                if (uname(g_utsname) != 0) goto err_ret;
        }
#endif
        return 1;
err_ret:
        return 0;
}

const char* osname()
{
        if (!osinit()) goto err_ret;
#ifdef PLATFORM_LINUX
        return g_utsname->sysname;
#endif
        return 1;
err_ret:
        return NULL;
}

const char* osvers()
{
        if (!osinit()) goto err_ret;
#ifdef PLATFORM_LINUX
        return g_utsname->version;
#endif
        return 1;
err_ret:
        return NULL;
}