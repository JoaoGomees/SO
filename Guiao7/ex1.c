#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int ctrl_c_counter = 0;
int segundos = 0;

void sigint_handler (int signum)
{
	printf("\n");
	printf("Tempo passado: %d\n", segundos);
	ctrl_c_counter++;
}

void sigalrm_handler (int signum)
{
	segundos++;
	alarm(1);
}

void sigquit_handler (int signum)
{
	printf("Carregou %d vezes em ctrl_c\n", ctrl_c_counter);
}

int main (int argc, char ** argv)
{
	if(signal(SIGINT,sigint_handler) == SIG_ERR)
	{
		perror ("signal");
		exit(1);
	}

	if(signal(SIGALRM,sigalrm_handler) == SIG_ERR)
	{
		perror ("signal");
		exit(1);
	}

	if(signal(SIGQUIT,sigquit_handler) == SIG_ERR)
	{
		perror ("signal");
		exit(1);
	}

	alarm(1);
	while (1)
	{
		pause();
	}

	return 0;
}
