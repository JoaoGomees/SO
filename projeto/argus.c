#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "argus.h"


int fd;

void ctrl_c_handlerClient(int signum){
	close(fd);
	_exit(0);
}

int main (int argc, char** argv)
{
	int bytes_read;
	char buffer [256];
	char* token;
	fd = open("fifo",O_WRONLY);

	signal(SIGINT, ctrl_c_handlerClient);
	
	if(argc > 1){
		char send [80];

		if(argc == 2)
		{
			bytes_read = sprintf(send, "%s\n", argv[2]);
			write(fd,send,bytes_read);
		}
		if(argc == 3)
		{
			bytes_read = sprintf(send, "%s %s\n", argv[1], argv[2]);
			write(fd,send,bytes_read);
		}


	}
	else{
		while ((bytes_read = read (0,buffer,255)) > 0)
		{
			write(fd,buffer,bytes_read);
			close(fd);
			buffer[bytes_read-1] = ' ';
			token = strtok(buffer," ");

			if (!strcmp(token,"historico") || !strcmp(token,"listar") || !strcmp(token,"ajudar") || !strcmp(token,"output"))
			{
				fd = open("fifo",O_RDONLY);
				while ((bytes_read = read (fd,buffer,256)) > 0)
				{
					write(1,buffer,bytes_read);

				}

				close(fd);
			}

			fd = open("fifo",O_WRONLY);

		}
	}

	close (fd);
	return 0;
}
