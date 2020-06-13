#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

ssize_t myreadln (int fd, char* line, size_t size)
{
    int lineSize = 0;
    char buffer[2];
    
    while (lineSize < size && read(fd,buffer,2) > 0)
    {
        if(buffer[0] != '\n'&& buffer[1] != '\n')
        {
            line[lineSize++] = buffer[0];
            line[lineSize++] = buffer[1];
        }
            
        
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
