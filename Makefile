
CC = gcc
ERRORS = -Wall -Werror -Wextra
FLAGS = -std=c11 $(ERRORS) src/main.c src/agenda.c src/contato.c src/telefones.c

output: src/main.c src/agenda.c src/agenda.h src/contato.c src/contato.h src/telefones.c src/telefones.h
	$(CC) -g $(FLAGS) -o main

clean:
	rm main
