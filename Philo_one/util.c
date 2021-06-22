/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 15:01:03 by amouhtal          #+#    #+#             */
/*   Updated: 2021/06/22 15:01:04 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	int			i;
	long long	res;
	int			sign;

	i = 0;
	res = 0;
	sign = 1;
	while ((str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || str[i] == '\r'
			|| str[i] == '\f' || str[i] == '\v') && str[i])
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9' && str[i])
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

int	ft_free(t_frame **frame, char *msg)
{
	if ((*frame)->philo)
	{
		free((*frame)->philo);
		(*frame)->philo = NULL;
	}
	if ((*frame)->fork)
	{
		free((*frame)->fork);
		(*frame)->fork = NULL;
	}
	if ((*frame))
	{
		free((*frame));
		(*frame) = NULL;
	}
	if (msg)
		printf("%s", msg);
	return (1);	
}

unsigned long	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

t_frame	*mutex_init(t_frame *frame)
{
	int	i;

	i = 0;
	while (i < frame->nbr_of_philo)
	{
		if (pthread_mutex_init(&frame->fork[i], NULL))
			return (NULL);
		frame->philo[i].frame = frame;
		frame->philo[i].value = i;
		frame->philo[i].nbr_of_meal = 0;
		frame->philo[i].rfork = (frame->philo[i].value + 1) % frame->nbr_of_philo;
		frame->philo[i].lfork = (frame->philo[i].value + 1) % frame->nbr_of_philo;
		i++;
	}
	return (frame);
}
