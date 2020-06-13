#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main (int argc, char* argv[])
{
    printf("pid processo: %d, pid pai: %d\n", getpid(), getppid());
    return 0;
}
