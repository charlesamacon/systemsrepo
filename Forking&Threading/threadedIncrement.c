/*
Jordan Sanders
Julie Qiroz
Garrett Wescott
Charles Macon
*/
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid[2];
int x = 0, y = 0;

void inc_n(int *n)
{
   /* increment n to 100 */
   while(++(*n) < 1000000000);
}

void* doSomeThing(void *arg){
    pthread_t id = pthread_self();

    if(pthread_equal(id,tid[0])){
        inc_n(&x);
		printf("x increment finished\n");
    }
    else{
        inc_n(&y);
		printf("y increment finished\n");
    }
    return NULL;
}

int main(void)
{
    int i = 0;
    printf("x: %d, y: %d\n", x, y);
	
    while(i < 2){
        pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
		i++;
    }
	pthread_join(tid[0], NULL);
	printf("x: %d, y: %d\n", x, y);
    return 0;
}
