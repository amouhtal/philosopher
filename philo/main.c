/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 11:50:21 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/16 18:18:32 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*check_if_sated(void *arg)
{
	t_frame		*frame;
	t_philo1	*philo;

	philo = (t_philo1 *)arg;
	frame = philo->frame;
	while (!philo->one_meal)
	{
		if (frame->nbr_of_meal == philo->nbr_of_meal)
		{
			if (philo->one_meal == 0)
			{
				frame->already_eated++;
				philo->one_meal++;
			}
		}
		usleep(100);
	}
	return (NULL);
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
		pthread_mutex_lock(&philo->eat);
		if (time > philo->time_end
			&& (philo->frame->nbr_of_philo != philo->frame->already_eated))
		{
			pthread_mutex_lock(&frame->print);
			philo->timestamp = time - frame->start;
			printf("%llu\t%d\tdied\n", philo->timestamp, philo->index);
			pthread_mutex_unlock(&frame->main);
		}
		pthread_mutex_unlock(&philo->eat);
		usleep(500);
	}
	return (NULL);
}

void	print_routine(t_philo1 *philo, char msg, int sleep)
{
	pthread_mutex_lock(&philo->frame->print);
	philo->timestamp = get_time() - philo->frame->start;
	if (msg == '0')
		printf("%llu %d has taken a fork\n", philo->timestamp, philo->index);
	else if (msg == '1')
		printf("%llu %d is eating\n", philo->timestamp, philo->index);
	else if (msg == '2')
		printf("%llu %d is sleeping\n", philo->timestamp, philo->index);
	else
		printf("%llu %d is thinking\n", philo->timestamp, philo->index);
	if (philo->frame->nbr_of_philo == philo->frame->already_eated && msg == '1')
	{
		write(1, "simulation done\n", 16);
		pthread_mutex_unlock(&philo->frame->main);
		return ;
	}
	pthread_mutex_unlock(&philo->frame->print);
	if (sleep != NOT)
		usleep(sleep * 1000);
}

static void	*routine(void *arg)
{
	t_philo1	*philo;
	pthread_t	th;
	int			i;

	i = 0;
	philo = (t_philo1 *)arg;
	pthread_mutex_init(&philo->eat, NULL);
	pthread_create(&th, NULL, &check_if_starving, arg);
	pthread_detach(th);
	if (philo->frame->nbr_of_meal != NOT)
	{
		pthread_create(&th, NULL, check_if_sated, arg);
		pthread_detach(th);
	}
	while (1)
	{
		lock_fork(philo->index - 1, philo->rfork, philo);
		ft_eat(philo);
		unlock_fork(philo->index - 1, philo->rfork, philo);
		print_routine(philo, '2', philo->frame->time_to_sleep);
		print_routine(philo, '3', NOT);
		i++;
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
		if (pthread_create(&th, NULL, &routine,
				(void *)(&frame->philo[i])))
			return (ft_free(frame, "fail to create thread\n"));
		pthread_detach(th);
		usleep(100);
	}
	pthread_mutex_lock(&frame->main);
	return (ft_free(frame, NULL));
}
