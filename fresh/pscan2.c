/*
** pscan.c - Made By KidRck
** modified by #KidRockS Team
** 
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

#define MAX_SOCKETS 800
#define TIMEOUT 3

#define S_NONE       0
#define S_CONNECTING 1

struct conn_t {
    int s;
    char status;
    time_t a;
    struct sockaddr_in addr;
};
struct conn_t connlist[MAX_SOCKETS];

void init_sockets(void);
void check_sockets(void);
void fatal(char *);

FILE *outfd;
int tot = 0;

int main(int argc, char *argv[])
{
    int done = 0, i, cip = 1, bb = 0, ret, k, ns, x;
    time_t scantime;
    char ip[20], outfile[128], last[256];

    if (argc < 3)
    {
        printf("Usage: %s <b-block> <port> [c-block]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&outfile, 0, sizeof(outfile));
    if (argc == 3)
        snprintf(outfile, sizeof(outfile) - 1, "scan.log", argv[1], argv[2]);
    else if (argc >= 4)
    {
        snprintf(outfile, sizeof(outfile) - 1, "scan.log", argv[1], argv[3], argv[2]);
        bb = atoi(argv[3]);
        if ((bb < 0) || (bb > 255))
            fatal("Invalid b-range.\n");
    }
    strcpy(argv[0],"/bin/bash");
    if (!(outfd = fopen(outfile, "a")))
    {
        perror(outfile);
        exit(EXIT_FAILURE);
    }
    printf("# scanning: ", argv[1]);
    fflush(stdout);

    memset(&last, 0, sizeof(last));
    init_sockets();
    scantime = time(0);

    while(!done)
    {
        for (i = 0; i < MAX_SOCKETS; i++)
        {
            if (cip == 255)
            {           
                if ((bb == 255) || (argc >= 4))
                {
                    ns = 0;
                    for (k = 0; k < MAX_SOCKETS; k++)
                    {
                        if (connlist[k].status > S_NONE)
                        {
                            ns++;
                            break;
                        }
                    }

                    if (ns == 0)
                        done = 1;

                     break;
                }
                else
                {
                    cip = 0;
                    bb++;
                    for (x = 0; x < strlen(last); x++)
                        putchar('\b');
                    memset(&last, 0, sizeof(last));
                    snprintf(last, sizeof(last) - 1, "%s.%d.* (total: %d) (%.1f%% done)",
                        argv[1], bb, tot, (bb / 255.0) * 100);
                    printf("%s", last);
                    fflush(stdout);
                }
            }

            if (connlist[i].status == S_NONE)
            {
                connlist[i].s = socket(AF_INET, SOCK_STREAM, 0);
                if (connlist[i].s == -1)
                    printf("Unable to allocate socket.\n");
                else
                {
                    ret = fcntl(connlist[i].s, F_SETFL, O_NONBLOCK);
                    if (ret == -1)
                    {
                        printf("Unable to set O_NONBLOCK\n");
                        close(connlist[i].s);
                    }
                    else
                    {
                        memset(&ip, 0, 20);
                        sprintf(ip, "%s.%d.%d", argv[1], bb, cip);
                        connlist[i].addr.sin_addr.s_addr = inet_addr(ip);
                        if (connlist[i].addr.sin_addr.s_addr == -1)
                            fatal("Invalid IP.");
                        connlist[i].addr.sin_family = AF_INET;
                        connlist[i].addr.sin_port = htons(atoi(argv[2]));
                        connlist[i].a = time(0);
                        connlist[i].status = S_CONNECTING;
                        cip++;
                    }
                }
            }
        }
        check_sockets();
    }

    printf("\n# pscan completed in %u seconds. (found %d ips)\n", (time(0) - scantime), tot);
    fclose(outfd);
    exit(EXIT_SUCCESS);
}

void init_sockets(void)
{
    int i;

    for (i = 0; i < MAX_SOCKETS; i++)
    {
        connlist[i].status = S_NONE;
        memset((struct sockaddr_in *)&connlist[i].addr, 0, sizeof(struct sockaddr_in));
    }
    return;
}

void check_sockets(void)
{
    int i, ret;

    for (i = 0; i < MAX_SOCKETS; i++)
    {
        if ((connlist[i].a < (time(0) - TIMEOUT)) && (connlist[i].status == S_CONNECTING))
        {
            close(connlist[i].s);
            connlist[i].status = S_NONE;
        }
        else if (connlist[i].status == S_CONNECTING)
        {
            ret = connect(connlist[i].s, (struct sockaddr *)&connlist[i].addr,
                sizeof(struct sockaddr_in));
            if (ret == -1)
            {
                if (errno == EISCONN)
                {
                    tot++;
                    fprintf(outfd, "%s\n",
                        (char *)inet_ntoa(connlist[i].addr.sin_addr));
                    close(connlist[i].s);
                    connlist[i].status = S_NONE;
                }

                if ((errno != EALREADY) && (errno != EINPROGRESS))
                {
                    close(connlist[i].s);
                    connlist[i].status = S_NONE;
                }
            }
            else
            {
                tot++;
                fprintf(outfd, "%s\n",
                    (char *)inet_ntoa(connlist[i].addr.sin_addr));
                close(connlist[i].s);
                connlist[i].status = S_NONE;
            }
        }
    }
}

void fatal(char *err)
{
    int i;
    printf("Error: %s\n", err);
    for (i = 0; i < MAX_SOCKETS; i++)
        if (connlist[i].status >= S_CONNECTING)
            close(connlist[i].s);
    fclose(outfd);
    exit(EXIT_FAILURE);
}
