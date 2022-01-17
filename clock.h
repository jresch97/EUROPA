#ifndef EUROPA_CLOCK_H
#define EUROPA_CLOCK_H

#include <stdint.h>

#define MSPERS 1000
#define USPERS 1000000
#define NSPERS 1000000000

uint64_t clkfreq  ();
uint64_t clkelapt ();
uint64_t clkelaps ();
uint64_t clkelapms();
uint64_t clkelapus();
uint64_t clkelapns();
void     clkslept (uint64_t t);
void     clksleps (uint64_t s);
void     clkslepms(uint64_t ms);
void     clkslepus(uint64_t us);
void     clkslepns(uint64_t ns);

#endif
