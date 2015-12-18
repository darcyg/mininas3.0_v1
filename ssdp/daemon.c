#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

#include "daemon.h"

int daemonize(void)
{
    int sid, i, fd0, fd1, fd2;

    switch(fork())
    {
        /* fork error */
        case -1:
            perror("fork()");
            exit(1);

            /* child process */
        case 0:
            /* obtain a new process group */
            if( (sid = setsid()) < 0)
            {
                perror("setsid()");
                exit(1);
            }

            /* close all descriptors */
            for (i=getdtablesize();i>=0;--i) close(i);

            fd0 = open("/dev/null", O_RDWR); /* open stdin */
            fd1 = dup(0); /* stdout */
            fd2 = dup(0); /* stderr */

			if(fd0 != 0 || fd1 != 1 || fd2 != 2){
				perror("setsid()");
                exit(1);
			}

            umask(027);
            chdir("/"); /* chdir to /tmp ? */

            return sid;

            /* parent process */
        default:
            exit(0);
    }
}
