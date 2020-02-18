#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>


int main(int argc, char** argv)
{

    //parent 
    //child 1 
    //child 1.1 
    //child 1.2

    //create parent to child 1 pipe here
    int pipe_p_c[2];
    if( pipe(pipe_p_c) == -1){
        printf("Pipe p->c1 error");
        return 1;
    }

    //create child 1 to child 11 pipe here
    int pipe_1_11[2];
    if(pipe(pipe_1_11) == -1){
        printf("Error child 1 -> child 1.1 pipe\n");
        return 1;
    }

    //create child to child 12 here pipe
    int pipe_1_12[2];
    if(pipe(pipe_1_12) == -1){
        printf("Error child 1 -> child 1.2 pipe\n");
        return 1;
    }

    //create the processes here
    pid_t pid; 
    pid = fork();

    //check for error
    if( pid < 0 ) {
        printf("Process error\n");
    }

    if( pid > 0 ){
        //we are in parent process 
        printf("<---------PARENT BEGIN------------------->\n");
        
    } 

    if( pid == 0) {
        
        //child 1.1
        pid_t pid_11;
        pid_11 = fork();

        if( pid_11 == 0){
            printf("<---------CHILD 1.1 BEGIN------------------->\n");
            
        } else { 
            //child 1.2
            pid_t pid_12;
            pid_12 = fork();
            if(pid_12 == 0){
                printf("<---------CHILD 1.2 BEGIN------------------->\n");
            } else{
                printf("<---------CHILD 1 BEGIN------------------->\n");
            }
        } 
    }

    return 0;
}
