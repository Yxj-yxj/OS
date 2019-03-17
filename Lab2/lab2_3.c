#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"

int main()
{
	pid_t p1, p2, p3, p4, p5;
	int cnt=0;
	printf("p1's pid is %d,and its parent_pid is %d\n", getpid(),getppid());
	while ((p2 = fork()) == -1);
	if (p2==0)			//P2进程
	{
	printf("p2's pid is %d,and its parent_pid is %d\n", getpid(), getppid());
		p4 = fork();
		if (p4 == 0)
		{
		printf("p4's pid is %d,and its parent_pid is %d\n", getpid(), getppid());
		}
		else{
		p5 = fork();
		if (p5 == 0)
		{
			printf("p5's pid is %d,and its parent_pid is %d\n", getpid(), getppid());
		}}
	}
	else
	{
		p3=fork();
		if (p3 == 0)
		{
			printf("p3's pid is %d,and its parent_pid is %d\n", getpid(), getppid());
		}
	}
}
