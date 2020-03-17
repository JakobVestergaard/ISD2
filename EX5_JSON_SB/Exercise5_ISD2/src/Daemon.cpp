/**
 * @File Daemon.c
 * @author Steffen Breinbjerg
 * Elektronik Ingeniør, Aarhus universitet Herning. 4th. Semester
 * @date 07-02-2020
 * @brief For deamonizing the program.
 * @copyright Free for everyone
 * @details
 *
 */

#include "Daemon.h"


int daemonize() {

	int process_id = 0;
	int sid = 0;

	/** Creating Child process */
	process_id = fork();

	if (process_id < 0) {
		//Add error handling
		exit(EXIT_FAILURE);
	}
	/** Fork retuner id = 0 i child process og child processs ID i parent process.
	 *  If id > 0 then parent process. Need to terminate.*/
	if (process_id > 0) {
//		cout << "Parent process terminating. Child process id:" << process_id
//				<< endl;
		//Tilføj syslog istedet
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();
	if (sid < 0) {
//		cout << "Failure creating new session" << endl;
		//Add syslog
		exit(EXIT_FAILURE);
	}
	/** Closing Stdin, stder and stdout */
	close(STDIN_FILENO);
	close(STDERR_FILENO);
	close(STDOUT_FILENO);

	return 0;

}

