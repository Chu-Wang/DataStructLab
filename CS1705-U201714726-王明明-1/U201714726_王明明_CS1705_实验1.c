/* Linear Table On Sequence Structure */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2
#define MAX_NUM 10

typedef int status;
typedef int ElemType; //数据元素类型定义

/*-------page 22 on textbook -------*/
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef struct{  //顺序表（顺序结构）的定义
	ElemType * elem;
	int length;
	int listsize;
}SqList;

FILE *fp;

/*-----page 19 on textbook ---------*/
status IntiaList(SqList &L);
status DestroyList(SqList &L);
status ClearList(SqList &L);
status ListEmpty(SqList L);
int ListLength(SqList L);
status GetElem(SqList L,int i,ElemType *e);
status LocateElem(SqList L, ElemType e, status(*compare)(ElemType,ElemType));
status compare(ElemType a, ElemType b);
status PriorElem(SqList L,ElemType cur_e,ElemType *pre_e);
status NextElem(SqList L,ElemType cur_e,ElemType *next_e);
status ListInsert(SqList &L,int i,ElemType e);
status ListDelete(SqList *L,int i,ElemType *e);
status ListTrabverse(SqList L);
status SaveList(SqList L, char *filename);
status LoadList(SqList *L, char *filename);
/*--------------------------------------------*/
int main(void){
    char filename[40];
    int op=1;
    int i;
    int i_num=1;
    SqList L[MAX_NUM];
    for(i=0;i<MAX_NUM;i++)
    {
        L[i].elem = NULL;
        L[i].listsize = 0;
        L[i].length = 0;
    }
    //上面的for循环是用来生成没有存储空间的线性表
    ElemType e, cur_e , pre_e, next_e;
  while(op){
        /**
        *利用最简单的printf来制作简易的菜单，可供选择；
        *简洁美观的菜单有助于平复测试时的心情!!!
        */
	system("cls");	//用于清屏
    printf("\n\n");
	printf("      \t\t\tMenu for Linear Table On Sequence Structure \n");
	printf("  可在%d个顺序表进行多表操作，初始化请先操作功能15,默认在第一个表上操作\n", MAX_NUM);
	printf("  ------------------------------------------------------------------------------\n");
	printf("**\t\t\t1. IntiaList       7.  LocateElem\t\t\t**\n");
	printf("**\t\t\t2. DestroyList     8.  PriorElem\t\t\t**\n");
	printf("**\t\t\t3. ClearList       9.  NextElem \t\t\t**\n");
	printf("**\t\t\t4. ListEmpty       10. ListInsert\t\t\t**\n");
	printf("**\t\t\t5. ListLength      11. ListDelete\t\t\t**\n");
	printf("**\t\t\t6. GetElem         12. ListTrabverse\t\t\t**\n");
	printf("**\t\t\t13.SaveList	    14. LoadList\t\t\t**\n");
	printf("**\t\t\t0. Exit            制作时间：2018.11.13\t\t\t**\n");
	printf("**\t\t\t15.ChooseList(请先进行此选项以选择在哪个表上进行操作)\n");
	printf("**\t\t\t本实验已有文件sss，可通过函数14进行加载\n");
	printf("  -------------------------------------王明明-----------------------------------\n");
	printf("请选择你的操作[0--15]:\n");
	scanf("%d",&op);//选择op的值,用于switch
    switch(op){
	   case 1:
         //第一种情况是初始化线性表
		 if(IntiaList(L[i_num])==OK)
         {

             printf("请输入要保存的线性表名称\n");
             scanf("%s", filename);
             printf("线性表创建成功\n");
         }
		     else printf("线性表创建失败！\n");
		 getchar();getchar();
		 break;

	   case 2:
		 //第二种情况是用来销毁线性表
		 if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
		 if(DestroyList(L[i_num])==OK)
         {
             printf("销毁线性表成功!\n");
         }
         else printf("销毁线性表失败！\n");
		 getchar();getchar();
		 break;

	   case 3:
		 //用于重置线性表
		 if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
         if(ClearList(L[i_num])==OK)
         {
            printf("线性表重置成功！\n");
         }
         else printf("线性表重置失败！\n");
		 getchar();getchar();
		 break;

	   case 4:
		 //判断是否为空
		 if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
         if(ListEmpty(L[i_num])==TRUE)
         {
             printf("文件为空！\n");
         }
         else printf("线性表不是空表！\n");
		 getchar();getchar();
		 break;

	   case 5:
	       //得到线性表长度
	       if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
         printf("线性表表长为%d\n",ListLength(L[i_num]));
		 getchar();getchar();
		 break;

	   case 6:
	       //得到某个元素
	       if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
         printf("请输入要取结点的位置：\n");
		 scanf("%d",&i);
		 if(GetElem(L[i_num],i,&e)==OK)
		 printf("第%d个结点的元素是：%d\n",i,e);
		 else  printf("输入位置错误！\n");
		 getchar();getchar();
		 break;

	   case 7:
	       //确定元素位置，容易出错
	       if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
         printf("请输入数据元素值：\n");
		 scanf("%d",&e);
		 if(i=LocateElem(L[i_num],e,compare))
		 printf("%d元素位于第%d个位置！\n",e,i);
		 else  printf("该元素不存在!\n");
		 getchar();getchar();
		 break;

	   case 8:
	       //求出前驱结点
	       if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
		 printf("请输入数据元素：\n");
		 scanf("%d",&cur_e);
		 PriorElem(L[i_num],cur_e,&pre_e);
		 if(PriorElem(L[i_num],cur_e,&pre_e)==OK)
		 printf("其前驱元素为：%d\n",pre_e);
		 else if(PriorElem(L[i_num],cur_e,&pre_e)==OVERFLOW)
		 printf("顺序表中没有该元素！\n");
		 else  printf("其不存在前驱元素！\n");
		 getchar();getchar();
		 break;

	   case 9:
	       //求出后置节点
	       if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
         printf("请输入数据元素：\n");
		 scanf("%d",&cur_e);
		 if(NextElem(L[i_num],cur_e,&next_e)==OK)
		 printf("其后继元素为：%d\n",next_e);
		 else if(NextElem(L[i_num],cur_e,&pre_e)==FALSE)
		 printf("其不存在后继元素！\n");
		 else
            {printf("顺序表中没有该元素！\n");}
		 getchar();getchar();
		 break;

	   case 10:
	       //插入元素
	       if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
		 printf("请输入您要插入的数据元素：\n");
		 scanf("%d",&e);
		 printf("请输入您要插入的数据元素的位置：\n");
		 scanf("%d",&i);
		 if(ListInsert(L[i_num],i,e)==OK)
		 printf("插入数据元素成功！\n");
		 else
		 printf("插入数据元素失败！\n");
		 getchar();getchar();
		 break;

	   case 11:
	       //删除元素
	       if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
		 printf("请输入您要删除的数据元素的位置：\n");
		 scanf("%d",&i);
		 if(ListDelete(&L[i_num],i,&e)==OK)
		 printf("删除数据元素成功！\n");
		 else
		 printf("删除数据元素失败！\n");
		 getchar();getchar();
		 break;

	   case 12:
	       //遍历线性表中的元素
	       if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
         if(!ListTrabverse(L[i_num])) printf("线性表是空表！\n");
		 getchar();getchar();
		 break;

       case 13:
           //保存文件
           if(L[i_num].elem == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
         if(SaveList(L[i_num], filename)==OK)
		 	printf("文件保存成功\n文件名为%s\n",filename);
         break;

       case 14:
        //加载文件，需要输入需要加载的名称
         printf("请输入要加载的文件名:\n ");
		 scanf("%s", filename);
		 if(LoadList(&L[i_num], filename)==OK)
            {
                printf("文件加载成功\n");
		    }
         break;

       case 15:
           //选择在哪个表进行操作
           printf("请输入要在第几个表操作:\n ");
           printf("*只支持在%d个顺序表上操作*\n",MAX_NUM);
		   scanf("%d",&i_num);
		   printf("正在对第%d个表进行操作\n",i_num);
		 if((i_num<1)||(i_num>10))
		 {
		 	printf("请选择正确范围！\n");
		 	i_num=1;
		 }
		 getchar(); getchar();
		 break;
         break;
	   case 0:
        //退出菜单，退出整个程序
         break;
	}//end of switch
  }//end of while
  printf("欢迎下次再使用本辣鸡系统!\n");
}//end of main()
/*--------page 23 on textbook --------------------*/
/***************************************************************
*函数名称：IntiaList
*函数功能：构造一个空的线性表
*注释：初始条件是线性表L不存在已存在；操作结果是构造一个空的线性表。
*返回值类型：status类型
****************************************************************/
status IntiaList(SqList &L){
	L.elem = (ElemType *)malloc( LIST_INIT_SIZE * sizeof (ElemType));
    if(!L.elem) exit(OVERFLOW);//如果空间不足，创建失败
	L.length=0;
    L.listsize=LIST_INIT_SIZE;
	return OK;
}


/***************************************************************
*函数名称：DestoryList
*函数功能：销毁线性表
*注释：初始条件是线性表L已存在；操作结果是销毁线性表L
*返回值类型：status类型
****************************************************************/
status DestroyList(SqList &L)
{
	if(L.elem)
	free(L.elem);
	L.elem = NULL;
	L.length = 0;
	L.listsize = 0;
	return OK;
}


/***************************************************************
*函数名称：ClearList
*函数功能：重置顺序表
*注释：初始条件是线性表L已存在；操作结果是将L重置为空表。
*返回值类型：status类型
****************************************************************/
status ClearList(SqList &L)
{
    L.length=0;
    return OK;
}



/***************************************************************
*函数名称：ListEmpty
*函数功能：判断线性表是否为空
*注释：初始条件是线性表L已存在；操作结果是若L为空表则返回TRUE,否则返回FALSE。
*返回值类型：status类型
****************************************************************/
status ListEmpty(SqList L)
{
    if(L.length==0)
    {
        return TRUE;
    }
    return FALSE;

}



/***************************************************************
*函数名称：ListLength
*函数功能：求线性表的表长
*注释：初始条件是线性表已存在；操作结果是返回L中数据元素的个数。
*返回值类型：int类型
****************************************************************/
int ListLength(SqList L)
{
    return L.length;
}


/***************************************************************
*函数名称：GetElem
*函数功能：得到某一个元素的值
*注释：初始条件是线性表已存在，1≤i≤ListLength(L)；操作结果是用e返回L中第i个数据元素的值
*返回值类型：status类型
****************************************************************/
status GetElem(SqList L,int i,ElemType *e)
{
   if(i<1||i>L.length)
   {
    return ERROR;
   }
   *e = L.elem[i-1];
   return OK;
}




/***************************************************************
*函数名称：LocateElem
*函数功能：查找元素
*注释：初始条件是线性表已存在；操作结果是返回L中第1个与e满足关系compare（）
       关系的数据元素的位序，若这样的数据元素不存在，则返回值为0。
*返回值类型：status类型
****************************************************************/
status LocateElem(SqList L,ElemType e,status(*compare)(ElemType,ElemType))
{
    int i;
    for(i=0;i<L.length;i++)
    {
        if(compare(L.elem[i],e))
            return ++i;
    }
    return 0;
}



/***************************************************************
*函数名称：compare
*函数功能：比较大小，服务于LocateList函数
*注释：输入两个ElemType类型的值
*返回值类型：status类型
****************************************************************/
status compare(ElemType a, ElemType b)
{
	if(a == b)
	return TRUE;
	else  return FALSE;
}




/***************************************************************
*函数名称：PriorElem
*函数功能：求元素的前驱
*注释：初始条件是线性表L已存在；操作结果是若cur_e是L的数据元素，且不是第一个，
       则用pre_e返回它的前驱，否则操作失败，pre_e无定义。
*返回值类型：status类型
****************************************************************/
status PriorElem(SqList L,ElemType cur_e,ElemType *pre_e)
{
  int i;
  for(i=0;i<L.length;i++)
  {
      if(L.elem[i]==cur_e && i==0)
      {
          return ERROR;
      }
      else if(L.elem[i]== cur_e)
      {
          *pre_e = L.elem[i-1];
          return OK;
      }
  }
  return OVERFLOW;
}





/***************************************************************
*函数名称：NextElem
*函数功能：求后继节点
*输入输出：初始条件是线性表L已存在；操作结果是若cur_e是L的数据元素，且不是最后一个，
           则用next_e返回它的后继，否则操作失败，next_e无定义。
*返回值类型：status类型
****************************************************************/
status NextElem(SqList L,ElemType cur_e,ElemType *next_e)
{
    int i;
    for(i=0;i<(L.length-1);i++)
    {
        if(L.elem[i]==cur_e)
        {
            *next_e = L.elem[i+1];
            return OK;
        }

    }
    if(i==L.length-1 && (L.elem[i]!=cur_e)) return OVERFLOW;
    else return FALSE;
}



/***************************************************************
*函数名称：ListInsert
*函数功能：插入元素
*注释：初始条件是线性表L已存在且非空，1≤i≤ListLength(L)+1；
*      操作结果是在L的第i个位置之前插入新的数据元素e
*返回值类型：status类型
****************************************************************/
status ListInsert(SqList &L,int i,ElemType e)
{
    int *p,*q,*newbase;
    if(i<1||i>L.length+1)
    {
        printf("插入位置不正确!\n");
        return ERROR;
    }

    if(L.length>=L.listsize){
        newbase = (ElemType *)realloc(L.elem,(L.listsize + LISTINCREMENT)*sizeof(ElemType));
        if(!newbase) exit(OVERFLOW);
        L.elem = newbase;
        L.listsize += LISTINCREMENT;
    }
    q = &(L.elem[i-1]);
    for(p=&(L.elem[L.length-1]);p>=q;--p) *(p+1) = *p;
    *q=e;
    ++L.length;
	return OK;

}
//这是课本上面的关于插入算法的实现





/***************************************************************
*函数名称：ListDelete
*函数功能：删除元素
*注释：初始条件是线性表L已存在且非空，1≤i≤ListLength(L)；
*      操作结果：删除L的第i个数据元素，用e返回其值。
*返回值类型：status类型
****************************************************************/
status ListDelete(SqList *L,int i,ElemType *e)
{
    if(i<1||i>L->length)
        return ERROR;//删除的位数不正确
	int j;
	*e=L->elem[i-1];
	for (j = i - 1; j<L->length; j++)
		L->elem[j] = L->elem[j + 1];
	L->length--;
	return OK;
}




/***************************************************************
*函数名称：ListTrabverse
*函数功能：遍历顺序表
*注释：输出顺序表的值
*返回值类型：status类型
****************************************************************/
status ListTrabverse(SqList L){
   int i;
   printf("\n-----------all elements -----------------------\n");
   for(i=0;i<L.length;i++) printf("%d ",L.elem[i]);
   printf("\n------------------ end ------------------------\n");
   return L.length;
}






/***************************************************************
*函数名称：SaveList
*函数功能：保存线性表
*注释：将线性表保存，参考附录B，其中关于写入元素个数和长度的问题理解不够清楚
*返回值类型：
****************************************************************/
status SaveList(SqList L, char* filename)
{
	int i = 0;
	if ((fp = fopen(filename, "w")) == NULL)
	{
		printf("文件保存失败\n");
		return ERROR;
	}
	fprintf(fp, "%d ", L.length);//保存的时候，也将L的长度保存到了文件
	fprintf(fp, "%d ", L.listsize);//将每个元素的大小也保存到了文件里
	while (i < L.length)
		fprintf(fp, "%d ", L.elem[i++]);//利用循环，将元素依次存进去
	fclose(fp);//关闭文件
	return OK;
}







/***************************************************************
*函数名称：LoadList
*函数功能：加载文件
*注释：加载文件，以便功能的测试，文件名要正确
*返回值类型：status类型
****************************************************************/
status LoadList(SqList *L, char *filename)
{
	int i = 0;
	if ((fp = fopen(filename, "r")) == NULL)
	{
		printf("文件加载失败\n");
		return ERROR;
	}
	fscanf(fp, "%d ", &L->length);
	fscanf(fp, "%d ", &L->listsize);
	L->elem = (ElemType *)malloc(L->listsize * sizeof(ElemType));
	if (!L->elem) exit(OVERFLOW);
	while (i < L->length)
		fscanf(fp, "%d ", &L->elem[i++]);//利用循环，依次读出文件中的内容
	fclose(fp);
	return OK;
}

