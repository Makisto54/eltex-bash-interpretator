#include <sys/wait.h>
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 

int main(int argc, char *argv[])
{
    char *first = "ls";
    char *second = "grep";
    char *specify = "a*";
    int pfd[2]; 
    if(pipe(pfd) == -1)
    {
        fprintf(stderr, "Pipe create error\n");
        exit(1);
    } 

    switch(fork()) 
    {
        case -1:
            printf("Proc 1 create error\n");
            exit(1);
        case 0:
            if(close(pfd[0]) == -1)
            {
                fprintf(stderr, "Closing pipe 1 error\n");
                exit(1);
            }
            if(pfd[1] != STDOUT_FILENO) 
            { 
                if(dup2(pfd[1], STDOUT_FILENO) == -1)
                {
                    fprintf(stderr, "Dup2 error\n");
                    exit(1);
                }
                if(close(pfd[1]) == -1)
                {
                    fprintf(stderr, "Closing pipe 2 error\n");
                    exit(1);
                }
            }
            execlp(first, first, (char *) NULL); 
            exit(1);
        default: 
            break;
     }

     switch(fork())
     {
         case -1:
             printf("Proc 2 create error\n");
             exit(1);
         case 0: 
             if(close(pfd[1]) == -1)
             {
                 fprintf(stderr, "Closing pipe 2 error\n");
                 exit(1);
             }
             if(pfd[0] != STDIN_FILENO) 
             { 
                 if(dup2(pfd[0], STDIN_FILENO) == -1)
                 {
                     fprintf(stderr, "Dup1 error\n");
                     exit(1);
                 }
                 if(close(pfd[0]) == -1)
                 {
                     fprintf(stderr, "Closing pipe 2 error\n");
                     exit(1);
                 }
             }
             execlp(second, second, specify, (char *) NULL); 
             exit(1);
         default: 
             break;
    }

    if(close(pfd[0]) == -1)
    {
        fprintf(stderr, "Closing pipe 1 error\n");
        exit(1);
    }
    if(close(pfd[1]) == -1)
    {
        fprintf(stderr, "Closing pipe 2 error\n");
        exit(1);
    }
    if(wait(NULL) == -1)
    exit(1);
    if(wait(NULL) == -1)
    exit(1);

    exit(EXIT_SUCCESS);
}
