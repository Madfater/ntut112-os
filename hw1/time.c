#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define SHM_SIZE 64

void SharedMemoryVersion(char *argv[])
{
	// First Version: Shared Memory
	struct timeval start, end;
	double elapsed_time;
	pid_t pid;

	int fd_shm;
	void *shm_ptr;

	fd_shm = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
	if (fd_shm == -1)
	{
		perror("shm_open");
		exit(EXIT_FAILURE);
	}

	ftruncate(fd_shm, SHM_SIZE);
	shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
	if (shm_ptr == MAP_FAILED)
	{
		perror("mmap");
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
		// Get the starting time
		gettimeofday(&start, NULL);

		// Write the starting time to shared memory
		memcpy(shm_ptr, &start, sizeof(struct timeval));

		// Execute the command
		execvp(argv[1], &argv[1]);

		// If execvp() fails
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else
	{
		// Wait for the child process to terminate
		wait(NULL);

		// Read the starting time from shared memory
		memcpy(&start, shm_ptr, sizeof(struct timeval));

		// Get the ending time
		gettimeofday(&end, NULL);

		// Calculate elapsed time
		elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

		printf("Shared Memory Version\n");
		printf("Elapsed time: %.6f seconds\n", elapsed_time);

		// Clean up
		munmap(shm_ptr, SHM_SIZE);
		shm_unlink("/shared_memory");
		exit(EXIT_SUCCESS);
	}
}

void PipeVersion(char *argv[])
{
	// Second Version: Pipe
	
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	SharedMemoryVersion(argv);
	PipeVersion(argv);

	return 0;
}
