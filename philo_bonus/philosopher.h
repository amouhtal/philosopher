/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 14:02:56 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/09 12:44:43 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <semaphore.h>
# include <sys/time.h>
# include <signal.h>

# define SEMAPRINT "semamain"
# define SEMAFORK "semafork"
# define SEMAMAIN "semaprint"
# define SEMEATS "semeats"
# define NOT -1

int				g_already_eated;
typedef struct s_philo
{
	int				index;
	int				nbr_of_meal;
	int				one_meal;
	struct s_frame	*frame;
	uint64_t		time_end;
	uint64_t		timestamp;
}	t_philo;

typedef struct s_frame
{
	t_philo			*philo;
	pthread_mutex_t	*fork;
	uint64_t		start;
	uint64_t		time_of_thread;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				*pids;
	int				nbr_of_meal;
	int				nbr_of_philo;
	pid_t			pid;
	sem_t			*forks;
	sem_t			*main;
	sem_t			*print;
	sem_t			*nbr_to_eats;
}	t_frame;

int			ft_atoi(const char *str);
int			ft_free(t_frame *frame, char *msg);
uint64_t	time_to_die(int appended_time);
uint64_t	get_time(void);
t_frame		*init_frame(t_frame **frame, int ac, char **av);
void		ft_putnbr_fd(uint64_t n, int fd);
int			is_alph(char **av, int ac);
#endif
