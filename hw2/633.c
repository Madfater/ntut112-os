#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
sem_t licenses;
pthread_mutex_t mutex;
typedef int id;
int request_licenses(id uid, int quota)
{
	int temp = 0;
	pthread_mutex_lock(&mutex);
	printf("user %10d request %2d licenses:", uid, quota);
	sem_getvalue(&licenses, &temp);
	if (temp >= quota)
	{
		printf("accepted\t|avaliable licenses:%2d\n", temp - quota);
		for (int i = 0; i < quota; i++)
		{
			sem_wait(&licenses);
		}
		temp = 0;
	}
	else
	{
		printf("failed\t|avaliable licenses:%2d\n", temp);
		temp = -1;
	}
	pthread_mutex_unlock(&mutex);
	return temp;
}
void release_licenses(id uid, int quota)
{
	pthread_mutex_lock(&mutex);
	int temp = 0;
	sem_getvalue(&licenses, &temp);
	printf("user %10d release %2d licenses\t\t|avaliable licenses:%2d\n", uid, quota, temp + quota);
	for (int i = 0; i < quota; i++)
	{
		sem_post(&licenses);
	}
	pthread_mutex_unlock(&mutex);
}
void *user(void *param)
{
	int item;
	id uid = rand();
	while (1)
	{
		int quota = rand() % 5 + 1;
		if (request_licenses(uid, quota) == 0)
		{
			sleep(rand() % 5 + 1);
			release_licenses(uid, quota);
		}
		sleep(rand() % 10 + 1);
	}
}

int main(int argc, char *argv[])
{
	int worker_count;
	if (argc == 1)
	{
		printf("Enter count you want: ");
		scanf("%d", &worker_count);
	}
	else
	{
		sscanf(argv[1], " %d", &worker_count);
	}
	if (worker_count < 1)
	{
		printf("Invalid count\n");
		return 1;
	}
	sem_init(&licenses, 0, worker_count);
	pthread_mutex_init(&mutex, 0);

	pthread_t *user_array = calloc(worker_count, sizeof(pthread_t));
	for (uint64_t i = 0; i < worker_count; i++)
	{
		pthread_create(&user_array[i], NULL, user, NULL);
	}
	sleep(100);
	for (uint64_t i = 0; i < worker_count; i++)
	{
		pthread_cancel(user_array[i]);
	}
	sem_destroy(&licenses);
	pthread_mutex_destroy(&mutex);
	free(user_array);
	return 0;
}