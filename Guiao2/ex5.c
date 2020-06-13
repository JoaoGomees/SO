#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main ()
{
    
    int pid;
    int status;
    
    for(int i = 0; i < 10; i++)
    {
        if(fork() != 0)
        {
            break;
        }
        
        else
        {
            printf("Sou o processo %d e sou filho do processo %d\n",getpid(),getppid());
        }
    }
    
    while((pid = wait(&status)) != -1)
    {
        printf("Sou o processo %d e o meu procesos filho %d terminou execução\n",getpid(),pid);
    }
    return 0;
}
