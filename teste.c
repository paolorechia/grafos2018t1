#include <stdlib.h>
#include <stdio.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {

  grafo g = le_grafo(stdin);

  if ( !g )

    return 1;

  recomendacoes(g);

  escreve_grafo(stdout, g);

  return destroi_grafo(g);
}
