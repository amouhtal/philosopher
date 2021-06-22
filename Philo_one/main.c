/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 11:50:21 by amouhtal          #+#    #+#             */
/*   Updated: 2021/06/22 14:54:15 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
		if (frame->nbr_of_meal == philo->nbr_of_meal)
		{
			pthread_mutex_lock(&frame->print);
			printf("similation done\n");
			pthread_mutex_unlock(&frame->main);
		}
		usleep(100);
	}
	return (NULL);
}

static void	print_routine(t_philo1 *philo, char *msg, int sleep)
{
	t_frame	*frame;

	frame = philo->frame;
	philo->time_of_thread = get_time() - frame->start;
	pthread_mutex_lock(&frame->print);
	printf("%d\t%d\t%s \n", philo->time_of_thread, philo->value + 1, msg);
	pthread_mutex_unlock(&frame->print);
	if (sleep != -1)
		usleep(sleep * 1000);
}

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
	while (1)
	{
		pthread_mutex_lock(&frame->fork[philo->value]);
		print_routine(philo, "philo has taken first fork", -1);
		pthread_mutex_lock(&frame->fork[philo->lfork]);
		print_routine(philo, "philo has taken second fork", -1);
		philo->time_end = (time = get_time()) + frame->time_to_die;
		philo->nbr_of_meal++;
		print_routine(philo, "philo is eating", frame->time_to_eat);
		pthread_mutex_unlock(&frame->fork[philo->value]);
		pthread_mutex_unlock(&frame->fork[philo->rfork]);
		print_routine(philo, "philo is sleeping", frame->time_to_sleep);
		print_routine(philo, "philo is thinking", 1);
	}
	return (NULL);
}

t_frame	*intial(t_frame *frame, int ac, char **av)
{
	frame = (t_frame *) malloc(sizeof(t_frame));
	if (!frame)
		return (NULL);
	if (ac < 5 || ac > 6)
	{
		printf("wrong numbers of arg\n");
		return (NULL);
	}
	frame->nbr_of_philo = ft_atoi(av[1]);
	frame->time_to_die = ft_atoi(av[2]);
	frame->time_to_eat = ft_atoi(av[3]);
	frame->time_to_sleep = ft_atoi(av[4]);
	frame->nbr_of_meal = -1;
	if (av[5])
		frame->nbr_of_meal = ft_atoi(av[5]);
	frame->philo = malloc(sizeof(t_philo1) * frame->nbr_of_philo);
	if (!frame->philo)
		return (NULL);
	frame->fork = malloc(sizeof(pthread_mutex_t) * frame->nbr_of_philo);
	if (!frame->fork)
		return (NULL);
	frame = mutex_init(frame);
	if (!frame)
		return (NULL);
	return (frame);
}

int	main(int	ac,	char **av)
{
	t_frame		*frame;
	pthread_t	th;
	int			i;

	frame = NULL;
	frame = intial(frame, ac, av);
	if (!frame)
		return (ft_free(&frame, NULL));
	pthread_mutex_init(&frame->print, NULL);
	pthread_mutex_init(&frame->main, NULL);
	pthread_mutex_lock(&frame->main);
	frame->start = get_time();
	i = 0;
	while (i < frame->nbr_of_philo)
	{
		if (pthread_create(&th, NULL, &routine, (void *)(&frame->philo[i++])))
			return (ft_free(&frame, "fail to crate threads\n"));
		pthread_detach(th);
		usleep(100);
	}
	pthread_mutex_lock(&frame->main);
	return (ft_free(&frame, NULL));
}
