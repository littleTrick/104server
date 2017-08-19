#include "timestamp.h"
#include <stdio.h>
#include <sys/time.h>


const char *Timestamp::kFormat = "%4d-%02d-%02d %02d:%02d:%02d.%06d";

const char *Timestamp::to_string() const
{
    struct tm tm_time;
	time_t seconds = microseconds_ / kMicroSecondsPerSecond;
    localtime_r(&seconds, &tm_time);
    snprintf(stime_, sizeof stime_, 
			 kFormat,
             tm_time.tm_year + 1900,
             tm_time.tm_mon + 1,
             tm_time.tm_mday,
             tm_time.tm_hour,
             tm_time.tm_min,
             tm_time.tm_sec,
             microseconds_ % kMicroSecondsPerSecond);
    return stime_;
}

Timestamp Timestamp::current()
{
    struct timeval curr;
    ::gettimeofday(&curr, NULL);
    return Timestamp(curr.tv_sec * kMicroSecondsPerSecond + curr.tv_usec);
}

