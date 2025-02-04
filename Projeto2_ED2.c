#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Utilizando duas estruturas;
typedef struct heap_Reserva {
    int codigo;
    int prioridade;
}reserva;

//Essa guarda o vetor;
typedef struct heap{
	reserva *vetor; //Vetor para armazenar codigo e prioridade;
	int tamanho; //Serve para comparar tamanho e capacidade;
	int capacidade;
}hp;

//Utilizando codigo do moodle e modificando para o projeto;
typedef struct AVL{
	int codigo;
	int capacidade;
	int Cont_Direita;
	int Cont_Esquerda;
	struct AVL *dir;
	struct AVL *esq;	
}no;

//hash
typedef struct EstadoSala {
    int codigo;
    char estado[15]; // "disponível", "bloqueada", etc.
    struct EstadoSala *prox;
}estadoSala;

typedef struct TabelaHash {
    estadoSala **tabela; //para tabela com array de lista encadeada
   int tamanho;
}tabelaHash;

//typedef hp *filaPrioridade; Não é necessario;

//AVL
no *criarNo(int codigo, int capacidade);
no *insereNo(no *avl, int codigo, int capacidade); 
no *balancear(no *avl);
no *rotacaoEsquerda(no *avl);
no *rotacaoDireita(no *avl);

//operacoes
no *consultar(no **avl, int codigo);
void exibirArvore(no *avl);
void liberarArvore(no *avl); 

//Heap
hp *criarHeap(int capacidade);
void inserirReserva(hp *heap1, int codigo, int prioridade);
void troca(reserva *v1, reserva *v2);
void exibirReserva(hp *heap);
void atualizar(hp *heap, int codigo, int Nova_Prioridade);
void heapify(hp *heap, int i);
hp *cancelarHeap(hp *heap, int codigo);
void liberarHeap(hp *heap);

//hash
tabelaHash* criarTabela(int tamanho);
int funcaoHash(tabelaHash *hs, int codigo_sala);
void inserirEstado(tabelaHash *hs, int codigo, const char *estado);

//operaçoes hash
char* consultarEstado(tabelaHash *hs, int codigo);
void atualizarEstado(tabelaHash *hs, int codigo, char *novo_estado);
void liberarHash(tabelaHash *hs);

int main(){	

	FILE *arq=NULL;
 	arq=fopen("entrada2.txt","r"); //Altere a entrada para que o codigo funcione;
 
 	if(!arq)
 		return 1;
 		
	no *avl_Raiz = NULL;
	hp *vetor = NULL;

	
 	char s1[25];
	int codigo, tamanho = 1, tam_tabela = 10; //Tamanho inicia com 1 para armazenar o primeiro elemento do vetor;
	
	tabelaHash *tabela = criarTabela(tam_tabela);; //Tamanho inicia com 1 para armazenar o primeiro elemento do vetor;
	
	//O mesmo verificador que utilizei no projeto 1;
	while (fscanf(arq, "%s", s1) == 1){
		switch(s1[0]){
		
		//Case utilizando a primeira, if caso tenho mais de uma chamada com a letra;
		case 'C': {
			if(strcmp(s1, "CRIAR_SALA") == 0){
				int capacidade;
				fscanf(arq, "%d %d", &codigo, &capacidade);
				avl_Raiz = insereNo(avl_Raiz, codigo, capacidade);
				vetor = criarHeap(tamanho); //Ja cria o heap na chamada da avl;
				inserirEstado(tabela, codigo, "disponivel");
				tamanho++; //adiciona mais um antes de criar o proximo heap;
					 
			 }else if (strcmp(s1, "CONSULTAR_SALA") == 0) {
   				 fscanf(arq, "%d", &codigo);
   				 no *aux1 = consultar(&avl_Raiz, codigo); //Guarda o valor da consulta;
   				  printf("Consulta: \n");
    				if (aux1)
      					  printf("Codigo: %d, Capacidade: %d\n", aux1->codigo, aux1->capacidade);
   					 else
       					 printf("Sala %d inexistente\n", codigo);
       					 
					
					}else if(strcmp(s1, "CANCELAR_RESERVA") == 0){
						fscanf(arq, "%d", &codigo);
						cancelarHeap(vetor, codigo); //Não precisa apagar a sala, apenas cancelar a reserva; 
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
			int reserva;
			if(strcmp(s1, "RESERVAR_SALA") == 0){
				fscanf(arq, "%d %d", &codigo, &reserva);
				if(consultar(&avl_Raiz, codigo) != NULL){
					inserirReserva(vetor, codigo, reserva); //Consulta para depois inserir;	
				}else{
					printf("Numero %d Inexistente\n", codigo);
				}
			}
			
		}break;
		
		case 'A': {
			int reserva;
			fscanf(arq, "%d %d", &codigo, &reserva);
				atualizar(vetor, codigo, reserva); //Troca a prioridade de uma sala; 
		}break;
		
		case 'B':{
		fscanf(arq, "%d", &codigo);
            if (consultar(&avl_Raiz, codigo) != NULL) { // Verifica se a sala existe
                atualizarEstado(tabela, codigo, "bloqueada"); // Atualiza o estado na Tabela Hash
                printf("Sala %d %s.\n", codigo, consultarEstado(tabela, codigo));
            } else {
                printf("Sala %d não encontrada.\n", codigo);
            }
        }break;
        
        case 'D':{
        fscanf(arq, "%d", &codigo);
            if (consultar(&avl_Raiz, codigo) != NULL) { // Verifica se a sala existe
                atualizarEstado(tabela, codigo, "disponivel"); // Atualiza o estado na Tabela Hash
                printf("Sala %d %s.\n", codigo, consultarEstado(tabela, codigo));
            } else {
                printf("Sala %d não encontrada.\n", codigo);
            }
			
		}break;
	 }
	}
	
	fclose(arq); //Fecha arquivo;
	liberarArvore(avl_Raiz);
	liberarHeap(vetor);
	liberarHash(tabela);
	
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
void exibirArvore(no *avl) {
    if (avl != NULL) {
        exibirArvore(avl->esq);
        printf("Codigo: %d, Capacidade: %d\nAltura Esquerda: %d, Altura Direita: %d\n", avl->codigo, avl->capacidade, avl->Cont_Esquerda, avl->Cont_Direita);
        printf("\n");
        exibirArvore(avl->dir);
	}
}

void liberarArvore(no *avl) {
    if (avl == NULL) 
	return;
	
    liberarArvore(avl->esq);
    liberarArvore(avl->dir);
    free(avl);
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

/*acha o codigo e altera a prioridade, porem pode usar
			consultar(heap);
*/
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
    printf("Reserva com codigo %d nao encontrada.\n", codigo);
    
}
//Tirado do moodle porem adicionando int para funcionar
void heapify(hp *heap, int i){
 	int maior = i;
    int filhoEsquerda = 2 * i;
    int filhoDireita = 2 * i + 1;

    if (filhoEsquerda <= heap->tamanho && heap->vetor[filhoEsquerda].prioridade > heap->vetor[maior].prioridade) {
        maior = filhoEsquerda;
    }
    if (filhoDireita <= heap->tamanho && heap->vetor[filhoDireita].prioridade > heap->vetor[maior].prioridade) {
        maior = filhoDireita;
    }
    if (maior != i) {
        troca(&heap->vetor[i], &heap->vetor[maior]);
        heapify(heap, maior);
    }
	
}

hp *cancelarHeap(hp *heap, int codigo){
    if (heap == NULL) {
        printf("Sem heap\n");
        return NULL;
    }

    int i;
    for (i = 1; i <= heap->tamanho; i++) {
        if (heap->vetor[i].codigo == codigo) {
            heap->vetor[i] = heap->vetor[heap->tamanho];  // Substitui pelo último
            heap->tamanho--; // Diminui o tamanho da heap
            heapify(heap, i); // Ajusta a heap
            printf("\nReserva com codigo %d cancelada.\n", codigo);
            return heap;
        }
    }

    printf("Reserva com codigo %d nao encontrada.\n", codigo);
    return heap;
}

void exibirReserva(hp* heap) {
	int i;
    	for (i = 1; i <= heap->tamanho; i++) {
        	printf("Codigo: %d, Prioridade: %d\n", heap->vetor[i].codigo, heap->vetor[i].prioridade);
    	}
}

void liberarHeap(hp *heap){
    if (heap == NULL) 
	return;
	
    free(heap->vetor);
    free(heap);
}

//hash
tabelaHash* criarTabela(int tamanho){
    tabelaHash *hs = (tabelaHash*)malloc(sizeof(tabelaHash));
    hs->tamanho = tamanho;
    hs->tabela = (estadoSala**)calloc(tamanho, sizeof(estadoSala*)); // Inicializa com NULL
    return hs;
}

int funcaoHash(tabelaHash *hs, int codigo){
    return codigo % hs->tamanho; // Usa o código da sala para calcular o índice
}

void inserirEstado(tabelaHash *hs, int codigo, const char *estado){
    int indice = funcaoHash(hs, codigo);

    // Cria um novo nó para o estado da sala
    estadoSala *novo = (estadoSala*)malloc(sizeof(estadoSala));
    novo->codigo = codigo;
    strcpy(novo->estado, estado);
    novo->prox = NULL;

    // Insere na lista encadeada (tratamento de colisões)
    if (hs->tabela[indice] == NULL) {
        hs->tabela[indice] = novo; // Primeiro nó na posição
    } else {
        // Percorre a lista encadeada até o final
        estadoSala *atual = hs->tabela[indice];
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo; // Adiciona no final
    }
}

void atualizarEstado(tabelaHash *hs, int codigo, char *novo_estado){
    int indice = funcaoHash(hs, codigo);

    // Percorre a lista encadeada no índice correspondente
    estadoSala *atual = hs->tabela[indice];
    while (atual != NULL) {
        if (atual->codigo == codigo) {
            strcpy(atual->estado, novo_estado); // Atualiza o estado
            return;
        }
        atual = atual->prox;
    }

    // Se a sala não foi encontrada
    printf("Sala %d nao encontrada.\n", codigo);
}

char* consultarEstado(tabelaHash *hs, int codigo){
    int indice = funcaoHash(hs, codigo);

    // Percorre a lista encadeada no índice correspondente
    estadoSala *atual = hs->tabela[indice];
    while (atual != NULL) {
        if (atual->codigo == codigo) {
            return atual->estado; // Retorna o estado da sala
        }
        atual = atual->prox;
    }

    // Se a sala não foi encontrada
    return "Sala nao encontrada";
}

void liberarHash(tabelaHash *hs){
    int i;
	if (hs == NULL) {
        return;
    }
    // Percorre cada posição da tabela
    for (i = 0; i < hs->tamanho; i++) {
        estadoSala *atual = hs->tabela[i];
        while (atual != NULL) {
            estadoSala *temp = atual;
            atual = atual->prox;
            free(temp); // Libera cada nó da lista encadeada
        }
    }
    // Libera o array da tabela e a estrutura da Tabela Hash
    free(hs->tabela);
    free(hs);
}
