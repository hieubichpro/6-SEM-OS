#include "main.h"

typedef int Myfunc(const char *, const struct stat *, int);
static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);
static int myfunc(const char *pathname, const struct stat *statptr, int type);
static int counter(const char *pathname, const struct stat *statptr, int type);
typedef int Handler(const char *,const struct stat *, int);
static int mydopath(const char *filename, int depth, Handler *func);
static int  my_myftw(char *, Handler *);
void print_info(int nreg, int ndir, int nblk, int nchr, int nfifo, int nslink, int nsock);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;
static long my_nreg, my_ndir, my_nblk, my_nchr, my_nfifo, my_nslink, my_nsock, my_ntot;

int main(int argc, char *argv[]) 
{
    int ret; 
	clock_t start, end;

    if (argc != 2) 
    {
		printf("ERROR, wrong arguments.\nUse: ./app <dir>\n");
		exit(-1);
	} 
	
	start = clock();
	ret = myftw(argv[1], myfunc); //выполняет всю работу
	end = clock();
	printf("Время работы функции myftw: %ld\n", end - start);

	print_info(nreg, ndir, nblk, nchr, nfifo, nslink, nsock);

	printf("\n\n");
	start = clock();
	ret = my_myftw(argv[1], counter); //выполняет всю работу
	end = clock();
	print_info(my_nreg, my_ndir, my_nblk, my_nchr, my_nfifo, my_nslink, my_nsock);
	printf("Время работы функции my_myftw: %ld\n", end - start);

	return 0;
}

void print_info(int nreg, int ndir, int nblk, int nchr, int nfifo, int nslink, int nsock)
{
	int ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0) 
        ntot = 1;
    printf("обычные файлы = %7ld, %5.2f %%\n", nreg, nreg*100.0/ntot);
    printf("каталоги = %7ld, %5.2f %%\n", ndir, ndir*100.0/ntot);
    printf("специальные файлы блочных устройств = %7ld, %5.2f %%\n", nblk, nblk*100.0/ntot); 
    printf("специальные файлы символьных устройств = %7ld, %5.2f %%\n", nchr, nchr*100.0/ntot); 
    printf("FIFO = %7ld, %5.2f %%\n", nfifo, nfifo*100.0/ntot);
    printf("символические ссылки = %7ld, %5.2f %%\n", nslink, nslink*100.0/ntot); 
    printf("сокеты = %7ld, %5.2f %%\n", nsock, nsock*100.0/ntot);  
	printf("Total:\t%7ld\n", ntot);
}

static int  my_myftw(char *filename, Handler *func)
{
	return mydopath(filename, 0, func);
}

static int mydopath(const char *filename, int depth, Handler *func)
{
	struct stat statbuf;
	struct dirent * dirp;
	DIR *dp;
	int ret = 0;

	if (lstat(filename, &statbuf) < 0) // ошибка  
		return(func(filename, &statbuf, FTW_NS));

	for (int i = 0; i < depth; ++i)
		printf("|\t");

	if (S_ISDIR(statbuf.st_mode) == 0) // не каталог 
		return(func(filename, &statbuf, FTW_F)); // отобразить в дереве 

	if ((ret = func(filename, &statbuf, FTW_D)) != 0)
		return(ret);

	if ((dp = opendir(filename)) == NULL) // каталог недоступен
		return(func(filename, &statbuf, FTW_DNR));
    
	chdir(filename);
	while ((dirp = readdir(dp)) != NULL && ret == 0)
	{
		if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0 ) 
		{
			ret = mydopath(dirp->d_name, depth + 1, func);
		}
	}
    
	chdir("..");

	if (closedir(dp) < 0)
		perror("Невозможно закрыть каталог");

	return(ret);    
}

static int counter(const char *pathname, const struct stat *statptr, int type)
{
	switch(type)
	{
		case FTW_F: 
			printf( "-- %s\n", pathname);
			switch(statptr->st_mode & S_IFMT)
			{
				case S_IFREG: my_nreg++; break;
				case S_IFBLK: my_nblk++; break;
				case S_IFCHR: my_nchr++; break;
				case S_IFIFO: my_nfifo++; break;
				case S_IFLNK: my_nslink++; break;
				case S_IFSOCK: my_nsock++; break;
				case S_IFDIR: 
					perror("Католог имеет тип FTW_F"); 
					return(-1);
			}
			break;
		case FTW_D: 
			printf( "--%s/\n", pathname);
			my_ndir++; 
			break;
		case FTW_DNR:
			perror("К одному из каталогов закрыт доступ."); 
			return(-1);
		case FTW_NS:
			perror("Ошибка функции stat."); 
			return(-1);
		default: 
			perror("Неизвестый тип файла."); 
			return(-1);
	}
	return(0);
}

//эта функция myftw  
char * path_alloc(size_t *sizep) /* если удалось выделить память, */
{ 
    char *ptr; size_t size;
    if (posix_version == 0) posix_version = sysconf(_SC_VERSION);
    if (xsi_version == 0) xsi_version = sysconf(_SC_XOPEN_VERSION); 
    if (pathmax == 0) {
        errno = 0;
        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0)
        {
            if (errno == 0) pathmax = PATH_MAX_GUESS; /* если константа не определена */
            else perror("ошибка вызова pathconf с параметром _PC_PATH_MAX");
        } 
        else
        {
            pathmax++; /* добавить 1, так как путь относительно корня */
        } 
    } 

    if ((posix_version < 200112L) && (xsi_version < 4))
        size = pathmax + 1; 
    else 
        size = pathmax;

    if ((ptr = malloc(size)) == NULL) 
        perror("malloc error for pathname"); 
    if (sizep != NULL)
        *sizep = size;
    return(ptr); 
}

static char *fullpath; /* полный путь к каждому из файлов */
static size_t pathlen = 128;
static int  myftw(char *pathname, Myfunc *func) /* возвращает то, что вернула функция func() */
{
    fullpath = path_alloc(&pathlen); 

    if (pathlen <= strlen(pathname))
    { 
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) 
            perror("ошибка вызова realloc");
    } 
    strcpy(fullpath, pathname);
    return(dopath(func)); 
}

static int dopath(Myfunc* func) /* возвращает то, что вернула функция func() */
{ 
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp; int ret, n;

    if (lstat(fullpath, &statbuf) < 0) /* ошибка вызова функции stat */ 
        return(func(fullpath, &statbuf, FTW_NS));

    if (S_ISDIR(statbuf.st_mode) == 0) /* не каталог */ 
        return(func(fullpath, &statbuf, FTW_F)); 
  
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0) 
        return(ret); 

    n = strlen(fullpath); 
    if (n + NAME_MAX + 2 > pathlen)
    {
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            perror("ошибка вызова realloc"); 
    } 
    fullpath[n++] = '/';
    fullpath[n] = 0;
    if ((dp = opendir(fullpath)) == NULL) /* каталог недоступен */ 
        return(func(fullpath, &statbuf, FTW_DNR)); 

    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) 
            continue;  /* пропустить каталоги "." и ".." */ 
        strcpy(&fullpath[n], dirp->d_name); /* добавить имя после слеша */
        if ((ret = dopath(func)) != 0) /* рекурсия */
            break; /* выход по ошибке */ 
    } 
    fullpath[n-1] = 0; /* стереть часть строки от слеша и до конца */
    if (closedir(dp) < 0)  
        perror("Невозможно закрыть каталог");
    return(ret); 
} 

static int myfunc(const char *pathname, const struct stat *statptr, int type)
{ 
    switch (type) {
        case FTW_F: 
            switch (statptr->st_mode & S_IFMT)
            { 
                case S_IFREG: nreg++; break;
                case S_IFBLK: nblk++; break; 
                case S_IFCHR: nchr++; break;
                case S_IFIFO: nfifo++; break;
                case S_IFLNK: nslink++; break;
                case S_IFSOCK: nsock++; break;
                case S_IFDIR: /* каталоги должны иметь type = FTW_D*/ 
                    perror("Католог имеет тип FTW_F"); 
					return(-1);
            } 
            break; 
        case FTW_D: 
			ndir++; break;
        case FTW_DNR:
			perror("К одному из каталогов закрыт доступ."); 
			return(-1);
		case FTW_NS:
			perror("Ошибка функции stat."); 
			return(-1);
        default:  
            perror("Неизвестый тип файла."); 
			return(-1);} 
    return(0); 
}

