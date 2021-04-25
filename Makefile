
FLAGS = -std=c11 -Wall -Werror -Wextra

output: main.o agenda.o contato.o telefones.o
	gcc $(FLAGS) main.o agenda.o contato.o telefones.o -o main

agenda.o: src/agenda.c src/agenda.h 
	gcc -c $(FLAGS) src/agenda.c -o agenda.o

contato.o: src/contato.c src/contato.h
	gcc -c $(FLAGS) src/contato.c -o contato.o

telefones.o: src/telefones.c src/telefones.h
	gcc -c $(FLAGS) src/telefones.c -o telefones.o

main.o: src/main.c
	gcc -c $(FLAGS) src/main.c

clean:
	rm *.o main
