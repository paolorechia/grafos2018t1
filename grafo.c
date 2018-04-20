#include <stdio.h>
#include <string.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

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

/* Corrigir essa funcao, retorna erro */
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

grafo recomendacoes(grafo g){
    Agnode_t *n;
    Agedge_t *e;
    for (n = agfstnode(g); n; n = agnxtnode(g,n)) {
        printf("Vértice: %s\n",agnameof(n));
        for (e = agfstedge(g,n); e; e = agnxtedge(g,e,n)){
                if (!strcmp(agnameof(n), agnameof(aghead(e))))
                    printf("---->Vizinho: %s\n",agnameof(agtail(e)));
                else
                    printf("---->Vizinho: %s\n",agnameof(aghead(e)));
        }
    }

  return g;
}

//------------------------------------------------------------------------------
