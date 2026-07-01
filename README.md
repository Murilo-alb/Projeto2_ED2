# Sistema de Gerenciamento e Reserva de Salas 🏢

Este projeto é um sistema em linguagem **C** desenvolvido para gerenciar o cadastro, status e a fila de reservas de salas. A aplicação lê comandos a partir de um arquivo de texto e executa operações complexas utilizando três estruturas de dados avançadas trabalhando em conjunto.

Projeto acadêmico desenvolvido para a Universidade Tecnológica Federal do Paraná (UTFPR).

## 🧠 Arquitetura e Estruturas de Dados

O diferencial deste projeto é a integração eficiente de três estruturas distintas para otimizar diferentes tipos de buscas e atualizações:

* **Árvore AVL (`struct AVL`):** * **Uso:** Armazena o catálogo de salas cadastradas (código e capacidade).
    * **Vantagem:** Garante que a árvore permaneça balanceada, permitindo inserções e consultas de salas com complexidade de tempo `O(log n)`.
* **Fila de Prioridade / Max-Heap (`struct heap`):** * **Uso:** Gerencia as reservas ativas.
    * **Vantagem:** Mantém a fila ordenada por nível de prioridade da reserva. O uso de Heap (vetor dinâmico) permite que a extração ou atualização da reserva mais prioritária seja extremamente rápida.
* **Tabela Hash (`struct TabelaHash`):** * **Uso:** Controla o estado dinâmico de cada sala (ex: "disponível" ou "bloqueada").
    * **Vantagem:** Utiliza encadeamento (`struct EstadoSala *prox`) para tratamento de colisões, permitindo a verificação e alteração do status da sala em tempo médio `O(1)`.

## ⚙️ Comandos Suportados

O sistema lê as instruções de um arquivo de entrada (ex: `entrada2.txt`). Os comandos processados incluem:

| Comando | Ação |
| :--- | :--- |
| `CRIAR_SALA <codigo> <capacidade>` | Insere a sala na AVL, inicializa sua Heap e define status como disponível na Hash. |
| `CONSULTAR_SALA <codigo>` | Busca a sala na árvore AVL e exibe sua capacidade. |
| `RESERVAR_SALA <codigo> <prioridade>`| Verifica se a sala existe (AVL) e não está bloqueada (Hash), inserindo a reserva na Heap. |
| `CANCELAR_RESERVA <codigo>` | Remove uma reserva específica da Fila de Prioridade, reajustando o vetor (Heapify). |
| `LISTAR_SALAS` | Imprime a árvore AVL mostrando códigos, capacidades e o balanceamento (alturas). |
| `LISTAR_RESERVAS` | Imprime todas as reservas ativas e suas respectivas prioridades. |
| `A <codigo> <nova_prioridade>` | Atualiza a prioridade de uma reserva existente na Heap. |
| `B <codigo>` | Bloqueia uma sala atualizando seu estado na Tabela Hash. |
| `D <codigo>` | Desbloqueia (disponibiliza) uma sala na Tabela Hash. |

## 🚀 Como Executar

### Pré-requisitos
* Um compilador C (como GCC).

### Instalação e Execução

1. Clone o repositório:
   ```bash
   git clone [https://github.com/seu-usuario/nome-do-repositorio.git](https://github.com/seu-usuario/nome-do-repositorio.git)
