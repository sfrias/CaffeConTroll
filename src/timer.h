//
//  timer.h
//  moka
//
//  Created by Ce Zhang on 1/12/15.
//  Copyright (c) 2015 Hazy Research. All rights reserved.
//

#ifndef moka_timer_h
#define moka_timer_h

#if defined __MACH__ && __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>
#include <sys/time.h>
#else
#include <time.h>
#endif

#if defined __MACH__ && __APPLE__
//clock_gettime is not implemented on OSX
int clock_gettime(int /*clk_id*/, struct timespec* t) {
  struct timeval now;
  int rv = gettimeofday(&now, NULL);
  if (rv) return rv;
  t->tv_sec  = now.tv_sec;
  t->tv_nsec = now.tv_usec * 1000;
  return 0;
}

#define CLOCK_MONOTONIC 0
#endif


class Timer {
  public:

    struct timespec _start;
    struct timespec _end;

    Timer(){
      clock_gettime(CLOCK_MONOTONIC, &_start);
    }

    ~Timer(){}

    inline void restart(){
      clock_gettime(CLOCK_MONOTONIC, &_start);
    }

    inline float elapsed(){
      clock_gettime(CLOCK_MONOTONIC, &_end);
      return (_end.tv_sec - _start.tv_sec) + (_end.tv_nsec - _start.tv_nsec) / 1000000000.0;
    }


};

#endif
