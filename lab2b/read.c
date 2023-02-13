#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <device> <n>\n", argv[0]);
        return 1;
    }

    char *device = argv[1];
    int n = atoi(argv[2]);

    int fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char buffer[n];
    int bytes_read = read(fd, buffer, n);
    if (bytes_read == -1) {
        perror("read");
        return 1;
    }

    printf("Read %d bytes from %s:\n%.*s\n", bytes_read, device, bytes_read, buffer);

    close(fd);
    return 0;
}