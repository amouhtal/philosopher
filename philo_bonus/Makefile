NAME = push_swap
SRCS = main.c util.c
OBJS = $(SRCS:.c=.o)


CC = gcc
FLAGS = -Wall -Wextra -Werror
%.o:%.c *.h
	$(cc) $(FLAGS) -c $< -o $@

all: *.h
	@$(MAKE) $(NAME) 
$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) 
clean:
	rm -rf *.o
fclean:clean
	rm -rf $(NAME)
re: fclean all