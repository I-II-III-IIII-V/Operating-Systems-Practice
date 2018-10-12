#include <stdio.h>
#include <unistd.h>
#include <string.h>
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
        fflush(stdout);
        fprintf(stdout, "\nosh>");

		size_t arg_count = 0;

        // Read user input
		while (fgets(buffer, BUFFER_SIZE - 1, stdin))
		{
			const char * argument = buffer;
			// Parse into array
			// Malloc every arg? or is it inefficient?
			while (sscanf(argument, "%s", arg_array[arg_count]) != EOF) 
			{
				arg_count ++;
				// Just ignore the rest of the arg
				if (arg_count >= MAX_ARG)
					break;
		
				// No more arguments
				argument = strchr(argument, ' ');
				if (argument == NULL || ++argument == NULL) 
					break;
			}

			for (size_t i = 0; i < arg_count; i++) 
				printf("%s ", arg_array[i]);

			// User pressed enter; Loop exit condition 
			if (strchr(buffer, '\n')) 
				break;
		}

        // Fork child process
        // Child invoke exec

        // parent will invoke wait() unless command included &
    }

    return 0;
}
