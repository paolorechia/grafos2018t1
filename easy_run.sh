#!/bin/bash


./teste < $1 > saida.dot;
dot -Tpdf saida.dot -o saida.pdf;
evince saida.pdf;
