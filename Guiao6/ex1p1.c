#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main ()
{
	if (mkfifo("fifo.txt",0666) == 0)
		printf("Correu bem!\n");

	else printf("Erro ao criar fifo\n");

	return 0;
}
