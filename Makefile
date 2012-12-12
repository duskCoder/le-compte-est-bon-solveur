CC      = gcc
CFLAGS  = -Wall -W -Wextra -std=c99 -g
NAME    = solver

$(NAME): .solver.o .main.o
	$(CC) -o $@ $^ $(LDFLAGS)

.solver.o: solver.c solver.h
	$(CC) -o $@ -c $< $(CFLAGS)

.main.o: main.c solver.h
	$(CC) -o $@ -c $< $(CFLAGS)

