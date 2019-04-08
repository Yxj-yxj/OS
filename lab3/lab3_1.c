#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

sem_t sem23,sem4; 

int main()
{
int cnt=0,i;
	i=sem_init(&sem23,1,0);
	printf("%d\n",i);
	i=sem_init(&sem4,1,0);
printf("%d\n",i);

	pid_t p1, p2, p3, p4;
	
	p1 = fork();
	if (p1==0)			//P2进程
	{
		printf("I\'am the process P1\n");
		i=sem_post(&sem23);
		printf("%d\n",i);
	}
	else
	{
		i=sem_wait(&sem23);
printf("%d\n",i);
		p2 = fork();
		if (p2==0)			//P2进程
		{
			printf("I\'am the process P2\n");
			sem_post(&sem23);
			sem_post(&sem4);
		}
		else
		{
			sem_wait(&sem23);
			p3=fork();
			if (p3 == 0)
			{
				printf("I\'am the process P3\n");
				sem_post(&sem4);
			}
			else if(p3>0)
			{
				sem_wait(&sem4);
				p4 = fork();
				if (p4 == 0)
				{
					printf("I\'am the process P4\n");
					sem_post(&sem4);
				}
				/*sem_wait(sem4);
				p4 = fork();
				if (p4 == 0)
				{
					printf("I\'am the process P4\n");
					sem_post(sem4);
					exit(0);
				}
				else{
				exit(0);
				}*/
			}
			else {printf("erro");}
			
		}
	}
}
