#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define TAM_ALFABETO 26
#define TAM_TABELA 6

// Estrutura de Dados da Mansao 
struct No {
    char valor[50];
    char pista[50];
    struct No* esquerda;
    struct No* direita;
};

struct NoTrie {
    struct NoTrie* filhos[TAM_ALFABETO];
    bool FimDePalabra;
};

typedef struct NoIndice {
    char pista[50];
    char suspeito[50];
    struct NoIndice* proximo;
} NoIndice;

NoIndice* tabela_hash[TAM_TABELA];

// Prototipos 
struct No* criarNo(const char* valor, const char* pista);
void emOrdem(struct No* raiz);
void liberar(struct No* raiz);
bool explorarSalasComPistas(struct No* raiz, struct NoTrie* raizTrie);
void limparBuffer();

struct NoTrie* criarNoTrie();
void inserirTrie(struct NoTrie* raizTrie, const char* palavra);
bool buscarTrie(struct NoTrie* raizTrie, const char* palavra);
void listarPalavras(struct NoTrie* no, char* buffer, int nivel);
void normalizar(const char* entrada, char* saida);

int funcaoHash(const char* chave);
void inserirHash(const char* pista, const char* suspeito);
NoIndice* buscarHash(const char* pista);
void inserirTodasPistasNaHash(struct NoTrie* no, char* buffer, int nivel);

// Liberar memoria da tabela hash
void liberarTabelaHash();

// inserir pista no suspeito
const char* obterSuspeito(const char* pista) {
    
    if (strcmp(pista, "faca") == 0) return "Carlos";
    if (strcmp(pista, "sangue") == 0) return "Alex";
    if (strcmp(pista, "gaveta") == 0) return "Alice";
    if (strcmp(pista, "livro") == 0) return "Lucas";
    if (strcmp(pista, "forno") == 0) return "Livia";
    if (strcmp(pista, "retrato") == 0) return "Alex";
    return "Desconhecido";
}

int main() {
    int resp;

    // Inicializa a tabela hash
    for (int i = 0; i < TAM_TABELA; i++) {
        tabela_hash[i] = NULL;
    }

    // inicializacao da arvore mansao com as pistas
    struct NoTrie* raizTrie = criarNoTrie();
    char buffer[100];
    struct No* raiz = criarNo("Hall de Entrada", " ");

    if (raiz == NULL) {
        return 1;
    }

    // Criar um filho à esquerda
    raiz->esquerda = criarNo("Cozinha", "forno quente");
    raiz->esquerda->esquerda = criarNo("Quarto", "Retrato Qubrado");
    raiz->esquerda->direita = criarNo("Banheiro", "Sangue");

    // Criar um filho a Direita
    raiz->direita = criarNo("Biblioteca", "Livro sumido");
    raiz->direita->esquerda = criarNo("Despensa", "Gaveta Aberta");
    raiz->direita->direita = criarNo("Cozinha", "Faca Sumiu");

    do {   
        printf("\n====MENU DA MANSAO====\n");
        printf("1 - Navegar pela Mansao\n");
        printf("2 - Listar Pistas Encontradas\n");
        printf("3 - Buscar Suspeito por Pista\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &resp);
        limparBuffer();     

        switch (resp) {
            case 1: {
                printf("\n[AVISO] Voce esta explorando a Mansao...\n");
                explorarSalasComPistas(raiz, raizTrie);
                printf("\nExploracao concluida.\n");
                break;
            }
            case 2: {
                printf("\n[AVISO] Pistas coletadas:\n");
                listarPalavras(raizTrie, buffer, 0);

                // Reinicia a tabela hash para evitar duplicatas
                liberarTabelaHash();
                for (int i = 0; i < TAM_TABELA; i++) tabela_hash[i] = NULL;

                char buf[100];
                inserirTodasPistasNaHash(raizTrie, buf, 0);
                printf("\nPistas associadas aos suspeitos conforme regras!\n");
                break;
            }
            case 3: {
                char pista[50];
                printf("Digite a pista (ex: sangue, faca): ");
                if (fgets(pista, sizeof(pista), stdin) == NULL) break;
                pista[strcspn(pista, "\n")] = 0;

                char pista_normal[50];
                normalizar(pista, pista_normal);

                if (strlen(pista_normal) == 0) {
                    printf("Pista invalida.\n");
                    break;
                }

                NoIndice* resultado = buscarHash(pista_normal);
                if (resultado) {
                    printf("Suspeito associado à pista '%s': %s\n", pista_normal, resultado->suspeito);
                } else {
                    printf("Nenhum suspeito encontrado para a pista '%s'.\n", pista_normal);
                }
                break;
            }
            case 0:
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (resp != 0);

    liberar(raiz);
    liberarTabelaHash();
    return 0;
}

// === Implementações ===


/// @brief Cria um No 
struct No* criarNo(const char* valor, const char* pista) {
    struct No* novo = (struct No*) malloc(sizeof(struct No));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória!\n");
        return NULL;
    }
    strncpy(novo->valor, valor, 49);
    strncpy(novo->pista, pista, 49);
    novo->valor[49] = '\0';
    novo->pista[49] = '\0';
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

/// @brief percore a arvore pelos comodos 
/// @param raiz 
/// @param raizTrie 
/// @return 
bool explorarSalasComPistas(struct No* raiz, struct NoTrie* raizTrie) {
    if (raiz == NULL) return true;

    if (!explorarSalasComPistas(raiz->esquerda, raizTrie)) return false;

    printf("Comodo: %s, Pista: %s\n", raiz->valor, raiz->pista);

    // Divide a pista em palavras
    char copia[100];
    strncpy(copia, raiz->pista, 99);
    copia[99] = '\0';

    char* token = strtok(copia, " ,.!?:;\"\'\n\t");
    while (token != NULL) {
        char palavra_norm[50];
        normalizar(token, palavra_norm);
        if (strlen(palavra_norm) > 0) {
            inserirTrie(raizTrie, palavra_norm);
        }
        token = strtok(NULL, " ,.!?:;\"\'\n\t");
    }

    printf("[AVISO] Deseja ir para o proximo comodo (s/n): ");
    char resp;
    scanf(" %c", &resp);
    limparBuffer();

    if (resp == 'n' || resp == 'N') return false;

    return explorarSalasComPistas(raiz->direita, raizTrie);
}

/// @brief libera a memoria 
/// @param raiz 
void liberar(struct No* raiz) {
    if (raiz != NULL) {
        liberar(raiz->esquerda);
        liberar(raiz->direita);
        free(raiz);
    }
}
/// @brief limpa o buffer
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

struct NoTrie* criarNoTrie() {
    struct NoTrie* novo = malloc(sizeof(struct NoTrie));
    if (!novo) return NULL;
    novo->FimDePalabra = false;
    for (int i = 0; i < TAM_ALFABETO; i++) {
        novo->filhos[i] = NULL;
    }
    return novo;
}

void inserirTrie(struct NoTrie* raizTrie, const char* palavra) {
    struct NoTrie* atual = raizTrie;
    for (int i = 0; palavra[i] != '\0'; i++) {
        int idx = palavra[i] - 'a';
        if (atual->filhos[idx] == NULL) {
            atual->filhos[idx] = criarNoTrie();
        }
        atual = atual->filhos[idx];
    }
    atual->FimDePalabra = true;
}

void listarPalavras(struct NoTrie* no, char* buffer, int nivel) {
    if (no && no->FimDePalabra) {
        buffer[nivel] = '\0';
        printf("  - %s\n", buffer);
    }
    if (!no) return;
    for (int i = 0; i < TAM_ALFABETO; i++) {
        if (no->filhos[i]) {
            buffer[nivel] = 'a' + i;
            listarPalavras(no->filhos[i], buffer, nivel + 1);
        }
    }
}

void normalizar(const char* entrada, char* saida) {
    int j = 0;
    for (int i = 0; entrada[i] != '\0'; i++) {
        char c = entrada[i];
        if (c >= 'A' && c <= 'Z') c += 32;
        if (c >= 'a' && c <= 'z') {
            saida[j++] = c;
        }
    }
    saida[j] = '\0'; // ← SÓ AQUI!
}

int funcaoHash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += (unsigned char)chave[i];
    }
    return soma % TAM_TABELA;
}

void inserirHash(const char* pista, const char* suspeito) {
    int idx = funcaoHash(pista);
    NoIndice* novo = malloc(sizeof(NoIndice));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabela_hash[idx];
    tabela_hash[idx] = novo;
}

NoIndice* buscarHash(const char* pista) {
    int idx = funcaoHash(pista);
    NoIndice* atual = tabela_hash[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void inserirTodasPistasNaHash(struct NoTrie* no, char* buffer, int nivel) {
    if (!no) return;

    if (no->FimDePalabra) {
        buffer[nivel] = '\0';
        const char* suspeito = obterSuspeito(buffer);
        inserirHash(buffer, suspeito);
    }

    for (int i = 0; i < TAM_ALFABETO; i++) {
        if (no->filhos[i]) {
            buffer[nivel] = 'a' + i;
            inserirTodasPistasNaHash(no->filhos[i], buffer, nivel + 1);
        }
    }
}

void liberarTabelaHash() {
    for (int i = 0; i < TAM_TABELA; i++) {
        NoIndice* atual = tabela_hash[i];
        while (atual) {
            NoIndice* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabela_hash[i] = NULL;
    }
}