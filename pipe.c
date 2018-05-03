// This code sample is execerpted from P. Wang's Intro to ANSI C on UNIX
// and partially modified for  CSC345 class
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
typedef char *String;
#define MAXLINE 80
#define CHUNK 10
#define WHITE "\t \n"
#define MAXARG 20

int main()
{
	int fds[2]; // an array of 2 file descriptors fpr a pipe

	int i, j, pid1, pid2, status, ip;

	int len;
	char cmd[MAXLINE], subcmd1[MAXLINE], subcmd2[MAXLINE];
	String argl[MAXARG];

	strcpy(subcmd1,"ls -l");
	strcpy(subcmd2,"more");
	printf( "command1 is %s command2 is %s \n ", subcmd1, subcmd2);



	pipe(fds);			// create a pipe

	if ((pid1=fork()) == 0)			// child process
		{
			i = 0;
			argl[i++] = strtok(subcmd2, WHITE);
			while ( i < MAXARG &&
				(argl[i++] = strtok(NULL, WHITE)) != NULL);


			close (fds[1]);		// close a write descriptor
			dup2(fds[0], 0);
			close(fds[0]);
			execvp( argl[0], argl);
			_exit(1);
		}

	if ((pid2=fork()) == 0)			// child process
		{
			i = 0;
			argl[i++] = strtok(subcmd1, WHITE);
			while ( i < MAXARG &&
				(argl[i++] = strtok(NULL, WHITE)) != NULL);

			close (fds[0]);		// close a reading file descriptor
			dup2(fds[1], 1);
			close(fds[1]);
			execvp( argl[0], argl);
			_exit(1);
		}


			close(fds[0]);
			close(fds[1]);
			while (wait(&status) != pid2) ;
			printf("a child has finished\n");
	return 0;
}
