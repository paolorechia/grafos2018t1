#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

/*************************************************/
/* Estrutura de dados auxiliar - lista encadeada */
/*************************************************/
typedef struct cel_struct{
    char key[48];
    struct cel_struct * nxt;
} tnode;

typedef struct head{
    tnode * node;
    int size;
} thead;

//Funçao que inicializa a lista usada para achar a diferença e a interseccao.
static thead * l_init(){
    thead * head = malloc(sizeof(thead));
    head->node = malloc(sizeof(tnode));
    head->node->nxt = NULL;
    head->size = 0;
    strcpy(head->node->key,"#HEAD#");
    return head;
}

//Funçao usada para inserir os chars (vizinhança) na lista daquele vertice.
static void l_insert(thead * head, char * new){
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

//Funçao auxiliar para ajudar a debugar o programa.
static void l_print(thead * head){
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

static int l_size(thead * head){
    return head->size;
}

/* Funcao recursiva para limpar lista a partir do ultimo no*/
static int rec_clear(tnode * node){
    if (node->nxt != NULL){
        rec_clear(node->nxt);
    }
//    printf("Freeing node of name: %s\n", node->key);
    node->nxt = NULL;
    free(node);
    return 0;
}

//Funçao para limpar a lista.
static int l_clear(thead * head){
    tnode * node = head->node;
    if (head->node->nxt == NULL){
        return 0;
    }
    rec_clear(head->node->nxt);
    head->node->nxt = NULL;
    head->size = 0;
    return 1;
}

// Desaloca memoria
static void l_free(thead *head){
    l_clear(head);
    free(head->node);
    free(head);
}
// Busca sequencial de elemento na lista
static int l_search(thead* head, char * buscado){
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
//Funcao que verifica a interseccao entre os dois vertives.
static thead * l_intersection(thead *l1, thead * l2){
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

// cria uma copia da lista
static thead * l_copy(thead *list)
{
    // No auxiliar
    thead* new_list = l_init();
    tnode * lnode = list->node->nxt;
    while (lnode)
    {
        l_insert(new_list, lnode->key); // Insere chave na lista nova
        lnode = lnode->nxt;         // Proximo elemento da lista
    }
    return new_list; //Return de head of the copy.
}

// Filtra l1 com os elementos da lista2
// (Retorna elementos de l1 nao encontrados em l2)
// Utilizado para calcular a diferenca de conjunto
static thead * l_filter(thead *l1, thead *l2){
    // se l1 eh vazia, resultado eh uma lista vazia (l1)
    if (l1->node->nxt == NULL){
        return l1;
    }
    // se l2 eh vazia, nenhum filtro precisa ser aplicado, retorna l1
    // cria uma copia para evitar double free
    if (l2->node->nxt == NULL){
        return l_copy(l1);
    }
    thead * filtered = l_init();
    tnode * node = l1->node->nxt;
    while (node){
        if (!l_search(l2, node->key)){
            l_insert(filtered, node->key);
        }
        node = node->nxt;
    }
    return filtered;
}
/*************************************************/
/* Fim da estrutura de dados lista encadeada     */
/*************************************************/

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

// Verifica se o tipo do vertice eh 'c' (eh consumidor)
static int eh_consumidor(Agnode_t *n, Agsym_t* sym){
    return strchr(agxget(n, sym), 'c');
}

	
// Visita a vizinhanca do vertice e armazena os vizinhos
// em uma lista encadeada que eh retornada
static thead * visita_vizinhanca(Agraph_t *g, Agnode_t *n){
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
    return vizinhos;
}

// Dados um grafo ponderado, dois vertices, o conjunto interseccao das vizinhancas
// e um conjunto diferenca das vizinhancas, gera as recomendacoes no grafo ponderado
static void gera_recomendacoes(Agraph_t *h, Agnode_t * v, Agnode_t * u, \
                        thead * interseccao, thead * diferenca){
                        
                    Agnode_t * x;
                    Agnode_t * y;
                    Agedge_t * e;
                    Agsym_t* sym = agattr(h,AGEDGE,"weight", 0);
                    if (interseccao->size >= diferenca->size){
                        //printf("%d %d\n", interseccao->size, diferenca->size);
                        // Temos condicao de recomendacao
                        // Se o conjunto diff v - u != Vazio
                        if (diferenca->node->nxt != NULL){
                            //printf("Dif\n");
                            // adicionar v em H
                            x = agnode(h, agnameof(v), TRUE);
                            // Para cada u E diff_vu
                            tnode * node = diferenca->node->nxt;
                            while (node){
                                //printf("Aqui\n");
                                //printf("%d\n", node);
                                // adicionar u em H
                                y = agnode(h, node->key, TRUE);
                                e = agedge(h,x,y, "", FALSE);
                                if (!e){
                                    // adicionar aresta {u, w} no grafo H com peso = 1
                                    e = agedge(h,x,y, "", TRUE);
                                    //printf("%s\n", agxget(e, sym));
                                    // set weight 1
                                    agxset(e, sym, "1");
//                                    printf("%s recomendou %s para %s\n", agnameof();
                                }   
                                else{   
                                    // senao incrementa peso de {u, w} em 1
                                    //printf("Incrementa aresta!\n");
                                    int new_weight = atoi(agxget(e, sym));
                                    // set weight++
                                    new_weight++;
                                    char number[10];
                                    sprintf(number, "%d", new_weight);
                                    agxset(e, sym, number);
                                }
                                node = node->nxt;
                                //printf("%d\n", node);
                            }
                        }
                    }
}

grafo recomendacoes(grafo g){
    Agnode_t *v;
    Agnode_t *u;
    // H sera o grafo de recomendacoes
    Agraph_t *h;
    h = agopen("H", Agstrictundirected, NULL);
	agattr(h,AGEDGE,"weight", "0");
    Agsym_t* sym = agattr((Agraph_t*)g,AGNODE,"tipo", 0);
    for (v = agfstnode((Agraph_t*)g); v; v = agnxtnode((Agraph_t*)g,v)) {
        if (eh_consumidor(v, sym)){
            
            //printf("Vértice: %s (tipo:%s)\n",agnameof(v), agxget(v,sym));
            thead * lista1 = visita_vizinhanca((Agraph_t*)g, v);
            //printf("------> Vertice v com %d vizinho(s)\n", l_size(lista1));
            //l_print(lista1);
            u = agnxtnode((Agraph_t*)g, v);
            while (u){
                if (eh_consumidor(u, sym)){
                    //printf("----> Comparando com u: %s\n",agnameof(u));
                    thead * lista2 = visita_vizinhanca((Agraph_t*)g, u);
                    thead * interseccao = l_intersection(lista1, lista2);
                    thead * diff_vu = l_filter(lista1, interseccao);
                    thead * diff_uv = l_filter(lista2, interseccao);
                    // Imprimindo listas
                    //printf("------> Vertice u com %d vizinho(s)\n", l_size(lista2));
                    //l_print(lista2);
                    //printf("------> Interseccao com %d elemento(s)\n", l_size(interseccao));
                    //l_print(interseccao);
                    //printf("------> Diff viz(v) - viz(u) com %d elemento(s)\n", l_size(diff_vu));
                    //l_print(diff_vu);
                    //printf("------> Diff viz(u) - viz(v) com %d elemento(s)\n", l_size(diff_uv));
                    //l_print(diff_uv);

                    /* Gerar arestas de recomendacoes*/
                    gera_recomendacoes(h, v, u, interseccao, diff_uv);
                    gera_recomendacoes(h, u, v, interseccao, diff_vu);

                    l_free(lista2);
                    l_free(interseccao);
                    l_free(diff_vu);
                    l_free(diff_uv);
                }
                u = agnxtnode((Agraph_t*)g, u);
            }
            l_free(lista1);
        }
    }
  return (grafo)h;
}

//------------------------------------------------------------------------------

