#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>


int main ()
{
    int pid;
    int status;
    
    for (int i = 1; i < 11; i++)
    {
        if(fork() == 0)
        {
            printf("Meu processo: %d ---- Processo do Pai: %d\n",getpid(), getppid());
            _exit(i);
        }
    }
    
    while ((pid = wait(&status)) != -1) {
        printf("O processo filho %d terminou a execução com codigo de saida %d\n", pid, WEXITSTATUS(status));
    }
    
}
