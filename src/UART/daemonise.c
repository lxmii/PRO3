/*
 * daemonise.c
 *
 *  Created on: 15 Feb 2019
 *      Author: klaus
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

void daemonise()
{
	pid_t pid, sid;

	//	Daemonise the program
	// 	First fork a child process
	pid = fork ();
	if (pid < 0)
	{
		perror ("Process did not fork!\n");
		exit (EXIT_FAILURE);
	}
	// If we got a good PID, then we can exit the parent process.
	if (pid > 0)
	{
		// Parent dying
		exit (EXIT_SUCCESS);
	}
	// Child is running from here
	// Change the file mode mask
	umask (0);

	char str[255];
	// Create a new SID for the child process
	sid = setsid ();
	if (sid < 0)
	{
		// Log any failures here
		sprintf (str, "sid = %d", sid);
		perror (str);
		exit (EXIT_FAILURE);
	}

	// Change the current working directory
	if ((chdir ("/")) < 0)
	{
		// Log any failures here
		exit (EXIT_FAILURE);
	}
	// Close out the standard file descriptors
	close (STDIN_FILENO);
	close (STDOUT_FILENO);
	close (STDERR_FILENO);

}


