#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h>

int main(int argc, char *argv[]){
    //criar pipe
    int pipe_fd[2];
    if(pipe(pipe_fd) < 0){
        perror("pipe");
        exit(1);
    }

    //criar processo filho
    int pid = -1;
    if((pid = fork()) == 0){
        //herda descritores do pipe
        //fechamos descritores que nao serao utilizados no processo filho
        close(pipe_fd[1]);

        //temos extremidade de leitura do pipe
        char buf[128];
        int bytes = read(pipe_fd[0], buf, 128);

        printf("li %d bytes: %s\n", bytes, buf);
        _exit(0);
    }
    else {
        close(pipe_fd[0]);

        //temos extremidade de escrita do pipe
        char* str = "JHASDJASBNJBEBEU";
        write(pipe_fd[1], str, strlen(str) + 1);

        wait(NULL);
    }





    return 0;
}
