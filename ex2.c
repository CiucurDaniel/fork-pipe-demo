#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFFSIZE 1024
int file;//global file variable
char content[1000];
char buf[BUFFSIZE];
int n;

char *p; // for STRTOK
char words[100][100];//used or words transfer
char received_words[100][100];//used for transfer receive

char lowerCaseWords[100][100];//used to receive lower case words
char upperCaseWords[100][100];//used to receive upper case words

int main(int argc, char** argv)
{

    //parent reads from file all
    //child 1 primeste cuvintele 
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
        
        //open the file and read it
        if( (file = open(argv[1], O_RDONLY)) < 0 ){
            printf("Error at file openning\n");
            return 1;
        }

        //file was created successfully then read from it
        while( n = read(file, buf, BUFFSIZE)){
            strcpy(content, buf);
        }

        p = strtok(content, " \n,.");
        int words_counter=0;
        //aici in while o sa am fiecare cuvant pe rand p* = "Ana" p[0]=A;""
        while(p!=NULL){
            int letters_count = 0;
            int numbers_count = 0;
            //loop through words seprated by separator
            for(int i = 0; i<strlen(p); i++){
                if(isalpha(p[i])){
                    letters_count++;
                } else {
                    numbers_count++;
                }

                if(letters_count == strlen(p) && strlen(p)>0){
                    strcpy(words[words_counter++],p);
                }
            }
            p=strtok(NULL, " \n,.");
        }

        //CHECKER TO SEE IF PARENT PRINTS
        for(int z; z<words_counter;z++){
            printf("word: %s\n", words[z]);
        }
        //dau doar cuvintele la copil
        write(pipe_p_c[1], words, sizeof(words) );
    } 


    //child 1_1 litere mici 
    // child 1_2 litere mari

    if( pid == 0) {

        //child 1.1
        pid_t pid_11;
        pid_11 = fork();

        if( pid_11 == 0){
            printf("<---------CHILD 1.1 BEGIN------------------->\n");
            //read the lowercase words from child 1
            char receive[100][100];
            read(pipe_1_11[0], receive, BUFFSIZE);

            int k=0;
            while(strlen(receive[k])>0){
                printf("Uppercase word: %s\n", receive[k]);
                k++;
            }
            
        } else { 
            //child 1.2
            pid_t pid_12;
            pid_12 = fork();
            if(pid_12 == 0){
                printf("<---------CHILD 1.2 BEGIN------------------->\n");
                char receive[100][100];
                read(pipe_1_12[0], receive, BUFFSIZE);

                int k=0;
                while(strlen(receive[k])>0){
                    printf("Lowercase word: %s\n", receive[k]);
                    k++;
                }

            } else{
                printf("<---------CHILD 1 BEGIN------------------->\n");
                //read from parent
                read(pipe_p_c[0], received_words, BUFFSIZE); 

                //CHECKER TO SEE IF PARENT PRINTS
                int k=0;
                while(strlen(received_words[k])>0){
                    printf("word: %s\n", received_words[k]);
                    k++;
                }

                int word = 0;
                int counterLower = 0;
                int counterUpper = 0;
                while(strlen(received_words[word])>0){
                    int howManyUpper=0, howManyLower=0;
                    for(int i = 0; i <strlen(received_words[word]); i++){
                        if(received_words[word][i] >= 'a' && received_words[word][i] <= 'z'){
                            howManyLower++;
                        }
                        if(received_words[word][i] >= 'A' && received_words[word][i] <= 'Z'){
                            howManyUpper++;
                        }
                    }

                    if(howManyLower == strlen(received_words[word])) {
                        strcpy(lowerCaseWords[counterLower++], received_words[word]);
                        printf("LC: %s\n", received_words[word]);
                    }
                    if(howManyUpper == strlen(received_words[word])) {
                        strcpy(upperCaseWords[counterUpper++], received_words[word]);
                        printf("UC: %s\n", received_words[word]);
                    }
                    howManyLower=0;
                    howManyUpper=0;
                    word++;
                }
                //write to child 11
                write(pipe_1_11[1], upperCaseWords, sizeof(upperCaseWords));
                
                //write to child 12
                write(pipe_1_12[1], lowerCaseWords, sizeof(lowerCaseWords));

            }
        } 
    }

    return 0;
}
