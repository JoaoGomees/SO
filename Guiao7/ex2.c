#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv)
{
	if (argc < 3)
	{
		printf("error\n");
		return 5;
	}

	int files_count = argc - 2;
	char** files = argv + 2;

	for (int i = 0; i < files_count; i++)
	{
		if (fork() == 0)
		{
			printf("grep número % d com identificador %d\n",i,getpid());

			if (execlp ("grep","grep",argv[1], files[i],NULL) < 0)
			{
				perror ("exec grep");
				exit (10);
			}
		}
	}

	int status = 0;
	int found = 0;
	int pid = -1;

	while ((pid = wait(&status)) > 0 && !found)
	{
		if (WIFEXITED(status))
		{
			switch(WEXITSTATUS(status))
			{
				case 0: found = 1;
						break;
			}
		}
	}

	// cas nao se tenha encontrado a palavra, retorna 1. neste caso, todos os filhos terminaram
	if (!found)
		return 1;



	return 0;
}
