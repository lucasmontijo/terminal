#include<stdlib.h>
#include<stdio.h>

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
		status = executarComando(argumentos);

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