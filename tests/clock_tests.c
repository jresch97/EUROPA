#include <stdio.h>
#include <stdlib.h>

#include "clock.h"

static const uint64_t SLEPS = 1;

int main(int argc, char *argv[])
{
        printf("clkfreq  =%lu\n", clkfreq());
        printf("clkelapt =%lu\n", clkelapt());
        printf("clkelaps =%lu\n", clkelaps());
        printf("clkelapms=%lu\n", clkelapms());
        printf("clkelapus=%lu\n", clkelapus());
        printf("clkelapns=%lu\n", clkelapns());
        printf("clkslept (%lu)...\n", SLEPS * clkfreq());
        clkslept(SLEPS * clkfreq());
        printf("clksleps (%lu)...\n", SLEPS);
        clksleps(SLEPS);
        printf("clkslepms(%lu)...\n", SLEPS * MSPERS);
        clkslepms(SLEPS * MSPERS);
        printf("clkslepus(%lu)...\n", SLEPS * USPERS);
        clkslepus(SLEPS * USPERS);
        printf("clkslepns(%lu)...\n", SLEPS * NSPERS);
        clkslepns(SLEPS * NSPERS);
        return EXIT_SUCCESS;
}
