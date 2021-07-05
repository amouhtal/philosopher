/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 14:14:12 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/05 17:18:58 by amouhtal         ###   ########.fr       */
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

uint64_t	time_to_die(int appended_time)
{
	uint64_t	time;

	time = get_time();
	return (time + appended_time);
}

void	kill_process(t_frame *frame)
{
	int	i;

	sem_wait((frame)->main);
	i = 0;
	while (i < (frame)->nbr_of_philo)
		kill(frame->pids[i++], SIGKILL);
	sem_destroy(frame->main);
	sem_destroy(frame->print);
	sem_destroy(frame->forks);

}

int	ft_free(t_frame *frame, char *msg)
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

t_frame	*init_philo(t_frame *frame)
{
	int	i;

	i = 0;
	while (i < frame->nbr_of_philo)
	{
		frame->philo[i].frame = frame;
		frame->philo[i].value = i;
		frame->philo[i].one_meal = 0;
		frame->philo[i].nbr_of_meal = 0;
		i++;
	}
	sem_unlink(SEMAMAIN);
	sem_unlink(SEMAFORK);
	sem_unlink(SEMAPRINT);
	return (frame);
}

t_frame	*init_frame(t_frame **frame, int ac, char **av)
{
	*frame = (t_frame *)malloc(sizeof(t_frame));
	if (!*frame)
		return (NULL);
	if (ac < 5 || ac > 6)
	{
		printf("wrong numbers of arg\n");
		return (NULL);
	}
	(*frame)->nbr_of_philo = ft_atoi(av[1]);
	(*frame)->time_to_die = ft_atoi(av[2]);
	(*frame)->time_to_eat = ft_atoi(av[3]);
	(*frame)->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		(*frame)->nbr_of_meal = ft_atoi(av[5]);
	(*frame)->already_eated = 0;
	(*frame)->nbr_of_meal = -1;
	(*frame)->philo = malloc(sizeof(t_philo) * (*frame)->nbr_of_philo);
	if (!(*frame)->philo)
		return (NULL);
	(*frame) = init_philo((*frame));
	if (!(*frame))
		return (NULL);
	return ((*frame));
}
