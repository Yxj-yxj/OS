#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
	int pid;
	pid=fork();
switch(pid)
{
		case -1:
			printf("fork fail!\n");
			exit(1);
		case 0:
			execl("/usr/bin/vi","vi","/home/yxj/Desktop/OStest2",NULL);
			printf("exec fail!\n");
			exit(1);
		default:
			for (int i=0;;i++){};
			printf("vi completed!\n");
			exit(1);
}
}
