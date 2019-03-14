#include<stdio.h>
int main()
{
    char* msg = "Hello World";
	int len = 11;
	int result = 0;
 
    __asm__ __volatile__("movl %2, %%edx;\n\r" /*传入参数：要显示的字符串长度*/
             "movl %1, %%ecx;\n\r" /*传入参赛：文件描述符（stdout）*/
			 "movl $1, %%ebx;\n\r" /*传入参数：要显示的字符串*/
			 "movl $4, %%eax;\n\r" /*系统调用号：4 sys_write*/
			 "int  $0x80" /*触发系统调用中断*/
             :"=m"(result) /*输出部分：本例并未使用*/
             :"m"(msg),"r"(len)  /*输入部分：绑定字符串和字符串长度变量*/
             :"%eax"); 
    	
	return 0;
}
