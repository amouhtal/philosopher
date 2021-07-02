/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 14:02:51 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/02 18:53:49 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

uint64_t	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

t_frame	*init_philo(t_frame *frame)
{
	int	i;

	i = 0;
	while (i < frame->nbr_of_philo)
	{
		frame->philo[i].frame = frame;
		frame->philo[i].value = i;
		frame->philo[i].nbr_of_meal = 0;
		i++;
	}
	return (frame);
}

t_frame *init_frame(t_frame *frame, int ac, char **av)
{
	frame = (t_frame *)malloc(sizeof(t_frame));
	if (!frame)
		return (NULL);
	if (ac < 5 || ac > 6)
	{
		printf("wrong numbers of arg\n");
		return (NULL);
	}
	frame->nbr_of_philo = ft_atoi(av[1]);
	frame->time_to_die = ft_atoi(av[2]);
	frame->time_to_eat = ft_atoi(av[3]);
	frame->time_to_sleep = ft_atoi(av[4]);
	frame->nbr_of_meal = -1;
	if (av[5])
		frame->nbr_of_meal = ft_atoi(av[5]);
	frame->philo = malloc(sizeof(t_philo) * frame->nbr_of_philo);
	if (!frame->philo)
		return (NULL);
	frame = init_philo(frame);
	if (!frame)
		return (NULL);
	sem_unlink(SEMAMAIN);
	sem_unlink(SEMAFORK);
	sem_unlink(SEMAPRINT);
	return (frame);
}

static void *check_if_starving(void *arg)
{
	unsigned long time;
	t_frame *frame;
	t_philo *philo;

	philo = (t_philo *)arg;
	frame = philo->frame;
	while (1)
	{
		time = get_time();
		if (time > philo->time_end)
		{
			sem_wait(frame->print);
			philo->timestamp = time - frame->start;
			printf("%llu died %d\n", philo->timestamp, philo->value + 1);
			sem_post(frame->main);
		}
		if (frame->nbr_of_meal == philo->nbr_of_meal)
		{
			sem_wait(frame->print);
			printf("similation done\n");
			sem_post(frame->main);
		}
		usleep(1000);
	}
	return (NULL);
}

static void print_routine(t_philo *philo, char *msg, int sleep)
{
	t_frame *frame;

	frame = philo->frame;
	philo->time_of_thread = get_time() - frame->start;
	sem_wait(frame->print);
	printf("%d\t philo %d\t%s \n", philo->time_of_thread, philo->value + 1, msg);
	sem_post(frame->print);
	if (sleep != NOT)
		usleep(sleep * 1000);
}

void	routine(t_philo *philo, t_frame *frame)
{
	pthread_t th;

	pthread_create(&th, NULL, &check_if_starving, (void*)philo);
	pthread_detach(th);
	philo->time_end = time_to_die(frame->time_to_die);
	while (1)
	{
		sem_wait(frame->forks);
		print_routine(philo, "has taken a fork", NOT);
		sem_wait(frame->forks);
		print_routine(philo, "has taken a fork", NOT);
		philo->time_end = time_to_die(frame->time_to_die);
		print_routine(philo, "is eating", frame->time_to_eat);
		sem_post(frame->forks);
		sem_post(frame->forks);
		if (frame->nbr_of_meal != NOT)
			philo->nbr_of_meal++;
		print_routine(philo, "is sleeping", frame->time_to_sleep);
		print_routine(philo, "is thinkin", NOT);
	}
}

int	main(int ac, char **av)
{
	t_frame *frame;
	int i;

	i = 0;
	frame = NULL;
	frame = init_frame(frame, ac, av);
	if (!frame)
		return (ft_free(frame, NULL));

	frame->main =  sem_open(SEMAMAIN, O_CREAT, 0644, 1);
	frame->forks = sem_open(SEMAFORK, O_CREAT, 0644, frame->nbr_of_philo);
	frame->print = sem_open(SEMAPRINT, O_CREAT, 0644, 1);
	sem_wait(frame->main);
	frame->pids = (int*)malloc(sizeof(int) * frame->nbr_of_philo);
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
