#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

int main(void)
{
    pid_t child_pid;
    char *messages[6] = {"ayyyyyyyy", 
                         "zzzzzzzz"};
                         
    int sockets[2];
    char buf[32];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == -1)
    {
        perror("socketpair() error");
        exit(1);
    }

    if ((child_pid = fork()) == -1)
    {
        perror("Can't fork\n");
        exit(1);
    }
    else if (child_pid == 0)
    {
        // sleep(1);
        close(sockets[1]);
        write(sockets[0], messages[0], strlen(messages[0]) + 1);
        printf("Child (pid: %d)   send: %s\n", getpid(), messages[0]);
        sleep(1);
        read(sockets[0], buf, strlen(messages[1]) + 1);
        printf("Child (pid: %d)   receive: %s\n", getpid(), buf);
        close(sockets[0]);
		exit(0);
    }
    else
    {
        sleep(1);
        read(sockets[1], buf, strlen(messages[0]) + 1);
        printf("Parent               receive: %s\n", buf);

        write(sockets[1], messages[1], strlen(messages[1]) + 1);
        printf("Parent               send: %s\n", messages[1]);
    }

    close(sockets[0]);
    close(sockets[1]);

    int status;
    waitpid(child_pid, &status, 0);

    if (WIFEXITED(status))
    {
        printf("Child (pid: %d)   exited with code %d\n", \
            child_pid, WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        printf("Child (pid: %d)   received signal %d\n", \
            child_pid, WTERMSIG(status));
    }
    else if (WIFSTOPPED(status))
    {
        printf("Child (pid: %d)   received signal %d\n", \
            child_pid, WSTOPSIG(status));
    }

    return 0;
}
