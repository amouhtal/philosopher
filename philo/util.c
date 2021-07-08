/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 15:01:03 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/08 15:41:00 by amouhtal         ###   ########.fr       */
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

uint64_t	time_to_die(int appended_time)
{
	uint64_t	time;

	time = get_time();
	return (time + appended_time);
}

static t_frame	*mutex_init(t_frame *frame)
{
	int	i;

	i = 0;
	frame->already_eated = 0;
	while (i < frame->nbr_of_philo)
	{
		if (pthread_mutex_init(&frame->fork[i], NULL))
			return (NULL);
		frame->philo[i].frame = frame;
		frame->philo[i].index = i + 1;
		frame->philo[i].one_meal = 0;
		frame->philo[i].nbr_of_meal = 0;
		frame->philo[i].rfork = (frame->philo[i].index)
			% frame->nbr_of_philo;
		i++;
	}
	return (frame);
}

t_frame	*intial(t_frame **frame, int ac, char **av)
{
	*frame = (t_frame *)malloc(sizeof(t_frame));
	if (!(*frame))
		return (NULL);
	if (ac < 5 || ac > 6)
		return (NULL);
	if (!ft_init_arg((frame), av))
		return (NULL);
	(*frame)->philo = malloc(sizeof(t_philo1) * (*frame)->nbr_of_philo);
	if (!(*frame)->philo)
		return (NULL);
	(*frame)->fork = malloc(sizeof(pthread_mutex_t) * (*frame)->nbr_of_philo);
	if (!(*frame)->fork)
		return (NULL);
	(*frame) = mutex_init((*frame));
	if (!(*frame))
		return (NULL);
	return ((*frame));
}
