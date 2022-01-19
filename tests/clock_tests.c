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
        printf("clkfreq  =%llu\n", clkfreq());
        printf("clkelap  =%llu\n", clkelap());
        printf("clkelaps =%llu\n", clkelaps());
        printf("clkelapms=%llu\n", clkelapms());
        printf("clkelapus=%llu\n", clkelapus());
        printf("clkelapns=%llu\n", clkelapns());
        printf("clkslep  (%llu)...\n", SLEPS * clkfreq());
        clkslep(SLEPS * clkfreq());
        printf("clksleps (%llu)...\n", SLEPS);
        clksleps(SLEPS);
        printf("clkslepms(%llu)...\n", SLEPS * MSPERS);
        clkslepms(SLEPS * MSPERS);
        printf("clkslepus(%llu)...\n", SLEPS * USPERS);
        clkslepus(SLEPS * USPERS);
        printf("clkslepns(%llu)...\n", SLEPS * NSPERS);
        clkslepns(SLEPS * NSPERS);
        return EXIT_SUCCESS;
}
