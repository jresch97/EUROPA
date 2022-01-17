#ifndef EUROPA_CLOCK_H
#define EUROPA_CLOCK_H

#define MSPERS 1000
#define USPERS 1000000
#define NSPERS 1000000000

long long int clkfreq  (); /* system clock tick frequency per second */
long long int clkelapt (); /* elapsed ticks */
long long int clkelaps (); /* elapsed seconds */
long long int clkelapms(); /* elapsed milliseconds */
long long int clkelapus(); /* elapsed microseconds */
long long int clkelapns(); /* elapsed nanoseconds */

#endif
