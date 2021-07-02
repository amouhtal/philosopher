/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 11:50:21 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/02 19:02:48 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

uint64_t	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

static void *check_if_starving(void *arg)
{
	uint64_t time;
	t_frame *frame;
	t_philo1 *philo;

	philo = (t_philo1 *)arg;
	frame = philo->frame;
	while (1)
	{
		time = get_time();
		if (time > philo->time_end)
		{
			philo->timestamp = time - frame->start;
			pthread_mutex_lock(&frame->print);
			printf("%llu died %d\n", philo->timestamp, philo->value + 1);
			pthread_mutex_unlock(&frame->main);
		}
		if (frame->nbr_of_meal == philo->nbr_of_meal)
		{
			if(philo->i == 0)
			{
				frame->already_eated++;
				philo->i++;
			}
			if (frame->already_eated == frame->nbr_of_philo)
			{
				pthread_mutex_lock(&frame->print);
				printf("similation done\n");
				pthread_mutex_unlock(&frame->main);
			}		
		}
		usleep(1000);
	}
	return (NULL);
}

static void print_routine(t_philo1 *philo, char *msg, int sleep)
{
	t_frame *frame;

	frame = philo->frame;
	pthread_mutex_lock(&frame->print);
	philo->timestamp = get_time() - frame->start;
	printf("%llu\t%d\t%s \n", philo->timestamp, philo->value + 1, msg);
	pthread_mutex_unlock(&frame->print);
	if (sleep != NOT)
		usleep(sleep * 1000);
}

static void *routine(void *arg)
{
	pthread_t th;
	t_frame *frame;
	t_philo1 *philo;

	philo = (t_philo1 *)arg;
	frame = philo->frame;
	philo->time_end = time_to_die(frame->time_to_die);
	pthread_create(&th, NULL, &check_if_starving, arg);
	pthread_detach(th);
	while (1)
	{
		pthread_mutex_lock(&frame->fork[philo->value]);
		print_routine(philo, "has taken a fork", NOT);
		pthread_mutex_lock(&frame->fork[philo->rfork]);
		print_routine(philo, "has taken a fork", NOT);
		philo->time_end = time_to_die(frame->time_to_die);
		print_routine(philo, "is eating", frame->time_to_eat);
		pthread_mutex_unlock(&frame->fork[philo->value]);
		pthread_mutex_unlock(&frame->fork[philo->rfork]);
		philo->nbr_of_meal++;
		print_routine(philo, "is sleeping", frame->time_to_sleep);
		print_routine(philo, "is thinking", NOT);
	}
	return (NULL);
}

int main(int ac, char **av)
{
	t_frame *frame;
	pthread_t th;
	int i;

	frame = NULL;
	frame = intial(frame, ac, av);
	if (!frame)
		return (ft_free(frame, NULL));
	pthread_mutex_init(&frame->print, NULL);
	pthread_mutex_init(&frame->main, NULL);
	pthread_mutex_lock(&frame->main);
	frame->start = get_time();
	i = 0;
	while (i < frame->nbr_of_philo)
	{
		if (pthread_create(&th, NULL, &routine, (void *)(&frame->philo[i++])))
			return (ft_free(frame, "fail to create thread\n"));
		pthread_detach(th);
		usleep(100);
	}
	pthread_mutex_lock(&frame->main);
	return (ft_free(frame, NULL));
}
