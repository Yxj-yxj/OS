
#include<stdio.h>
#include<random>
#include<time.h>
#define Max 64				//内存页数上限
#define Mainarrlen 1000		//总页表数上限
#define maxsuparrlen 12
#define random(x) rand()%x
int mainarr[Mainarrlen];
int arr[Max];
int arrlen = 0;
int fifoflag = 0;				//先进先出flag
int used[Max] = { 0 };			//初始化页面访问位 置为0
int modifiesd[Max];			//申请修改位数组
int suparr[maxsuparrlen];			//申请2位缓冲数组
int suparrlen = 0;
int suparrflag = 0;				//辅助数组置换flag

/*iflack(int p)
function :判断是否有缺页
parm :   p---要访问的页号
rtn  :   1---缺页   0---不缺页*/
int iflack(int p)
{
	for (int i = 0; i < arrlen; i++)
	{
		if (arr[i] == p)
		{
			used[i] = 1;
			return 0;
		}
	}
	return 1;
}

/*replace(int p, int n)
function :内存满时缺页调用的内存页替换函数
parm:   p---要替换成的页号   n---要替换的内存中页的数组下标*/
void replace(int p, int n)
{
	arr[n] = p;
}

/*inserte(int p)
function : 内存未存放满调用的内存页插入函数
parm :   p---要替换成的页号   n---要替换的内存中页的数组下标
*/
void inserte(int p)
{
	arr[arrlen++] = p;			//插入后内存中长度增1
}


//outputbuf: 输出内存中存的页号
void outputmem()
{
	for (int i = 0; i < Max; i++)
	{
		printf("%d  ", arr[i]);
	}
	if (suparrlen > 0)
	{
		printf("此时缓存数组为:");
		for (int i = 0; i < suparrlen; i++)
		{
			printf("%d   ", suparr[i]);
		}
	}
	printf("\n");
}

/*最佳置换算法
function: 缺页情况下寻找应替换的内存中的页号
parm : *mainarr---要查找的页表   p---要替换的缺页的页号   pindex---要替换的缺页页号下标
*/
int BestReplace(int* mainarr, int p, int pindex)
{
	int lastindex = -1;
	int rpl;				//被替换掉的页
	int replaceindex = 0;
	for (int i = 0; i < arrlen; i++)				//将内存中的页循环遍历
	{
		int flag = 0;
		for (int j = pindex + 1; j < Mainarrlen; j++)		//将每个页向后找相同的出现，寻找最晚出现的
		{
			if (arr[i] == mainarr[j])
			{
				if (lastindex < j)	//如果找到更远的，替换index
				{
					lastindex = j;
					replaceindex = i;
				}
				break;				//找到相等就跳出循环
			}
			if (j == Mainarrlen - 1 && arr[i] != mainarr[j])		//如果找完没有出现，直接将此页作为被替换页
			{
				replaceindex = i;
				flag = 1;
			}
		}
		if (flag == 1)
		{
			break;
		}
	}
	rpl = arr[replaceindex];
	replace(p, replaceindex);
	return rpl;
}

/*先进先出置换算法
function：按先进先出寻找要置换的页号
parm：  p---要替换的页数
*/
int FIFOReplace(int p)
{
	int rpl;
	rpl = arr[fifoflag % Max];				//arr轮着替换
	replace(p, fifoflag % Max);
	fifoflag++;
	return rpl;
}

/*最近最久未使用算法
function:	向前寻找最近最久未使用的
parm : *mainarr---要查找的页表   p---要替换的缺页的页号   pindex---要替换的缺页页号下标
*/
int LRU(int* mainarr, int p, int pindex)
{
	int lastindex = pindex;
	int rpl;				//被替换掉的页
	int replaceindex = 0;		//替换的下标
	for (int i = 0; i < arrlen; i++)				//将内存中的页循环遍历
	{
		int flag = 0;
		for (int j = pindex; j >=0; j--)		//将每个页向后找相同的出现，寻找最晚出现的
		{
			if (arr[i] == mainarr[j])
			{
				if (lastindex > j)	//如果找到更远的，替换index
				{
					lastindex = j;
					replaceindex = i;
				}
				break;				//找到相等就跳出循环
			}
			if (j == 0 && arr[i] != mainarr[j])		//如果找完没有出现，直接将此页作为被替换页
			{
				replaceindex = i;
				flag = 1;
			}
		}
		if (flag == 1)
		{
			break;
		}
	}
	rpl = arr[replaceindex];
	replace(p, replaceindex);
	return rpl;
}

/*Clock算法
function：对每页都设置一个访问位和修改位，这里我们模拟随机设置修改位为1，再按顺序置换
parm:	p---要替换的缺页的页号
*/
int Clock(int p)
{
	for (int i = 0; i < Max; i++)				//修改位随机初始化
	{
		modifiesd[i] = rand() % 2;
	}
	int rpl;
	int count = 1;			//遍历两遍
	while (count--)
	{
		for (int i = 0; i < Max; i++)			//第一轮遍历寻找{0,0}，如果没有不作任何操作
		{
			if (used[i] == 0 && modifiesd[i] == 0)
			{
				rpl = arr[i];
				replace(p, i);
				return rpl;
			}
			else
			{
				continue;
			}
		}
		for (int i = 0; i < Max; i++)			//第二轮遍历寻找{0,1}，如果不是的话将访问位置0
		{
			if (used[i] == 0 && modifiesd[i] == 1)
			{
				rpl = arr[i];
				replace(p, i);
				return rpl;
			}
			else
			{
				used[i] = 0;				//第四次访问时不会执行这里
			}
		}
	}
}

/*页面缓冲算法PBA
function：	设置一个缓冲数组用于从中取数
parm：p---要替换的缺页的页号
*/
int PBA(int p)
{
	int rpl;
	for (int i = 0; i < suparrlen+1; i++)
	{
		if (p == suparr[i])					//如果相等
		{
			suparr[i] = arr[fifoflag% Max];				//将缓存中的与内存中的进行交换
			arr[fifoflag]= p;
			fifoflag++;
			break;
		}
		if (i == suparrlen && suparr[i] != p)
		{
			if (suparrlen < maxsuparrlen)
			{
				//辅助数组不命中且不满,直接插入，长度加一
				suparr[suparrlen++] = p;
				return p;
			}
			else
			{
				//辅助数组不命中且满,选择一个替换
				rpl = suparr[suparrflag % maxsuparrlen];
				suparr[suparrflag++ % maxsuparrlen] = p;
				return rpl;
			}
		}
	}
	return -1;				//缓存与内存进行置换
}

void createarr() {
	//srand((int)time(0));// 设置随机种子
	int N = 1000;// 虚拟内存尺寸    
	int p = 0;// 工作集起始位置  
	int e = 5;// 工作集大小     
	int m = 6;// 工作集移动率    
	int t = 6;// 一个范围在0和10之间的值t  
	for (int i = 0; i < Mainarrlen / m; ++i) {
		for (int j = i * m; j < i * m + 6; ++j) {
			mainarr[j] = (random(e) + p) % N;
		}
		int r = random(10) > t ? 1 : 0;
		if (r < t) {
			p = random(N) % N;
		}
		else {
			p = (p + 1) % N;
		}
	}
}


/*主函数*/
int main()
{
	int p, rpl,lacknum=0;
	//int mainarr[30] = { 1, 2, 3, 4, 5, 6, 7,6,5,4,3,2,1,6,7,8,9,10,9,8,7,6,7,8,9,10,11,12,13,14 };
	//int mainarr[12] = { 1,2,3,4,1,2,5,1,2,3,4,5 };
	createarr();
	for (int i = 0; i < Mainarrlen; i++)
	{
		p = mainarr[i];				//取出将要访问的页
		if (iflack(p))
		{
			//缺页的情况
			if (arrlen < Max)
			{
				//内存未存满，直接插入
				inserte(p);
				//outputmem();
			}
			else
			{
				//内存存满,使用不同算法选择要替换的内存页,并且替换
				//rpl = BestReplace(mainarr, p, i);
				//rpl = FIFOReplace(p);
				//rpl = LRU(mainarr, p, i);
				//rpl = Clock(p);
				//rpl = PBA(p);
				printf("访问%d时发生缺页置换,%d---->%d\n", p, rpl, p);
				//outputmem();
			}
			lacknum++;
		}//end if
		else
		{
			//不缺页
			//printf("访问%d时没有缺页\n", p);
			//outputmem();
			continue;
		}//end else
	}//end for
	printf("缺页率为%lf", lacknum / 1000.0);
}
