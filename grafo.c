#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

/* Estrutura de dados auxiliar - lista encadeada */
typedef struct cel_struct{
    char chave[50];
    struct cel_struct * prox;
} tnodo;

void inicia_lista(tnodo * nodo, char * new){
    strcpy(nodo->chave, new);
    nodo->prox=NULL;
}

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

void imprime_inverso(tnodo *nodo){
    if (nodo->prox==NULL)
    {
        printf("%d ", nodo->chave);
        return;
    }
    else
    {
        imprime_inverso(nodo->prox);
        printf("%d ", nodo->chave);
    }
        
}

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

void busca_lista(tnodo * nodo, int buscado){
    if (nodo->chave != buscado) 
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
    


#define DEBUG 1
//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo pode ser direcionado ou não
// 
// o grafo tem um nome, que é uma "string"

typedef grafo * agraph_t;

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso,
//         ou 
//         0, caso contrário

int destroi_grafo(grafo g) {
    return ! agclose((Agraph_t *)g);
}
//------------------------------------------------------------------------------
// lê um grafo no formato dot de input
// 
// devolve o grafo lido,
//         ou 
//         NULL, em caso de erro 

grafo le_grafo(FILE *input) {

    Agraph_t *g;
    if (g = agread(input, NULL)){

        return g;
    }
    else return NULL;
}
//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot.
//
// devolve o grafo escrito,
//         ou 
//         NULL, em caso de erro 

grafo escreve_grafo(FILE *output, grafo g) {
    if (g = agwrite(g, output)){
        return g;
    }
    else return NULL;
}
//------------------------------------------------------------------------------
// devolve o grafo de recomendações de g
//
// cada aresta {c,p} de H representa uma recomendação do produto p
// para o consumidor c, e tem um atributo "weight" que é um inteiro
// representando a intensidade da recomendação do produto p para o
// consumidor c.
//
// cada vértice de g tem um atributo "tipo" cujo valor é 'c' ou 'p',
// conforme o vértice seja consumidor ou produto, respectivamente

int eh_consumidor(Agnode_t *n, Agsym_t* sym){
    return strchr(agxget(n, sym), 'c');
}



// guardar essa funcao para depois


tnodo visita_vizinhanca(Agraph_t *g, Agnode_t *n){
            Agedge_t *e;
            tnodo lista;
            inicia_lista(&lista, "");
            for (e = agfstedge(g,n); e; e = agnxtedge(g,e,n)){
                    if (!strcmp(agnameof(n), agnameof(aghead(e)))){
//                        printf("---->Vizinho: %s\n",agnameof(agtail(e)));
                        insere_lista(&lista, agnameof(agtail(e)));
                    }
                    else{
//                        printf("---->Vizinho: %s\n",agnameof(aghead(e)));
                        insere_lista(&lista, agnameof(aghead(e)));
                    }
            }
            imprime_lista(&lista);
            return lista;
}



grafo recomendacoes(grafo g){
    Agnode_t *v;
    Agnode_t *u;
    Agedge_t *e;
//    char str[100];
    Agsym_t* sym = agattr(g,AGNODE,"tipo", 0);
    for (v = agfstnode(g); v; v = agnxtnode(g,v)) {
        if (eh_consumidor(v, sym)){
            
            printf("Vértice: %s (tipo:%s)\n",agnameof(v), agxget(v,sym));
            visita_vizinhanca(g, v);
            u = agnxtnode(g, v);
            while (u){
                if (eh_consumidor(u, sym)){
                    printf("----> Comparando com u: %s\n",agnameof(u));
                    visita_vizinhanca(g, u);
                }
                u = agnxtnode(g, u);

            }
            
        }
    }
  return g;
}

//------------------------------------------------------------------------------

