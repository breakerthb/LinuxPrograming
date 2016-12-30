#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int alarm_fired = 0;

void ding(int sig)
{
    alarm_fired = 1;
}

int main()
{
    pid_t pid;
    
    printf("alarm application starting\n");
    
    pid = fork();
    switch(pid)
    {
    case -1:
        perror("fork failed.");
        exit(1);
        break;
    case 0: //child
        sleep(5);
        kill(getppid(), SIGALRM); //getppid()得到父进程PID
        exit(0);
        break;
    default:
        // Do nothing
        break;
    }
    
    printf("waiting for alarm to go off\n");
    (void)signal(SIGALRM, ding);
    
    pause();
    if(alarm_fired)
        printf("ding\n");
        
    printf("done\n");
    exit(0);
}