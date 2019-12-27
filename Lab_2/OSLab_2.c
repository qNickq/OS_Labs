#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char* argv[])
{
    struct stat fileStat;
    
    for (size_t i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }
    
    for (size_t i = 0; i < ; i++)
    {
        
    }
    
    return 0;
}