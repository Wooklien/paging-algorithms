#make file for A2

EXEC = a2
TORM = A2.o
CC = g++
CCFLAGS = -g -O3 -Wall -Wpointer-arith -Wcast-qual -Wwrite-strings 

$(EXEC):	A2.o 
	$(CC) ${CCFLAGS} -o $(EXEC) A2.o

A2.o:	A2.cpp A2.h
		${CC} ${CCFLAGS} -c A2.cpp             

clean:
	rm -f *.o core a.out $(EXEC) ${TORM}
