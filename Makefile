all: create_A sparse

create_A: create_A.c
	cc -o create_A create_A.c

sparse: sparse.c
	cc -o sparse sparse.c

makefileA:;
	./create_A
