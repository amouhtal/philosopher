CC = gcc
FLAGS = -Wall -Wextra -Werror
SRCS = main.c util.c
OBJ = $(SRCS=*.o)
%.o=%.c
	$(CC) $(FLAGS) -c $(SRCS) -o 

all:

clean :
	rm -rf *.c
fclean : clean
	rm rf *.o