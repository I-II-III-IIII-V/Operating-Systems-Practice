#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

#define MEMORY_NAME "TimeTest"
#define SIZE 4096

pid_t pid = -1;
int fd = -1;
char *shared_memory_ptr = NULL;
char *execute_name = NULL;
const char *shared_memory_name;
struct timeval timet_t = {0};

// Internal helper function to cleanup global variables
void __clean_up();

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "usage: %s <program name>\n", argv[0]);
        return 0;
    }

    if (strcmp(argv[0], argv[1]) == 0)
    {
        fprintf(stderr, "Program will keep spawning new process\n");
        return 0;
    }

    atexit(__clean_up);


    execute_name = strdup(basename(argv[1]));
    if (!execute_name)  
    {
        fprintf(stderr, "%d: %s\n", __LINE__, strerror(errno));
        return 1;
    }
        
    // Make shared memory so that child process can access it too

    // Retrieve shared memory object
    fd = shm_open(MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
    {
        fprintf(stderr, "%d: %s\n", __LINE__, strerror(errno));
        return 1;
    }

    // Extend the memory to specific size
    if (ftruncate(fd, SIZE) == -1)
    {
        fprintf(stderr, "%d: %s\n", __LINE__, strerror(errno));
        return 1;
    }

    // Map memory to the shared memory object
    shared_memory_ptr = (char *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_memory_ptr == MAP_FAILED)
    {   
        fprintf(stderr, "%d: %s\n", __LINE__, strerror(errno));
        return 1;
    }

    pid = fork();
    if (pid == -1) 
    {
        fprintf(stderr, "%d: %s\n", __LINE__, strerror(errno));
        return 1;
    }

    // Child process
    if (pid == 0)
    {
        // record current time
        gettimeofday(&timet_t, NULL);
        
        // Write time to shared region of memory
        // I don't know how this can fail at this stage
        sprintf(shared_memory_ptr, "%ld", timet_t.tv_usec);

        // Execute program
        if (execl(argv[1], execute_name, (char *)NULL) == -1)
        {
            fprintf(stderr, "%d: %s\n", __LINE__, strerror(errno));
            return 1;
        }   
    }

    // Parent process
    if (pid > 0)
    {
        int status = -1;
        long init_time;
        long time_result;
        waitpid(pid, &status, 0);

        // Get current time as soon as child exits
        gettimeofday(&timet_t, NULL);

        // Read from memory to get initial time
        init_time = atol(shared_memory_ptr);
        time_result = timet_t.tv_usec - init_time;
        fprintf(stdout, "time taken: %ld microseconds\n", time_result);
    }

    return 0;
}

// Function to deallocate memory
void __clean_up()
{
    if (fd > 0)
    {
        shm_unlink(MEMORY_NAME);
    }

    if (shared_memory_ptr) 
    {
        munmap(shared_memory_ptr, SIZE);
    }
       
    if (execute_name)
    {
        free(execute_name);
    }
}
    

