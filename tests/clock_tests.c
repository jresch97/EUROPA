#include <stdio.h>
#include <stdlib.h>

#include "clock.h"

int main(int argc, char *argv[])
{
        printf("clkfreq  =%lld\n", clkfreq());
        printf("clkelapt =%lld\n", clkelapt());
        printf("clkelaps =%lld\n", clkelaps());
        printf("clkelapms=%lld\n", clkelapms());
        printf("clkelapus=%lld\n", clkelapus());
        printf("clkelapns=%lld\n", clkelapns());
        printf("clkslept(%lld)...\n", 5 * clkfreq());
        clkslept(5 * clkfreq());
        printf("clksleps(5)...\n");
        clksleps(5);
        printf("clkslepms(5000)...\n");
        clkslepms(5000);
        printf("clkslepus(5000000)...\n");
        clkslepus(5000000);
        printf("clkslepns(5000000000)...\n");
        clkslepns(5000000000);
        return EXIT_SUCCESS;
}
