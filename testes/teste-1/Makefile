all:servidorteste

servidorteste:servidor.o
	gcc servidor.o -o servidor

servidor.o:servidor.c
	gcc -c servidor.c

clean:
	rm -f *.o servidor *
