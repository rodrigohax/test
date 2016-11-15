#include <stdio.h>
#define K 4

int candidatos[K];
void init();
void votar();
void resultados();
int main(){
	init();
	votar();
	return 0;	
}

void init(){
	int i;
	for(i=0;i<K;i++){
		candidatos[i] = 0;
	}
}

void votar(){
	printf("Selecione una opcion\n");
	printf("1.- Charlatan\n");
	printf("2.- Dario menos charlatan\n");
	printf("3.- Flaito aka el poeta\n");
	printf("4.- La coxina peppa\n");
	int opcion;
	while(1){
		printf("Ingrese su candidato\n");
		scanf("%d", &opcion);
		switch(opcion){
			case 1:
			printf("vote por charlatan 1\n");
			candidatos[0]++;
			resultados();
			break;
			case 2:
			printf("vote x dario\n");
			candidatos[1]++;
			resultados();
			break;
			case 3:
			printf("vote x flakito\n");
			candidatos[2]++;
			resultados();
			break;
			case 4:
			printf("vote x peppa\n");
			candidatos[3]++;
			resultados();
			break;
			default : 
			printf("elije una weaxdddddd\n");
			break;
		}
	}
}

void resultados(){
	printf("**************************************\n");
	printf("************ RESULTADOS **************\n");
	printf("**************************************\n");
	printf("Charlatan: %d\n",candidatos[0]);
	printf("Dario: %d\n",candidatos[1]);
	printf("Flakito: %d\n", candidatos[2]);
	printf("Peppa: %d\n", candidatos[3]);
}