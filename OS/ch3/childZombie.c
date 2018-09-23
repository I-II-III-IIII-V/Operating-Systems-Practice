#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    pid = fork();
    if (pid == -1) 
    {
        fprintf(stderr, "Failed to create a child process\n");
        return 1;
    }

    // Child process
    if (pid == 0)
    {
        exit(0);
    }

    // Parent process
    if (pid > 0) 
    {
        int status;
        sleep(15);
        waitpid(pid, &status, 0);
        exit(0);
    } 

    return 0;
}
