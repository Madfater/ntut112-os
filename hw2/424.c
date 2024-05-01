#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
typedef struct
{
	int *arr_x;
	int *arr_y;
	int length;
	int *ret;
} thread_args;

void *func(void *param)
{
	thread_args *args = (thread_args *)param;
	int n = args->length;
	int *arr_x = args->arr_x;
	int *arr_y = args->arr_y;
	for (int i = 0; i < n; i++)
	{
		arr_x[i] = rand() % 10000;
		arr_y[i] = rand() % 10000;
	}
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		int temp = 0;
		temp += arr_x[i] * arr_x[i];
		temp += arr_y[i] * arr_y[i];
		if (temp < 100000000)
		{
			count++;
		}
	}
	*args->ret = count;
	return NULL;
}

int main(int argc, char *argv[])
{
	int length;
	if (argc == 1)
	{
		printf("Enter the number of points you want: ");
		scanf("%d", &length);
	}
	else
	{
		sscanf(argv[1], " %d", &length);
	}
	if (length < 1)
	{
		printf("Invalid point\n");
		return 1;
	}
	srand(time(NULL));
	uint32_t *arr_x = calloc(length, sizeof(uint32_t));
	uint32_t *arr_y = calloc(length, sizeof(uint32_t));

	int count = 0;

	pthread_t worker;
	thread_args args = {arr_x, arr_y, length, &count};
	pthread_create(&worker, NULL, func, &args);
	pthread_join(worker, NULL);

	double PI = 0;
	PI = 4.0 * count / length;
	printf("PI = %lf\n", PI);
	free(arr_x);
	free(arr_y);
	pthread_exit(NULL);
	return 0;
}