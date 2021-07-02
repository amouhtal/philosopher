# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/02 13:02:04 by amouhtal          #+#    #+#              #
#    Updated: 2021/07/02 14:57:28 by amouhtal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus
SRCS = main.c util.c
OBJS = $(SRCS:.c=.o)


CC = gcc
FLAGS = -Wall -Wextra -Werror
%.o:%.c *.h
	$(CC) $(FLAGS) -c $< -o $@

all: *.h
	@$(MAKE) $(NAME) 
$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) 
clean:
	rm -rf *.o
fclean:clean
	rm -rf $(NAME)
re: fclean all