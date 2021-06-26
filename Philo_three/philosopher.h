/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 14:02:56 by amouhtal          #+#    #+#             */
/*   Updated: 2021/06/25 12:15:03 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <semaphore.h>
#include <sys/time.h>
# define SEMAPRINT "semamain"
# define SEMAFORK "semafork"
# define SEMAMAIN "semaprint"


typedef struct s_philo
{
	int value;
	unsigned long	time_end;
	struct s_frame *frame;
	int nbr_of_meal;
}t_philo;

typedef struct s_frame
{
	t_philo *philo;
	int		*pids;
	unsigned long start;
	unsigned long time_of_thread;
	int nbr_of_philo;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int nbr_of_meal;
	pid_t pid;
	sem_t *forks;
	sem_t *main;
	sem_t *print;
	pthread_mutex_t *fork;
}t_frame;

int						ft_atoi(const char *str);
int						ft_free(t_frame **frame, char *msg);
#endif