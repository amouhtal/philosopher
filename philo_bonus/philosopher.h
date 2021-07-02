/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 14:02:56 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/02 17:43:47 by amouhtal         ###   ########.fr       */
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
# define NOT -1

typedef struct s_philo
{
	int				value;
	uint64_t		time_end;
	struct s_frame	*frame;
	uint64_t		timestamp;
	int				nbr_of_meal;
	int				time_of_thread;
}	t_philo;

typedef struct s_frame
{
	t_philo			*philo;
	int				*pids;
	uint64_t		start;
	uint64_t		time_of_thread;
	int				nbr_of_philo;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	int				nbr_of_meal;
	pid_t			pid;
	sem_t			*forks;
	sem_t			*main;
	sem_t			*print;
	pthread_mutex_t	*fork;
}	t_frame;

int			ft_atoi(const char *str);
int			ft_free(t_frame *frame, char *msg);
uint64_t	time_to_die(int appended_time);
uint64_t	get_time(void);
#endif
