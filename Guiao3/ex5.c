#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char const * argv[]){

    //ciclo a percorrer argv
    for(int i = 1; i<argc; i++){
        //fork()
        //se for processo filho
            //execlp(argv[i], argv[i], NULL);
            //verificar se exec correu bem senao _exit(1);
    }
    
    //ciclo a percorrer argv(dado por argc)
    for()
        //wait(...)

    return 0;   
}