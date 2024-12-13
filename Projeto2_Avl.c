#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct heap{
	int *vetor;
	int capacidade;
}hp;

typedef struct AVL{
	int codigo;
	int capacidade;
	int Cont_Direita;
	int Cont_Esquerda;
	struct AVL *dir;
	struct AVL *esq;	
	hp *prioridade;
}no;

//AVL
no *criarNo(int codigo, int capacidade);
no *insereNo(no *avl, int codigo, int capacidade);
no *balancear(no *avl);
no *rotacaoEsquerda(no *avl);
no *rotacaoDireita(no *avl);

//Heap
hp *criarHeap(int capacidade);
void reservar(hp *heap, int capacidade);
void troca(int* v1, int* v2);
void exibirReserva(hp *heap);

//operacoes
no *consultar(no **avl, int codigo);


void exibirArvore(no *avl);
void exluirNo(no *avl);

int main()
{	FILE *arq=NULL;
 	arq=fopen("entrada2.txt","r");
 
 	if(!arq)
 		return 1;
 	
	no *avl_Raiz = NULL;
 	char s1[25];
 	int codigo;
	
	while(!feof(arq)){
		fscanf(arq, "%s", &s1);
	switch(s1[0]){
		
		case 'C': {
			if(strcmp(s1, "CRIAR_SALA") == 0){
				int capacidade;
				fscanf(arq, "%d %d", &codigo, &capacidade);
				avl_Raiz = insereNo(avl_Raiz, codigo, capacidade);
	
			 }else{
				 if(strcmp(s1, "CONSULTAR_SALA") == 0){
			 		no *aux1 = NULL;
			 		fscanf(arq, "%d", &codigo);
			 		aux1 = consultar(&avl_Raiz, codigo);
			 	
				 	if(aux1 != NULL){
				 		printf("SALA: \n");
			 			printf("Codigo: %d, Capacidade: %d\n", aux1->codigo, aux1->capacidade);
			 			}
			 		}
				}
			}break;
		
		case 'R':{
				int reserva;
				fscanf(arq, "%d %d", &codigo, &reserva);
				if(consultar(&avl_Raiz, codigo) != NULL){
				reservar(avl_Raiz->prioridade, reserva);
			}else
				printf("Sala nao existe");
			
		}break;
			
		case 'L':{
			if(strcmp(s1, "LISTAR_SALAS") == 0){
				printf("\n");
				exibirArvore(avl_Raiz);
				}	
				
			if(strcmp(s1,"LISTAR_RESERVAS") == 0){
				printf("\n");
				exibirReserva(avl_Raiz->prioridade);
				printf("\n");s
			}
			}break;
		}
	}
	return 0;
}

//AVL
no *criarNo(int codigo, int capacidade)
{
	no* avl = (no *)malloc(sizeof(no));
	avl->codigo = codigo;
	avl->capacidade = capacidade;
	avl->Cont_Direita = 0;
	avl->Cont_Esquerda = 0;
	avl->dir = NULL;
	avl->esq = NULL;
	avl->prioridade = criarHeap(capacidade);
	return avl;
}

//Codigo allefer com algumas modificacoes;
no *insereNo(no *avl, int codigo, int capacidade)
{
	if(avl == NULL)
		return criarNo(codigo, capacidade);
		
		if(avl->codigo > codigo){
			avl->esq = insereNo(avl->esq, codigo, capacidade);
		}else if(avl->codigo < codigo){
			avl->dir = insereNo(avl->dir, codigo, capacidade);
		}
	
		if(avl->esq){
			if(avl->esq->Cont_Esquerda > avl->esq->Cont_Direita)
				avl->Cont_Esquerda = avl->esq->Cont_Esquerda + 1;
			else
				avl->Cont_Esquerda = avl->esq->Cont_Direita + 1;
		}else
			avl->Cont_Esquerda = 0;
			
		if(avl->dir){
			if(avl->dir->Cont_Esquerda > avl->dir->Cont_Direita)
				avl->Cont_Direita = avl->dir->Cont_Esquerda + 1;
			else
				avl->Cont_Direita = avl->dir->Cont_Direita + 1;
		}else
			avl->Cont_Direita = 0;
		
		return balancear(avl);	
}

no *balancear(no *avl)
{
	if(!avl)
		return NULL;
		
	else{
		int cont1 = avl->Cont_Direita - avl->Cont_Esquerda;
		
		if(cont1 == 2){
			int cont2 = avl->dir->Cont_Direita - avl->dir->Cont_Esquerda;
			
			if(cont2 >= 0){
				avl = rotacaoEsquerda(avl);
				
			}else{
				avl->dir = rotacaoDireita(avl->dir);
				avl = rotacaoEsquerda(avl);
			}
		}else if(cont1 == -2){
			int cont2 = avl->esq->Cont_Direita = avl->esq->Cont_Esquerda;
			
			if(cont2 <= 0){
				avl = rotacaoDireita(avl);
		
			}else{
				avl->esq = rotacaoEsquerda(avl->esq);
				avl = rotacaoDireita(avl);
			}
		}
		
		return avl;
	}
}

no* rotacaoEsquerda(no* avl) {
    no* aux = avl->dir;
    avl->dir = aux->esq;
    aux->esq = avl;

    if (avl->dir == NULL) {
        avl->Cont_Direita = 0;
    } else if (avl->dir->Cont_Esquerda > avl->dir->Cont_Direita) {
        avl->Cont_Direita = avl->dir->Cont_Esquerda + 1;
    } else {
        avl->Cont_Direita = avl->dir->Cont_Direita + 1;
    }

    if (aux->esq->Cont_Esquerda > aux->esq->Cont_Direita) {
        aux->Cont_Esquerda = aux->esq->Cont_Esquerda + 1;
    } else {
        aux->Cont_Esquerda = aux->esq->Cont_Direita + 1;
    }

    return aux;
}

no* rotacaoDireita(no* avl) {
    no* aux = avl->esq;
    avl->esq = aux->dir;
    aux->dir = avl;

    if (avl->esq == NULL) {
        avl->Cont_Esquerda = 0;
    } else if (avl->esq->Cont_Esquerda > avl->esq->Cont_Direita) {
        avl->Cont_Esquerda = avl->esq->Cont_Esquerda + 1;
    } else {
        avl->Cont_Esquerda = avl->esq->Cont_Direita + 1;
    }

    if (aux->dir->Cont_Esquerda > aux->dir->Cont_Direita) {
        aux->Cont_Direita = aux->dir->Cont_Esquerda + 1;
    } else {
        aux->Cont_Direita = aux->dir->Cont_Direita + 1;
    }

    return aux;
}

no *consultar(no **avl, int codigo)
{
	if((*avl) == NULL){
		printf("Erro\n");
		return NULL;
	}
	
	if((*avl)->codigo == codigo) {
        return (*avl);
    }
    else {
        if((*avl)->codigo < codigo) {
            return consultar(&(*avl)->dir, codigo);
        }
        else {
            return consultar(&(*avl)->esq, codigo);
        }
    }
}

void exibirArvore(no* avl) {
    if (avl != NULL) {
    	exibirArvore(avl->esq);
         printf("Codigo: %d, Capacidade: %d\nAltura Esquerda: %d, Altura Direita: %d\n", avl->codigo, avl->capacidade,avl->Cont_Esquerda, avl->Cont_Direita);
         printf("\n");
        exibirArvore(avl->dir);
    }
}

//HEAP
hp *criarHeap(int capacidade) {
    hp *heap = (hp*) malloc(sizeof(hp));
    heap->vetor = (int*) malloc((capacidade + 1) * sizeof(int));
    heap->vetor[0] = 0;
    heap->capacidade = capacidade;
    return heap;
}

//Reservar pego do moodle
void reservar(hp *heap, int capacidade){
		
	if (heap->vetor[0] == heap->capacidade) {
        int* vet = realloc(heap->vetor, (heap->capacidade * 2 + 1) * sizeof(int));
        if (vet != NULL) {
            heap->vetor = vet;
            heap->capacidade *= 2;
        } else {
            printf("Erro ao realocar memoria!\n");
            return;
        }
    }
    heap->vetor[0]++;
    int i = heap->vetor[0];
    
    heap->vetor[i] = capacidade;
    
    while (i > 1 && heap->vetor[i] > heap->vetor[i / 2]) {
        troca(&heap->vetor[i], &heap->vetor[i / 2]);
        i /= 2;
    }
}

void troca(int* v1, int* v2){
	int aux = *v1;
    *v1 = *v2;
    *v2 = aux;
}

void exibirReserva(hp *heap){
	int i;
	if (heap->vetor[0] == 0) {
    	printf("Heap vazio!\n");
        return;
    }
    printf("Lista de Reservas: ");
    for (i = 1; i <= heap->vetor[0]; i++) {
        printf("%d ", heap->vetor[i]);
    }
    printf("\n");
	}

