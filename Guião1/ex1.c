#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

/*
s
ssize_t read(int fildes, void *buf, size_t nbyte);
ssize_t write(int fildes, const void *buf, size_t nbyte);
off_t lseek(int fd, off_t offset, int whence);
int close(int fildes); */

//funcao que cria o ficheiro f1 para o ficheiro f2 que se não existir também é criado;
void mycp (char* f1, char*f2)
{
    ssize_t size;
    
    int fd1 = open(f1,O_RDONLY); // abrir ficheiro um apenas em modo leitura
    int fd2 = open(f2,O_WRONLY | O_CREAT,0666); // abrir ficheiro dois em modo de escrita
    char buffer[128]; // buffer que vai lendo o ficheiro 1 e escreve no ficheiro 2
    
    while ((size = read(fd1,buffer,10)) > 0)
    {
        write(fd2,buffer,10);
    }
    
    close(fd1);
    close(fd2);
}

int main (int argc, char*argv[]) {
    if (argc == 3)
    {
        mycp(argv[1],argv[2]);
    }
    
    return 0;
}

