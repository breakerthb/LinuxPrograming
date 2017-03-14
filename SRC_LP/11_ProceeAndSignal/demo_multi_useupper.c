/*  This code, useupper.c, accepts a file name as an argument
    and will respond with an error if called incorrectly.  */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char* filename;
    int i;
    pid_t pid;

    if(argc < 2) 
    {
        fprintf(stderr, "usage: run_multi_useupper file\n");
        exit(1);
    }
    
    for (i = 0; i < argc; i++)
    {
        pid = fork();
        
        switch(pid)
        {
            case -1:
                perror("fork failed");
                break;
            case 0: // child
                printf("---->child : %d\n", i + 1);
                filename = argv[i + 1];
                
                if(!freopen(filename, "r", stdin)) 
                {
                    fprintf(stderr, "could not redirect stdin to file %s\n", filename);
                    exit(2);
                }
            
                execl("./run_upper", "run_upper", NULL);
            
                perror("could not exec ./run_upper");
                
                break;
            default: // parent
                // Do nothing
                break;
                
        }
        
        sleep(2);
        
        if (pid != 0)
        {
            int stat_val;
            pid_t child_pid;
            
            for (i = 0; i < argc - 1; i++)
            {
                child_pid = wait(&stat_val);    
                
                printf("----> a child return\n");
            }
            
        }
    }
    
 

















    exit(3);
}
