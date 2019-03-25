#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>

/*************************
 struct timeval {
    time_t      tv_sec;         // seconds 
    suseconds_t tv_usec;        // microseconds 
 };
 struct itimerval {
    struct timeval it_interval; // Interval of periodic timer 
    struct timeval it_value;    // Time until next expiration
 };
*********************/

int hh, mm, ss, tick;

void timer_handler (int sig)
{
  // printf("timer_handler: signal=%d\n", sig);
  printf("%02d:%02d:%02d\r", hh, mm, ss);
  fflush(stdout);
  ss++;
  tick++;
  if(ss == 60)
  {
	ss = 0;
	mm++;
	if(mm == 60)
	{
		mm = 0;
		hh++;
		if(hh == 24)
		{
			hh = 0;
		}
	}
  }
}

int main ()
{
 struct itimerval itimer;
 tick = hh = mm = ss = 0;
 
 signal(SIGALRM, &timer_handler);
 
 /* Configure the timer to expire after 1 sec */
 itimer.it_value.tv_sec  = 1;
 itimer.it_value.tv_usec = 0;

 /* and every 1 sec after that */
 itimer.it_interval.tv_sec  = 1;
 itimer.it_interval.tv_usec = 0;

 setitimer (ITIMER_REAL, &itimer, NULL);

 while (1);
}
