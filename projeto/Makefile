all: argus argusd

argus: argus.o
	gcc -Wall argus.o -o argus

argus.o: argus.c
	gcc -c argus.c $ ./argus.h

argusd: argusd.o estrutura.o
	gcc -Wall argusd.o estrutura.o -o argusd

argusd.o: argusd.c 
	gcc -c argusd.c $ ./estrutura.h $ ./argus.h

estrutura.o: estrutura.c
	gcc -c estrutura.c

clean:	
	rm argus argusd
	rm *.o *.txt

	


