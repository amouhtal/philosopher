/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 14:02:51 by amouhtal          #+#    #+#             */
/*   Updated: 2021/06/28 15:41:29 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

unsigned long	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

t_frame	*mutex_init(t_frame *frame)
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

t_frame *intial(t_frame *frame, int ac, char **av)
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
	frame = mutex_init(frame);
	if (!frame)
		return (NULL);
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
			printf("philo %d time : %lu timeofdie : %lu\n",
					philo->value, time, philo->time_end);
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
	if (sleep != -1)
		usleep(sleep * 1000);
}

void routine(t_philo *philo, t_frame *frame)
{
	pthread_t th;
	unsigned long time;

	time = get_time();
	philo->time_end = time + frame->time_to_die;
	pthread_create(&th, NULL, &check_if_starving, (void*)philo);
	pthread_detach(th);
	while (1)
	{
		sem_wait(frame->forks);
		print_routine(philo, "take first fork", -1);
		sem_wait(frame->forks);
		print_routine(philo, "take seconde fork", -1);
		philo->time_end = get_time() + frame->time_to_die;
		frame->time_of_thread = get_time() - frame->start;
		print_routine(philo, "is eating", frame->time_to_eat);
		philo->nbr_of_meal++;
		sem_post(frame->forks);
		sem_post(frame->forks);
		print_routine(philo, "is sleeping", frame->time_to_sleep);
		print_routine(philo, "is thinkin", -1);
	}
}

int main(int ac, char **av)
{
	t_frame *frame;
	t_philo *philo;
	pthread_t th;
	int i;

	i = 0;
	frame = NULL;
	frame = intial(frame, ac, av);
	if (!frame)
		return (ft_free(&frame, NULL));
	sem_unlink(SEMAMAIN);
	sem_unlink(SEMAFORK);
	sem_unlink(SEMAPRINT);

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
		{
			frame->pids[i] = frame->pid;
			i++;
		}		
		usleep(100);
	}
	i = 0;
	sem_wait(frame->main);
	while (i < frame->nbr_of_philo)
		kill(frame->pids[i++],SIGKILL);
	return (ft_free(&frame, NULL));
}
