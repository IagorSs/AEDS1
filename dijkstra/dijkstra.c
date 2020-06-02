#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<limits.h>

#define MAXNUMVERTICES  20
#define MAXNUMARESTAS   4500
#define FALSE           0
#define TRUE            1
#define INFINITO        INT_MAX

//fonte: http://www2.dcc.ufmg.br/livros/algoritmos/cap7/codigo/c/7.22-dijkstramatriz.c

void imprimeCaminho(int destino_temp);

typedef int TipoValorVertice;

typedef int TipoPeso;

typedef struct TipoGrafo {
  TipoPeso Mat[MAXNUMVERTICES + 1][MAXNUMVERTICES + 1];
  int  NumVertices;
  int  NumArestas;
  char cidadesAdicionadas[MAXNUMVERTICES][20];
} TipoGrafo;

typedef TipoValorVertice TipoApontador;

typedef int TipoIndice;

typedef struct TipoItem {
  TipoPeso Chave;
} TipoItem;

typedef TipoItem TipoVetor[MAXNUMVERTICES + 1];

TipoApontador Aux;
int  i, NArestas;
short FimListaAdj;
TipoValorVertice Adj;
TipoGrafo Grafo;
TipoPeso Peso;
TipoValorVertice NVertices;
TipoIndice n, indice;   /*Tamanho do heap*/
TipoValorVertice Raiz, Destino;
int menorCaminho[MAXNUMVERTICES+1]; //cidade de menor caminho entre a raiz e parametro é menorCaminho[parametro]

void FGVazio(TipoGrafo *Grafo) //procedimento que zera toda a matriz do grafo
{ int  i, j;
  for (i = 0; i <= Grafo->NumVertices; i++) 
    for (j = 0; j <= Grafo->NumVertices; j++)
      Grafo->Mat[i][j] = 0;
}

void InsereAresta(TipoValorVertice V1, TipoValorVertice V2, TipoPeso *Peso, 
          TipoGrafo *Grafo) //procedimento que insere o peso entre dois vertices no grafo

{ Grafo->Mat[V1][V2] = *Peso;
}

short  ExisteAresta(TipoValorVertice Vertice1, TipoValorVertice Vertice2, 
            TipoGrafo *Grafo) // retorna 0 ou 1 dependendo de existir a aresta
{ return (Grafo->Mat[Vertice1][Vertice2] > 0);
}

/*-- Operadores para obter a lista de adjacentes --*/

short ListaAdjVazia(TipoValorVertice *Vertice, TipoGrafo *Grafo) // Se o vertice não tem nenhum vértice adjescente esta função retornará TRUE
{ TipoApontador Aux = 0;
  short ListaVazia = TRUE;
  while (Aux < Grafo->NumVertices && ListaVazia){ 
    if (Grafo->Mat[*Vertice][Aux] > 0)
      	ListaVazia = FALSE;
    else
    	Aux++;
    }
  return (ListaVazia == TRUE);
} 

TipoApontador PrimeiroListaAdj(TipoValorVertice *Vertice, TipoGrafo *Grafo) // retorna o primeiro adjascente ao vértice passado como parametro
{ TipoValorVertice Result;
  TipoApontador Aux = 0;
  short Listavazia = TRUE;
  while (Aux < Grafo->NumVertices && Listavazia) 
    { if (Grafo->Mat[*Vertice][Aux] > 0) 
      { 
      	Result = Aux;
      	Listavazia = FALSE;
      } 
      else
      	Aux++;
    }
  if (Aux == Grafo->NumVertices)
  printf("Erro: Lista adjacencia vazia (PrimeiroListaAdj)\n");
  return Result;
}

void ProxAdj(TipoValorVertice *Vertice, TipoGrafo *Grafo, 
         TipoValorVertice *Adj, TipoPeso *Peso, TipoApontador *Prox, 
         short *FimListaAdj)
{ /* --Retorna Adj apontado por Prox--*/
// Esse procedimento vai atualizar o peso com base em um próximo previamente já encontrado e atualizar também o próximo vertice para quando chamar novamente esse bloco
// Esse procedimento também informa se a lista de adjascencia terminou
  *Adj = *Prox;
  *Peso = Grafo->Mat[*Vertice][*Prox];
  (*Prox)++;
  while (*Prox < Grafo->NumVertices && Grafo->Mat[*Vertice][*Prox] == 0)
    (*Prox)++;
  if (*Prox == Grafo->NumVertices)
  	*FimListaAdj = TRUE;
} 

void ImprimeGrafo(TipoGrafo *Grafo)

{ int i, j;
  printf("   ");
  for (i = 0; i < Grafo->NumVertices; i++)
  	printf(" %3d", i);
  putchar('\n');
  for (i = 0; i < Grafo->NumVertices; i++) 
    { printf("%3d", i);
      for (j = 0; j < Grafo->NumVertices; j++)
        printf(" %3d", Grafo->Mat[i][j]);
      putchar('\n');
    }
}  

void RefazInd(TipoIndice Esq, TipoIndice Dir, TipoItem *A, TipoPeso *P, 
            TipoValorVertice *Pos) // ?????

{ 	TipoIndice i = Esq;
	int j;
	TipoItem x;
  	j = i * 2;
  	x = A[i];
  	while (j <= Dir){
  		if (j < Dir){
  			if (P[A[j].Chave] > P[A[j+1].Chave])
  				j++; 
	      }
	    if (P[x.Chave] <= P[A[j].Chave])
	    	goto L999;
	    A[i] = A[j];
	    Pos[A[j].Chave] = i;
	    i = j;
	    j = i * 2;
    }
  L999:  A[i] = x;  Pos[x.Chave] = i;
} 

void Constroi(TipoItem *A,  TipoPeso *P,  TipoValorVertice *Pos)

{ TipoIndice Esq;
  Esq = n / 2 + 1;
  while (Esq > 1) 
    { Esq--;
      RefazInd(Esq, n, A, P, Pos);
    }
} 

TipoItem RetiraMinInd(TipoItem *A,  TipoPeso *P,  TipoValorVertice *Pos)
{ TipoItem Result;
  if (n < 1) 
  { printf("Erro: heap vazio\n");
    return Result;
  }
  Result = A[1];
  A[1] = A[n]; Pos[A[n].Chave] = 1;
  n--;
  RefazInd(1, n, A, P, Pos);
  return Result;
}  

void DiminuiChaveInd(TipoIndice i, TipoPeso ChaveNova, TipoItem *A, 
             TipoPeso *P,  TipoValorVertice *Pos)
{ TipoItem x;
  if (ChaveNova > P[A[i].Chave]) 
  { printf("Erro: ChaveNova maior que a chave atual\n");
    return;
  }
  P[A[i].Chave] = ChaveNova;
  while (i > 1 && P[A[i / 2].Chave] > P[A[i].Chave]) 
    { x = A[i / 2];
      A[i / 2] = A[i];
      Pos[A[i].Chave] = i / 2;
      A[i] = x;
      Pos[x.Chave] = i;
      i /= 2;
    }
} 

void Dijkstra(TipoGrafo *Grafo, TipoValorVertice *Raiz)
{ TipoPeso P[MAXNUMVERTICES + 1];
  TipoValorVertice Pos[MAXNUMVERTICES + 1];
  long Antecessor[MAXNUMVERTICES + 1];
  TipoVetor A;
  TipoValorVertice u, v;
  TipoItem temp;
  for (u = 0; u <= Grafo->NumVertices; u++)
  { /*Constroi o heap com todos os valores igual a INFINITO*/
    Antecessor[u] = -1; P[u] = INFINITO;
    A[u+1].Chave = u;   /*Heap a ser construido*/
    Pos[u] = u + 1;
  }
  n = Grafo->NumVertices;
  P[*(Raiz)] = 0;
  Constroi(A, P, Pos);
  while (n >= 1) 
  { /*enquanto heap nao vazio*/
    temp = RetiraMinInd(A, P, Pos);
    u = temp.Chave;
    if (!ListaAdjVazia(&u, Grafo))
    { Aux = PrimeiroListaAdj(&u, Grafo);
      FimListaAdj = FALSE;
      while (!FimListaAdj){
        ProxAdj(&u, Grafo, &v, &Peso, &Aux, &FimListaAdj);
          if (P[v] > (P[u] + Peso)){
              P[v] = P[u] + Peso;
              Antecessor[v] = u;
              DiminuiChaveInd(Pos[v], P[v], A, P, Pos);
              menorCaminho[v] = Antecessor[v];
          }
      }
    } 
  }
  printf("Menor percurso:");
  imprimeCaminho(Destino);
  printf("\n");
  printf("Distancia total: %i Km\n",P[Destino]);
}

// A parir foi codado por mim

typedef struct TipoCidade{
  char nome[20];
}TipoCidade;

void insereCidade(TipoCidade cidade, TipoGrafo *Grafo){
  for(int i=0;i < Grafo->NumVertices;i++){
    if(Grafo->cidadesAdicionadas[i][0]== '0'){
      strcpy(Grafo->cidadesAdicionadas[i],cidade.nome);
      Grafo->NumVertices++;
      goto FINISH;
    }
  }
  FINISH: ;
}

void limpaCidades(TipoGrafo *Grafo){
  char stringNula[20];
  for(int i = 0;i < 20;i++)
    stringNula[i] = '0';
  stringNula[20] = '\0';
  for(int i = 0;i < MAXNUMVERTICES;i++)
    strcpy(Grafo->cidadesAdicionadas[i],stringNula);
}

bool confereCidade(TipoCidade cidade,TipoGrafo Grafo){ //retorna TRUE se a cidade já estiver adicionada no grafo
  for(int i=0; i<Grafo.NumVertices; i++){
    if(strcmp(Grafo.cidadesAdicionadas[i],cidade.nome) == 0){
      return true;
    }
  }
  return false;
}

int indiceCidade(char *nome_cidade){
  for(int i=0; i< Grafo.NumVertices; i++){
    if(strcmp(Grafo.cidadesAdicionadas[i],nome_cidade) == 0)
      return i;
  }
  printf("Erro ao buscar indice da cidade passada!\n");
  return -1;
}

void imprimeCaminho(int destino_temp){
  if(destino_temp == Raiz){
    printf(" %s", Grafo.cidadesAdicionadas[destino_temp]);
  }
    else{
      imprimeCaminho(menorCaminho[destino_temp]);
      printf(" %s", Grafo.cidadesAdicionadas[destino_temp]);
  }
}

int main(){
  FILE *input;
  char arquivo[20];
  TipoCidade cidade_1,cidade_2;
  indice = 0;
  printf("Digite o nome do arquivo de entrada: ");
  scanf("%s",arquivo);
  input = fopen(arquivo,"r");
  if(input == NULL){
    printf("Arquivo não foi encontrado, programa encerrado!\n");
    return 0;
  }
  limpaCidades(&Grafo);
  FGVazio(&Grafo);
  fscanf(input, "%i", &NArestas);
  NArestas *= 2;
  Grafo.NumArestas = NArestas;
  Grafo.NumVertices = 1;

  while(NArestas > 0){
    fscanf(input, "%s", cidade_1.nome);
    if(confereCidade(cidade_1,Grafo) == false)
      insereCidade(cidade_1,&Grafo);
    fscanf(input, "%s", cidade_2.nome);
    if(confereCidade(cidade_2,Grafo) == false)
      insereCidade(cidade_2,&Grafo);
    fscanf(input, "%i", &Peso);
    InsereAresta(indiceCidade(cidade_1.nome),indiceCidade(cidade_2.nome),&Peso,&Grafo);
    InsereAresta(indiceCidade(cidade_2.nome),indiceCidade(cidade_1.nome),&Peso,&Grafo);
    NArestas-= 2;
  }
  Grafo.NumVertices--;
  fscanf(input, "%s", cidade_1.nome);
  fscanf(input, "%s", cidade_2.nome);
  Raiz = indiceCidade(cidade_1.nome);
  Destino = indiceCidade(cidade_2.nome);
  Dijkstra(&Grafo, &Raiz);
  fclose(input);
  return 0;
}