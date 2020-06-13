#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main ()
{
	int fd;
	if( fd = (open("fifo.txt",O_WRONLY)) < 0)
	{
		perror("Error no open");
		exit(1);
	}

	char buf[100];

	int bytes_read = 0;

	while ((bytes_read = read (0,buf,100)) > 0)
	{
		write(fd,buf,bytes_read);
	}
}
