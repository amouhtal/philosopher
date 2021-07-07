/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 12:00:06 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/06 13:01:07 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <errno.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

# define NOT -1

typedef struct s_philo1
{
	struct s_frame	*frame;
	uint64_t		timestamp;
	uint64_t		time_end;
	int				nbr_of_meal;
	int				rfork;
	int				index;
	int				one_meal;
}	t_philo1;

typedef struct s_frame
{
	uint64_t		start;
	int				nbr_of_philo;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	uint64_t		nbr_of_cycle;
	int				nbr_of_meal;
	int				*index;
	pthread_mutex_t	*fork;
	t_philo1		*philo;
	pthread_mutex_t	main;
	pthread_mutex_t	print;
	int				already_eated;
}					t_frame;

int						ft_atoi(const char *str);
int						ft_free(t_frame *frame, char *msg);
t_frame					*mutex_init(t_frame *frame);
t_frame					*intial(t_frame **frame, int ac, char **av);
uint64_t				time_to_die(int appended_time);
uint64_t				get_time(void);
int						ft_init_arg(t_frame **frame, char **av);
void					ft_putnbr_fd(uint64_t n, int fd);
#endif
