#include <stdio.h>
#include <stdlib.h>

#include "clock.h"

static const long long int SLEPS = 1;

int main(int argc, char *argv[])
{
        printf("clkfreq  =%lld\n", clkfreq());
        printf("clkelapt =%lld\n", clkelapt());
        printf("clkelaps =%lld\n", clkelaps());
        printf("clkelapms=%lld\n", clkelapms());
        printf("clkelapus=%lld\n", clkelapus());
        printf("clkelapns=%lld\n", clkelapns());
        printf("clkslept(%lld)...\n", SLEPS * clkfreq());
        clkslept(SLEPS * clkfreq());
        printf("clksleps(%lld)...\n", SLEPS);
        clksleps(SLEPS);
        printf("clkslepms(%lld)...\n", SLEPS * MSPERS);
        clkslepms(SLEPS * MSPERS);
        printf("clkslepus(%lld)...\n", SLEPS * USPERS);
        clkslepus(SLEPS * USPERS);
        printf("clkslepns(%lld)...\n", SLEPS * NSPERS);
        clkslepns(SLEPS * NSPERS);
        return EXIT_SUCCESS;
}
