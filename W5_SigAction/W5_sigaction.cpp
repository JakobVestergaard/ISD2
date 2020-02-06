//============================================================================
// Name        : W5_Simple_CatchSignal.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>

// Uncomment to debug
//#define debug

void sig_handler(int sig, siginfo_t *siginfo, void *context)
{
	{ 
        // Send a cancellation signal to the listener
		perror ("Received a terminate signal");
		// Brutally kill the listener
		printf("\nDo stuff!");
	}
}

int main(void)
{
    struct sigaction act;
    #ifdef debug
    printf("\nReady to go!");
    #endif
    memset(&act, '\0', sizeof(act));

    // Install a signal handler
	// Use the sa_sigaction field because the handles has two
	// additional parameters
	act.sa_sigaction = &sig_handler;

    // Install the signal handler and relate it to the SIGTERM flag
	if (sigaction (SIGTERM, &act, NULL) < 0)
	{
		printf("\nsigaction failed to install");
	}
    #ifdef debug
    printf("\nAll set ready to terminate on request!");
    #endif
    while(1);
}
