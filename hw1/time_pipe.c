#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	struct timeval start, end;
	double elapsed_time;
	pid_t pid;

	int pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(pipefd[0]); // Close the read end of the pipe in the child

		// Get the starting time
		gettimeofday(&start, NULL);

		// Write the starting time to the pipe
		write(pipefd[1], &start, sizeof(struct timeval));

		// Execute the command
		execvp(argv[1], &argv[1]);

		// If execvp() fails
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else
	{
		// Close the write end of the pipe in the parent
		close(pipefd[1]);

		// Wait for the child process to terminate
		wait(NULL);

		// Read the starting time from the pipe
		read(pipefd[0], &start, sizeof(struct timeval));

		// Get the ending time
		gettimeofday(&end, NULL);

		// Calculate elapsed time
		elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

		printf("Pipe Version\n");
		printf("Elapsed time: %.6f seconds\n", elapsed_time);

		// Clean up
		close(pipefd[0]);
		exit(EXIT_SUCCESS);
	}

	return 0;
}