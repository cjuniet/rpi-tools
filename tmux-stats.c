// Copyright (c) 2013, Christophe Juniet <christophe at juniet dot net>
// You may do anything with this work that copyright law would normally
// restrict, so long as you retain the above notice(s) and this license
// in all redistributed copies and derived works. There is no warranty.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 64

int g_delay = 1000000;

size_t get_load(char *buf, size_t len)
{
  char line[BUFSIZE];
  size_t n = 0;
  FILE *fp = fopen("/proc/loadavg", "r");
  if (fp == NULL) exit(EXIT_FAILURE);

  fgets(line, BUFSIZE, fp);
  n = snprintf(buf, len, "L:%s ", strtok(line, " "));

  fclose(fp);

  return n;
}

void get_cpu_usage(int *used, int *unused)
{
  char line[BUFSIZE];
  int cpu_user, cpu_nice, cpu_sys, cpu_idle;
  FILE *fp = fopen("/proc/stat", "r");
  if (fp == NULL) exit(EXIT_FAILURE);

  fgets(line, BUFSIZE, fp);
  strtok(line, " "); // discard 'cpu'
  cpu_user = atoi(strtok(NULL, " "));
  cpu_nice = atoi(strtok(NULL, " "));
  cpu_sys  = atoi(strtok(NULL, " "));
  cpu_idle = atoi(strtok(NULL, " "));

  *used = cpu_user + cpu_nice + cpu_sys;
  *unused = cpu_idle;

  fclose(fp);
}

size_t get_cpu(char *buf, size_t len)
{
  size_t n = 0;
  int usedA, unusedA, usedB, unusedB;
  int used, total;

  get_cpu_usage(&usedA, &unusedA);

  usleep(g_delay);

  get_cpu_usage(&usedB, &unusedB);

  used = usedB - usedA;
  total = used + (unusedB - unusedA);
  n = snprintf(buf, len, "C:%d%% ", (int)(100.0 * used / total));

  return n;
}

int get_mem_next_value(FILE *fp)
{
  char line[BUFSIZE];
  fgets(line, BUFSIZE, fp);
  strtok(line, " "); // discard name
  return atoi(strtok(NULL, " "));
}

size_t get_mem(char *buf, size_t len)
{
  size_t n = 0;
  int total, used;
  FILE *fp = fopen("/proc/meminfo", "r");
  if (fp == NULL) exit(EXIT_FAILURE);

  // get the total and free memory values
  total = get_mem_next_value(fp);
  used = total - get_mem_next_value(fp);

  n = snprintf(buf, len, "M:%d%% ", (int)(100.0 * used / total));

  fclose(fp);

  return n;
}

int main(int argc, char *argv[])
{
  char buf[BUFSIZE];
  size_t len = 0;

  // you can give the cpu stats polling interval in seconds
  // it will be capped between 1,000,000 usec and 10,000,000 usec
  if (argc > 1) {
    g_delay = atoi(argv[1]) * 1000000;
    if (g_delay <  1000000) g_delay =  1000000;
    if (g_delay > 10000000) g_delay = 10000000;
  }

  len += get_load(buf + len, BUFSIZE - len);

  len += get_cpu(buf + len, BUFSIZE - len);

  len += get_mem(buf + len, BUFSIZE - len);

  buf[len-1] = '\0'; // eat last space
  printf("%s", buf); // no new line!

  return 0;
}
