#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

extern char tempHolder;

void timer_handler (int);
void init_setitimer();
