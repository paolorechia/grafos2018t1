#include <stdlib.h>
#include <stdio.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {

  grafo g = le_grafo(stdin);

  if ( !g )

    return 1;

  grafo h = recomendacoes(g);

  escreve_grafo(stdout, g);
  escreve_grafo(stdout, h);

  destroi_grafo(h);
  return destroi_grafo(g);
}
