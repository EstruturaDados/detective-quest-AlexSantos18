#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Estrutura de Dados da Mansao 

struct No
{
    char valor[50];
    struct No* esquerda;
    struct No* direita;
};



// Prototipos 

struct No* criarNo(char* valor);
void preOrdem(struct No* raiz);
void emOrdem(struct No* raiz);
void posOrdem(struct No* raiz);
void liberar(struct No* raiz);






int main()
{
    int resp;

    struct No* raiz = criarNo("Hall de Entrada");
    raiz->esquerda = criarNo("Sala de Estar");
    raiz->direita = criarNo("Biblioteca");
    raiz->esquerda->esquerda = criarNo("Quarto");
    raiz->esquerda->direita = criarNo("Banheiro");
    raiz->direita->direita = criarNo("Cozinha");
    raiz->direita->esquerda = criarNo("Despensa");



    do
    {   
        printf("\n====MENU DA MANSAO====\n");
        printf("\n");
        printf("1 - Navegar pelo lado Esquerdo\n");
        printf("2 - Navegar pelo lado Direito\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &resp);     

        switch (resp)
        {
        case 1:
        { 
            printf("\n[AVISO] Voce esta percorrendo o lado Esquerdo da Mansao\n");
            emOrdem(raiz->esquerda);
            printf("\n");    
            break;

        }
        case 2:
        {
            printf("\n[AVISO] Voce esta percorrendo o lado Direito da Mansao\n");
            emOrdem(raiz->direita);
            printf("\n");
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
struct No* criarNo(char* valor){
    struct No* novo = (struct No*) malloc(sizeof(struct No));
    strcpy(novo->valor, valor);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
    
}

void preOrdem(struct No* raiz){
    if (raiz != NULL){
        printf("Comodo: %s \n", raiz->valor);
        preOrdem(raiz->esquerda);
        preOrdem(raiz->direita);
    }
}

void emOrdem(struct No* raiz){
    if (raiz != NULL){
        emOrdem(raiz->esquerda);
        printf("Comodo: %s \n", raiz->valor);
        emOrdem(raiz->direita);
    }
}

void posOrdem(struct No* raiz){
    if (raiz != NULL){
        posOrdem(raiz->esquerda);
        posOrdem(raiz->direita);
        printf("Comodo: %s \n", raiz->valor);
    }
}

void liberar(struct No* raiz){
    if (raiz != NULL){
        liberar(raiz->esquerda);
        liberar(raiz->direita);
        free(raiz);
    }
}

