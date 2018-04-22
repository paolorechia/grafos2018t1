#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

/* Estrutura de dados auxiliar - lista encadeada */
/* Adaptar codigo da lista

Incluir:

1 - metodo de contagem do tamanho da lista (testar)
2 - metodo de interseccao de duas listas (arrumar)
3 - metodo de diferenca de duas listas
4 - desalocar (free) na lista inteira

*/
typedef struct cel_struct{
    char chave[50];
    struct cel_struct * prox;
} tnodo;

typedef struct head{
    tnodo * nodo;
    int size;
} thead;

thead * inicia_lista(){
    thead * head = malloc(sizeof(thead));
    head->nodo = malloc(sizeof(tnodo));
    head->nodo->prox = NULL;
    head->size = 0;
    strcpy(head->nodo->chave,"#HEAD#");
    return head;
}
void insere_lista(thead * head, char * new){
    tnodo * nodo = head->nodo;
    while (nodo->prox != NULL){
        nodo = nodo -> prox;
    }
    nodo->prox=malloc(sizeof(tnodo));
    strcpy(nodo->prox->chave,new);
    nodo->prox->prox=NULL;
    head->size += 1;
    return;
}

int tamanho_lista(thead * head){
    return head->size;
}
/*
tnodo * inicia_lista(){
    tnodo * nodo = malloc(sizeof(tnodo));
    strcpy(nodo->chave, "#HEAD#");
    nodo->prox=NULL;
    return nodo;
}
*/

void imprime_lista(tnodo * nodo){

    tnodo * aux = nodo;
    while (aux->prox != NULL){
        aux = aux->prox;
        printf("%s ", aux->chave);
    }
    putchar('\n');
}
/*
void imprime_lista(tnodo * nodo){
    printf("%s ", nodo->chave);
    if (nodo->prox==NULL)
    {
        putchar('\n');
        return; 
    }
    else
        imprime_lista(nodo->prox);
}
*/

void imprime_inverso(tnodo *nodo){
    if (nodo->prox==NULL)
    {
        printf("%s ", nodo->chave);
        return;
    }
    else
    {
        imprime_inverso(nodo->prox);
        printf("%s ", nodo->chave);
    }
        
}

/*
void insere_lista(tnodo * nodo, int new){
    if (nodo->prox!=NULL)
        insere_lista(nodo->prox, new);
    else
    {
        nodo->prox=malloc(sizeof(tnodo));
        strcpy(nodo->prox->chave,new);
        nodo->prox->prox=NULL;
    }
    return;
}
*/

void busca_lista(tnodo * nodo, char * buscado){
    if (!strcmp(nodo->chave, buscado))
        if (nodo->prox!=NULL)
            busca_lista(nodo->prox, buscado);
        else
        {
            puts("Elemento nao encontrado");
            return;
        }
    else
        puts("Elemento encontrado");
    return;
}


tnodo * interseccao(tnodo *lista1, tnodo *lista2){
    tnodo * lista_inter;
    tnodo * aux1;
    tnodo * aux2;

    imprime_lista(lista1);
    imprime_lista(lista2);
    aux1 = lista1;
    aux2 = lista2;

    inicia_lista(lista_inter, "");

    printf("%d\n", aux1->prox);
    while (aux1->prox != NULL){
        while (aux2->prox != NULL){
            if (strcmp(aux1->chave, aux2->chave)){
                insere_lista(lista_inter, aux1->chave);
            }
            if (aux2->prox != NULL){
                aux2 = aux2->prox;
            }
        } 
        if (aux1->prox != NULL){
            aux1 = aux1->prox;
        }
    }
    return lista_inter;
}

void remocao_lista(tnodo *nodo, int buscado, tnodo *ant){
    if (nodo->chave != buscado)
        if (nodo->prox != NULL)
            remocao_lista(nodo->prox, buscado, nodo);
        else
        {
            puts("Remocao impossivel, elemento nao encontrado");
            return;
        }
            
    else
    {
        printf("Removendo elemento %d...\n", nodo->chave);
        ant->prox=nodo->prox;
        free(nodo);
        return;
    }
    return;

}
/* valor eh retornado em k */

int main(int argc, char * argv[])
{
    thead * lista1 = inicia_lista();
    insere_lista(lista1, "Teste");
    printf("Lista com %d elemento(s)\n", tamanho_lista(lista1));
    inicia_lista(lista1, "Teste");
    printf("Lista com %d elemento(s)\n", tamanho_lista(lista1));
    insere_lista(lista1, "Cel");
    printf("Lista com %d elemento(s)\n", tamanho_lista(lista1));
    insere_lista(lista1, "TV");
    printf("Lista com %d elemento(s)\n", tamanho_lista(lista1));
    insere_lista(lista1, "Arroz");
    printf("Lista com %d elemento(s)\n", tamanho_lista(lista1));
    insere_lista(lista1, "Hmm");
    printf("Lista com %d elemento(s)\n", tamanho_lista(lista1));
/*
    imprime_lista(lista1);

    tnodo lista2;
    inicia_lista(&lista2, "Head");
    insere_lista(&lista2, "Cel");
    insere_lista(&lista2, "TV");
    insere_lista(&lista2, "Arroz");
    
    tnodo * lista_inter = interseccao(lista1, &lista2);
    imprime_lista(lista_inter);
*/
    return 0;

}