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

long long int clkfreq()
{
#ifdef PLATFORM_LINUX
        return NSPERS;
#elif  PLATFORM_WIN32
        static LARGE_INTEGER f = { -1 };
        if (f.QUAD_PART < 0) {
                if (!QueryPerformanceFrequency(&f)) return -1;
        }
ret:    return f.QUAD_PART;
#endif
}

long long int clkelapt()
{
#ifdef PLATFORM_LINUX
        TIMESPEC t;
        clock_gettime(CLOCK_MONOTONIC_RAW, &t);
        return t.tv_sec * NSPERS + t.tv_nsec;
#elif  PLATFORM_WIN32
        LARGE_INTEGER t;
        if (QueryPerformanceCounter(&t)) {
                return t.QUAD_PART;
        }
        return -1;
#endif
}

long long int clkelaps()
{
        return clkelapt() / clkfreq();
}

long long int clkelapms()
{
        return clkelapt() / (clkfreq() / MSPERS);
}

long long int clkelapus()
{
        return clkelapt() / (clkfreq() / USPERS);
}

long long int clkelapns()
{
        return clkelapt() / (clkfreq() / NSPERS);
}

void clkslept(long long int t)
{
#ifdef PLATFORM_LINUX
        TIMESPEC s;
        s.tv_sec  = t / clkfreq();
        s.tv_nsec = t - (s.tv_sec * NSPERS);
        nanosleep(&s, NULL);
#elif  PLATFORM_WIN32
        long long     a;
        LARGE_INTEGER s1, s2;
        a = 0;
        QueryPerformanceCounter(&s1);
        do {
                QueryPerformanceCounter(&s2);
                a += s2.QUAD_PART - s1.QUAD_PART;
                s1 = s2;
        } while (a <= t);
#endif
}

void clksleps(long long int s)
{
        clkslept(s * clkfreq());
}

void clkslepms(long long int ms)
{
        clkslept(ms * (clkfreq() / MSPERS));
}

void clkslepus(long long int us)
{
        clkslept(us * (clkfreq() / USPERS));
}

void clkslepns(long long int ns)
{
        clkslept(ns * (clkfreq() / NSPERS));
}
