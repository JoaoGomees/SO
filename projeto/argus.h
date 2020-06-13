#ifndef ARGUS_H
#define ARGUS_H

#include "estrutura.h"

void ctrl_c_handlerClient(int signum);
void write_log(int bytes_finais);
void kill_tarefa(int tarefa, int death);
processo getTarefa(int pid);
void alarm_handler(int signum);
void deadchild_handler(int signum);
void ctrl_c_handler(int signum);
void tarefasExecucao (processo p);
int getNumberComandos (char* frase, char tok);
char ** arrayExecutar(char* comandos, int numberComandos, char * tok);

#endif
