#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

int ticketCount=1000,temp;//定义
sem_t b,r,multex;

	//购票线程
	void *buy(void *arg){
	while(1){
	if(ticketCount==0)printf("无票，无法购票\n");
	sem_wait(&b);
	sem_wait(&multex);
	temp=ticketCount;
	pthread_yield();
	temp=temp-1;
	sem_post(&r);
	pthread_yield();
	ticketCount=temp;
	printf("成功购票，还剩%d张车票\n",ticketCount);
	sem_post(&multex);
	}
	}
	//退票线程
	void *refund(void *arg) {
	while(1){
	if(ticketCount==1000)printf("无票，无法退票\n");
	sem_wait(&r);
	sem_wait(&multex);
	temp=ticketCount;
	pthread_yield();
	temp=temp+1;
	sem_post(&b);
	pthread_yield();
	ticketCount=temp;
	printf("成功退票，还剩%d张车票\n",ticketCount);
	sem_post(&multex);
	}
	}
	int main(void) {
	pthread_t pid, cid;
	//初始化信号量
	sem_init(&b,0,1000);
	sem_init(&r,0,0);
	sem_init(&multex,0,1);
	//创建两个线程
	pthread_create(&pid,NULL,buy,NULL);
	pthread_create(&cid,NULL,refund,NULL);
	//等待两个线程的汇合
pthread_join(pid,NULL);
pthread_join(cid,NULL);
//销毁信号量
sem_destroy(&b);
sem_destroy(&r);
return 0;
}

