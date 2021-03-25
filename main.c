#include <stdio.h> 
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h> 
#include <stdlib.h>
#include <setjmp.h>

sigjmp_buf buf; 
int sigcounter=0; 
void handler()
{
    sigcounter++;
    if(sigcounter>5)
    {
        if(fork()==0)
        {
            execl("/usr/bin/find","find",".","-maxdepth","1","-name","*.c",NULL); 
        }
        else{
            wait(NULL);
        }
    }
    siglongjmp(buf,1);
}

int main() {
    struct sigaction keyboard_interrupt;
    keyboard_interrupt.sa_handler = handler; 
    sigemptyset(&keyboard_interrupt.sa_mask); 
    sigprocmask(0, 0, &keyboard_interrupt.sa_mask);
    keyboard_interrupt.sa_flags = 0;
    sigaction(SIGINT, &keyboard_interrupt, 0);

    char* file=malloc(253*sizeof(char)); 
    char** filelist=malloc(2*sizeof(char*));
    filelist[0]=malloc(4*sizeof(char));
    filelist[0]="gcc";
    int last=1;
    while(1)
    {
        sigsetjmp(buf,1); 
        if(last==1) {
            printf("Input source filenames or enter \"quit\" to exit\n");
        }
        else
        {
            printf("Add more source filenames or enter \"done\" to finish entering files\n");
        }
        scanf("%s",file);
        size_t count=strlen(file);
        if(strcmp(file,"done")==0) {
            if (last!=1) { 
                if (fork() == 0) {
                    filelist[last]=malloc(sizeof(char));
                    filelist[last]=0;
                    execv("/usr/bin/gcc", filelist);
                } else {
                    int status;
                    last=1;
                    wait(&status); 
                    //printf("Status = %d\n",status);
                    filelist=(char**)realloc(filelist,2*sizeof(char*));
                    if(!status) 
                    {
                        printf("Program has been translated from source\n");
                        if(fork()==0)
                        {
                            printf("Program output:\n");
                            execl("./a.out","a.out",NULL);
                        }
                        else
                        {
                            wait(NULL); 
                        }
                    } else {
                        printf("Something went wrong\n");
                    }
                }
            }
            else{
                printf("Error, no filenames were entered\n");
            }
        }
        else if(strcmp(file,"quit")==0)
        {
            printf("Goodbye!\n");
            exit(0);
        }
        else
        {
            filelist[last]=malloc(count*sizeof(char));
            strcpy(filelist[last],file);
            last++;
            filelist=(char**)realloc(filelist,(last+1)*sizeof(char*));
            sigsetjmp(buf,1);
        }
    }
}
