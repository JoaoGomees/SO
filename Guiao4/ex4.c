#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

// STILL WORKING NISTO
int main(int argc, char* argv[])
{
    int fakeInput = open(argv[1],O_RDONLY);
    int fakeOutput = open(argv[2],O_WRONLY | O_CREAT,0666);
    
    int newInput = dup2(fakeInput,0);
    int newOuput = dup2(fakeOutput,1);
    
    close(fakeInput);
    close(fakeOutput);
    
    int i = 0;
    int j = 0;
    char* buffer[] =
    {
        
    }
    
    
    
    if(execv(buffer[0],buffer) < 0)
        perror("Erro ao correr o exec\n");
    
    
}
