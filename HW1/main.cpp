
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>

#define COMMAND1 "ls"
#define ARG1 "/"
#define COMMAND2 "wc"
#define ARG2 "-l"


int main()
{
    int Pipe[2];
    int origStdin, origStdout;
    int childPID;
   
    origStdin = dup(0);
    origStdout = dup(1);
    
    pipe(Pipe);
    
    if( (childPID = fork()) < 0 )
    {
        perror( "Fork Error" );
        exit(-1);
    }
    
    else if( childPID == 0)
    {
        /* Force our stdout to be the write end of the Pipe we formed */
        dup2( Pipe[1], 1 );
        
        close( origStdout );
        close( origStdin );
        close( Pipe[0] );
        close( Pipe[1] );
        
        printf("\n");
        execlp( COMMAND1, COMMAND1, ARG1, NULL );
        perror("Execl error");
    }
    
   else
    {
    /* Force our stdin to be the read side of the pipe we made */
    dup2( Pipe[0], 0 );
    
    /* Force our stdout to be the original standard out */
    dup2( origStdout, 1 );

    close( Pipe[0] );
    close( Pipe[1] );
    close( origStdout );
    close( origStdin );

    execlp( COMMAND2, COMMAND2,ARG2, NULL );
    
    perror( "EXEC Error");
    }
    
    return 0;
}
