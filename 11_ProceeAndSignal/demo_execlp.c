#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Running ps with execlp\n");
    
    execlp("ps", "ps", "ax", NULL);
    
    printf("Done.\n");
    
    return 0;
}
