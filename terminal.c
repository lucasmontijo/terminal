#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int func_cd(char **args);
int func_help(char **args);
int func_exit(char **args);

//Comandos inclusos no terminal
char *inclusas_str[] = {
  "cd",
  "help",
  "exit"
};

//funções atreladas aos comandos inclusos no terminal
int (*inclusas_func[]) (char **) = {&func_cd, &func_help, &func_exit};

//numero de funcionalidades inclusas no terminal 
int num_inclusas() {
	return sizeof(inclusas_str) / sizeof(char *);
}

//função cd para navegar em diretórios
int func_cd(char **args){
	if(args[1] == NULL) printf("Sem argumento para o cd. \n");
	else{
		if(chdir(args[1]) != 0) perror("lsh");
	}
	return 1;
}

//função para sair do terminal
int func_exit(char **args){
	return 0;
}

//função help ajuda
int func_help(char **args){
	printf("Digite o nome dos programas, argumentos e em seguida digite enter.\n");
	printf("Algumas funcionalidades já estão inclusas no terminal, como as seguintes:\n");

	for(int i=0; i<num_inclusas(); i++){
		printf("------->%s\n", inclusas_str[i]);
	}
}

int executarTerminal(char **args){
	if(args[0]==NULL) return 1;
	for(int i=0; i<num_inclusas; i++){
		if(strcm(args[0], inclusas_str[i]) == 0) return (inclusas_func[i])(args);
	}
	return executarComando(args);
}

//função para executar os comandos
int executarComando(char** argumentos){
	pid_t pid, wpid;
	int status;

	pid = fork();

	if(pid==0){
		if(execvp(argumentos[0], argumentos) == -1) perror("lsh"); //quando o exec retorna algo, deu erro
		exit(EXIT_FAILURE);
	}else if(pid < 0){ //erro de fork
		perror("lsh"); //exibe qual foi o erro do fork
	}else{ //caso dê bom
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status)); //espera o processo mudar de estado
	}
	return 1;
}

#define delimitador " \t\r\n\a"
char **dividirLinha(char *linha){ //onde a linha será dividida em tokens, onde houver um determinado caractere
	int tamBuffer = 64;
	int pos = 0;
	char **tokens = malloc(tamBuffer * sizeof(char*));
	char* token;

	if(!tokens){
		printf("Erro de alocacao!\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(linha, delimitador);

	while(token != NULL){
		tokens[pos] = token;
		pos++;

		//caso a posicao seja maior que a definida no buffer
		if(pos >= tamBuffer){
			tamBuffer += 64;
			tokens = realloc(tokens, tamBuffer*sizeof(char*));
			if(!tokens){
				printf("Erro de alocacao. \n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, delimitador);
	}
	tokens[pos] = NULL;
	return tokens;
}

//função para ler linhha digitada
char *lerLinha(void){
	int tamBuffer = 1024;
	int pos = 0;
	char *buffer = malloc(sizeof(char) * tamBuffer);
	int aux;

	if(!buffer){
		printf("Erro de alocação.");
		exit(EXIT_FAILURE);
	}

	while(1){
		aux = getchar();
		if(aux == '\n' || c == EOF){
			buffer[pos] = '\0';
			return buffer;
		} else buffer[pos] = aux;
		pos++;

		//Caso o tamanho do buffer seja insuficiente
		if(pos >= tamBuffer){
			tamBuffer += 1024;
			buffer = realloc(buffer, tamBuffer);
			if(!buffer){
				printf("Erro de alocacao!\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

void loopTerminal(void){ //loop principal do terminal
	char **argumentos;
	char* linha;
	int status;

	do{
		printf("-----> ");
		linha = lerLinha();
		argumentos = dividirLinha(linha);
		status = executarTerminal(argumentos);

		free(linha);
		free(argumentos);

	}while(status);
}

int main(int argc, char **argv){
	//chamada para função de loop do terminal
	//ler linha > dividir os argumentos > executar o comando
	loopTerminal();
	return 0;
}