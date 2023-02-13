/* poll_input.c

Licensed under GNU General Public License v2 or later.
*/
#include <poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define errExit(msg)        \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int getReadyDev(struct pollfd* pfds, int n){
    for(;;){
        int id = rand() % n;
        if (pfds[id].revents != 0) return id;
    }
    return -1;
}

int main(int argc, char *argv[])
{
    int nfds, num_open_fds;
    struct pollfd *pfds;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s file...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    num_open_fds = nfds = argc - 1;
    pfds = calloc(nfds, sizeof(struct pollfd));
    if (pfds == NULL)
        errExit("malloc");

    /* Open each file on command line, and add it 'pfds' array. */

    for (int j = 0; j < nfds; j++)
    {
        pfds[j].fd = open(argv[j + 1], O_WRONLY);
        if (pfds[j].fd == -1)
            errExit("open");

        printf("Opened \"%s\" on fd %d\n", argv[j + 1], pfds[j].fd);

        pfds[j].events = POLLOUT;
    }

    /* Keep calling poll() as long as at least one file descriptor is
        open. */

    while (num_open_fds > 0)
    {
        int ready;
        printf("sleeping for 5\n");
        sleep(5);
        printf("About to poll()\n");
        ready = poll(pfds, nfds, -1);
        if (ready == -1)
            errExit("poll");

        printf("Num of ready devs: %d\n", ready);

        int rand_dev = getReadyDev(pfds, nfds);

        /* Deal with array returned by poll(). */

        for (int j = 0; j < nfds; j++)
        {
            char buf[1] = {'a'};

            if (pfds[j].revents != 0)
            {
                if (pfds[j].revents & POLLOUT)
                {
                    if(rand_dev != j) continue;
                    printf("writing <a> to fd=%d\n", pfds[j].fd);
                    ssize_t s = write(pfds[j].fd, buf, sizeof(buf));
                    if (s == -1)
                        errExit("write");
                    printf("    wrote %zd bytes: %.*s\n",
                           s, (int)s, buf);
                }
                else
                { /* POLLERR | POLLHUP */
                    printf("    closing fd %d\n", pfds[j].fd);
                    if (close(pfds[j].fd) == -1)
                        errExit("close");
                    num_open_fds--;
                }
            }
        }
    }

    printf("All file descriptors closed; bye\n");
    exit(EXIT_SUCCESS);
}