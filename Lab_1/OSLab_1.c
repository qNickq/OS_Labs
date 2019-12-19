#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>


void atExitFunc(pid_t pid)
{
    printf("AtExit for process: %d\n" , pid);
}

int main (int argc , char* argv[])
{ 
    printf("Process with pid: %d has parent with pid: %d\n", getpid(), getppid());

    pid_t pid;
    pid = fork();

    if(pid != 0)
    {
        printf("Process with pid: %d create new process with pid: %d\n", getpid(), pid); 
        waitpid(pid, NULL,  0); 
    }
    else
    {
        printf("Process with pid: %d has parent with pid: %d\n", getpid(), getppid());
    }
      
    atExitFunc(getpid());
    return 0;
}