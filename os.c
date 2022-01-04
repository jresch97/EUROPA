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

#include <stddef.h>
#include <string.h>

#ifdef PLATFORM_LINUX
#include <sys/utsname.h>
#endif

#ifdef PLATFORM_LINUX
static struct utsname gutsname;
#endif

int osinit ()
{
#ifdef PLATFORM_LINUX
        if (strlen(gutsname.sysname) > 0) return 1;
        if (uname(&gutsname) != 0)        return 0;
#endif
        return 1;
}

const char* osname ()
{
        if (!osinit()) return NULL;
#ifdef PLATFORM_LINUX
        return gutsname.sysname;
#endif
}

const char* osvers ()
{
        if (!osinit()) return NULL;
#ifdef PLATFORM_LINUX
        return gutsname.release;
#endif
}

const char* osarch ()
{
        if (!osinit()) return NULL;
#ifdef PLATFORM_LINUX
        return gutsname.machine;
#endif
}