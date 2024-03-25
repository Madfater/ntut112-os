#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE 4096

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source-file> <destination-file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    pid_t pid;
    ssize_t num_read;
    char buffer[BUF_SIZE];

    // Create a pipe
    if (pipe(pipefd) == -1) {
        error("pipe");
    }

    // Fork a child process
    pid = fork();
    if (pid == -1) {
        error("fork");
    } else if (pid == 0) { // Child process
        close(pipefd[1]); // Close the write end of the pipe in the child

        // Open the destination file for writing
        int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (dest_fd == -1) {
            error("open");
        }

        // Read from the pipe and write to the destination file
        while ((num_read = read(pipefd[0], buffer, BUF_SIZE)) > 0) {
            if (write(dest_fd, buffer, num_read) != num_read) {
                error("write");
            }
        }
        if (num_read == -1) {
            error("read");
        }

        // Close the read end of the pipe in the child
        close(pipefd[0]);

        // Close the destination file
        close(dest_fd);

        exit(EXIT_SUCCESS);
    } else { // Parent process
        close(pipefd[0]); // Close the read end of the pipe in the parent

        // Open the source file for reading
        int source_fd = open(argv[1], O_RDONLY);
        if (source_fd == -1) {
            error("open");
        }

        // Read from the source file and write to the pipe
        while ((num_read = read(source_fd, buffer, BUF_SIZE)) > 0) {
            if (write(pipefd[1], buffer, num_read) != num_read) {
                error("write");
            }
        }
        if (num_read == -1) {
            error("read");
        }

        // Close the write end of the pipe in the parent
        close(pipefd[1]);

        // Close the source file
        close(source_fd);

        // Wait for the child process to complete
        wait(NULL);

        exit(EXIT_SUCCESS);
    }

    return 0;
}
