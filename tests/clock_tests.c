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
    
        return EXIT_SUCCESS;
}
