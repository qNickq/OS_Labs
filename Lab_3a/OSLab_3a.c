#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 128

int main()
{
    time_t curTime;
    struct tm * timeInfo; 
    
    int fileDescr[2];
    if(pipe(fileDescr) < 0)
    {
        printf("Can\'t create pipe\n");
        return 1;
    } 

    pid_t child;
    child = fork();
    if(child == 0)
    { 
        sleep(1);
        ssize_t size = 0;

        char res[SIZE];

        close(fileDescr[1]);
        size += read(fileDescr[0], res , SIZE);
        close(fileDescr[0]);
        
        if(size <= 0)
        {
            printf("Can\'t read string\n");
            return -1;    
        }
        printf("Child read : %ld ", (size_t)size);

        time(&curTime); 
        timeInfo = localtime(&curTime);
        
        printf("%s\n", res);
        printf("Children time : %s\n", asctime(timeInfo));
        
    } 
    else if(child > 0)
    {
        time(&curTime); 
        timeInfo = localtime(&curTime);

        char strToPipe[SIZE];
        sprintf(strToPipe, "from PID : %d\nParent time : %s", getpid(), asctime(timeInfo));

        close(fileDescr[0]);
        ssize_t size = 0;
        
        size += write(fileDescr[1], (void*)strToPipe, strlen(strToPipe));
        close(fileDescr[1]);
        printf("\nParent write : %ld.\n", (size_t)size);
        printf("Parent exit.\n\n");
        
        waitpid(child, NULL, 0); 
    }    
    else
    {
        printf("Can't create a child process!");
        return -1;
    }
    return 0; 
    
}
