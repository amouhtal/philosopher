#include "philosophers.h"

static unsigned long	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

static void	*chekin_nugget(void *arg)
{
	unsigned long	time;
	t_frame			*frame;
	t_philo1		*philo;

	philo = (t_philo1 *)arg;
	frame = philo->frame;
	while (1)
	{
		time = get_time();
		if (time > philo->time_end)
		{
			pthread_mutex_lock(&frame->print);
			printf("philo %d time : %lu timeofdie : %lu\n",
				philo->value, time, philo->time_end);
			pthread_mutex_unlock(&frame->main);
		}
		usleep(100);
	}
	return (NULL);
}

static void	print_(t_philo1 *philo, char *msg)
{
	t_frame	*frame;

	frame = philo->frame;
	philo->time_of_thread = get_time() - frame->start;
	pthread_mutex_lock(&frame->print);
	printf("%d %d\t%s \n", philo->time_of_thread, philo->value + 1, msg);
	pthread_mutex_unlock(&frame->print);
}
//1066337

static void	*routine(void *arg)
{
	pthread_t		th;
	t_frame			*frame;
	t_philo1		*philo;
	unsigned long	time;

	philo = (t_philo1 *)arg;
	frame = philo->frame;
	time = get_time();
	philo->time_end = time + frame->time_to_die;
	pthread_create(&th, NULL, &chekin_nugget, arg);
	pthread_detach(th);
	while (1)
	{
		pthread_mutex_lock(&frame->fork[philo->value]);
		print_(philo, "philo has taken first fork");
		pthread_mutex_lock(&frame->fork[(philo->value + 1)
			% frame->nbr_of_philo]);
		print_(philo, "philo has taken second fork");
		philo->time_end = (time = get_time()) + frame->time_to_die;
		print_(philo, "philo is eating");
		usleep(frame->time_to_eat * 1000);
		pthread_mutex_unlock(&frame->fork[philo->value]);
		pthread_mutex_unlock(&frame->fork[(philo->value + 1)
			% frame->nbr_of_philo]);
		print_(philo, "philo is sleeping");
		usleep(frame->time_to_sleep * 1000);
		print_(philo, "philo is thinking");
	}
	return (NULL);
}

int	main(int	ac,	char **av)
{
	t_frame		*frame;
	pthread_t	th;
	int			i;

	i = 0;
	frame = (t_frame *) malloc(sizeof(t_frame));
	if (ac < 5 || ac > 6)
	{
		perror("wrong numbers of arg");
		return (1);
	}
	frame->nbr_of_philo = ft_atoi(av[1]);
	frame->time_to_die = ft_atoi(av[2]);
	frame->time_to_eat = ft_atoi(av[3]);
	frame->time_to_sleep = ft_atoi(av[4]);
	frame->philo = malloc(sizeof(t_philo1) * frame->nbr_of_philo);
	frame->fork = malloc(sizeof(pthread_mutex_t) * frame->nbr_of_philo);
	i = 0;
	while (i < frame->nbr_of_philo)
	{
		pthread_mutex_init(&frame->fork[i], NULL);
		frame->philo[i].frame = frame;
		frame->philo[i].value = i;
		i++;
	}
	pthread_mutex_init(&frame->print, NULL);
	pthread_mutex_init(&frame->main, NULL);
	pthread_mutex_lock(&frame->main);
	i = 0;
	frame->start = get_time();
	while (i < frame->nbr_of_philo)
	{
		if (pthread_create(&th, NULL, &routine,
				(void *)(&frame->philo[i])) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
		pthread_detach(th);
		usleep(100);
		i++;
	}
	pthread_mutex_lock(&frame->main);
	free(frame->philo);
	free(frame->fork);
	free(frame);
	return (0);
}
