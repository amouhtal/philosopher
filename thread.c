# include <pthread.h>
# include <errno.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

int prime[10] = {20, 21, 22, 23, 24 , 25, 26, 27 , 28, 29};

pthread_mutex_t block;
pthread_mutex_t block2;

void *routine(void *arg)
{
	int i;

	// pthread_mutex_lock(&block);
	i = *(int*)arg;
	write(1,"hello world 222\n", 16);
	// pthread_mutex_unlock(&block);
	return NULL;
}

int main(int ac, char **av)
{
	pthread_t th;
	int i;

	i = 0;
	pthread_mutex_init(&block, NULL);
	pthread_mutex_init(&block2, NULL);

	while (i < 100000)
	{
		if (pthread_create(&th, NULL, &routine, &i))
			perror("Fail to create thread\n");
		pthread_detach(th);
		i++;
	}
	while (1)
		;
}