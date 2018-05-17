#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void * thread_function(void * arg);

int num_threads;
int times_to_eat;
pthread_mutex_t chop_sticks[32];
char start = 0;

int main(int argc, char * argv[])
{
		if(argc != 3)
		{
				printf("invalid args\n");
				exit(1);
		}

		times_to_eat = atoi( argv[1]);
		num_threads = atoi( argv[2]);
		int philo_id[128];


		pthread_t thread_id[num_threads];
		int times_eaten[num_threads];

		int i, j;
		printf("About to init mutex\n");
		for(i=0; i < num_threads; i++)
		{
				//chop_sticks[i] = PTHREAD_MUTEX_INITIALIZER;
				pthread_mutex_init(&chop_sticks[i], NULL);
				continue;
		}

		for(i=0; i < num_threads; i++)
		{
				philo_id[i] = i;

				pthread_create( &thread_id[i], NULL, &thread_function, &philo_id[i]);
		}
		start = 1;

		for(j=0; j < num_threads; j++)
		{
				pthread_join(thread_id[j], NULL);
		}
}

//first we need to know why it's void *
void * thread_function( void * arg)
{
	while(start == 0)
		{
			;
		}

	int philo_id = *((int *) arg);
	printf("Thread %d starting...\n", philo_id);
	int times = 0;

	while(times < times_to_eat)
		{
			//pick up "chop sticks"
			if (philo_id % 2 == 1) {
				//if odd
				pthread_mutex_lock(&chop_sticks[philo_id]);
				pthread_mutex_lock(&chop_sticks[(philo_id + 1) % num_threads]);
			} else {
				//if even
				pthread_mutex_lock(&chop_sticks[(philo_id + 1) % num_threads]);
				pthread_mutex_lock(&chop_sticks[philo_id]);
			}

			times++;
			sleep(5);
			printf("Philosopher %d is eating.	They have eaten %d time(s) now\n", philo_id, times);

			//put down "chop sticks"
			pthread_mutex_unlock(&chop_sticks[(philo_id + 1) % num_threads]);
			pthread_mutex_unlock(&chop_sticks[philo_id]);

			//idk why we want this but it was in the assignment so okay
			printf("Philosopher %d is thinking.\n", philo_id);
		}
	printf("Philosopher %d is done\n",philo_id);

}
