#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/time.h>
#include <signal.h>

#define SEMMAIN "semamin"
#define SEMFORK "semfork"
#define SEMPRINT "semprint"
int nbr_of_philo = 4;
pid_t *pids;
pid_t pid;
sem_t *forks;
sem_t *semprint;

int routine()
{
	sem_wait(forks);
	printf("take a fork\n");
	sem_wait(forks);
	printf("take a fork\n");



	return 0;
}

int		main(int ac, char **av)
{
	int i;
	sem_t *sem_main;
	i = 0;

	sem_unlink(SEMFORK);
	sem_unlink(SEMMAIN);
	sem_unlink(SEMPRINT);

	forks = sem_open(SEMFORK, O_CREAT, 0644, nbr_of_philo);
	sem_main = sem_open(SEMMAIN, O_CREAT, 0644, 1);
	semprint = sem_open(SEMPRINT, O_CREAT, 0644, 1);
	sem_wait(sem_main);
	pids = malloc(sizeof(pid_t) * nbr_of_philo);

	while (i < nbr_of_philo)
	{
		pid = fork();
		if (pid == 0)
		{
			routine();
			exit(0);
		}
		else
			pids[i++] = pid;
	}
	i = 0;
	while (i < nbr_of_philo)
	{
		kill(pids[i], SIGKILL);
		i++;
	}
	sem_wait(sem_main);

	return 0;
}
