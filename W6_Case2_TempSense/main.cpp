#include <iostream>
#include "TCPServer.h"
#include "setitimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <pthread.h>

TCPServer tcp;

extern char tempHolder = 0;

void * loop(void * m)
{
    /*When a detached thread terminates, its resources are
       automatically released back to the system without the need for
       another thread to join with the terminated thread.*/
    //pthread_detach(pthread_self());
	while(1)
	{
		//srand(time(NULL));
		//char ch = 'a' + rand() % 26;
		//string s(1,ch);
        string str2 = "GET_TEMP";
        size_t len = str2.length();
		string str = tcp.getMessage();
        if(str.compare(0, len, str2) == 0){
            //cout << "Temperature: " << tempHolder << endl;
            tcp.Send("Temperature: "+tempHolder);          
        } else if(str != ""){
			cout << "Message:" << str << endl;
			tcp.Send("client message: "+str); 
            // +str+ concatinates "client message" with "incoming string" + "string3"
		} 
        tcp.clean();
		usleep(1000);
	}
	tcp.detach();
}

void forky() {
	pid_t process_id = 0;
	pid_t sid = 0;

	// Create child process
	process_id = fork();

	// Indication of fork() failure
	if (process_id < 0) {
		printf("fork failed!\n");
		// Return failure in exit status
		exit(1);
	}

	// PARENT PROCESS. Need to kill it.
	if (process_id > 0) {
		printf("process_id of child process %d \n", process_id);
		// return success in exit status
		exit(0);
	}

	//unmask the file mode
	umask(0);

	//set new session
	sid = setsid();

	if (sid < 0) {
		// Return failure
		exit(1);
	}

    cout << "I am now a daemon, closing stdin/out/error\n" << endl;
	// Close stdin. stdout and stderr
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

int main()
{
    //forky();
    init_setitimer();
   	pthread_t msg;
	tcp.setup(1987);
	if( pthread_create(&msg, NULL, loop, (void *)0) == 0)
	{
		tcp.receive();
	}
	return 0;
}
