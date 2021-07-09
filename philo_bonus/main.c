/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 14:02:51 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/09 11:13:54 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static	void	check_if_sated(t_philo *philo)
{
	t_frame	*frame;
	int		i;

	i = -1;
	frame = philo->frame;
	if (frame->nbr_of_meal)
		while (++i < frame->nbr_of_philo)
			sem_wait(frame->nbr_to_eats);
	sem_wait(frame->print);
	printf("similation done\n");
	sem_post(frame->main);
}

static	void	*check_if_starving(void *arg)
{
	uint64_t	time;
	t_frame		*frame;
	t_philo		*philo;

	philo = (t_philo *)arg;
	frame = philo->frame;
	philo->time_end = time_to_die(frame->time_to_die);
	while (1)
	{
		time = get_time();
		if (time > philo->time_end)
		{
			sem_wait(frame->print);
			philo->timestamp = time - frame->start;
			printf("%llu died %d\n", philo->timestamp, philo->index + 1);
			sem_post(frame->main);
		}
		if (frame->nbr_of_meal == philo->nbr_of_meal)
			check_if_sated(philo);
		usleep(1000);
	}
	return (NULL);
}

static	void	print_routine(t_philo *philo, char msg, int sleep)
{
	sem_wait(philo->frame->print);
	philo->timestamp = get_time() - philo->frame->start;
	ft_putnbr_fd(philo->timestamp, 1);
	write(1, "\t", 1);
	ft_putnbr_fd(philo->index + 1, 1);
	write(1, "\t", 1);
	if (msg == '0')
		write(1, "has taken a fork\n", 17);
	else if (msg == '1')
		write(1, "is eating\n", 10);
	else if (msg == '2')
		write(1, "is sleeping\n", 12);
	else
		write(1, "is thinking\n", 12);
	sem_post(philo->frame->print);
	if (sleep != NOT)
		usleep(sleep * 1000);
}

static void	routine(t_philo *philo, t_frame *frame)
{
	pthread_t	th;

	pthread_create(&th, NULL, &check_if_starving, (void *)philo);
	pthread_detach(th);
	while (1)
	{
		sem_wait(frame->forks);
		print_routine(philo, '0', NOT);
		sem_wait(frame->forks);
		print_routine(philo, '0', NOT);
		philo->time_end = time_to_die(frame->time_to_die);
		print_routine(philo, '1', frame->time_to_eat);
		if (frame->nbr_of_meal != NOT)
			philo->nbr_of_meal++;
		sem_post(frame->nbr_to_eats);
		sem_post(frame->forks);
		sem_post(frame->forks);
		print_routine(philo, '2', frame->time_to_sleep);
		print_routine(philo, '3', NOT);
	}
}

int	main(int ac, char **av)
{
	t_frame	*frame;
	int		i;

	i = 0;
	frame = NULL;
	frame = init_frame(&frame, ac, av);
	if (!frame)
		return (ft_free(frame, "Check arguments !\n"));
	frame->main = sem_open(SEMAMAIN, O_CREAT, 0644, 1);
	frame->forks = sem_open(SEMAFORK, O_CREAT, 0644, frame->nbr_of_philo);
	frame->print = sem_open(SEMAPRINT, O_CREAT, 0644, 1);
	frame->nbr_to_eats = sem_open(SEMEATS, O_CREAT, 0644, 0);
	sem_wait(frame->main);
	frame->pids = (int *)malloc(sizeof(int) * frame->nbr_of_philo);
	frame->start = get_time();
	while (i < frame->nbr_of_philo)
	{
		frame->pid = fork();
		if (frame->pid == 0)
			routine(&frame->philo[i], frame);
		else
			frame->pids[i++] = frame->pid;
		usleep(100);
	}
	return (ft_free(frame, NULL));
}
