/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 11:29:32 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/11 14:18:39 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_alph(char **av)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (av[++j])
	{
		i = 0;
		while (av[j][i])
			if (av[j][i] < '0' || av[j][i++] > '9')
				return (0);
	}
	return (1);
}

void	mutex_destroy(t_frame *frame)
{
	int	i;

	i = 0;
	if (frame)
	{
		pthread_mutex_destroy(&frame->print);
		pthread_mutex_destroy(&frame->main);
		while (i < frame->nbr_of_philo)
			pthread_mutex_destroy(&frame->fork[i++]);
	}
}

int	ft_free(t_frame *frame, char *msg)
{
	mutex_destroy(frame);
	if (frame && frame->philo)
	{
		free(frame->philo);
		frame->philo = NULL;
	}
	if (frame && frame->fork)
		free(frame->fork);
	if (frame)
	{
		free(frame);
		frame = NULL;
	}
	if (msg)
	{
		printf("%s", msg);
		return (1);
	}
	return (0);
}

uint64_t	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

uint64_t	time_to_die(int appended_time)
{
	uint64_t	time;

	time = get_time();
	return (time + appended_time);
}
