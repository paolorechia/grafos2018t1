------------------------------------------------------------
UFPR - DINF
CI065 - ALGORITMOS E TEORIA DOS GRAFOS
Prof. Renato Carmo
Alunos: Fulano - GRR
        Paolo Andreas Stall Rechia- GRR 20135196
        Luís Mateus M. Bossi      - GRR 20151799

------------------------------------------------------------

Tópicos:

1. Pseudo-código do algoritmo implementado.
2. Detalhes relevantes da implementação.


1. Algoritmo implementado:

    Para cada v E V(G)
        Se v.tipo == 'c':
            Para cada u E V(G), tal que u != v e u.tipo == 'c':
                Para cada u E vizinhanca(v)
                

---------------------------------------------------------

Considereaçoes: 

Para uma melhor forma de analisar o código, utilizamos uma lista
de caracteres para marcar a vizinhança de um vértice. Sendo assim,
quando fossemos verificar o tamanho da interseccao e da diferença entre
as vizinhanças, seria muito mais simples e didático para analisar o código.
Mesmo sabendo que essa nao é a forma mais otimizada de resolver o problema,
pois seria possivel utilizar da própria biblioteca para calcular a diferença
e a intersecçao, preferimos montar o código dessa forma para que fosse melhor
interpretádo e mais facilmente corrigido. 