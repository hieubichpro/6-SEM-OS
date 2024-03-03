#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <unistd.h> 
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <time.h>

#define FTW_F 1 // файл, не являющийся каталогом
#define FTW_D 2 // каталог
#define FTW_DNR 3 // каталог, недоступный для чтения
#define FTW_NS 4 // файл, информацию о котором нельзя получить с помощью stat

#ifdef PATH_MAX 
static long pathmax = PATH_MAX; 
#else static long pathmax = 0; 
#endif 
static long posix_version = 0;
static long xsi_version = 0; 
#define PATH_MAX_GUESS 1024