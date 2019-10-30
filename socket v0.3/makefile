all:servidorteste

servidorteste:servidor.o
	gcc servidor.o -o servidor

servidor.o:servidor.cpp
	gcc -c servidor.cpp

clean:
	rm -f *.o servidor *
