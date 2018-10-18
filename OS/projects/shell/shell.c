#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE 80
#define BUFFER_SIZE 1024
#define MAX_ARG 40
#define MAX_ARG_LENGTH 128
int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	char *arg_array[MAX_ARG];
	const char * white_space =  " \t\r\n\v\f";
    // compiler should not optimize this variable
    volatile bool run = true;

    while (run)
    {
		pid_t pid = 0;
		bool new_line = false;
		size_t index = 0;

        fflush(stdout);
        fprintf(stdout, "osh>");

        // read user input
		while (!new_line && fgets(buffer, BUFFER_SIZE - 1, stdin))
		{
			const char * argument = buffer;

			// user pressed enter; Loop exit condition 
			if (strchr(buffer, '\n')) 
				new_line = true;

			arg_array[index] = strtok(buffer, white_space);

			while (arg_array[index] != NULL)
			{
				index++;
				// too many arguments provided!
				if (index == MAX_ARG - 1)
					arg_array[index] = NULL;

				arg_array[index] = strtok(NULL, white_space);
			}
		}

		// check if exit before anything else
		if (strcmp(arg_array[0], "exit") == 0)
		{
			return 0;
		}

        // fork child process
		pid = fork();
		if (pid == -1)
		{
			//error
			return 1;
		}

		// parent
		if (pid > 0)
		{
			int status = 0;
			waitpid(pid, &status, 0);
		}

		// child
		if (pid == 0) 	
		{
			if (execvp(arg_array[0], arg_array) == -1)
				exit(1);
		}

        // parent will invoke wait() unless command included &
	}

    return 0;
}
