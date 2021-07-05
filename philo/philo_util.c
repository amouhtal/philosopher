/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 11:29:32 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/05 18:07:50 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_init_arg(t_frame **frame, char **av)
{
	(*frame)->nbr_of_philo = ft_atoi(av[1]);
	(*frame)->time_to_die = ft_atoi(av[2]);
	(*frame)->time_to_eat = ft_atoi(av[3]);
	(*frame)->time_to_sleep = ft_atoi(av[4]);
	(*frame)->nbr_of_meal = -1;
	if (av[5])
		(*frame)->nbr_of_meal = ft_atoi(av[5]);
	if ((*frame)->nbr_of_philo <= 0 || (*frame)->time_to_die < 0 \
		|| (*frame)->time_to_eat < 0 || (*frame)->time_to_sleep < 0)
	{
		printf("check arguments\n");
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
	{
		free(frame->fork);
		frame->fork = NULL;
	}	
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

void	ft_putnbr_fd(uint64_t n, int fd)
{
	char	str[13];
	int		length;

	if (n == 0)
		str[0] = '0';
	length = 0;
	while (n != 0)
	{
		str[length++] = '0' + (n % 10);
		n = (n / 10);
	}
	if (length > 0)
		length--;
	while (length >= 0)
		write(fd, &str[length--], 1);
}

uint64_t	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}
