#include "philosophers.h"

unsigned long get_time()
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);

	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void *chekin_nugget(void *arg)
{
	unsigned long time;
	t_frame *frame;
	t_philo1 *philo;

	philo = (t_philo1 *)arg;
	frame = philo->frame;
	while (1)
	{
		if((time = get_time()) > philo->time_end)
		{
			pthread_mutex_lock(&philo->frame->print);
			printf("philo %d time : %lu timeofdie : %lu\n", philo->value, time, philo->time_end);
			pthread_mutex_unlock(&philo->frame->main);
		}
	}
	return (NULL);
}
// 1624017119226  1624017119421
static void *routine(void *arg)
{
	int  i = 0;
	pthread_t th;
	t_frame *frame;
	t_philo1 *philo;
	unsigned long time;
	struct timeval current_time;
	philo = (t_philo1*)arg;
	frame = philo->frame;
	time = get_time();
	philo->time_end = time + frame->time_to_die;
	pthread_create(&th, NULL, &chekin_nugget, arg);
	usleep(100);
	pthread_detach(th);
	while (1)
	{
		////////////////////////// First fork//////////////////////////////
		pthread_mutex_lock(&frame->fork[philo->value]);

		gettimeofday(&current_time, NULL);
		philo->time_of_thread = get_time() - frame->start;
		pthread_mutex_lock(&frame->print);
		printf("%d %d\tphilo has taken first fork \n",  philo->time_of_thread, philo->value + 1);
		pthread_mutex_unlock(&frame->print);

		////////////////////////// Second fork//////////////////////////////
		pthread_mutex_lock(&frame->fork[(philo->value + 1) % frame->nbr_of_philo]);

		gettimeofday(&current_time, NULL);
		philo->time_of_thread = get_time() - frame->start;
		pthread_mutex_lock(&frame->print);
		printf("%d %d\tphilo has taken seconde fork \n", philo->time_of_thread, philo->value + 1);
		pthread_mutex_unlock(&frame->print);

		philo->time_of_thread = get_time() - frame->start;
		//time = get_time();
		philo->time_end = (time = get_time()) + frame->time_to_die;
		// printf("\t\t\t\033[0;33m %lu \033[0;32m %lu \n",time, philo->time_end);
		pthread_mutex_lock(&frame->print);
		printf("%d %d\tphilo is eating \n", philo->time_of_thread, philo->value + 1);
		pthread_mutex_unlock(&frame->print);
		usleep(frame->time_to_eat * 1000);

		pthread_mutex_unlock(&frame->fork[philo->value]);
		pthread_mutex_unlock(&frame->fork[(philo->value + 1) % frame->nbr_of_philo]);
		
		////////////////////////// sleep //////////////////////////////
		gettimeofday(&current_time, NULL);
		philo->time_of_thread = get_time() - frame->start;
		pthread_mutex_lock(&frame->print);
		printf("%d %d\tphilo is sleeping \n",philo->time_of_thread, philo->value + 1);
		pthread_mutex_unlock(&frame->print);
		usleep(frame->time_to_sleep * 1000);

		////////////////////////// thinkin fork//////////////////////////////
		gettimeofday(&current_time, NULL);
		philo->time_of_thread = get_time() - frame->start;
		pthread_mutex_lock(&frame->print);
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
