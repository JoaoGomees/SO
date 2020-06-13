#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "ex3.c"

int main (int argc, char* argv[])
{
    int lineNumber = 0;
    char buffer[200];
    
    int size;
    
    while((size = myreadln(0,buffer,200)) > 0)
    {
        write(1,buffer,size);
        printf("  %d\n",lineNumber++);
    }
          
}

