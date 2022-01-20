/**
 *
 * Copyright (C) 2021 Jared B. Resch
 *
 * This file is part of EUROPA.
 * 
 * EUROPA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * EUROPA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with EUROPA. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "clock.h"
#include "platform.h"

#ifdef PLATFORM_LINUX
#include <time.h>
#include <unistd.h>
#endif

#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif

#ifdef PLATFORM_LINUX
typedef struct timespec TIMESPEC;
#endif

uint64_t clkfreq()
{
#ifdef PLATFORM_LINUX
        TIMESPEC t;
        clock_getres(CLOCK_MONOTONIC_RAW, &t);
        return t.tv_sec * NSPERS + t.tv_nsec;
#endif
#ifdef PLATFORM_WIN32
        LARGE_INTEGER f;
        if (QueryPerformanceFrequency(&f)) return f.QuadPart;
        return 0;
#endif
}

uint64_t clkelap()
{
#ifdef PLATFORM_LINUX
        TIMESPEC t;
        clock_gettime(CLOCK_MONOTONIC_RAW, &t);
        return t.tv_sec * NSPERS + t.tv_nsec;
#endif
#ifdef PLATFORM_WIN32
        LARGE_INTEGER t;
        if (QueryPerformanceCounter(&t)) return t.QuadPart;
        return 0;
#endif
}

uint64_t clkelaps()
{
        uint64_t f;
        f = clkfreq();
        return f > 0 ? (uint64_t)(clkelap() / f) : 0;
}

uint64_t clkelapms()
{
        uint64_t f;
        f = clkfreq();
        return f > 0 ? (uint64_t)(clkelap() / (f / (double)MSPERS)) : 0;
}

uint64_t clkelapus()
{
        uint64_t f;
        f = clkfreq();
        return f > 0 ? (uint64_t)(clkelap() / (f / (double)USPERS)) : 0;
}

uint64_t clkelapns()
{
        uint64_t f;
        f = clkfreq();
        return f > 0 ? (uint64_t)(clkelap() / (f / (double)NSPERS)) : 0;
}

void clkslep(uint64_t t)
{
        if (t == 0) return;
#ifdef PLATFORM_LINUX
        uint64_t f;
        TIMESPEC s;
        f = clkfreq();
        if (f > 0) {
                s.tv_sec = t / clkfreq();
                s.tv_nsec = t - s.tv_sec * NSPERS;
                nanosleep(&s, NULL);
        }
#endif
#ifdef PLATFORM_WIN32
        uint64_t      a;
        LARGE_INTEGER s1, s2;
        a = 0;
        QueryPerformanceCounter(&s1);
        do {
                QueryPerformanceCounter(&s2);
                a += s2.QuadPart - s1.QuadPart;
                s1 = s2;
        } while (a <= t);
#endif
}

void clksleps(uint64_t s)
{
        clkslep(s * clkfreq());
}

void clkslepms(uint64_t ms)
{
        clkslep((uint64_t)(ms * (clkfreq() / (double)MSPERS)));
}

void clkslepus(uint64_t us)
{
        clkslep((uint64_t)(us * (clkfreq() / (double)USPERS)));
}

void clkslepns(uint64_t ns)
{
        clkslep((uint64_t)(ns * (clkfreq() / (double)NSPERS)));
}
