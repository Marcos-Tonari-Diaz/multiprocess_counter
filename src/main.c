/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>

int is_prime(int in){
	if (in<2)
		return 0;
	int d;
	//int sqrt_in= (int) sqrt((double) in);
	for (d=2; d < in; d++){
		if (in%d == 0)
			return 0;
	}
	return 1;
}

int main() {
	//vetor de PIDs dos filhos
	pid_t filhos[4];

	int i;

	//Por memoria compartilhada
	/* Definir flags de protecao e visibilidade de memoria */
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;

	/* Criar area de memoria compartilhada */
	//quatro contadores
	int* counter;
	counter = (int*) mmap(NULL, sizeof(int)*4, protection, visibility, 0, 0);
	//inicializa os contadores
	for (i=0;i<4;i++){
	       *(counter+i)=0;
	}	       

	//entrada
	i=0; 
	int j=0;
	int n_primes=0;
	char in = getchar();
	char buffer[256];
	unsigned long long int numbers[20];
	while (in!='\n'){
		if (in!=' '){
			buffer[i]=in;
			i++;
		}
		else{
			buffer[i]='\0';
			numbers[j] = strtoull(buffer, NULL, 10);
			i=0;
			j++;
		}
		in = getchar();
		if (in=='\n'){
			buffer[i]='\0';
			numbers[j] = strtoull(buffer, NULL, 10);
		}
	} 

	//particao do vetor de numeros que um processo ira consumir
	int partition=0;
	int k;
	//cria os filhos
	for (i=0;i<4;i++){
		filhos[i]=fork();
		if (filhos[i]==0){
			partition=i;
			//percorre os numeros
			for (k=0; k<=j; k++){
				//alterna a contagem entre os processos
				if (k%4==partition){
					if (is_prime(numbers[k])==1){
						(*(counter+i))++;
					}
				}
			}
		exit(0);
		}
	}
	//espera os filhos terminarem
	for (i=0;i<4;i++){
		waitpid(filhos[i], NULL, 0);
	}
	//processo pai soma os contadores
	for (i=0;i<4;i++){
		n_primes+=*(counter+i);
	}
	printf("%d\n", n_primes);
	return 0;
	
}
