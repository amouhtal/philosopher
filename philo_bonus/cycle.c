/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/16 12:23:27 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/16 18:28:57 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	ft_take_fork(t_philo *philo)
{
	sem_wait(philo->frame->forks);
	print_routine(philo, '0', NOT);
	sem_wait(philo->frame->forks);
	print_routine(philo, '0', NOT);
}

void	ft_eat(t_philo *philo)
{
	sem_wait(philo->is_eating);
	if (philo->frame->nbr_of_meal != NOT)
		philo->nbr_of_meal++;
	if (philo->frame->nbr_of_meal == philo->nbr_of_meal
		|| philo->frame->nbr_of_meal == 0)
	{
		if (philo->frame->already_eated == 0)
			sem_post(philo->frame->nbr_to_eat);
		philo->frame->already_eated++;
	}
	philo->time_end = time_to_die(philo->frame->time_to_die);
	print_routine(philo, '1', philo->frame->time_to_eat);
	sem_post(philo->is_eating);
}

void	ft_release_fork(t_frame *frame)
{
	sem_post(frame->forks);
	sem_post(frame->forks);
}
