#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estrutura.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// inicia a estrutura
void initEstrutura (processo p, int id, char* tarefa, int numeroComandos)
{
	p -> sp = 0;
	p -> id = id;
	p -> terminou = 0;
	p -> tarefa = malloc(strlen(tarefa) * sizeof(char*));
	p -> pids = malloc(numeroComandos * 2 * sizeof(int*));
	p -> pidsTerminados = malloc(numeroComandos * 2 * sizeof(int*));
	p -> death = 0;
	strcpy(p -> tarefa,tarefa);

	for (int i = 0; i < numeroComandos*2; i++)
		p -> pidsTerminados[i] = 0;
}

// adiciona um pid à lista de pids
void addPid (processo p, int pid)
{
	p -> pids[p -> sp++] = pid;
}

// funcao que indica que um pid terminou, e verifica se a tarefa em si ja acabou
void setTerminou (processo p, int pid)
{
	for (int i = 0; i < p -> sp; i++)
	{
		if ((p -> pids[i] == pid) && !p -> pidsTerminados[i])
		{
			p -> pidsTerminados[i] = 1;
			break;
		}
	}

	int checka = 1;
	for (int i = 0; i < p -> sp; i++)
	{
		if (!p -> pidsTerminados[i])
		{
			checka = 0;
			break;
		}
	}

	p -> terminou = checka;

	if (checka)
	{
		int fd = open("historico.txt",O_CREAT | O_APPEND |O_WRONLY,0666);
		int bytes_read;
		char buffer[256];

		if(p -> death == 0)
			bytes_read = sprintf(buffer,"Tarefa %d concluída: %s\n",p->id,p -> tarefa);

		else if(p -> death == 1)
			bytes_read = sprintf(buffer,"Tarefa %d max inatividade: %s\n",p->id,p -> tarefa);

		else if(p -> death == 2)
			bytes_read = sprintf(buffer,"Tarefa %d max execucao: %s\n",p->id,p -> tarefa);

		else if(p -> death == 3)
			bytes_read = sprintf(buffer,"Tarefa %d terminada: %s\n",p->id,p -> tarefa);



		write(fd,buffer,bytes_read);
		close(fd);

	}

}

// funcao que devolve os pids de todos os processos envolvidos, useful para quando tivermos de funçar a terminaçao de uma tarefa
int* getPids (processo p)
{
	return p -> pids;
}

void freeEstrutura (processo p)
{
	free (p -> tarefa);
	free (p -> pids);
	free (p -> pidsTerminados);

	free(p);
}



        
    
    
    
    
    
    
    
    
    
    

