
CC = gcc
ERRORS = -Wall -Werror -Wextra -Wno-unused-parameter
FLAGS = -std=c11 $(ERRORS) src/main.c src/agenda.c src/contato.c src/telefones.c
CURSES = -lncurses -lpanel -lmenu -lform

output: src/main.c src/agenda.c src/agenda.h src/contato.c src/contato.h src/telefones.c src/telefones.h
	$(CC) -g $(FLAGS) $(CURSES) -o main

clean:
	rm main
