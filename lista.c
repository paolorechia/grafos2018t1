#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

/* Estrutura de dados auxiliar - l encadeada */
/* Adaptar codigo da l

Incluir:

1 - metodo de contagem do size da l (done)
2 - metodo de interseccao de duas ls (arrumar)
3 - metodo de diferenca de duas ls
4 - metodo de limpar a lista (done)
5 - desalocar (free) a lista inteira (done)

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
}
int l_clear(thead * head){
    tnode * node = head->node;
    if (head->node->nxt == NULL){
        return 0;
    }
    rec_clear(head->node->nxt);
    head->node->nxt = NULL;
    head->size = 0;
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

thead * l_filter(thead *l1, thead *l2){
    thead * filtered = l_init();
    // if l1 is empty, result is an empty list (l1)
    // if l2 is empty, no node to filter, result is l1
    if (l1->node->nxt == NULL || l2->node->nxt == NULL){
        return l1;
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
int main(int argc, char * argv[])
{
    thead *list_del = l_init();
    l_clear(list_del);
    l_free(list_del);

    thead * list1 = l_init();
    l_insert(list1, "Cel");
    l_insert(list1, "TV");
    l_insert(list1, "Arroz");
    l_insert(list1, "Hmm");
    printf("Lista com %d elemento(s)\n", l_size(list1));
    l_print(list1);

    thead * list2 = l_init();
    l_insert(list2, "Cel");
    l_insert(list2, "TV");
    l_insert(list2, "Feijoada");
    printf("Lista com %d elemento(s)\n", l_size(list2));
    l_print(list2);

    
    thead * l_inter = l_intersection(list1, list2);
    l_print(l_inter);
    printf("Lista com %d elemento(s)\n", l_size(l_inter));

    thead * l_diff1 = l_filter(list1, l_inter);
    l_print(l_diff1);
    printf("Lista com %d elemento(s)\n", l_size(l_diff1));

    thead * l_diff2 = l_filter(list2, l_inter);
    l_print(l_diff2);
    printf("Lista com %d elemento(s)\n", l_size(l_diff2));

    l_clear(list1);
    l_free(list1);
    l_free(list2);
    l_free(l_inter);
    l_free(l_diff1);
    l_free(l_diff2);
    return 0;

}
