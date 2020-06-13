#ifndef ESTRUTURA_H
#define ESTRUTURA_H
typedef struct processo
{
	int sp;					// contador para sabermos em que size do array de pids vamos
	int id;					// id do processo
	int terminou;			// diz se este processo ja terminou ou nao
	char* tarefa;			// string com a tarefa
	int* pids;				// array com os pids dos processsos encarregados desta tareaf
	int* pidsTerminados;
	int death;				// 0->concluida		1->max-inatividade 		2->max-execucao		3->terminada
	struct processo* next; // apontar para a proxima lista
}*processo;

void initEstrutura (processo p, int id, char* tarefa, int numeroComandos);
void addPid (processo p, int pid);
void setTerminou (processo p, int pid);
int* getPids (processo p);
void freeEstrutura (processo p);

#endif
