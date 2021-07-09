/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 15:15:58 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/09 17:10:15 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	unlock_fork(int first_fork, int seconde_fork, t_philo1	*philo)
{
	pthread_mutex_unlock(&philo->frame->fork[first_fork]);
	pthread_mutex_unlock(&philo->frame->fork[seconde_fork]);
}

void	lock_fork(int first_fork, int seconde_fork, t_philo1	*philo)
{
	pthread_mutex_lock(&philo->frame->fork[first_fork]);
	print_routine(philo, '0', NOT);
	pthread_mutex_lock(&philo->frame->fork[seconde_fork]);
	print_routine(philo, '0', NOT);
}

void	ft_eat(t_philo1	*philo)
{
	philo->time_end = time_to_die(philo->frame->time_to_die);
	print_routine(philo, '1', philo->frame->time_to_eat);
	if (philo->frame->nbr_of_meal != NOT)
		philo->nbr_of_meal++;
}
