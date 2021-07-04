/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 11:50:21 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/04 15:26:31 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
// 292416
void ft_putnbr_fd(uint64_t n, int fd)
{
	char str[13];
	int length;

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

uint64_t get_time(void)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

int check_if_sated(t_philo1 *philo)
{
	t_frame *frame;

	frame = philo->frame;
	if (philo->one_meal == 0)
	{
		frame->already_eated++;
		philo->one_meal++;
	}
	if (frame->already_eated == frame->nbr_of_philo)
	{
		pthread_mutex_lock(&frame->print);
		write(1, "similation done\n", 16);
		return (0);
	}
	return (1);
}

int check_if_starving(t_philo1 *philo)
{
	uint64_t time;
	t_frame *frame;

	frame = philo->frame;
	time = get_time();
	if (time > philo->time_end)
	{
		pthread_mutex_lock(&frame->print);
		philo->timestamp = time - frame->start;
		printf("%llu died %d\n", philo->timestamp, philo->value + 1);
		return (0);
	}
	if (frame->nbr_of_meal == philo->nbr_of_meal)
		return (check_if_sated(philo));
	return (1);
}

static void print_routine(t_philo1 *philo, char msg, int sleep)
{
	pthread_mutex_lock(&philo->frame->print);
	philo->timestamp = get_time() - philo->frame->start;
	ft_putnbr_fd(philo->timestamp, 1);
	write(1, "\t", 1);
	ft_putnbr_fd(philo->value, 1);
	write(1, "\t", 1);
	if (msg == '0')
		write(1, "has taken a fork\n", 17);
	else if (msg == '1')
		write(1, "is eating\n", 10);
	else if (msg == '2')
		write(1, "is sleeping\n", 12);
	else if (msg == '3')
		write(1, "is thinking\n", 12);
	pthread_mutex_unlock(&philo->frame->print);
	if (sleep != NOT)
		usleep(sleep * 1000);
}

static void *routine(void *arg)
{
	t_philo1 *philo;

	philo = (t_philo1 *)arg;
	philo->time_end = time_to_die(philo->frame->time_to_die);
	while (1)
	{
		pthread_mutex_lock(&philo->frame->fork[philo->value]);
		print_routine(philo, '0', NOT);
		pthread_mutex_lock(&philo->frame->fork[philo->rfork]);
		print_routine(philo, '0', NOT);
		philo->time_end = time_to_die(philo->frame->time_to_die);
		print_routine(philo, '1', philo->frame->time_to_eat);
		pthread_mutex_unlock(&philo->frame->fork[philo->value]);
		pthread_mutex_unlock(&philo->frame->fork[philo->rfork]);
		philo->nbr_of_meal++;
		print_routine(philo, '2', philo->frame->time_to_sleep);
		print_routine(philo, '3', NOT);
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
	frame->start = get_time();
	i = -1;
	while (++i < frame->nbr_of_philo)
	{
		if (pthread_create(&th, NULL, &routine, (void *)(&frame->philo[i])))
			return (ft_free(frame, "fail to create thread\n"));
		pthread_detach(th);
		usleep(100);
	}
	while (1)
	{
		i = 0;
		while (i < frame->nbr_of_philo)
		{
			// usleep(1);
			if (!check_if_starving(&frame->philo[i++]))
				return (ft_free(frame, NULL));
		}
	}
}
