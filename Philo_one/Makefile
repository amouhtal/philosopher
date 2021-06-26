NAME = philo
CC = gcc
FLAGS = -Wall -Wextra -Werror
SRCS = main.c util.c
OBJS = $(SRCS:.c=.o)

%.o:%.c *.h
	$(CC) $(FLAGS) -c $< -o $@ 

all: *.h
	@$(MAKE) $(NAME)
$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean :
	rm -rf *.o
fclean : clean
	rm -rf $(NAME)
re: fclean all