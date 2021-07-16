/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 18:13:41 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/16 18:26:08 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	is_alph(char **av, int ac)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	if (ac < 5 || ac > 6)
		return (0);
	while (av[++j])
	{
		i = 0;
		while (av[j][i])
			if (av[j][i] < '0' || av[j][i++] > '9')
				return (0);
	}
	return (1);
}

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

static void	kill_process(t_frame *frame)
{
	int			i;
	pthread_t	th;

	i = 0;
	if (frame)
	{
		if (frame->nbr_of_meal != NOT)
		{
			pthread_create(&th, NULL, &check_if_sated, (void *)frame);
			pthread_detach(th);
		}
		sem_wait(frame->main);
		while (i < (frame)->nbr_of_philo)
			kill(frame->pids[i++], SIGKILL);
		sem_unlink(SEMAMAIN);
		sem_unlink(SEMAFORK);
		sem_unlink(SEMAPRINT);
	}
}

int	ft_end(t_frame *frame, char *msg)
{
	kill_process(frame);
	if (frame && frame->philo)
	{
		free(frame->philo);
		frame->philo = NULL;
	}
	if (frame && frame->pids)
		free(frame->pids);
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
