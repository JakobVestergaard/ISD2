#include "setitimer.h"


void timer_handler (int signum)
{
    static int count = 0;
    printf ("timer expired %d times\n", ++count);
    // Do stuff
    // HERE
    tempHolder = tempHolder + 1;
    std::cout << "Temperatureholder has a value of " << tempHolder << std::endl;
}

void init_setitimer()
{
    struct sigaction sa;
    struct itimerval timer;

    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction (SIGVTALRM, &sa, NULL);

    /* Configure the timer to expire after 0 msec... */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 100;
    /* ... and every 5 sec after that. */
    timer.it_interval.tv_sec = 5;
    timer.it_interval.tv_usec = 0;
    /* Start a virtual timer. It counts down whenever this process is
    executing. */
    setitimer (ITIMER_VIRTUAL, &timer, NULL);

    /* Do busy work. */
    //while (1);
}
