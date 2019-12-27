#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 128

int main()
{ 
    time_t curTime;
    struct tm * timeInfo; 

    unlink("file_fifo");

    if((mkfifo("file_fifo", 0666)) == -1)
    {
        printf("Can\'t create FIFO!\n");
        _exit(1);
    } 
  
    int fileDescr;

    pid_t child;
    child = fork();

    if(child == 0)
    { 
        sleep(1);
        ssize_t size = 0;

        char res[SIZE];
        
        if((fileDescr = open("file_fifo", O_RDONLY)) < 0)
        {
            printf("Cant open FIFO to read!\n");
            _exit(1);
        }
        size = read(fileDescr, res , SIZE);
        close(fileDescr);
        
        if(size <= 0)
        {
            printf("Can\'t read string.\n");
            _exit(1);   
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

        ssize_t size = 0;
        if((fileDescr = open("file_fifo", O_WRONLY)) < 0)
        {
            printf("Cant open FIFO to write!");
            _exit(1);
        }
        size = write(fileDescr, (void*)strToPipe, strlen(strToPipe));
        close(fileDescr);

        printf("\nParent write : %ld.\n", (size_t)size);
        printf("Parent exit.\n\n");
        
        waitpid(child, 0, 0); 
    }    
    else
    {
        printf("Can't create a child process!");
        _exit(1);
    }
    return 0; 
    
}
