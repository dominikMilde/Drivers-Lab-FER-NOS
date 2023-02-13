/* simple program that writes to given file */

#include <poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fd;
	if (argc < 3) {
		fprintf(stderr, "Usage: %s file-name text\n", argv[0]);
		return -1;
	}

	char *buffer = argv[2];

	fd = open(argv[1], O_WRONLY);
	if (fd == -1) {
		perror("open failed");
		return -1;
	}

	ssize_t written = write(fd, buffer, strlen(buffer));
	if (written == -1) {
		perror("Error writing to dev file.\n");
		return -1;
	}
    else{
        fprintf(stderr, "Wrote %ld bytes to dev file.\n", written);
    }

	return 0;
}
