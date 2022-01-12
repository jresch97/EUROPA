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
#include <unistd.h>
#include <sys/utsname.h>
#endif

#ifdef PLATFORM_WIN32
#include <Windows.h>
#include <VersionHelpers.h>
#include <lmcons.h>
#endif

#ifdef PLATFORM_LINUX
static struct utsname gutsname;
#endif

int osinit ()
{
#ifdef PLATFORM_LINUX
        if (strlen(gutsname.sysname) > 0) return 1;
        if (uname(&gutsname) != 0)        return 0;
        return 1;
#endif
#ifdef PLATFORM_WIN32
        return 1;
#endif
        return 0;
}

const char* osname()
{
        if (!osinit()) return NULL;
#ifdef PLATFORM_LINUX
        if (strcmp(gutsname.sysname, "Linux") == 0) return "GNU/Linux";
        return gutsname.sysname;
#endif
#ifdef PLATFORM_WIN32
        return "Microsoft Windows";
#endif
        return NULL;
}

const char* osrels ()
{
        if (!osinit()) return NULL;
#ifdef PLATFORM_LINUX
        return gutsname.release;
#endif
#ifdef PLATFORM_WIN32
        if      (IsWindowsServer())            return "Server";
        else if (IsWindows10OrGreater())       return "10";
        else if (IsWindows8Point1OrGreater())  return "8.1";
        else if (IsWindows8OrGreater())        return "8";
        else if (IsWindows7SP1OrGreater())     return "7 SP1";
        else if (IsWindows7OrGreater())        return "7";
        else if (IsWindowsVistaSP2OrGreater()) return "Vista SP2";
        else if (IsWindowsVistaSP1OrGreater()) return "Vista SP1";
        else if (IsWindowsVistaOrGreater())    return "Vista";
        else if (IsWindowsXPSP3OrGreater())    return "XP SP3";
        else if (IsWindowsXPSP2OrGreater())    return "XP SP2";
        else if (IsWindowsXPSP1OrGreater())    return "XP SP1";
        else if (IsWindowsXPOrGreater())       return "XP";
#endif
        return NULL;
}

const char* osvers ()
{
        if (!osinit()) return NULL;
#ifdef PLATFORM_LINUX
        return gutsname.version;
#endif
        return NULL;
}

const char* osarch ()
{
        if (!osinit()) return NULL;
#ifdef PLATFORM_LINUX
        return gutsname.machine;
#endif
        return NULL;
}

const char* osmach ()
{
        if (!osinit()) return NULL;
#ifdef PLATFORM_LINUX
        return gutsname.nodename;
#endif
        return NULL;
}

const char* osuser ()
{
#ifdef PLATFORM_LINUX
        return getlogin();
#endif
#ifdef PLATFORM_WIN32
        static CHAR  buf[UNLEN + 1];
        static DWORD buflen = UNLEN + 1;
        if (GetUserNameA(buf, &buflen)) return (const char*)buf;
#endif
        return NULL;
}