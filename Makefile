CC = gcc

single: 
	gcc single.c beggar.c shuffle.c -o single -lgsl -lgslcblas -lm

byn:
	gcc byn.c beggar.c -o byn -lgsl -lgslcblas -lm
