#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct
{
	int *arr;
	int length;
} thread_args;

void *fib(void *param)
{
	thread_args *args = (thread_args *)param;
	int n = args->length;
	int *arr = args->arr;
	for (int i = 0; i < n; i++)
	{
		if (i < 2)
		{
			arr[i] = i;
			continue;
		}
		arr[i] = arr[i - 1] + arr[i - 2];
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	int length;
	if (argc == 1)
	{
		printf("Enter sqeuence length you want: ");
		scanf("%d", &length);
	}
	else
	{
		sscanf(argv[1], " %d", &length);
	}
	if (length < 1)
	{
		printf("Invalid length\n");
		return 1;
	}

	uint32_t *arr = calloc(length, sizeof(uint32_t));

	pthread_t worker;
	thread_args args = {arr, length};
	pthread_create(&worker, NULL, fib, &args);
	pthread_join(worker, NULL);

	for (int i = 0; i < length; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
	free(arr);
	pthread_exit(NULL);
	return 0;
}