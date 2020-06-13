#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
    int pid;
    int status;
    int x = 0;
    
    for (int i = 1; i < 11; i++)
    {
        if((pid = fork()) == 0)
        {
            printf("Meu processo: %d ---- Processo do Pai: %d\n",getpid(), getppid());
            _exit(x);
        }
        
        else
        {
            wait(&status);
            printf("Filho %d terminou\n", WEXITSTATUS(status));
        }
        x++;
    }
}



