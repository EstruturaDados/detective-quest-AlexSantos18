#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#define TAM_ALFABETO 26

// Estrutura de Dados da Mansao 

struct No
{
    char valor[50];
    char pista[50];
    struct No* esquerda;
    struct No* direita;
};

struct NoTrie
{
    struct NoTrie* filhos[TAM_ALFABETO];
    bool FimDePalabra;
};



// Prototipos 

struct No* criarNo(const char* valor, const char* pista);
void emOrdem(struct No* raiz);
void liberar(struct No* raiz);
bool explorarSalasComPistas(struct No* raiz, struct NoTrie* raizTrie);
void limparBuffer();

struct NoTrie* criarNoTrie();
void inserirTrie(struct NoTrie* raizTrie, const char* palavra);
bool buscar(struct NoTrie* raizTrie, const char* palavra);
void listarPalavras(struct NoTrie* no, char* buffer, int nivel);
void normalizar(const char* entrada, char* saida);




int main()
{
    int resp;

    // inicializacao da arvore mansao com as pistas
    struct NoTrie* raizTrie = criarNoTrie();
    char buffer[100];
    struct No* raiz = criarNo("Hall de Entrada", " ");

    if (raiz == NULL) {
        return 1; // erro
    }

    // Criar um filho à esquerda
    raiz->esquerda = criarNo("Cozinha", "Forno esta quente");
    raiz->esquerda->esquerda = criarNo("Quarto", "Retrato quebrado");
    raiz->esquerda->direita = criarNo("Banheiro", "Sangue no chao");
    // Criar um filho a Direita
    raiz->direita = criarNo("Biblioteca", "Livro esta Caido");
    raiz->direita->esquerda = criarNo("Despensa", "Gaveta esta aberta");
    raiz->direita->direita = criarNo("Cozinha", "Faca sumiu");

    do
    {   
        printf("\n====MENU DA MANSAO====\n");
        printf("\n");
        printf("1 - Navegar pela Mansao\n");
        printf("2 - Listar Pista Encontrada\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &resp);
        limparBuffer();     

        switch (resp)
        {
        case 1:
        { 
            printf("\n[AVISO] Voce esta percorrendo o lado Esquerdo da Mansao\n");
            explorarSalasComPistas(raiz, raizTrie); // percore a arvore em ordem pelo lado esquerdo
            printf("\n");    
            break;

        }
        case 2:
        {
            printf("\n");
            printf("[AVISO] Pistas encontradas\n");
            printf("\n");
            listarPalavras(raizTrie, buffer, 0); // lista as pistas encontradas
            break;
        }
        case 0:
        {
            
            break;
        }
            
        
        default:
            break;
        }

            /* code */
    } while (resp != 0);
    liberar(raiz);


    return 0;
}



/// @brief Cria um novo no na arvore binaria 
struct No* criarNo(const char* valor, const char* pista) {
    // Aloca memoria para o novo no
    struct No* novo = (struct No*) malloc(sizeof(struct No));
    
    // Verifica se a alocação foi bem-sucedida
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória!\n");
        return NULL;
    }

    // Copia as strings para os campos (com limite de 49 caracteres + '\0')
    strncpy(novo->valor, valor, 49);
    novo->valor[49] = '\0';  

    strncpy(novo->pista, pista, 49);
    novo->pista[49] = '\0';

    // Inicializa filhos como NULL
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}



/// @brief percorer a arvore em ordem
/// @param raiz 
void emOrdem(struct No* raiz){
    if (raiz != NULL){
        emOrdem(raiz->esquerda);     
        printf("Comodo: %s \n", raiz->valor);       
        emOrdem(raiz->direita);
       
    }
}
/// @brief percore a arvore binaria
/// @param raiz arvore binaria
/// @param raizTrie inseri as pistas na arvore trie
/// @return 
bool explorarSalasComPistas(struct No* raiz, struct NoTrie* raizTrie) {
    if (raiz == NULL) {
        return true; // continua (não há nó para processar)
    }

    // 1. Explorar subárvore esquerda
    if (!explorarSalasComPistas(raiz->esquerda, raizTrie)) {
        return false; // usuário pediu para parar
    }

    // 2. Visitar nó atual
    printf("Comodo: %s, Pista: %s\n", raiz->valor, raiz->pista);
    
    char normalizada[100];
    normalizar(raiz->pista, normalizada);
    if (strlen(normalizada) > 0) { // só insere se houver letras
        inserirTrie(raizTrie, normalizada);
    }

    printf("[AVISO] Deseja ir para o proximo comodo (s/n): ");
    char resp;
    scanf(" %c", &resp); // espaço antes de %c consome lixo no buffer

    if (resp == 'n' || resp == 'N') {
        return false; // sinaliza parada imediata
    }

    // 3. Explorar arvore a  direita
    return explorarSalasComPistas(raiz->direita, raizTrie);
}


/// @brief liberar a memoria 
/// @param raiz 
void liberar(struct No* raiz){
    if (raiz != NULL){
        liberar(raiz->esquerda);
        liberar(raiz->direita);
        free(raiz);
    }
}

/// @brief limpa buffer da memoria
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/// @brief Cria um no na arvore Trie
struct NoTrie* criarNoTrie(){
    struct NoTrie* novoNo = (struct NoTrie*) malloc(sizeof(struct NoTrie));
    novoNo->FimDePalabra = false;
    for (int i = 0; i < TAM_ALFABETO; i++){
        novoNo->filhos[i] = NULL;
    }
    return novoNo;
}

/// @brief inseri a pista encontrada na arvore
/// @param raizTrie 
/// @param palavra normaliza a string tirando os espacos letras maiusculas
void inserirTrie(struct NoTrie* raizTrie, const char* palavra){
    struct NoTrie* atual = raizTrie;

    for (int i = 0; palavra[i] != '\0'; i++){
        int indice = palavra[i] - 'a';

        if (atual->filhos[indice] == NULL){
            atual->filhos[indice] = criarNoTrie();
        }
        atual = atual->filhos[indice];
    } 
    atual->FimDePalabra = true;
}

/// @brief busca na arvore trie
/// @param raizTrie 
/// @param palavra 
/// @return 
bool buscar(struct NoTrie* raizTrie, const char* palavra){
    struct NoTrie* atual = raizTrie;

    for (int i = 0; palavra[i] != '\0'; i++){
        int indice = palavra[i] - 'a';

        if (atual->filhos[indice] == NULL){
            return false;
        }
        atual = atual->filhos[indice];   
    }
    return atual != NULL && atual->FimDePalabra;
}

/// @brief lista as pistas da arvore trie
/// @param no 
/// @param buffer 
/// @param nivel 
void listarPalavras(struct NoTrie* no, char* buffer, int nivel){
    if (no->FimDePalabra){
        buffer[nivel] = '\0';
        printf("Pistas: %s \n", buffer);
    }

    for (int i = 0; i < TAM_ALFABETO; i++){
        if (no->filhos[i] != NULL){
            buffer[nivel] = 'a' + i;
            listarPalavras(no->filhos[i], buffer, nivel + 1);
        }

    }
    
}

/// @brief limpa a string de espacos e letra maiuscula
/// @param entrada 
/// @param saida 
void normalizar(const char* entrada, char* saida){
    int j = 0;
    for (int i = 0; entrada[i] != '\0'; i++ ){
        char c = entrada[i];
        if (c >= 'A' && c <= 'Z'){
            c += 32;
        }
        if (c >= 'a' && c <= 'z')
        {
            saida[j++] = c;
        }
        saida[j] = '\0';
        
    }
}
