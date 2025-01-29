#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct heap_Reserva {
    int codigo;
    int prioridade;
}reserva;

typedef struct heap{
	reserva *vetor;
	int tamanho;
	int capacidade;
}hp;


typedef struct AVL{
	int codigo;
	int capacidade;
	int Cont_Direita;
	int Cont_Esquerda;
	struct AVL *dir;
	struct AVL *esq;	
}no;


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

//Heap
hp *criarHeap(int capacidade);
void inserirReserva(hp *heap1, int codigo, int prioridade);
void troca(reserva *v1, reserva *v2);
void exibirReserva(hp *heap);
void atualizar(hp *heap, int codigo, int Nova_Prioridade);


int main()
{	FILE *arq=NULL;
 	arq=fopen("entrada1.txt","r");
 
 	if(!arq)
 		return 1;
	no *avl_Raiz = NULL;
	hp *vetor = NULL;
	
 	char s1[25];
	int codigo, tamanho = 1;
		
	while(!feof(arq)){
		fscanf(arq, "%s", &s1);
		switch(s1[0]){
		
		case 'C': {
			if(strcmp(s1, "CRIAR_SALA") == 0){
				int capacidade;
				fscanf(arq, "%d %d", &codigo, &capacidade);
				avl_Raiz = insereNo(avl_Raiz, codigo, capacidade);
				vetor = criarHeap(tamanho);
                    tamanho += 1;
			 }else if(strcmp(s1, "CONSULTAR_SALA") == 0){
			 		no *aux1 = NULL;
			 		fscanf(arq, "%d", &codigo);
			 		aux1 = consultar(&avl_Raiz, codigo);
				 	if(aux1 != NULL){
				 		printf("Consultar Sala: \n");
			 			printf("Codigo: %d, Capacidade: %d\n", aux1->codigo, aux1->capacidade);
			 			}
			 		}
		}break;
		
		case 'L':{
			if(strcmp(s1, "LISTAR_SALAS") == 0){
				printf("\n");
				exibirArvore(avl_Raiz);
				}else if(strcmp(s1,"LISTAR_RESERVAS") == 0){
				printf("\n");
				exibirReserva(vetor);
				printf("\n");
			}
	  }break;
	  
	  case 'R':{
			int reserva;
			if(strcmp(s1, "RESERVAR_SALA") == 0){
				fscanf(arq, "%d %d", &codigo, &reserva);
				if(consultar(&avl_Raiz, codigo) != NULL){
					inserirReserva(vetor, codigo, reserva);	
				}else{
					printf("Numero %d Inexistente\n", codigo);
				}
			}else if(strcmp(s1, "REMOVER_SALA") == 0){
				fscanf(arq, "%d", &codigo);
				if(consultar(&avl_Raiz, codigo) != NULL)
				removerAvl(avl_Raiz, codigo);
				else
					printf("\nSala inexistente");		
			}
			
		}break;
		
		case 'A': {
			int reserva;
			fscanf(arq, "%d %d", &codigo, &reserva);
				atualizar(vetor, codigo, reserva);
		}break;
		
		case 'B':{
			

		}break;
	 }
	}
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

        //Copiado do gpt, Remoção com dos filhos;
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


//Reservar pego do moodle
hp* criarHeap(int capacidade) {
    hp* heap = (hp*)malloc(sizeof(hp));
    heap->vetor = (reserva*)malloc((capacidade + 1) * sizeof(reserva)); // Aloca espaço para as reservas
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    return heap;
}

// Função para inserir uma reserva na heap
void inserirReserva(hp* heap, int codigo, int prioridade) {
     if (heap->tamanho >= heap->capacidade) {
        heap->capacidade *= 2;
        heap->vetor = (reserva*)realloc(heap->vetor, (heap->capacidade + 1) * sizeof(reserva));
    }

    // Armazenar código e prioridade diretamente
    heap->vetor[heap->tamanho + 1].codigo = codigo;
    heap->vetor[heap->tamanho + 1].prioridade = prioridade;

    heap->tamanho++;

    // Ajusta a posição da reserva para manter a propriedade da heap
    int i = heap->tamanho;
    while (i > 1 && heap->vetor[i].prioridade > heap->vetor[i / 2].prioridade) {  // Compara com a prioridade do pai
        troca(&heap->vetor[i], &heap->vetor[i / 2]);  // Troca a reserva
        i = i / 2;  // Desce na árvore para manter a propriedade da heap
    }
}

void troca(reserva *v1, reserva *v2) {
    reserva temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

void atualizar(hp *heap, int codigo, int Nova_Prioridade){
	int i;
    for (i = 1; i <= heap->tamanho; i++) {
        if (heap->vetor[i].codigo == codigo) {
            heap->vetor[i].prioridade = Nova_Prioridade;
            int j = i;
            while (j > 1 && heap->vetor[j].prioridade > heap->vetor[j / 2].prioridade) {
                troca(&heap->vetor[j], &heap->vetor[j / 2]);
                j = j / 2;
            }
            return;
        }
    }
    printf("Reserva com código %d não encontrada.\n", codigo);
}

void exibirReserva(hp* heap) {
	int i;
    	for (i = 1; i <= heap->tamanho; i++) {
        	printf("Codigo: %d, Prioridade: %d\n", heap->vetor[i].codigo, heap->vetor[i].prioridade);
    	}
}
