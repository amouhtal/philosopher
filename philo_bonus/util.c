/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 14:14:12 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/16 17:33:02 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

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

t_frame	*init_philo(t_frame *frame)
{
	int		i;
	char	*halfpart;
	char	*part;

	i = 0;
	while (i < frame->nbr_of_philo)
	{
		halfpart = ft_itoa(i);
		part = ft_strjoin("sem", halfpart);
		sem_unlink(part);
		frame->philo[i].is_eating = sem_open(part, O_CREAT, 0644, 1);
		frame->philo[i].frame = frame;
		frame->philo[i].index = i;
		frame->philo[i].one_meal = 0;
		frame->philo[i].nbr_of_meal = 0;
		i++;
		free(halfpart);
		free(part);
	}
	sem_unlink(SEMAMAIN);
	sem_unlink(SEMAFORK);
	sem_unlink(SEMAPRINT);
	sem_unlink(SEMEATS);
	return (frame);
}

t_frame	*init_frame(t_frame **frame, int ac, char **av)
{
	*frame = (t_frame *)malloc(sizeof(t_frame));
	if (!*frame)
		return (NULL);
	if (!is_alph(av, ac))
		return (0);
	(*frame)->nbr_of_philo = ft_atoi(av[1]);
	(*frame)->time_to_die = ft_atoi(av[2]);
	(*frame)->time_to_eat = ft_atoi(av[3]);
	(*frame)->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
	{
		(*frame)->nbr_of_meal = ft_atoi(av[5]);
		if ((*frame)->nbr_of_meal < 0)
			return (NULL);
	}
	else
		(*frame)->nbr_of_meal = -1;
	if ((*frame)->nbr_of_philo <= 0 || (*frame)->time_to_die < 0 \
		|| (*frame)->time_to_eat < 0 || (*frame)->time_to_sleep < 0)
		return (NULL);
	(*frame)->already_eated = 0;
	(*frame)->philo = malloc(sizeof(t_philo) * (*frame)->nbr_of_philo);
	if (!(*frame)->philo)
		return (NULL);
	return (init_philo((*frame)));
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
