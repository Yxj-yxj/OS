#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<semaphore.h>
sem_t buffer,rest;	//缓冲区的未被输出的字符　	//缓冲区剩余可接纳个数
char buf[10];
void *worker1() {
	int i=0;
	while(1){
	sem_wait(&rest);
	printf("\033[0m"); 
	scanf("%c",&buf[i%10]);
	i++;
	sem_post(&buffer);
	}
}
void *worker2() {
	int i=0;
	while(1){
	sem_wait(&buffer);
	printf("\033[31m");		//修改输出字符颜色，便于识别
	printf("%c",buf[i%10]);
	sleep(1);
	printf("\033[0m"); 
	i++;
	sem_post(&rest);
	}
}
int main()
{
sem_init(&buffer,0,0);
sem_init(&rest,0,10);
pthread_t p1, p2;
pthread_create(&p1, NULL, worker1, NULL);
pthread_create(&p2, NULL, worker2, NULL);
pthread_join(p1, NULL);
pthread_join(p2, NULL);
return 0;
}
