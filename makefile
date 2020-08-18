CC = gcc

#all : qgen cgen
all : qgen

qgen: qgen.o gene.o utils.o hashmap.o
	$(CC)  -Werror -Wall -g -o qgen qgen.o gene.o utils.o hashmap.o

cgen: cgen.o qgen.o gene.o utils.o hashmap.o
	$(CC)  -Werror -Wall -g -o cgen cgen.o gene.o utils.o hashmap.o

cgen.o: cgen.c gene.o utils.o hashmap.o
	$(CC) -c -o cgen.o cgen.c

qgen.o: qgen.c gene.o utils.o hashmap.o
	$(CC) -c -o qgen.o qgen.c

gene.o: gene.c utils.o hashmap.o
	$(CC) -c -o gene.o gene.c

utils.o: utils.c
	$(CC) -c -o utils.o utils.c

hashmap.o: hashmap.c
	$(CC) -c -o hashmap.o hashmap.c

clean:
	rm qgen.o hashmap.o gene.o utils.o qgen cgen
