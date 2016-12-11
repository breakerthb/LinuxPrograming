#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void ouch(int sig)
{
    printf("OUCH! - I got signal %d\n", sig);
    (void) signal(SIGINT, SIG_DFL); // 恢复默认处理
}

int main()
{
    (void) signal(SIGINT, ouch); // 设置ouch处理信号
    
    while(1)
    {
        printf("Hello ...\n");
        sleep(1);
    }
    
}