#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

int main (int argc, char * argv[]){

    int pid = fork();
    

    //checka se é o filho
    if (pid == 0){
        char* exec_args[] = {"ls","-l",NULL};
        
        if(execv("/bin/ls",exec_args) < 0)
            perror("Erro ao correr o exec\n");
        
    }
    
    //se nao for filho, é pai e entao espera pela terminação do filho
    else{
        wait(NULL);
        printf("Terminou a execução do programa\n");
    }
    
    return 0;
}
