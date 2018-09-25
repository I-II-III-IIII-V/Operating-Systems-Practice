#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#define MAX_LINE 80
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    char *arguments[MAX_LINE / 2 + 1];
	char buffer[BUFFER_SIZE];
    // compiler should not optimize this variable
    volatile bool run = true;

    while(run)
    {
        fprintf(stdout, "osh>");
        fflush(stdout);

        // Read user input
		while((fgets(buffer, BUFFER_SIZE, stdin))
		{
			// Parse arguments

		}
        // Fork child process

        // Child invoke exec

        // parent will invoke wait() unless command included &
    }

    return 0;
}
