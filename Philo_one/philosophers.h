
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_philo1
{
	int value;
	struct s_frame *frame;
	int time_of_thread;
	unsigned long time_end;

}	t_philo1;

typedef struct s_frame
{
	unsigned long start;
	int nbr_of_philo;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int *index;
	pthread_mutex_t *fork;
	t_philo1 *philo;
	pthread_mutex_t main;
	pthread_mutex_t print;
}t_frame;

int	ft_atoi(const char *str);

#endif