/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 14:14:12 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/02 18:50:09 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

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

uint64_t time_to_die(int appended_time)
{
	uint64_t time;

	time = get_time();
	return (time + appended_time);
}
void	kill_process(t_frame *frame)
{
	int i;

	sem_wait((frame)->main);
	i = 0;
	while (i < (frame)->nbr_of_philo)
		kill(frame->pids[i++], SIGKILL);
}

int	ft_free(t_frame *frame, char *msg)
{
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
		return 1;
	}
	kill_process(frame);
	return (0);	
} 
