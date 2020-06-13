#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main (int argc, char* argv[])
{
    pid_t pid;
    pid_t pid_filho;
    int status;
    
    switch ((pid = fork()))
    {
        case -1:
            perror ("Erro com fork");
            return 1;
            
        case 0:
            printf("Processo filho: %d, Processo pai: %d\n", getpid(), getppid());
            _exit(0);
            
        default:
            pid_filho = wait (&status);
            printf("Processo pai: %d, Processo - pai pai: %d, Processo filho: %d\n", getpid(), getppid(), pid);
            
    }
    
    
    return 0;
}
