/*
Jordan Sanders
Julie Qiroz
Garrett Wescott
Charles Macon
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

void inc_n(int *n)
{
   /* increment n to 100 */
   while(++(*n) < 1000000000);
}

int main()
{
	pid_t childpid;
	int * status;
	int x=0,y=0;
	printf("x: %d, y: %d\n", x, y);
	childpid = fork();
	
    if(childpid == 0){
		inc_n(&x);
		printf("x increment finished\n");
	}
	else{
	    inc_n(&y);
		wait(status);
		printf("y increment finished\n");
	}
	printf("x: %d, y: %d\n", x, y);
	
    return 1;
}

