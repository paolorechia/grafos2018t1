#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

/* Estrutura de dados auxiliar - l encadeada */
/* Adaptar codigo da l

Incluir:

1 - metodo de contagem do size da l (testar)
2 - metodo de interseccao de duas ls (arrumar)
3 - metodo de diferenca de duas ls
4 - desalocar (free) na l inteira

*/
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
    if (head->node->nxt == NULL){
        printf("Empty l\n");
        return;
    }
    tnode * node = head->node->nxt;
    while (node->nxt != NULL){
        printf("%s ", node->key);
        node = node->nxt;
    }
    printf("%s ", node->key);
    putchar('\n');
}

int l_size(thead * head){
    return head->size;
}


/* Old code */
/*
void busca_l(tnode * node, char * buscado){
    if (!strcmp(node->key, buscado))
        if (node->nxt!=NULL)
            busca_l(node->nxt, buscado);
        else
        {
            puts("Elemento nao encontrado");
            return;
        }
    else
        puts("Elemento encontrado");
    return;
}


tnode * interseccao(tnode *l1, tnode *l2){
    tnode * l_inter;
    tnode * aux1;
    tnode * aux2;

    l_print(l1);
    l_print(l2);
    aux1 = l1;
    aux2 = l2;

    l_init(l_inter, "");

    printf("%d\n", aux1->nxt);
    while (aux1->nxt != NULL){
        while (aux2->nxt != NULL){
            if (strcmp(aux1->key, aux2->key)){
                l_insert(l_inter, aux1->key);
            }
            if (aux2->nxt != NULL){
                aux2 = aux2->nxt;
            }
        } 
        if (aux1->nxt != NULL){
            aux1 = aux1->nxt;
        }
    }
    return l_inter;
}

void remocao_l(tnode *node, int buscado, tnode *ant){
    if (node->key != buscado)
        if (node->nxt != NULL)
            remocao_l(node->nxt, buscado, node);
        else
        {
            puts("Remocao impossivel, elemento nao encontrado");
            return;
        }
            
    else
    {
        printf("Removendo elemento %d...\n", node->key);
        ant->nxt=node->nxt;
        free(node);
        return;
    }
    return;

}
*/


int main(int argc, char * argv[])
{
    thead * list1 = l_init();
    l_insert(list1, "Cel");
    l_insert(list1, "TV");
    l_insert(list1, "Arroz");
    l_insert(list1, "Hmm");
    printf("Lista com %d elemento(s)\n", l_size(list1));
    l_print(list1);

/*
    tnode l2;
    l_init(&l2, "Head");
    l_insert(&l2, "Cel");
    l_insert(&l2, "TV");
    l_insert(&l2, "Arroz");
    
    tnode * l_inter = interseccao(l1, &l2);
    l_print(l_inter);
*/
    return 0;

}
