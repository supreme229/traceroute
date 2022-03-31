CPP = gcc
FLAGS = -std=gnu17 -Wall -Wextra -pedantic

main: Makefile main.o receive.o send.o tools.o
		$(CPP) $(FLAGS) -o main main.o receive.o send.o tools.o

main.o:  Makefile main.c
		$(CPP) $(FLAGS) -c main.c -o main.o

receive.o: Makefile receive.c
		$(CPP) $(FLAGS) -c receive.c -o receive.o

send.o: Makefile send.c
		$(CPP) $(FLAGS) -c send.c -o send.o

tools.o: Makefile tools.c
		$(CPP) $(FLAGS) -c tools.c -o tools.o

clean:
	rm -rf *.o 

distclean:
	rm -rf main *.o