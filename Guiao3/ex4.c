#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char *argv[]){


    char * exec_args = {
        "outro_programa",
        "arg1",
        "arg2",
        "arg3",
        NULL
    };

    int ret = execv("./print_args", exec_args);

    return ret;
}