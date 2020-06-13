#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include "argus.h"
#include "estrutura.h"


// variáveis globais
int id = 0;
int tempo_inatividade = 0;
int tempo_execucao = 0;
int tempo_inatividade_max = 100000;
int tempo_execucao_max = 100000;
processo p, p_atual;
char buffer[1024];
int fd;
int fd_historico;
int bytes_escritos = 0, bytes_atuais = -1;



//escreve os bytes até onde o output da tarefa vai
void write_log(int bytes_finais){
	int file_log = open("logid.txt", O_CREAT | O_APPEND | O_WRONLY, 0666);
	

	char byt_finais[256];
	int byt_finais_tamanho = sprintf(byt_finais, "%d\n", bytes_finais);
	write(file_log, byt_finais, byt_finais_tamanho);
	close(file_log);
}



// mata as threads de uma tarefa e atualiza a causa de morte
void kill_tarefa(int tarefa, int death){

	printf("\nTerminar tarefa %d.\n", tarefa);

	for (processo aux = p; aux != NULL; aux = aux->next)
	{
		if (aux -> id == tarefa)
		{
			for(int pd = 0; pd < aux->sp; pd++)
				kill(aux->pids[pd], SIGKILL);

			aux->death = death;
			break;
		}
	}
}

processo getTarefa(int pid){

	for (processo aux = p; aux != NULL; aux = aux->next)
	{
		for(int pd = 0; pd < aux->sp; pd++)
			if(aux->pids[pd] == pid) return aux;
	}
}


void alarm_handler(int signum){
	tempo_execucao ++;
	tempo_inatividade ++;
	printf("tempo: %d\n", tempo_execucao);
	alarm(1);
	if(tempo_inatividade >= tempo_inatividade_max){ write_log(bytes_atuais + bytes_escritos); 	_exit(1);}
	if(tempo_execucao >= tempo_execucao_max)	  { write_log(bytes_atuais + bytes_escritos);	_exit(2);}
}

void deadchild_handler(int signum){
	int pid, status;
	while((pid = waitpid(-1, &status, WNOHANG)) > 0){

		processo tarefa = getTarefa(pid);


		int death = WEXITSTATUS(status);

		if(death == 1 || death == 2){

			if(death == 1)  printf("\nTempo máximo de inatividade atingido.");
			if(death == 2)  printf("\nTempo máximo de execução atingido.");

			kill_tarefa(tarefa->id, death);
		}

		setTerminou(tarefa, pid);
	}
}

void ctrl_c_handler(int signum){
	unlink("fifo");
	close(fd);

	processo aux = p;
	processo tmp;

	while(aux != NULL)
	{
		tmp = aux -> next;
		freeEstrutura(aux);
		aux = tmp;
	}


	exit(0);
}


// funcao que vai buscar o id de todas as tarefas que já terminaram e imprime no screen
/*void tarefasTerminadas (processo p)
{
	processo aux = p;
	printf("\nHistorico:\n");

	while (aux != NULL)
	{
		if (aux -> terminou){

			printf("Tarefa %d, ", aux -> id);
			if(aux -> death == 0)	printf("concluida: ");
			if(aux -> death == 1)	printf("max inatividade: ");
			if(aux -> death == 2)	printf("max execucao: ");
			if(aux -> death == 3)	printf("terminada: ");
			printf("%s\n", aux -> tarefa);

		}

		aux = aux -> next;
	}
}*/

// funcao que vai buscar todas as tarefas ainda em execução
void tarefasExecucao (processo p)
{
	int fd = open("listar.txt",O_CREAT | O_APPEND | O_TRUNC|O_WRONLY,0666);
	processo aux = p;
	char buffer[256];
	int bytes_read;
	while (aux != NULL)
	{
		if (!aux -> terminou)
		{
			bytes_read = sprintf(buffer,"Tarefa %d: %s\n", aux -> id, aux -> tarefa);
			write(fd,buffer,bytes_read);
		}
		aux = aux -> next;
	}

	close(fd);
}

//funcao que recebe uma string e vê em quantas parter vai ser dividida
int getNumberComandos (char* frase, char tok)
{
	int resultado = 1;
	for (int i = 0; i < strlen(frase); i++)
	{
		if (frase[i] == tok)
		{
			resultado++;
		}
	}

	return resultado;
}

//funçao que passa uma string para array usando um token
char ** arrayExecutar(char* comandos, int numberComandos, char * tok){

	char** comando = malloc((numberComandos + 1) * sizeof(char*));

	char* token = strtok(comandos, tok);

	for(int c = 0; token != NULL; c++){

		comando[c] = malloc(strlen(token) * sizeof(char));
		strcpy(comando[c],token);

		token = strtok(NULL, tok);
	}

	return comando;
}




int main (int argc, char** argv)
{
	p_atual = p;
	int bytes_read;
	char buffer [256];
	int pid, status, n;
	unlink("fifo");
	int try1 = open("historico.txt", O_CREAT | O_TRUNC | O_RDWR, 0666);
	int try2 = open("log.txt", O_CREAT | O_TRUNC | O_RDWR, 0666);
	int try3 = open("logid.txt", O_CREAT | O_TRUNC | O_RDWR, 0666);
	close(try1);
	close(try2);
	close(try3);

	if( mkfifo("fifo", 0666) == -1 )
	{
	    perror("Named pipe failed!");
	    exit(-1);
	}

	signal(SIGINT, ctrl_c_handler);
	
	fd = open("fifo",O_RDONLY);

	
	char* token;

	while (1)
	{


		while((bytes_read = read (fd,buffer,255)) > 0)
		{


		buffer[bytes_read-1] = ' ';
		token = strtok(buffer," ");

		
		if (!strcmp(token,"tempo-inatividade"))
		{
			token = strtok(NULL, " ");
			tempo_inatividade_max = atoi(token);

			printf("\nTempo inatividade: %d\n", tempo_inatividade_max);

		}

		else if(!strcmp(token,"output"))
		{
			token = strtok(NULL, " ");
			int tarefa = atoi(token);
			char buffer[256];
			int sp = 0;

			int fd_logid = open("logid.txt",O_RDONLY,0666);
			char c;

			while(read(fd_logid,&c,1) > 0)
			{
				if (c != '\n')
					buffer[sp++] = c;

				else
				{
					char realStr [sp+1];
					for (int i = 0; i < sp; i++)
						realStr[i] = buffer[i];

					realStr[sp +1] = ':';

					char* token = strtok(realStr,":");
					if (atoi(token) == tarefa)
					{
						token = strtok(NULL,":");
						char* token2 = strtok(token," ");
						int start = atoi(token2);
						token2 = strtok(NULL," ");
						int end = atoi(token2);

						int fd_log = open("log.txt",O_RDONLY,0666);
						lseek(fd_log,start,SEEK_SET);

						char buffer2[end - start];
						read(fd_log,buffer2,end - start);
						close(fd);
						fd = open("fifo",O_WRONLY);
						write(fd,buffer2,end - start);
						close(fd);
						fd = open("fifo",O_RDONLY);

						close(fd_log);
						close(fd_logid);
						break;
					}

					sp = 0;

				}


			}
		}

		else if (!strcmp(token, "tempo-execucao"))
		{
			token = strtok(NULL, " ");
			tempo_execucao_max = atoi(token);

			printf("\nTempo execução: %d\n", tempo_execucao_max);
		}

		else if (!strcmp(token, "historico"))
		{
			fd_historico = open("historico.txt", O_RDONLY,0666);
			int leu;
			char bufferHistorico [256];
			close(fd);
			fd = open("fifo",O_WRONLY);
			while ((leu = read(fd_historico,bufferHistorico,256)) > 0)
			{
				write (fd,bufferHistorico,leu);
			}
			close(fd);
			close(fd_historico);
			fd = open("fifo",O_RDONLY);
		}

		else if (!strcmp(token, "listar"))
		{

			processo aux = p;
			int stringSize = 0;
			int stringCapacidade = 256;
			char* string = malloc(stringCapacidade*sizeof(char));
			int bytes_read;

			while (aux != NULL)
			{
				if (!aux -> terminou)
				{
					if (stringSize > stringCapacidade + 30)
					{
						string = realloc(string,stringCapacidade * 2);
						stringCapacidade *= 2;
					}

					bytes_read = sprintf(&string[stringSize],"Tarefa %d: %s\n", aux -> id, aux -> tarefa);
					stringSize += bytes_read;
				}
				aux = aux -> next;
			}
			close(fd);
			fd = open("fifo",O_WRONLY);
			write (fd,string,stringSize);

			free(string);
			close(fd);
			fd = open("fifo",O_RDONLY);

		}

		else if (!strcmp(token,"terminar"))
		{
			token = strtok(NULL, " ");
			int tarefa = atoi(token);
			
			kill_tarefa(tarefa, 3);

			int file = open("log.txt", O_CREAT | O_APPEND | O_RDWR, 0666);
			if(file < 0){
				perror("failed to open output file");
			}

			int bytes_finais = lseek(file, 0, SEEK_END);

			write_log(bytes_finais);
			close(file);
			
		}

		else if (!strcmp(token,"ajudar")){
			close(fd);
			fd = open("fifo",O_WRONLY);
			char * help = "\nGuia:\ntempo-inatividade segs\ntempo-execucao segs\nhistorico\nlistar\nterminar nr-tarefa\nexecutar 'p1 | p2 | ... | pn'\noutput nr-tarefa\n\n";
			write(fd,help,strlen(help));
			close(fd);
			fd = open("fifo",O_RDONLY);
		}

		else if (!strcmp(token,"executar"))
		{	

			char* token_pelicas = strtok(NULL,"'");
			printf("\nComandos recebidos: %s\n",token_pelicas);

			if(token_pelicas == NULL) exit(0);

			char* tarefa = strdup(token_pelicas);
			int numberComandos = getNumberComandos(token_pelicas, '|');

			//adcionar tarefa à estrutura
			if (id == 0)
			{
				p = malloc(sizeof(struct processo));
				p_atual = p;
				initEstrutura (p_atual, id, tarefa, numberComandos);	
			}

			else
			{
				processo x = malloc(sizeof(struct processo));
				p_atual -> next = x;
				p_atual = p_atual -> next;
				initEstrutura(p_atual, id, tarefa, numberComandos);
			}
			id++;

			char** comandos = arrayExecutar(token_pelicas, numberComandos, "|");
			
			//sinais
			signal(SIGALRM, alarm_handler);
			signal(SIGCHLD, deadchild_handler);


			//processar os comandos e fazer forks here

			int pp[numberComandos*2-1][2];
			
			for (int i = 0; i < numberComandos; i++)
			{
				printf("A executar comando: %s\n",comandos[i]);
				
				//criar os pipes principais
				if( pipe(pp[i*2]) < 0 ){
					perror("Error creating pipe!");
					return 1;
				}
//				printf("Pipe %d criado.\n", i*2);
				
				//verificar inatividade criando pipes que fazem interligaçoes entre pipes -> p1 | verifica inativ | p2
				if(i != numberComandos-1){
					if( pipe(pp[i*2+1]) < 0 ){
						perror("Error creating pipe!");
						return 1;
					}
//					printf("Pipe %d criado.\n", i*2+1);
				}

				int tamanhoExec = getNumberComandos(comandos[i], ' ');
				char** comando_exec = arrayExecutar(comandos[i], tamanhoExec, " ");
				
				comando_exec[tamanhoExec] = NULL;
				

				pid = fork();

				//executar o comando
				if(!pid){
					
					for (int j = 0; j < i*2-1; j++){
						close(pp[j][0]);
						close(pp[j][1]);
					}

					if(i!=0){
						dup2(pp[i*2-1][0], 0);
						close(pp[i*2-1][1]);
					}
					/*
					if(i==0){
						dup2(fd, 0);
					}*/

					dup2(pp[i*2][1], 1);
					close(pp[i*2][0]);

					if(execvp(comando_exec[0], comando_exec) < 0){

						perror("failed exec");
						_exit(-1);
					}
					close(fd);

				}
				
				addPid(p_atual, pid);

				pid = fork();

				//controlar inatividade e escrever para ficheiro
				if(!pid){
					alarm(1);

					for (int j = 0; j < i*2; j++){
						close(pp[j][0]);
						close(pp[j][1]);
					}

					//escrever para ficheiro
					if(i == (numberComandos-1)){

						close(pp[i*2][1]);

						int file_log = open("logid.txt", O_CREAT | O_APPEND | O_WRONLY, 0666);

						int file = open("log.txt", O_CREAT | O_APPEND | O_RDWR, 0666);
						if(file < 0){
							perror("failed to open output file");
						}

						bytes_atuais = lseek(file, 0, SEEK_END);
						
						char byt_atuais[256];
						int byt_atuais_tamanho = sprintf(byt_atuais, "%d:%d ", p_atual->id, bytes_atuais);
						write(file_log, byt_atuais, byt_atuais_tamanho);

						while((n = read(pp[i*2][0], buffer, 1024)) > 0){
							tempo_inatividade = 0;
							bytes_escritos += n;
							write(file, buffer, n);
						}

						write_log(bytes_atuais + bytes_escritos);

						close(pp[i*2][0]);
					}
						
					//checkar tempos de inatividade dos pipes principais
					else {		

						close(pp[i*2][1]);
						close(pp[i*2+1][0]);
						

						while((n = read(pp[i*2][0], buffer, 1024)) > 0){
							tempo_inatividade = 0;
							printf("Atividade no pipe comando%d - comando%d\n", i, i+1);

							write(pp[i*2+1][1], buffer, n);
						}
						close(pp[i*2][0]);
						close(pp[i*2+1][1]);
					}

					
					_exit(0);
				}


				addPid(p_atual, pid);


			}



			for(int i = 0; i < numberComandos*2-1; i++){
				close(pp[i][0]);
				close(pp[i][1]);
			}

			free(tarefa);
			free(comandos);

		}
		
		else printf("palavra errada\n");
		}
	}


	return 0;
}
