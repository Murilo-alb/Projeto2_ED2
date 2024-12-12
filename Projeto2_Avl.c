#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AVL
{
	int codigo;
	int capacidade;
	int Cont_Direita;
	int Cont_Esquerda;
	struct AVL *dir;
	struct AVL *esq;	
}no;

no *criarNo(int codigo, int capacidade);
no *insereNo(no *avl, int codigo, int capacidade);
no *balancear(no *avl);
no *rotacaoEsquerda(no *avl);
no *rotacaoDireita(no *avl);


void exibirArvore(no *avl);
void exluirNo(no *avl);

int main()
{	FILE *arq=NULL;
 	arq=fopen("entrada2.txt","r");
 
 	if(!arq)
 		return 1;
 	
	no *avl_Raiz = NULL;
 	char s1[25];
	
	while(!feof(arq)){
		fscanf(arq, "%s", &s1);
	
	switch(s1[0]){
		
		case 'C': {
			int codigo;
			
			if(strcmp(s1, "CRIAR_SALA") == 0){
				int capacidade;
				fscanf(arq, "%d %d", &codigo, &capacidade);
					avl_Raiz = insereNo(avl_Raiz, codigo, capacidade);
  	
			 }else if(strcmp(s1, "CONSULTAR_SALA") == 0){
		
			
			}else if(strcmp(s1, "CANCELAR_RESERVA") == 0){
				
				
				}	
			}break;
		}
	}
		exibirArvore(avl_Raiz);
	return 0;
}	

no *criarNo(int codigo, int capacidade)
{
	no* avl = (no *)malloc(sizeof(no));
	avl->codigo = codigo;
	avl->capacidade = capacidade;
	avl->Cont_Direita = 0;
	avl->Cont_Esquerda = 0;
	avl->dir = NULL;
	avl->esq = NULL;
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

void exibirArvore(no* avl) {
    if (avl != NULL) {
         printf("Codigo: %d, Capacidade: %d\nAltura Esquerda: %d, Altura Direita: %d\n", avl->codigo, avl->capacidade,avl->Cont_Esquerda, avl->Cont_Direita);
         printf("\n");
        exibirArvore(avl->esq);
        exibirArvore(avl->dir);
    }
}

