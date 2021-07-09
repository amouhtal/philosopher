/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 11:50:21 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/09 18:11:55 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	check_if_sated(t_philo1 *philo)
{
	t_frame	*frame;

	frame = philo->frame;
	if (philo->one_meal == 0)
	{
		frame->already_eated++;
		philo->one_meal++;
	}
	if (frame->already_eated == frame->nbr_of_philo)
	{
		pthread_mutex_lock(&frame->print);
		write(1, "simulation done\n", 16);
		pthread_mutex_unlock(&frame->main);
	}
	return (1);
}

static void	*check_if_starving(void *arg)
{
	uint64_t	time;
	t_frame		*frame;
	t_philo1	*philo;

	philo = (t_philo1 *)arg;
	frame = philo->frame;
	philo->time_end = time_to_die(philo->frame->time_to_die);
	while (1)
	{
		time = get_time();
		if (time > philo->time_end)
		{
			pthread_mutex_lock(&frame->print);
			philo->timestamp = time - frame->start;
			printf("%llu died %d\n", philo->timestamp, philo->index);
			pthread_mutex_unlock(&frame->main);
		}
		if (frame->nbr_of_meal == philo->nbr_of_meal)
			check_if_sated(philo);
		usleep(1000);
	}
	return (NULL);
}

void	print_routine(t_philo1 *philo, char msg, int sleep)
{
	pthread_mutex_lock(&philo->frame->print);
	philo->timestamp = get_time() - philo->frame->start;
	if (msg == '0')
		printf("%llu\t%d\thas taken a fork\n", philo->timestamp, philo->index);
	else if (msg == '1')
		printf("%llu\t%d\tis eating\n", philo->timestamp, philo->index);
	else if (msg == '2')
		printf("%llu\t%d\tis sleeping\n", philo->timestamp, philo->index);
	else
		printf("%llu\t%d\tis thinking\n", philo->timestamp, philo->index);
	pthread_mutex_unlock(&philo->frame->print);
	if (sleep != NOT)
		usleep(sleep * 1000);
}

static void	*routine(void *arg)
{
	t_philo1	*philo;
	pthread_t	th;

	philo = (t_philo1 *)arg;
	pthread_create(&th, NULL, &check_if_starving, arg);
	pthread_detach(th);
	while (1)
	{
		lock_fork(philo->index - 1, philo->rfork, philo);
		ft_eat(philo);
		unlock_fork(philo->index - 1, philo->rfork, philo);
		print_routine(philo, '2', philo->frame->time_to_sleep);
		print_routine(philo, '3', NOT);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_frame		*frame;
	pthread_t	th;
	int			i;

	frame = NULL;
	if (!intial(&frame, ac, av))
		return (ft_free(&*frame, "Check arguments \n"));
	pthread_mutex_init(&frame->print, NULL);
	pthread_mutex_init(&frame->main, NULL);
	pthread_mutex_lock(&frame->main);
	frame->start = get_time();
	i = -1;
	while (++i < frame->nbr_of_philo)
	{
		if (pthread_create(&th, NULL, &routine, (void *)(&frame->philo[i])))
			return (ft_free(frame, "fail to create thread\n"));
		pthread_detach(th);
		usleep(100);
	}
	pthread_mutex_lock(&frame->main);
	return (ft_free(frame, NULL));
}
