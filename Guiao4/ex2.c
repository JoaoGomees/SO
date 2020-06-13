#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){
    
    //cria os ficheiros para subsitutuir os std normais
    int fakeInput = open("/etc/passwd", O_RDONLY);
    int fakeOutput = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fakeError = open("erros.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    //alterar os stdin, stdout e stderror para os nossos ficheiros
    int fakeStdin = dup2(fakeInput, 0);
    int fakeStdout = dup2(fakeOutput, 1);
    int fakeStderr = dup2(fakeError, 2);

    //fecha os primeiros ficheiros
    close(fakeInput);
    close(fakeOutput);
    close(fakeError);
    
    int res;
    if((res = fork()) == 0)
    {
        char buffer[128];
        int r = 0;
        while ((r = read(0, buffer, 16)) > 0)
        {
            write(1, buffer, r);
            write(2, buffer, r);
        }
        
        //cat
        ssize_t re;
        char buffer1 [2048];
        
        while ((re = read(0,buffer1,1024)) > 0)
            write(1,buffer1,re);
        
        _exit(0);
    }
    
    int pid, status;
    pid = wait(&status);
    
    
    
    
    
    

    return 0;
}
