#if !defined(NANOS_H)
#include <time.h>
static inline
u64 nanos()
{
  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC_RAW,&start);
  return ((u64)start.tv_sec*SECONDS_TO_NANOSECONDS_MULT + (u64)start.tv_nsec);
}
#define NANOS_H
#endif
