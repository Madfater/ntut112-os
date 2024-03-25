#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUF_SIZE 4096

// Function to copy contents from source file to destination file
void copy_file(int source_fd, int dest_fd) {
    ssize_t num_read;
    char buffer[BUF_SIZE];

    // Read from the source file and write to the destination file
    while ((num_read = read(source_fd, buffer, BUF_SIZE)) > 0) {
        if (write(dest_fd, buffer, num_read) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if (num_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source-file> <destination-file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int source_fd, dest_fd;

    // Open the source file for reading
    source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Open the destination file for writing
    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (dest_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Copy the contents from the source file to the destination file
    copy_file(source_fd, dest_fd);

    // Close the file descriptors
    close(source_fd);
    close(dest_fd);

    return 0;
}