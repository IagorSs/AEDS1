#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char caracterAtual;
FILE *input;

int tamanhoMsg(){ // FUNCIONA
	int retorno = 0;
	rewind(input);
	caracterAtual = fgetc(input);
	while(caracterAtual != EOF){
		caracterAtual = fgetc(input);
		retorno++;
	}
	return retorno;
}

void gravarMsg(char *string,int tamanhoArq){ // FUNCIONA
	rewind(input);
	int i;
	for(i=0; i<tamanhoArq; i++)
		string[i] = fgetc(input);
	string[i] = '\0';
}

void imprimeSaida(char *msg){	
	rewind(input);
	printf("\n--------------------\nMensagem codificada:\n--------------------\n");
	caracterAtual = fgetc(input);
	while(caracterAtual != EOF){
		printf("%c",caracterAtual);
		caracterAtual = fgetc(input);
	}
	printf("\n----------------------\nMensagem decodificada:\n----------------------\n");
	for(int i=0;msg[i] != '\0';i++){
		printf("%c",msg[i]);
	}
	printf("\n");
}

void desconverteCaracter(char *msg, char *msg_d, int tamanhoArq){
	rewind(input);
	char caracterDecod;
	int i;
	for(i=0; i<tamanhoArq; i++){
		caracterAtual = msg[i];
		if(caracterAtual == '#')
			caracterDecod = ' ';
		else{
			caracterDecod = caracterAtual - 3;
			if((caracterDecod > 90 && caracterDecod < 97)||caracterDecod < 65)
				caracterDecod += 26;
		}
		msg_d[i] = caracterDecod;
	}
	if(msg_d[i-1] == ' ')
		msg_d[i-1] = '\0';
	msg_d[i] = '\0';
}

void trocaPares(char *msg_c,int n){
	int qtde_b = n/2;
	char bloco[qtde_b][2];
	char bloco_m[2];
	for(int i=0,j=0;i<qtde_b;i++,j+=2){
		bloco_m[0] = msg_c[j];
		bloco_m[1] = msg_c[j+1];
		strcpy(bloco[i],bloco_m);
	}
	for(int i=0,j=qtde_b-1;i<qtde_b/2;i+=2,j-=2){
		msg_c[j*2] = bloco[i][0];
		msg_c[j*2+1] = bloco[i][1];
		msg_c[i*2] = bloco[j][0];
		msg_c[i*2+1] = bloco[j][1];
	}
}

void trocaC2_2(char *msg_c,int tamanhoArq){ // FUNCIONANDO
	char temp;
	for(int i=0;i<tamanhoArq;i+=2){
		temp = msg_c[i+1];
		msg_c[i+1] = msg_c[i];
		msg_c[i] = temp;
	}
}

void decodificar(char *msg_c,char *msg_d,int tamanhoArq){
	trocaPares(msg_c,tamanhoArq);
	trocaC2_2(msg_c,tamanhoArq);
	desconverteCaracter(msg_c,msg_d,tamanhoArq);
}

int main(){
	char arquivo[40];
	int tamanhoArq;
	printf("Digite o nome do arquivo: ");
	scanf("%s",arquivo);
	input = fopen(arquivo,"r");
	tamanhoArq = tamanhoMsg()-1;
	char msg_original[tamanhoArq+1], msg_final[tamanhoArq+1];
	gravarMsg(msg_original,tamanhoArq);
	decodificar(msg_original,msg_final,tamanhoArq);
	imprimeSaida(msg_final);
	fclose(input);
	return 0;
}
