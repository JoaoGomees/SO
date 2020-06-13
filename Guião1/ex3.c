#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

ssize_t myreadln (int fd, char* line, size_t size)
{
    int lineSize = 0;
    char c;
    
    while (lineSize < size && read(fd,&c,1) > 0)
    {
        if(c != '\0' && c != '\n')
            line[lineSize++] = c;
        
        else return lineSize;
    }
    
    return lineSize;
}

/*
int main (int argc, char* argv[])
{
    int fd = open(argv[1],O_RDONLY);
    char buffer [200];
    ssize_t size = myreadln(fd,buffer,200);
    printf("Tamanho da linha: %d\n",size);
    printf("%s\n",buffer);
    close(fd);
}*/
