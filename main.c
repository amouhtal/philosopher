#include "philosophers.h"

void print(t_philo1 *philo)
{
	pthread_mutex_lock(&philo->frame->print);
	pthread_mutex_unlock(&philo->frame->print);
}

static void *routine(void *arg)
{
	int  i = 0;
	t_frame *frame;
	t_philo1 *philo;

	philo = (t_philo1*)arg;
	frame = philo->frame;

	while (1)
	{
		pthread_mutex_lock(&frame->fork[philo->value]);
		
		pthread_mutex_lock(&frame->print);
		printf("%d\tphilo has taken a fork \n", philo->value + 1);
		pthread_mutex_unlock(&frame->print);
		
		pthread_mutex_lock(&frame->fork[(philo->value + 1) % frame->nbr_of_philo]);

		pthread_mutex_lock(&frame->print);
		printf("%d\tphilo has taken the right fork \n", philo->value + 1);
		pthread_mutex_unlock(&frame->print);

		pthread_mutex_lock(&frame->print);
		printf("%d\tphilo is eating \n", philo->value + 1);
		pthread_mutex_unlock(&frame->print);
		usleep(frame->time_to_eat * 1000);
		// printf("philo %d is eating \n", *frame->index);
		// pthread_mutex_lock(frame->fork[9]);
		// printf("philo %d is sleeping \n", *frame->index);
		pthread_mutex_unlock(&frame->fork[philo->value]);
		pthread_mutex_unlock(&frame->fork[(philo->value + 1) % frame->nbr_of_philo]);
		
		pthread_mutex_lock(&frame->print);
		printf("%d\tphilo is sleeping \n", philo->value + 1);
		pthread_mutex_unlock(&frame->print);
		usleep(frame->time_to_sleep* 1000);
		
		pthread_mutex_lock(&frame->print);
		printf("%d\tphilo is thinking \n", philo->value + 1);
		pthread_mutex_unlock(&frame->print);

		// usleep(1000);
		// puts("here");
	}
   // pthread_mutex_unlock(&mutex);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_frame frame;
	pthread_t th;
	int	i;

	i = 0;
	if (ac < 5 || ac > 6)
	{
		perror("wrong numbers of arg");
		return (1);
	}
	frame.nbr_of_philo = ft_atoi(av[1]);
	frame.time_to_die = ft_atoi(av[2]);
	frame.time_to_eat = ft_atoi(av[3]);
	frame.time_to_sleep = ft_atoi(av[4]);
	frame.philo = malloc(sizeof(t_philo1) * frame.nbr_of_philo);
	frame.fork = malloc(sizeof(pthread_mutex_t) * frame.nbr_of_philo);
	i = 0;
	while (i < frame.nbr_of_philo)
	{
		pthread_mutex_init(&frame.fork[i],NULL);
		frame.philo[i].frame = &frame;
		frame.philo[i].value = i;
		i++;
	}
	pthread_mutex_init(&frame.print,NULL);
	pthread_mutex_init(&frame.main,NULL);
	pthread_mutex_lock(&frame.main);

	i = 0;
	while (i < frame.nbr_of_philo)
	{
		if (pthread_create(&th, NULL, &routine, (void*)(&frame.philo[i])) != 0)
		{
			perror("Failed to create thread");
			return 1;
		}
		pthread_detach(th);
		usleep(100);
		i++;
	}

	pthread_mutex_lock(&frame.main);
	
	// for (i = 0; i < 2; i++)
    //     if (pthread_join(th[i], NULL) != 0) 
    //         return 2;
	return (0);
}
