#include "philosophers.h"

unsigned long get_time()
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);

	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}
void print(t_philo1 *philo)
{
	pthread_mutex_lock(&philo->frame->print);
	pthread_mutex_unlock(&philo->frame->print);
}

void *chekin_nugget(void *arg)
{
	unsigned long time;
	unsigned long time_end;

	t_philo1 *philo;

	philo = (t_philo1 *)arg;
	time = get_time();
	time_end=time + 4000;
	while (time < time_end)
	{
		time = get_time();
	}
	pthread_mutex_lock(&philo->frame->print);
	printf("Fin\n");
	pthread_mutex_unlock(&philo->frame->main);

}



static void *routine(void *arg)
{
	int  i = 0;
	pthread_t th;
	t_frame *frame;
	t_philo1 *philo;
	struct timeval current_time;
	philo = (t_philo1*)arg;
	frame = philo->frame;

	pthread_create(&th, NULL, &chekin_nugget, arg);
	pthread_detach(th);
	while (1)
	{
		pthread_mutex_lock(&frame->fork[philo->value]);

		pthread_mutex_lock(&frame->print);
		gettimeofday(&current_time, NULL);
		philo->time_of_thread = get_time() - frame->start;
		printf("%d %d\tphilo has taken a fork \n",  philo->time_of_thread, philo->value + 1);
		pthread_mutex_unlock(&frame->print);
		
		pthread_mutex_lock(&frame->fork[(philo->value + 1) % frame->nbr_of_philo]);

		pthread_mutex_lock(&frame->print);
		gettimeofday(&current_time, NULL);
		philo->time_of_thread = get_time() - frame->start;
		printf("%d %d\tphilo has taken the right fork \n", philo->time_of_thread, philo->value + 1);
		pthread_mutex_unlock(&frame->print);

		pthread_mutex_lock(&frame->print);
		philo->time_of_thread = get_time() - frame->start;
		printf("%d %d\tphilo is eating \n", philo->time_of_thread, philo->value + 1);
		pthread_mutex_unlock(&frame->print);
		usleep(frame->time_to_eat * 1000);
		// printf("philo %d is eating \n", *frame->index);
		// pthread_mutex_lock(frame->fork[9]);
		// printf("philo %d is sleeping \n", *frame->index);
		pthread_mutex_unlock(&frame->fork[philo->value]);
		pthread_mutex_unlock(&frame->fork[(philo->value + 1) % frame->nbr_of_philo]);
		
		pthread_mutex_lock(&frame->print);
		gettimeofday(&current_time, NULL);
		philo->time_of_thread = get_time() - frame->start;
		printf("%d %d\tphilo is sleeping \n",philo->time_of_thread, philo->value + 1);
		pthread_mutex_unlock(&frame->print);
		usleep(frame->time_to_sleep* 1000);
		
		pthread_mutex_lock(&frame->print);
		gettimeofday(&current_time, NULL);
		philo->time_of_thread = get_time() - frame->start;
		printf("%d %d\tphilo is thinking \n",philo->time_of_thread, philo->value + 1);
		pthread_mutex_unlock(&frame->print);
	}
	return (NULL);
}

//function get_time() --> return time in milisecond
int	main(int ac, char **av)
{
	t_frame frame;
	pthread_t th;
	struct timeval current_time;
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
	frame.start = get_time();
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
	return (0);
}
