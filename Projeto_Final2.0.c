#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Utilizando codigo do moodle e modificando para o projeto;
typedef struct AVL{
	int codigo;
	int capacidade;
	int Cont_Direita;
	int Cont_Esquerda;
	struct AVL *dir;
	struct AVL *esq;	
}no;

//Utilizando duas estruturas;
typedef struct heap_Reserva {
    int codigo;
    int prioridade;
//Cria a primeira struct para armazenar codigo e prioridade
}reserva;

//Essa guarda o vetor de struct;
typedef struct heap{
	reserva *vetor; //Vetor para armazenar a struct reserva;
	int tamanho, capacidade; //Serve para comparar tamanho e capacidade;
}hp;

//Utilizando a mesma logica do heap
typedef struct hash_estado{
	int codigo;
	char estado[11];
	struct hash_estado *prox;
}hs;

typedef struct hash{
	hs *no;
	int tamanho;
}lista;

//typedef hp *filaPrioridade; Não é necessario;

//AVL
no *criarNo(int codigo, int capacidade);
no *insereNo(no *avl, int codigo, int capacidade); 
no *balancear(no *avl);
no *rotacaoEsquerda(no *avl);
no *rotacaoDireita(no *avl);

//operacoes
no *consultar(no **avl, int codigo);
no *removerAvl(no *avl, int codigo);
void exibirArvore(no *avl);

//heap
hp *criarHeap(int capacidade);
void inserirReserva(hp *heap1, reserva reserva1);
void troca(reserva *v1, reserva *v2);
void heapify(hp *heap, int i);
void exibirReserva(hp *heap);

//hash lista
void criarlista(lista *l);
void inserirLista(lista *l, int codigo, char *estado);
int consultarHash(lista *l, int codigo);
void exibirHash(lista *l);

//hash estado
void criarTabela(lista *l[]);

int main(){	
	FILE *arq=NULL;
 	arq=fopen("entrada2.txt","r"); //Altere a entrada para que o codigo funcione;
 
 	if(!arq)
 		return 1;
 		
	no *avl_Raiz = NULL;
	hp *vetor = NULL;
	
 	char s1[25];
	int codigo, tamanho = 1; //Tamanho inicia com 1 para armazenar o primeiro elemento do vetor;
	
	//O mesmo verificador que utilizei no projeto 1;
	while (fscanf(arq, "%s", s1) == 1){
		switch(s1[0]){
		
		//Case utilizando a primeira, if caso tenho mais de uma chamada com a letra;
		case 'C': {
			if(strcmp(s1, "CRIAR_SALA") == 0){
				int capacidade;
				fscanf(arq, "%d %d", &codigo, &capacidade);
				avl_Raiz = insereNo(avl_Raiz, codigo, capacidade);
				vetor = criarHeap(tamanho);
				tamanho++; //adiciona mais um antes de criar o proximo heap;
			 }else if (strcmp(s1, "CONSULTAR_SALA") == 0) {
   				 fscanf(arq, "%d", &codigo);
   				 no *aux1 = consultar(&avl_Raiz, codigo); //Guarda o valor da consulta;
   				
				   printf("Consulta: \n");
    				if (aux1)
      					  printf("Codigo: %d, Capacidade: %d\n", aux1->codigo, aux1->capacidade);
   					 else
       					 printf("Sala %d inexistente\n", codigo);
       			}
		}break;
		
		//Listar salas e reservas;
		case 'L':{
			if(strcmp(s1, "LISTAR_SALAS") == 0){
				printf("\nSALAS: \n");
				exibirArvore(avl_Raiz);
			}else if(strcmp(s1,"LISTAR_RESERVAS") == 0){
				printf("\n");
				exibirReserva(vetor);
				printf("\n");
			}
	  	}break;
	  	
	  	 case 'R':{
			int prioridade;
			if(strcmp(s1, "RESERVAR_SALA") == 0){
				fscanf(arq, "%d %d", &codigo, &prioridade);
				if(consultar(&avl_Raiz, codigo) != NULL){
					reserva sala;
					sala.codigo = codigo;
					sala.prioridade = prioridade;
					inserirReserva(vetor, sala); //Consulta para depois inserir;	
				}else{
					printf("Numero %d Inexistente\n", codigo);
				}
			}
		}break;
	
	 }
	}
	
	fclose(arq); //Fecha arquivo;
	return 0;
}

//Retirado do moodle;
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

//Insere e ja faz a contagem da altura;
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

//O mesmo do moodle;
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
			int cont2 = avl->esq->Cont_Direita - avl->esq->Cont_Esquerda;
			
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

//Não acho necessario dois ponteiros, mas funcionou;
no *consultar(no **avl, int codigo)
{
	if((*avl) == NULL){
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

//Remoção do github, porém com muitas modificações
no *removerAvl(no *avl, int codigo){
    if(avl == NULL){
        printf("\nSem salas para remover!");
        return NULL;
    }
    no *aux = NULL;
    
    if(avl->codigo > codigo){
        avl->esq = removerAvl(avl->esq, codigo);
        
    } else if(avl->codigo < codigo){
        avl->dir = removerAvl(avl->dir, codigo);
        
    } else if(avl->codigo == codigo) {
        if(avl->dir == NULL && avl->esq == NULL) {
            free(avl);
            return NULL;
        } else if(avl->dir == NULL && avl->esq != NULL) {
            aux = avl->esq;
            free(avl);
            return aux;
        } else if(avl->esq == NULL && avl->dir != NULL) {
            aux = avl->dir;
            free(avl);
            return aux;
        }

        //Copiado do gpt, Remoção com dois filhos;
        no *sucessor = avl->dir;
        while(sucessor->esq != NULL) {
            sucessor = sucessor->esq;
        }
        avl->codigo = sucessor->codigo;
        avl->capacidade = sucessor->capacidade;
        avl->dir = removerAvl(avl->dir, sucessor->codigo);
    }

    return balancear(avl);
}


void exibirArvore(no *avl) {
    if (avl != NULL) {
        exibirArvore(avl->esq);
        printf("Codigo: %d, Capacidade: %d\nAltura Esquerda: %d, Altura Direita: %d\n", avl->codigo, avl->capacidade, avl->Cont_Esquerda, avl->Cont_Direita);
        printf("\n");
        exibirArvore(avl->dir);
	}
}


//Heap retirado de um canal do youtube;
void troca(reserva *v1, reserva *v2){
	reserva aux = *v1;
	*v1 = *v2;
	*v2 = aux;
}

hp *criarHeap(int capacidade){
    hp *fila = (hp*)malloc(sizeof(hp));
    if (!fila) {
        printf("Erro ao alocar memória para a heap!\n");
        exit(1);
    }
    
    fila->vetor = (reserva*)malloc(sizeof(reserva) * capacidade);
    if (!fila->vetor) {
        printf("Erro ao alocar memória para o vetor da heap!\n");
        free(fila);
        exit(1);
    }

    fila->tamanho = 0;
    fila->capacidade = capacidade;
    return fila;
}

void inserirReserva(hp *heap1, reserva reserva1){
	if (heap1->tamanho >= heap1->capacidade) {
        printf("Heap cheio!\n");
        return;
    }

    heap1->vetor[heap1->tamanho] = reserva1;
    int i = heap1->tamanho;
    heap1->tamanho++;

    while (i > 0 && heap1->vetor[i].prioridade > heap1->vetor[(i - 1) / 2].prioridade) {
        troca(&heap1->vetor[i], &heap1->vetor[(i - 1) / 2]);
        i = (i - 1) / 2;
	}
}

//Tirado do moodle
void heapify(hp *heap, int i){
  int maior = i;
    int filho_esq = 2 * i + 1;
    int filho_dir = 2 * i + 2;

    if (filho_esq < heap->tamanho && heap->vetor[filho_esq].prioridade > heap->vetor[maior].prioridade)
        maior = filho_esq;
    
    if (filho_dir < heap->tamanho && heap->vetor[filho_dir].prioridade > heap->vetor[maior].prioridade)
        maior = filho_dir;
    
    if (maior != i) {
        troca(&heap->vetor[i], &heap->vetor[maior]);
        heapify(heap, maior);
    }
}
void exibirReserva(hp *heap){
		int i;
    	for (i = 0; i < heap->tamanho; i++) {
        	printf("Codigo: %d, Prioridade: %d\n", heap->vetor[i].codigo, heap->vetor[i].prioridade);
    	}
}

//hash
void criarLista(lista *l){
	l->no = NULL;
	l->tamanho = 0;
}

void inserirLista(lista *l, int codigo, char *estado){
		hs *novo = (hs*)malloc(sizeof(hs));
		
		if(novo){
			novo->codigo = codigo;
			strcpy(novo->estado, estado);
			novo->prox = l->no;
			l->no = novo;
		}else
			printf("\nErro ao alocar");
}

void consultarHash(lista *l, int codigo){
	hs *aux = l->no;
	while(aux & aux->codigo != codigo)
		aux = aux->prox;
		
		if(aux != NULL)
			printf("Sala %d esta: %s", aux->codigo, aux->estado);
		else
			return 0;
}

void exibirHash(lista *l){
	hs *aux = l->no;
	printf("Estado:\n");
	while(aux != NULL){
		printf("Sala: %d, estado: %s", aux->codigo, aux->estado);
		aux = aux->prox;
	}
}

void criarTabela(lista *l[], int tamanho){
	int i;
	for(i = 0;i < tamanho; i++){
		t[i] = 0;
	}
}

void redimensionarTabela(lista *l) {
    // Salvando o tamanho antigo
    int tamanhoAntigo = l->tamanho;
    l->tamanho = l->tamanho * 2;  // Aumentando o tamanho (dobrando)
    printf("Tabela redimensionada para o novo tamanho: %d\n", l->tamanho);
    
    // Alocando uma nova tabela maior
    hs **novaTabela = (hs **)malloc(sizeof(hs *) * l->tamanho);
    for (int i = 0; i < l->tamanho; i++) {
        novaTabela[i] = NULL;
    }

    // Re-hash de todas as entradas antigas para a nova tabela
    for (int i = 0; i < tamanhoAntigo; i++) {
        hs *aux = l->tabela[i];
        while (aux != NULL) {
            // Calculando o novo índice baseado no novo tamanho
            int novoIndex = aux->codigo % l->tamanho;
            
            // Inserir de volta na nova tabela
            hs *novo = (hs *)malloc(sizeof(hs));
            novo->codigo = aux->codigo;
            strncpy(novo->estado, aux->estado, 11);
            novo->prox = novaTabela[novoIndex];
            novaTabela[novoIndex] = novo;

            aux = aux->prox;
        }
    }
