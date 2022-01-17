#ifndef EUROPA_CLOCK_H
#define EUROPA_CLOCK_H

#define MSPERS 1000
#define USPERS 1000000
#define NSPERS 1000000000

long long int clkfreq  ();
long long int clkelapt ();
long long int clkelaps ();
long long int clkelapms();
long long int clkelapus();
long long int clkelapns();
void          clkslept (long long int t);
void          clksleps (long long int s);
void          clkslepms(long long int ms);
void          clkslepus(long long int us);
void          clkslepns(long long int ns);

#endif
