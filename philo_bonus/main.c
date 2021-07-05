/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 14:02:51 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/05 17:07:32 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

uint64_t	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	check_if_sated(t_philo *philo)
{
	t_frame	*frame;

	frame = philo->frame;
	if (philo->one_meal == 0)
	{	
		frame->already_eated++;
		philo->one_meal++;
	}
	if (frame->already_eated == frame->nbr_of_philo)
	{
		sem_wait(frame->print);
		printf("similation done\n");
		sem_post(frame->main);
	}	
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
			printf("%llu died %d\n", philo->timestamp, philo->value + 1);
			sem_post(frame->main);
		}
		if (frame->nbr_of_meal == philo->nbr_of_meal)
			check_if_sated(philo);
		usleep(1000);
	}
	return (NULL);
}

static	void	print_routine(t_philo *philo, char *msg, int sleep)
{
	t_frame	*frame;

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
	pthread_t	th;

	pthread_create(&th, NULL, &check_if_starving, (void *)philo);
	pthread_detach(th);
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
	t_frame	*frame;
	int		i;

	i = 0;
	frame = NULL;
	frame = init_frame(&frame, ac, av);
	if (!frame)
		return (ft_free(frame, "allocation failed !\n"));
	frame->main = sem_open(SEMAMAIN, O_CREAT, 0644, 1);
	frame->forks = sem_open(SEMAFORK, O_CREAT, 0644, frame->nbr_of_philo);
	frame->print = sem_open(SEMAPRINT, O_CREAT, 0644, 1);
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
