/* ***********************************************************
 * Dax Henson
 * 05-03-18
 * TechShell.c
 *
 * A new shell because there aren't enough shells in the world
 * *********************************************************** */

 //HEADER FILES
 #include <fcntl.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/wait.h>

//GLOBAL VARIABLES
//String buffer, token buffer, and Directory buffer
#define S_BUFFER 129
#define T_BUFFER 4
#define D_BUFFER 1024

//Global Arrays
char cwd[D_BUFFER];
char string[S_BUFFER];

//returns length of array
int lenArgs(char** args)
{
	char* 	arg = args[0];
	int 	length = 0;
	while (arg != NULL)
	{
		arg = args[++length];
	}
	return length;
}

// Scan array for string and return index
int scanFor(char** args, char* string)
{
	char* 	arg = args[0];
	int	pos = 0;			//Position counter
	while (arg != NULL)
	{
		//compare arg to string
		if (strcmp(arg, string) == 0)
			return pos;
		//increment position
		arg = args[++pos];
	}
	return -1;
}

// Display prompt then read input line
char* input(char* prompt)
{
	printf("%s %s", cwd, prompt);
	fgets(string, S_BUFFER, stdin);
	string[strcspn(string, "\n")] = 0;
	return string;
}

char** tok(char* line)
{
	int 	buffer 	= T_BUFFER;
	int 	pos 	= 0;
	char** 	tokens	= malloc(buffer * sizeof(char*));
	char* 	token;

	if (!tokens)
	{
		fprintf(stderr, "malloc error\n");
		exit(EXIT_FAILURE);
	}

	// split string by whitespace
	token = strtok(line, " ");
	while (token != NULL) {
		tokens[pos] = token;
		pos++;

		// array is bigger than buffer, reallocate
		if (pos >= buffer)
		{
			buffer += T_BUFFER;
			tokens = realloc(tokens, buffer * sizeof(char*));
			if (!tokens)
			{
				fprintf(stderr, "malloc error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, " ");
	}
	tokens[pos] = NULL;
	return tokens;
}

// Redirect output
void outredirect(char** args, int outIndex)
{
	int fp = open(args[outIndex + 1],
			O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
	if (dup2(fp, STDOUT_FILENO) == -1)
		perror("dup2 out");
	close(fp);
	args[outIndex] = NULL;
}

// Redirect input
void inredirect(char** args, int inIndex)
{
	int fp = open(args[inIndex + 1], O_RDONLY);
	if (dup2(fp, STDIN_FILENO) == -1)
		perror("dup2 in");
	close(fp);
	args[inIndex] = NULL;
}

//TODO
int child(char** args)
{
	pid_t   pid1;
	pid_t   pid2;

	int stat;
	int outIndex;
	int inIndex;

	pid1 = fork();
	if (pid1 == 0)
	{
		// child process
		// check for redirect
		if ((outIndex = scanFor(args, ">")) != -1)
		{
			outredirect(args, outIndex);
		}
		if ((inIndex = scanFor(args, "<")) != -1)
		{
			inredirect(args, inIndex);
		}
		// run command
		if (execvp(args[0], args) == -1)
			perror("child proc");
		exit(EXIT_FAILURE);
	}
	else if (pid1 < 0)
	{
		//fork failed if pid1<0
		printf("Child process creation failed.");
		exit(EXIT_FAILURE);
	}
	else
	{
		// parent process
		if ((pid2 = waitpid(pid1, &stat, WUNTRACED)) == -1) {
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
	}
	return 1;
}

int printcwd()
{
	if(getcwd(cwd, sizeof(cwd)) == NULL)
	{
		printf("Current working directory does not exist.");
		return -1;
	}
	else
	{
		printf("Current working directory is: %s\n", cwd);
		return 1;
	}
}

//runs chdir, then prints the new directory or responds to the error
int cd(char* arg)
{
	//if chdir returns an error
	if (chdir(arg) != 0)
	{
		printf("Directory not found.");
		return -1;
	}
	else
	{
		if(getcwd(cwd, sizeof(cwd)) == NULL)
		{
			printf("Current working directory does not exist.");
			return -1;
		}

		else
		{
			printcwd();
			return 1;
		}
	}
}
/*
int set(char x, char y, char z)
{
	x y = z
}
*/
int execute(char** args)
{
	//If no arguments are provided
	if (args[0] == NULL)
	//Not zero, does not end main loop
		return -1;

	//Required built-in commands
	//Exiting the program, just return an int that will cause the main loop to stop (so 0)
	if (strcmp(args[0], "exit") == 0)
		return 0;

	if (strcmp(args[0], "cd") == 0)
	{
		//If there is no directory given after cd, you return to home
		//There should be a directory given otherwise
		if (args[1] == NULL)
			return cd("/home");
		else
			return cd(args[1]);
	}
	if (strcmp(args[0], "pwd") == 0)
	{
		//If current working directory does not exist
		if(getcwd(cwd, sizeof(cwd)) == NULL)
		{
			printf("Current working directory does not exist.");
			return -1;
		}
		else
		{
			return printcwd();
		}
	}
/*	if (strcmp(args[0], "set") == 0)
	{
		if(args[1] == NULL)
		{
			printf("Variable type not specified.");
			return -1;
		}
		else
		{
			char x = args[1];
			return 1;
		}


		if(args[2] == NULL)
		{
			printf("Variable name not specified.");
			return -1;
		}
		else
		{
			char y = args[2];
			return 1;
		}
		if(args[3] == NULL)
		{
			printf("Variable value not specified.");
			return -1;
		}

		else
		{
			char z = args[3];
			return 1;


		}
	}
*/    //Start a new process
	return child(args);
}

int main()
{
	char**  args;
	char*   line;
	int     stat;
	getcwd(cwd, sizeof(cwd));

	do
	{
		//Want to get the current directory before and during the loop.
		getcwd(cwd, sizeof(cwd));
		line = input("~ ");
		args = tok(line);
		stat = execute(args);
	} while(stat);
	return 0;
}
