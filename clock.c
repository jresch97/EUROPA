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

/* TODO: Handle variable frequency properly without casting to double. */

static double clkfreqd();
static double clkelaptd();

double clkfreqd()
{
        return (double)clkfreq();
}

static double clkelaptd()
{
        return (double)clkelapt();
}

long long int clkfreq()
{
#ifdef PLATFORM_LINUX
        return NSPERS;
#endif
#ifdef PLATFORM_WIN32
        static LARGE_INTEGER f;
        if (f.QuadPart > 0)                return f.QuadPart;
        if (QueryPerformanceFrequency(&f)) return f.QuadPart;
        return -1;
#endif
}

long long int clkelapt()
{
#ifdef PLATFORM_LINUX
        TIMESPEC t;
        clock_gettime(CLOCK_MONOTONIC_RAW, &t);
        return t.tv_sec * NSPERS + t.tv_nsec;
#endif
#ifdef PLATFORM_WIN32
        LARGE_INTEGER t;
        if (QueryPerformanceCounter(&t)) return t.QuadPart;
        return -1;
#endif
}

long long int clkelaps()
{
        return (long long int)(clkelaptd() / clkfreqd());
}

long long int clkelapms()
{
        return (long long int)(clkelaptd() / (clkfreqd() / MSPERS));
}

long long int clkelapus()
{
        return (long long int)(clkelaptd() / (clkfreqd() / USPERS));
}

long long int clkelapns()
{
        return (long long int)(clkelaptd() / (clkfreqd() / NSPERS));
}

void clkslept(long long int t)
{
#ifdef PLATFORM_LINUX
        TIMESPEC s;
        s.tv_sec  = t / clkfreq();
        s.tv_nsec = t - (s.tv_sec * NSPERS);
        nanosleep(&s, NULL);
#endif
#ifdef PLATFORM_WIN32
        long long     a;
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

void clksleps(long long int s)
{
        clkslept((long long int)((double)s * clkfreqd()));
}

void clkslepms(long long int ms)
{
        clkslept((long long int)((double)ms * (clkfreqd() / MSPERS)));
}

void clkslepus(long long int us)
{
        clkslept((long long int)((double)us * (clkfreqd() / USPERS)));
}

void clkslepns(long long int ns)
{
        clkslept((long long int)((double)ns * (clkfreqd() / NSPERS)));
}
