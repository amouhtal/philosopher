#include <stdio.h>
#include <pthread.h>

void* routine()
{
	printf("routine \n");
	sleep(4);
	return NULL;
}

int	main()
{
	pthread_t t1, t2;

	pthread_create(&t1, NULL, &routine, NULL);
	pthread_create(&t2, NULL, &routine, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	return (0);
}