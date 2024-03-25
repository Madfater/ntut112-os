#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

void collatz_sequence(int start)
{
	printf("Child Process - Collatz Sequence: %d, ", start);
	while (start != 1)
	{
		if (start % 2 == 0)
		{
			start = start / 2;
		}
		else
		{
			start = 3 * start + 1;
		}
		printf("%d, ", start);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <starting_number>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int start = atoi(argv[1]);
	if (start <= 0)
	{
		fprintf(stderr, "Error: Please provide a positive integer as the starting number.\n");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		collatz_sequence(start);
	}
	else
	{
		wait(NULL);
	}

	return EXIT_SUCCESS;
}
