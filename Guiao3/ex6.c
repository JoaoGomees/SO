#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int my_system(char* command){
    int status;
    //dividir a string (strtok/strsep)   -----> ls e -l da main em duas strings como está em baixo
    //resultado: args[] = ["ls", "-l"];
    //fork_ret = fork();
    //  se for processo filho:
    //                      execvp(args[0], args)
    //                      se execvp retornou, _exit(cod de saida de erro)
    //  se for o pai: wait(&status);
    //                  return(WEXITSTATUS(status));
}

int main(int argc, char * argv[]){
    char comando1[] =  ;
    char comando2[] =  ;
    char comando3[] = ;

    int ret = my_system("ls -l");
    return 0;
}
