#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char* argv[])
{
    ssize_t re;
    char buffer [2048];
    
    while ((re = read(0,buffer,1024)) > 0)
        write(1,buffer,re);
    
    return 0;
}
