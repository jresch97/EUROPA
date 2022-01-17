#include "clock.h"

#include "platform.h"

#ifdef PLATFORM_LINUX

#include <time.h>
#include <unistd.h>

typedef struct timespec TIMESPEC;

#endif

#ifdef PLATFORM_WIN32

#include <Windows.h>

#endif

uint64_t clkfreq()
{
#ifdef PLATFORM_LINUX
        return NSPERS;
#endif
#ifdef PLATFORM_WIN32
        static LARGE_INTEGER f = { 0 };
        if (f.QuadPart > 0)                return f.QuadPart;
        if (QueryPerformanceFrequency(&f)) return f.QuadPart;
        return 0;
#endif
}

uint64_t clkelapt()
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
        return f > 0 ? clkelapt() / f : 0;
}

uint64_t clkelapms()
{
        uint64_t d;
        d = clkfreq() / MSPERS;
        return d > 0 ? clkelapt() / d : 0;
}

uint64_t clkelapus()
{
        uint64_t d;
        d = clkfreq() / USPERS;
        return d > 0 ? clkelapt() / d : 0;
}

uint64_t clkelapns()
{
        uint64_t d;
        d = clkfreq() / NSPERS;
        return d > 0 ? clkelapt() / d : 0;
}

void clkslept(uint64_t t)
{
#ifdef PLATFORM_LINUX
        TIMESPEC s;
        s.tv_sec  = t / clkfreq();
        s.tv_nsec = t - (s.tv_sec * NSPERS);
        nanosleep(&s, NULL);
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
        clkslept(s * clkfreq());
}

void clkslepms(uint64_t ms)
{
        clkslept(ms * (clkfreq() / MSPERS));
}

void clkslepus(uint64_t us)
{
        clkslept(us * (clkfreq() / USPERS));
}

void clkslepns(uint64_t ns)
{
        clkslept(ns * (clkfreq() / NSPERS));
}
