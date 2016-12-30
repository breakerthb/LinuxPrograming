/*  Begin as usual with the includes and declarations
    and then initialize inputs to handle input from the keyboard.  */

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static int alarm_fired = 0;

void ouch(int sig)
{
    printf("OUCH! - I got signal %d\n", sig);
    alarm_fired = 1;
}

int main()
{
    struct timeval timeout;
    int result;

    pid_t pid = fork();
    
    switch(pid)
    {
    case -1:
        perror("fork failed.");
        exit(1);
        break;
    case 0: //child
        while(1) 
        {
            timeout.tv_sec = 2;
            timeout.tv_usec = 500000;
    
            result = select(0, NULL, NULL, NULL, &timeout);
    
            switch(result) 
            {
            case 0:
                kill(getppid(), SIGALRM); //getppid()得到父进程PID
                break;
            case -1:
                perror("select");
                exit(1);
            default:
                //Do nothing
                break;
            }
        }
       
        exit(0);
        break;
    default:
        // Do nothing
        break;
    }
    
    printf("waiting for alarm\n");
    struct sigaction act;
    act.sa_handler = ouch;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
   
    sigaction(SIGALRM, &act, 0);
    
    while(1)
    {
        pause();
        if(alarm_fired)
        {
            printf("Time is up!\n");
            alarm_fired = 0;
        }
    }
    
    return 0;
}

