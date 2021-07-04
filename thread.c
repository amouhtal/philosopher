# include <pthread.h>
# include <errno.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>


pthread_mutex_t *forks;
pthread_mutex_t print;
pthread_mutex_t block;

int nbr_of_philo = 4;
int time_to_sleep = 200;
int time_to_eat = 200;
unsigned long time_to_die = 410;

typedef struct s_philo
{
	int index;
	unsigned long died;
}t_philo;


unsigned long	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void *check_starving(void *arg)
{
	unsigned long time;
	t_philo *philo;

	philo = (t_philo *)arg;
	while (1)
	{
		time = get_time();
		if (time > philo->died)
		{
			pthread_mutex_lock(&print);
			printf("%d is died\n", philo->index + 1);
			pthread_mutex_unlock(&block);
		}
		//sleep(500);
	}
	return NULL;
}

void *routine(void *arg)
{
	int i;
	t_philo *philo;
	pthread_t th;
	philo = (t_philo*)arg;
	philo->died = 0;
	philo->died = get_time() + time_to_die;
	pthread_create(&th, NULL, &check_starving, arg);
	// pthread_detach(th);
	while (1)
	{
		pthread_mutex_lock(&forks[philo->index]);

		pthread_mutex_lock(&print);
		printf("%d take fork \n", philo->index + 1);
		pthread_mutex_unlock(&print);

		pthread_mutex_lock(&forks[philo->index + 1 % nbr_of_philo]);

		pthread_mutex_lock(&print);
		printf("%d take fork \n", philo->index + 1);
		pthread_mutex_unlock(&print);

			philo->died = get_time() + time_to_die ;

		pthread_mutex_lock(&print);
		printf("%d is eating \n", philo->index + 1);
		pthread_mutex_unlock(&print);
		usleep(time_to_eat * 1000);
		pthread_mutex_unlock(&forks[philo->index]);
		pthread_mutex_unlock(&forks[philo->index + 1 % nbr_of_philo]);

		pthread_mutex_lock(&print);
		printf("%d is sleeping \n", philo->index + 1);
		pthread_mutex_unlock(&print);
		usleep(time_to_sleep * 1000);
		
		pthread_mutex_lock(&print);
		printf("%d is thinkin \n", philo->index + 1);
		pthread_mutex_unlock(&print);
	}
	return NULL;

}

void check(char **str)
{
	*str = "hello";
}

int main(int ac, char **av)
{
	pthread_t th;
	t_philo *philo;

	char *str = "mouhtal";

	printf("before: |%s|\n", str);
	check(&str);
	printf("after: |%s|\n", str);
}