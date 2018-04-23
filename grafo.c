#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

/* Estrutura de dados auxiliar - lista encadeada */
typedef struct cel_struct{
    char key[50];
    struct cel_struct * nxt;
} tnode;

typedef struct head{
    tnode * node;
    int size;
} thead;

thead * l_init(){
    thead * head = malloc(sizeof(thead));
    head->node = malloc(sizeof(tnode));
    head->node->nxt = NULL;
    head->size = 0;
    strcpy(head->node->key,"#HEAD#");
    return head;
}
void l_insert(thead * head, char * new){
    tnode * node = head->node;
    while (node->nxt != NULL){
        node = node -> nxt;
    }
    node->nxt=malloc(sizeof(tnode));
    strcpy(node->nxt->key,new);
    node->nxt->nxt=NULL;
    head->size += 1;
    return;
}
void l_print(thead * head){
    printf("--------->"); 
    if (head->node->nxt == NULL){
        printf("Empty l\n");
        return;
    }
    tnode * node = head->node->nxt;
    while (node){
        printf("%s ", node->key);
        node = node->nxt;
    }
    putchar('\n');
}

int l_size(thead * head){
    return head->size;
}
/* Recursive node free function */
int rec_clear(tnode * node){
    if (node->nxt != NULL){
        rec_clear(node->nxt);
    }
//    printf("Freeing node of name: %s\n", node->key);
    node->nxt = NULL;
    free(node);
    return 0;
}
int l_clear(thead * head){
    tnode * node = head->node;
    if (head->node->nxt == NULL){
        return 0;
    }
    rec_clear(head->node->nxt);
    head->node->nxt = NULL;
    head->size = 0;
    return 1;
}
void l_free(thead *head){
    l_clear(head);
    free(head->node);
    free(head);
}
/* Checks if element exists in list */
int l_search(thead* head, char * buscado){
    // if empty list
    if (head->node->nxt == NULL){
        return 0;
    }
    tnode * node = head->node->nxt;
    while (node){
        if (!strcmp(node->key, buscado)) return 1;
        node = node->nxt;
    }   
    return 0;
}

thead * l_intersection(thead *l1, thead * l2){
    thead * inter_l = l_init();
    // if either list is empty, intersection is empty
    if (l1->node->nxt == NULL || l2->node->nxt == NULL){
        return inter_l;
    } 
    // fetch first element
    tnode * node = l1->node->nxt;
    while (node){
        if (l_search(l2, node->key)){
            l_insert(inter_l, node->key);
        }
        node = node->nxt;
    }
    return inter_l;
}

thead * l_copy (thead *lcopy, thead *list)
{
    //Axiliar node for the main list
    thead* lnode = list;
    while (lnode)
    {
        lnode = lnode->nxt;         //Take the next element of the list (the first is the head)
        l_insert(lcopy, lnode->key) //Insert the key of the list in the copy
    }
    return lcopy; //Return de head of the copy.
}

thead * l_filter(thead *l1, thead *l2){
    thead * filtered = l_init();
    // if l1 is empty, result is an empty list (l1)
    // if l2 is empty, no node to filter, result is l1
    if (l1->node->nxt == NULL){
        return l1;
    }
    // create a copy to avoid free issues
    if (l2->node->nxt == NULL){
        return l_copy(filtered, l1);
    }
    tnode * node = l1->node->nxt;
    while (node){
        if (!l_search(l2, node->key)){
            l_insert(filtered, node->key);
        }
        node = node->nxt;
    }
    return filtered;
}
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


/* Consertar seg fault */
thead * visita_vizinhanca(Agraph_t *g, Agnode_t *n){
    Agedge_t *e;
    thead * vizinhos = l_init();
    for (e = agfstedge(g,n); e; e = agnxtedge(g,e,n)){
        if (!strcmp(agnameof(n), agnameof(aghead(e)))){
//          printf("---->Vizinho: %s\n",agnameof(agtail(e)));
            l_insert(vizinhos, agnameof(agtail(e)));
        }
        else{
//          printf("---->Vizinho: %s\n",agnameof(aghead(e)));
            l_insert(vizinhos, agnameof(aghead(e)));
        }
    }
    l_print(vizinhos);
    return vizinhos;
}

// implementar:
// interseccao(lista1, lista2)
// diferenca(lista1, lista) 


grafo recomendacoes(grafo g){
    Agnode_t *v;
    Agnode_t *u;
    //Agedge_t *e;
//    char str[100];
    Agsym_t* sym = agattr(g,AGNODE,"tipo", 0);
    for (v = agfstnode(g); v; v = agnxtnode(g,v)) {
        if (eh_consumidor(v, sym)){
            
            printf("Vértice: %s (tipo:%s)\n",agnameof(v), agxget(v,sym));
            thead * lista1 = visita_vizinhanca(g, v);
            u = agnxtnode(g, v);
            while (u){
                if (eh_consumidor(u, sym)){
                    printf("----> Comparando com u: %s\n",agnameof(u));
                    thead * lista2 = visita_vizinhanca(g, u);
                    thead * interseccao = l_intersection(lista1, lista2);
                    thead * diff_vu = l_filter(lista1, interseccao);
                    thead * diff_uv = l_filter(lista2, interseccao);
                    
                    printf("Dentro\n");
/*
                    // Imprimindo listas
                    printf("------> Vertice v com %d vizinho(s)\n", l_size(lista1));
                    l_print(lista1);
                    printf("------> Vertice u com %d vizinho(s)\n", l_size(lista2));
                    l_print(lista2);
                    printf("------> Interseccao com %d elemento(s)\n",\
                            l_size(interseccao));
                    l_print(interseccao);
                    printf("------> Diff viz(v) - viz(u) com %d elemento(s)\n",\
                            l_size(diff_vu));
                    l_print(diff_vu);
                    printf("------> Diff viz(u) - viz(v) com %d elemento(s)\n", \
                            l_size(diff_uv));
                    l_print(diff_uv);
*/


                    // Colocar aqui logica para modificar o grafo ponderado:
                    // Criar vertice
                    // Criar uma aresta com peso 1
                    // Incrementar aresta com peso
                    l_free(lista2);
                    l_free(interseccao);
                    l_free(diff_vu);
                    l_free(diff_uv);
                }
                u = agnxtnode(g, u);
            }
            printf("Fora\n");
//            l_print(lista1);
            l_free(lista1);
        }
    }
  return g;
}

//------------------------------------------------------------------------------

