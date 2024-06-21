#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#define BUF_SIZE 4096
#define PATH_MAX_LEN 128
char *stats[] = {
	"pid", "comm", "state", "ppid", "pgrp", "session", "tty_nr", "tpgid", "flags", "minflt",
	"cminflt", "majflt", "cmajflt", "utime", "stime", "cutime", "cstime", "priority", "nice", "num_threads",
	"itrealvalue", "starttime", "vsize", "rss", "rsslim", "startcode", "endcode", "startstack", "kstkesp", "kstkeip",
	"signal", "blocked", "sigignore", "sigcatch", "wchan", "nswap", "cnswap", "exit_signal", "processor", "rt_priority",
	"policy", "delayacct_blkio_ticks", "guest_time", "cguest_time", "start_data", "end_data", "start_brk", "arg_start", "arg_end",
	"env_start", "env_end", "env_code"};
void fprintf_environ(const char *proc, FILE *out)
{
	fprintf(out, "ENVIRON\n");
	char path[PATH_MAX_LEN];
	snprintf(path, PATH_MAX_LEN, "/proc/%s/environ", proc);
	FILE *f = fopen(path, "r");
	char buf[BUF_SIZE + 1] = "\0";
	int len;
	while ((len = fread(buf, 1, BUF_SIZE, f)) > 0)
	{
		for (int i = 0; i < len; ++i)
			if (buf[i] == 0)
				buf[i] = '\n';
		buf[len] = '\0';
		fprintf(out, "%s", buf);
	}
	fclose(f);
}
void fprintf_stat(const char *proc, FILE *out)
{
	fprintf(out, "===========\nSTAT\n");
	char path[PATH_MAX_LEN];
	snprintf(path, PATH_MAX_LEN, "/proc/%s/stat", proc);
	FILE *f = fopen(path, "r");
	char buf[BUF_SIZE + 1] = "\0";
	fread(buf, 1, BUF_SIZE, f);
	char *pch = strtok(buf, " ");
	int i = 0;
	while (pch != NULL)
	{
		fprintf(out, "%s: %s\n", stats[i++], pch);
		pch = strtok(NULL, " ");
	}
	fclose(f);
}
void fprintf_cmdline(const char *proc, FILE *out)
{
	fprintf(out, "CMDLINE\n");
	char path[PATH_MAX_LEN];
	snprintf(path, PATH_MAX_LEN, "/proc/%s/cmdline", proc);
	FILE *f = fopen(path, "r");
	char buf[BUF_SIZE + 1] = "\0";
	int len = fread(buf, 1, BUF_SIZE, f);
	buf[len] = 0;
	fprintf(out, "%s\n", buf);
	fclose(f);
}
void fprintf_fd(const char *proc, FILE *out)
{
	fprintf(out, "FD\n");
	struct dirent *dirp;
	DIR *dp;
	char fd_path[PATH_MAX_LEN];
	snprintf(fd_path, PATH_MAX_LEN, "/proc/%s/fd", proc);
	char str[BUF_SIZE] = "\0";
	char path[BUF_SIZE];
	dp = opendir(fd_path);
	while ((dirp = readdir(dp)) != NULL)
	{
		if ((strcmp(dirp->d_name, ".") != 0) &&
		    (strcmp(dirp->d_name, "..") != 0))
		{
			snprintf(path, BUF_SIZE, "%s/%s", fd_path, dirp->d_name);
			readlink(path, str, BUF_SIZE);
			fprintf(out, "%s -> %s\n", dirp->d_name, str);
		}
	}
	closedir(dp);
}
void fprintf_sym_cwd(const char *proc, FILE *out)
{
	fprintf(out, "CWD SYMLINK\n");
	char path[PATH_MAX_LEN];
	snprintf(path, PATH_MAX_LEN, "/proc/%s/cwd", proc);
	FILE *f = fopen(path, "r");
	char buf[BUF_SIZE + 1] = "\0";
	char str[BUF_SIZE + 1] = "\0";
	int len = fread(buf, 1, BUF_SIZE, f);
	buf[len] = 0;
	readlink(path, str, BUF_SIZE);
	fprintf(out, "cwd -> %s\n", str);
	fclose(f);
}
void fprintf_sym_exe(const char *proc, FILE *out)
{
	fprintf(out, "EXE SYMLINK\n");
	char path[PATH_MAX_LEN];
	snprintf(path, PATH_MAX_LEN, "/proc/%s/exe", proc);
	FILE *f = fopen(path, "r");
	char buf[BUF_SIZE + 1] = "\0";
	char str[BUF_SIZE + 1] = "\0";
	int len = fread(buf, 1, BUF_SIZE, f);
	buf[len] = 0;
	readlink(path, str, BUF_SIZE);
	fprintf(out, "exe -> %s\n", str);
	fclose(f);
}
void fprintf_sym_root(const char *proc, FILE *out)
{
	fprintf(out, "ROOT SYMLINK\n");
	char path[PATH_MAX_LEN];
	snprintf(path, PATH_MAX_LEN, "/proc/%s/root", proc);
	FILE *f = fopen(path, "r");
	char buf[BUF_SIZE + 1] = "\0";
	char str[BUF_SIZE + 1] = "\0";
	int len = fread(buf, 1, BUF_SIZE, f);
	buf[len] = 0;
	readlink(path, str, BUF_SIZE);
	fprintf(out, "root -> %s\n", str);
	fclose(f);
}
void fprintf_maps(const char *proc, FILE *out)
{
	fprintf(out, "MAPS\n");
	char path[PATH_MAX_LEN];
	snprintf(path, PATH_MAX_LEN, "/proc/%s/maps", proc);
	FILE *f = fopen(path, "r");
	char *buf;
	size_t buf_size = 0;
	ssize_t len;
	long addr_begin, addr_end, bytes = 0, rec_bytes;
	fprintf(out, "bytes     pages   addr                      perms  offset  dev   inode                     pathname\n");
	while ((len = getline(&buf, &buf_size, f)) > 0)
	{
		sscanf(buf, "%lx-%lx", &addr_begin, &addr_end);
		rec_bytes = addr_end - addr_begin;
		fprintf(out, "%8ld   %4ld   %s", rec_bytes, rec_bytes / 4096, buf);
		bytes += rec_bytes;
	}
	fclose(f);
	fprintf(out, "Size bytes: %ld\n", bytes);
	fprintf(out, "Size pages: %ld\n", bytes / 4096);
}
void fprintf_io(const char *proc, FILE *out)
{
	fprintf(out, "IO\n");
	char path[PATH_MAX_LEN];
	snprintf(path, PATH_MAX_LEN, "/proc/%s/io", proc);
	FILE *f = fopen(path, "r");
	char buf[BUF_SIZE + 1] = "\0";
	int len;
	while ((len = fread(buf, 1, BUF_SIZE, f)) > 0)
	{
		for (int i = 0; i < len; ++i)
			if (buf[i] == 0)
				buf[i] = '\n';
		buf[len] = '\0';
		fprintf(out, "%s", buf);
	}
	fclose(f);
}
void fprintf_comm(const char *proc, FILE *out)
{
	fprintf(out, "COMM\n");
	char path[PATH_MAX_LEN];
	snprintf(path, PATH_MAX_LEN, "/proc/%s/comm", proc);
	FILE *f = fopen(path, "r");
	char buf[BUF_SIZE + 1] = "\0";
	int len = fread(buf, 1, BUF_SIZE, f);
	buf[len] = 0;
	fprintf(out, "%s\n", buf);
	fclose(f);
}
void fprintf_task(const char *proc, FILE *out)
{
	fprintf(out, "TASK\n");
	char path[PATH_MAX_LEN];
	snprintf(path, PATH_MAX_LEN, "/proc/%s/task", proc);
	DIR *dp = opendir(path);
	struct dirent *dirp;
	while ((dirp = readdir(dp)) != NULL)
	{
		if ((strcmp(dirp->d_name, ".") != 0) &&
		    (strcmp(dirp->d_name, "..") != 0))
		{
			fprintf(out, "%s\n", dirp->d_name);
		}
	}
	closedir(dp);
}
void fprintf_page(uint64_t address, uint64_t data, FILE *out)
{
	fprintf(out, "0x%-16lx : %-16lx %-10ld %-10ld %-10ld %-10ld\n", address,
	data & (((uint64_t)1 << 55) - 1), (data >> 55) & 1,
	(data >> 61) & 1, (data >> 62) & 1, (data >> 63) & 1);
}
void fprintf_pagemap_info(const char *proc, FILE *out)
{
	fprintf(out, "PAGEMAP\n");
	fprintf(out, "       addr        : pfn          soft-dirty file/shared swapped    present\n");
	char path[PATH_MAX_LEN];
	snprintf(path, PATH_MAX_LEN, "/proc/%s/maps", proc);
	FILE *maps = fopen(path, "r");
	snprintf(path, PATH_MAX_LEN, "/proc/%s/pagemap", proc);
	int pm_fd = open(path, O_RDONLY);
	char buf[BUF_SIZE + 1] = "\0";
	int len;
	// чтение maps
	while ((len = fread(buf, 1, BUF_SIZE, maps)) > 0)
	{
		for (int i = 0; i < len; ++i)
			if (buf[i] == 0) buf[i] = '\n';
		buf[len] = '\0';
		// проход по строкам из maps
		// используем strtok_r вместо strtok для возможности разбиения на лексемы разных буферов
		char *save_row;
		char *row = strtok_r(buf, "\n", &save_row);
		while (row)
		{
			// получение столбца участка адресного пространства
			char *addresses = strtok(row, " ");
			char *start_str, *end_str;
			// получение начала и конца участка адресного пространства
			if ((start_str = strtok(addresses, "-")) && (end_str = strtok(NULL, "-")))
			{
				uint64_t start = strtoul(start_str, NULL, 16);
				uint64_t end = strtoul(end_str, NULL, 16);
				for (uint64_t i = start; i < end; i += sysconf(_SC_PAGE_SIZE))
				{
					uint64_t offset;
					// поиск смещения, по которому в pagemap находится информация о текущей странице
					uint64_t index = i / sysconf(_SC_PAGE_SIZE) * sizeof(offset);
					pread(pm_fd, &offset, sizeof(offset), index);
					fprintf_page(i, offset, out);
				}
			}
		row = strtok_r(NULL, "\n", &save_row);
		}
	}
	fclose(maps);
	close(pm_fd);
}
int main(int argc, char **argv)
{
	char *pid = argv[1];
	char *out = argv[2];
	FILE *f = fopen(out, "w");
	fprintf_environ(pid, f);
	fprintf_stat(pid, f);
	fprintf_cmdline(pid, f);
	fprintf_fd(pid, f);
	fprintf_sym_cwd(pid, f);
	fprintf_sym_exe(pid, f);
	fprintf_sym_root(pid, f);
	fprintf_maps(pid, f);
	fprintf_io(pid, f);
	fprintf_comm(pid, f);
	fprintf_task(pid, f);
	//fprintf_pagemap_info(pid, f);
	fclose(f);
}


