#!/bin/bash


./teste < $1 > saida.dot;
echo "Return code: $?"
dot -Tpdf saida.dot -o saida.pdf
evince saida.pdf
