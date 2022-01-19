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

#ifndef EUROPA_CLOCK_H
#define EUROPA_CLOCK_H

#include <stdint.h>

#define MSPERS 1000
#define USPERS 1000000
#define NSPERS 1000000000

uint64_t clkfreq  ();
uint64_t clkelap  ();
uint64_t clkelaps ();
uint64_t clkelapms();
uint64_t clkelapus();
uint64_t clkelapns();
void     clkslep  (uint64_t t);
void     clksleps (uint64_t s);
void     clkslepms(uint64_t ms);
void     clkslepus(uint64_t us);
void     clkslepns(uint64_t ns);

#endif
