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
	char arg_array[MAX_ARG][MAX_ARG_LENGTH];
    // compiler should not optimize this variable
    volatile bool run = true;

    while (run)
    {
		pid_t pid = 0;
        fflush(stdout);
        fprintf(stdout, "\nosh>");

		size_t arg_count = 0;

		// TODO: count how many white spaces has been passed in 
		// to calculate arg and use malloc instead
        // Read user input
		while (fgets(buffer, BUFFER_SIZE - 1, stdin))
		{
			const char * argument = buffer;
			// Parse into array
			// Malloc every arg? or is it inefficient?
			while (arg_count < MAX_ARG - 1 && sscanf(argument, "%s", arg_array[arg_count]) != EOF) 
			{
				arg_count ++;
		
				// No more arguments
				argument = strchr(argument, ' ');
				if (argument == NULL || ++argument == NULL) 
					break;
			}

			if (arg_count >= MAX_ARG - 1)
				break;


			// User pressed enter; Loop exit condition 
			if (strchr(buffer, '\n')) 
				break;
		}

		arg_array[arg_count][0] = '\0';

        // Fork child process
		pid = fork();
		if (pid == -1)
		{
			//error
			return 1;
		}

		// Parent
		if (pid > 0)
		{
			int status = 0;
			waitpid(pid, &status, 0);
		}

		// Child
		if (pid == 0) 	
		{
			char *pArg = arg_array[1];
			char **const ppArg = &pArg;
			if (execvp(arg_array[0], ppArg) == -1)
			{
				printf("%s\n", arg_array[0]);
				printf("%s\n", *ppArg);
				exit(1);
			//error
			}	
		}
        // Child invoke exec

        // parent will invoke wait() unless command included &
    }

    return 0;
}
