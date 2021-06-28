/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 12:00:06 by amouhtal          #+#    #+#             */
/*   Updated: 2021/06/28 15:29:48 by amouhtal         ###   ########.fr       */
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


typedef struct s_philo1
{
	int				value;
	struct s_frame	*frame;
	int				timestamp;
	unsigned long	time_end;
	int				nbr_of_meal;
	int				lfork;
	int				rfork;
}	t_philo1;

typedef struct s_frame
{
	unsigned long	start;
	int				nbr_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_of_cycle;
	int				nbr_of_meal;
	int				*index;
	pthread_mutex_t	*fork;
	t_philo1		*philo;
	pthread_mutex_t	main;
	pthread_mutex_t	print;
}					t_frame;

int						ft_atoi(const char *str);
int						ft_free(t_frame **frame, char *msg);
unsigned long			get_time(void);
t_frame					*mutex_init(t_frame *frame);
#endif
