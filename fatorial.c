#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NUMMAXDIG 36000


void imprimeVetor(short *numero,int tamanhoV){
	int i;
	for(i=0;numero[i]==0;i++);
	for(; i< tamanhoV; i++){
		printf("%hi",numero[i]);
	}
	printf("\n");
}

int tamanhoNumero(int numero){
	int i;
	for(i=0; numero != 0; i++){
		numero = numero/10;
	}
	return i;
}

int tamanhoNumeroV(short *A,int tamanhoV){
	int i;
	for(i=0; A[i] == 0; i++);
	return (tamanhoV - i);
}

void setVetor(short *vetor, int tamanho){
	for(int i=0; i<tamanho; i++){
		vetor[i] = 0;
	}
}

void ADD0(short *num, int qtde0, int tamanhoV){
	int i;
	for(i = 0; num[i] == 0; i++);
	for(i = i-qtde0; (i+qtde0)<tamanhoV; i++)
		num[i] = num[i+qtde0];
	for(;i<tamanhoV;i++)
		num[i] = 0;
}

short somar_1bit(short num_1, short num_2, short *CarryOut, short Carryin){
	short Resultado;
	Resultado = num_1+num_2+Carryin;
	*CarryOut = Resultado/10;
	return (Resultado%10);
}

void soma(short *num_1, short *num_2,short *result,int tamanhoV){
	short CarryIn = 0;
	short CarryOut;
	int tam_n1,tam_n2,tam_maior;
	tam_n1 = tamanhoNumeroV(num_1,tamanhoV);
	tam_n2 = tamanhoNumeroV(num_2,tamanhoV);
	if(tam_n1 > tam_n2)	tam_maior = tam_n1;
	else tam_maior = tam_n2;
	for(int i = tamanhoV - 1;i >= (tamanhoV - tam_maior -1);i--){
		result[i] = somar_1bit(num_1[i],num_2[i],&CarryOut,CarryIn);
		CarryIn = CarryOut;
	}
}

short mult_1bit(short A, short B, short *CarryOut, short CarryIn){
	short Resultado;
	Resultado = (A*B)+CarryIn;
	*CarryOut = Resultado/10;
	return (Resultado%10);
}

void multiplicacao(short *num_1, short *num_2, short *result, int tamanho_V1, int tamanho_V2){
	int qtde0 = 0;
	int tam_n1,tam_n2;
	int k;
	short CarryOut,CarryIn;
	tam_n1 = tamanhoNumeroV(num_1,tamanho_V1);
	tam_n2 = tamanhoNumeroV(num_2,tamanho_V2);
	short vetor_num[NUMMAXDIG];
	for(int i=tamanho_V2-1; i>=(tamanho_V2 - tam_n2) ; i--){
		setVetor(vetor_num,NUMMAXDIG);
		CarryIn = 0;
		k=1;
		for(int j=tamanho_V1-1; j>=(tamanho_V1 - tam_n1 -1);j--){
			vetor_num[NUMMAXDIG - k] = mult_1bit(num_1[j],num_2[i],&CarryOut,CarryIn);
			CarryIn = CarryOut;
			k++;
		}
		ADD0(vetor_num,qtde0,NUMMAXDIG);
		soma(result,vetor_num,result,NUMMAXDIG);
		qtde0++;
	}

}

void converter(int numero, short* vetor, int tamanhoV){
	for(int i=(tamanhoV-1); numero !=0 ; i--){
		 vetor[i]=numero%10;
		 numero = numero/10;
	}
}

void cpyV(short *A, short *B, int tamanhoV){
	for(int i=0; i<tamanhoV; i++)
		B[i] = A[i];
}

void fatorial(int num,short *result){
	short num_V[20];
	short backResult[NUMMAXDIG], resultTemp[NUMMAXDIG];
	setVetor(backResult,NUMMAXDIG);
	backResult[NUMMAXDIG - 1] = 1;
	for(;num>1;num--){
		setVetor(resultTemp,NUMMAXDIG);
		setVetor(num_V,20);
		converter(num, num_V, 20);
		multiplicacao(backResult, num_V, resultTemp,NUMMAXDIG,20);
		cpyV(resultTemp,backResult,NUMMAXDIG);
	}
	cpyV(backResult,result,NUMMAXDIG);
}

int main(){
	int input;
	short result[NUMMAXDIG];
	setVetor(result,NUMMAXDIG);
	printf("Digite um numero inteiro: ");
	scanf("%i",&input);
	fatorial(input,result);
	printf("Resultado: ");
	imprimeVetor(result,NUMMAXDIG);
	return 0;
}