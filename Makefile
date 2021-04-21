
FLAGS = -std=c11 -Wall -Werror -Wextra

output: main.o contato.o agenda.o
	gcc main.o contato.o agenda.o $(FLAGS) -o main

main.o: src/main.c
	gcc -c $(FLAGS) src/main.c

contato.o: src/contato.c src/contato.h
	gcc -c $(FLAGS) src/contato.c

agenda.o: src/agenda.c src/agenda.h
	gcc -c $(FLAGS) src/agenda.c

telefones.o: src/telefones.c src/telefones.h
	gcc -s $(FLAGS) src/telefones.c

clean:
	rm *.o main
