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

#include <stdio.h>
#include <stdlib.h>

#include "clock.h"

static const uint64_t SLEPS = 1;

int main(int argc, char *argv[])
{
        printf("clkfreq  =%lu\n", clkfreq());
        printf("clkelap  =%lu\n", clkelap());
        printf("clkelaps =%lu\n", clkelaps());
        printf("clkelapms=%lu\n", clkelapms());
        printf("clkelapus=%lu\n", clkelapus());
        printf("clkelapns=%lu\n", clkelapns());
        printf("clkslep  (%lu)...\n", SLEPS * clkfreq());
        clkslep(SLEPS * clkfreq());
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
