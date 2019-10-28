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
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}LNode, *LinkList;

FILE *fp;

status InitList(LinkList *L);
status DestroyList(LinkList *L);
status ClearList(LinkList *L);
status ListEmpty(LinkList L);
int ListLength(LinkList L);
status GetElem(LinkList L, int i, ElemType *e);
int LocateElem(LinkList L, ElemType e, status(*compare)(ElemType a, ElemType b));
status compare(ElemType a, ElemType b);
status PriorElem(LinkList L, ElemType cur_e, ElemType *pre_e);
status NextElem(LinkList L, ElemType cur_e, ElemType *next_e);
status ListInsert(LinkList *L, int i, ElemType e);
status ListDelete(LinkList *L, int i,ElemType *e);
status ListTrabverse(LinkList L);
status SaveList(LinkList L, char* filename);
status LoadList(LinkList *L, char *filename);

int main(){
	char filename[40];
	int op=1;
    int i,i_num=1;
    LinkList L[MAX_NUM];
    for (i = 0; i<MAX_NUM; i++)
	{
    L[i]=NULL;
	}
	ElemType e, cur_e, pre_e, next_e;
    while(op){
	system("cls");
    printf("\n\n");
    printf("      \t\t\tMenu for Linear Table On Sequence Structure \n");
	printf("  可在%d个顺序表进行多表操作，初始化请先操作功能15,默认在第一个表上操作\n", MAX_NUM);
	printf("  ------------------------------------------------------------------------------\n");
	printf("**\t\t\t1. InitList       7.  LocateElem\t\t\t**\n");
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
	scanf("%d",&op);
    switch(op)
	{
	   case 1:
         //第一种情况是初始化线性表
		 if(InitList(&L[i_num])==OK)
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
		 if(L[i_num] == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
		 if(DestroyList(&L[i_num])==OK)
         {
             printf("销毁线性表成功!\n");
         }
         else printf("销毁线性表失败！\n");
		 getchar();getchar();
		 break;

	   case 3:
		 //用于重置线性表
		 if(L[i_num] == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
         if(ClearList(&L[i_num])==OK)
         {
            printf("线性表重置成功！\n");
         }
         else printf("线性表重置失败！\n");
		 getchar();getchar();
		 break;

	   case 4:
		 //判断是否为空
		 if(L[i_num] == NULL)
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
	       if(L[i_num] == NULL)
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
	       if(L[i_num] == NULL)
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
		 //printf("\n----LocateElem功能待实现！\n");
		 if(L[i_num] == NULL)
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
	       if(L[i_num] == NULL)
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
	       if(L[i_num] == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
         printf("请输入数据元素：\n");
		 scanf("%d",&cur_e);
		 if(NextElem(L[i_num],cur_e,&next_e)==OK)
		 printf("其后继元素为：%d\n",next_e);
		 else if(NextElem(L[i_num],cur_e,&pre_e)==ERROR)
		 printf("顺序表中没有该元素！\n");
		 else
            {printf("其不存在后继元素！\n");}
		 getchar();getchar();
		 break;

	   case 10:
	       //插入元素
	       if(L[i_num] == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
		 printf("请输入您要插入的数据元素：\n");
		 scanf("%d",&e);
		 printf("请输入您要插入的数据元素的位置：\n");
		 scanf("%d",&i);
		 if(ListInsert(&L[i_num],i,e)==OK)
		 printf("插入数据元素成功！\n");
		 else
		 printf("插入数据元素失败！\n");
		 getchar();getchar();
		 break;

	   case 11:
	       //删除元素
	       if(L[i_num] == NULL)
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
	       if(L[i_num] == NULL)
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
           if(L[i_num] == NULL)
         {
             printf("线性表不存在!\n");
             getchar();getchar();
             break;
         }
         if(SaveList(L[i_num], filename)==OK)
		 	printf("文件保存成功!文件名为%s\n",filename);
		 	getchar();getchar();
         break;

	   case 14:
        //加载文件，需要输入需要加载的名称
         InitList(&L[i_num]);
         printf("请输入要加载的文件名:\n ");
		 scanf("%s", filename);
		 if(LoadList(&L[i_num], filename)==OK)
            {
                printf("文件加载成功\n");
		    }
		    getchar();getchar();
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

	   case 0:
         break;
	}//end of switch
  }//end of while
	printf("\t\t欢迎下次再使用本系统！\n");
}//end of main()
/*--------page 23 on textbook --------------------*/


/***************************************************************
*函数名称：IntiaList
*函数功能：构造一个空的线性表
*注释：初始条件是线性表L不存在已存在；操作结果是构造一个空的线性表。
*返回值类型：status类型
****************************************************************/
status InitList(LinkList *L)
{
	*L = (LinkList)malloc(sizeof(LNode));//动态分配
	if(*L == NULL)
	{
		exit(OVERFLOW);//如果没有足够的空间，创建失败
	}
	(*L)->data = 0;
	(*L)->next = NULL;//创建带有表头节点的链表，表头节点的数据域值为0
	return OK;
}

/***************************************************************
*函数名称：DestoryList
*函数功能：销毁线性表
*注释：初始条件是线性表L已存在；操作结果是销毁线性表L
*返回值类型：status类型
****************************************************************/
status DestroyList(LinkList *L)
{
	LinkList p, q;//指针p,q
	p = *L;//将指针p指向表头节点
	while(p)
	{
		q = p->next;//如果p不指向空，将q指向p的下一个节点
		free(p);//然后释放p
		p = q;//再将q所指向的节点赋给p
	}
	*L = NULL;//最后指针L指向空
	return OK;
}

/***************************************************************
*函数名称：ClearList
*函数功能：重置顺序表
*注释：初始条件是线性表L已存在；操作结果是将L重置为空表。
*返回值类型：status类型
****************************************************************/
status ClearList(LinkList *L)
{
	LinkList p, q;//创建两个指针p,q
	p = (*L)->next;//将p指向第一个节点
	while(p)
	{
		q = p->next;//当p不指向空时，q指向p的下一个节点
		free(p);//释放p
		p = q;//将q指向的节点赋给p
	}
	(*L)->next = NULL;//最后，将表头节点的指针域指向空
	return OK;
}

/***************************************************************
*函数名称：ListEmpty
*函数功能：判断线性表是否为空
*注释：初始条件是线性表L已存在；操作结果是若L为空表则返回TRUE,否则返回FALSE。
*返回值类型：status类型
****************************************************************/
status ListEmpty(LinkList L)
{
	if(L->next)
	{
		return FALSE;
	}
	return TRUE;//如果表头节点的指针域指向空，那么返回TRUE
}

/***************************************************************
*函数名称：ListLength
*函数功能：求线性表的表长
*注释：初始条件是线性表已存在；操作结果是返回L中数据元素的个数。
*返回值类型：int类型
****************************************************************/
int ListLength(LinkList L)
{
	int i = 0;//i用来统计次数，即表长
	LinkList p = L->next;//先将p指向表头节点的后一个节点，即第一个节点
	while(p)
	{
		i++;
		p = p->next;//如果p指向不为空，i的次数加一，p指向下一个节点
	}
	return i;//返回次数i，即表长
}


/***************************************************************
*函数名称：GetElem
*函数功能：得到某一个元素的值
*注释：初始条件是线性表已存在，1≤i≤ListLength(L)；操作结果是用e返回L中第i个数据元素的值
*返回值类型：status类型
****************************************************************/
status GetElem(LinkList L, int i, ElemType *e)
{
	int j = 1;
	LinkList p;
	p = L->next;//p指向表头节点后的第一个节点
	while(p && j < i)
	{
		p = p->next;//循环用来找到i位置节点
		++j;
	}
	if(!p || j>i)
	{
		return ERROR;//用来判断输入位置是否正确，空表等
	}
	*e = p->data;//用e取节点的元素
	return OK;
}

/***************************************************************
*函数名称：LocateElem
*函数功能：查找元素
*注释：初始条件是线性表已存在；操作结果是返回L中第1个与e满足关系compare（）
       关系的数据元素的位序，若这样的数据元素不存在，则返回值为0。
*返回值类型：status类型
****************************************************************/
int LocateElem(LinkList L, ElemType e, status(*compare)(ElemType a, ElemType b))
{
	int i = 0;
	LinkList p = L->next;//p指向第一个节点
	while(p)
	{
		i++;
		if((*compare)(p->data, e))//通过遍历法比较得到所要找的元素
		return i;//此时，找到了元素所在位置
		p = p->next;//没有找到时，p指向下一个节点，循环
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
	return TRUE;//比较输入的两个元素的大小，一样大则为TRUE
	else
	return FALSE;
}

/***************************************************************
*函数名称：PriorElem
*函数功能：求元素的前驱
*注释：初始条件是线性表L已存在；操作结果是若cur_e是L的数据元素，且不是第一个，
       则用pre_e返回它的前驱，否则操作失败，pre_e无定义。
*返回值类型：status类型
****************************************************************/
status PriorElem(LinkList L, ElemType cur_e, ElemType *pre_e)
{
	LinkList p = L->next;//p指向第一个节点
	if(p->data==cur_e) return ERROR;//如果第一个节点就是要找的元素，则没有前驱
	while(p->next != NULL && p->next->data != cur_e)
	{
		p = p->next;//通过循环，将p指针指向所要找的元素的前一个节点
	}
	if(p->next == NULL)//如果此时p指针指向空，则意味着表中没有该元素
	return OVERFLOW;

	*pre_e = p->data;//用pre_e取出p指向的节点的元素，即输入元素的前驱
	return OK;
}
/*status PriorElem(LinkList L, ElemType cur_e, ElemType *pre_e)
{
	LinkList p,q;
	p = L;
	q = L->next;
	if(q->data==cur_e) return ERROR;
	while(q!= NULL && q->data != cur_e)
	{
	    p = q;
		q = q->next;
	}
	if(q== NULL && p->data !=cur_e)
	return OVERFLOW;
	else if(q== NULL && p->data ==cur_e){
        return ERROR;
	}
	else {
        *pre_e = p->data;
        return OK;
	}
}*/
/***************************************************************
*函数名称：NextElem
*函数功能：求后继节点
*输入输出：初始条件是线性表L已存在；操作结果是若cur_e是L的数据元素，且不是最后一个，
           则用next_e返回它的后继，否则操作失败，next_e无定义。
*返回值类型：status类型
****************************************************************/
status NextElem(LinkList L, ElemType cur_e, ElemType *next_e)
{
	LinkList p = L->next;//p指向第一个节点
	while(p->next != NULL && p->data != cur_e)
	{
		p = p->next;//循环的方式找到所要找的元素的前一个节点
	}
	if(p->next == NULL && p->data != cur_e)//此时p指针指向空，p指向节点的值不是输入的元素，那么没有输入的元素
	return ERROR;
	if(p->next == NULL && p->data == cur_e)//此时p指针指向空，p指向节点的值是输入的元素，那么没有后继节点
	return OVERFLOW;
	*next_e = p->next->data;//剩余正常情况，用next_e取出p所指向节点的下一个节点的值
	return OK;
}

/***************************************************************
*函数名称：ListInsert
*函数功能：插入元素
*注释：初始条件是线性表L已存在且非空，1≤i≤ListLength(L)+1；
*      操作结果是在L的第i个位置之前插入新的数据元素e
*返回值类型：status类型
****************************************************************/
status ListInsert(LinkList *L, int i, ElemType e)
{
	int j = 1;
	LinkList p, q;//用两个指针确定插入位置的前后节点
	p = *L;//p指向表头节点（考虑到表头插入）
	while(p && j < i)
	{
		p = p->next;//扎到插入位置
		++j;
	}
	if(!p || j > i)
	{
		return ERROR;
	}
	q = (LinkList)malloc(sizeof(LNode));//给插入元素分配空间
	if(q == NULL)
	{
		exit(OVERFLOW);//没分配成功
	}
	q->data = e;
	q->next = p->next;//这三行代码是用来将元素存进去，同时将指针指向问题解决
	p->next = q;
	return OK;
}

/***************************************************************
*函数名称：ListDelete
*函数功能：删除元素
*注释：初始条件是线性表L已存在且非空，1≤i≤ListLength(L)；
*      操作结果：删除L的第i个数据元素，用e返回其值。
*返回值类型：status类型
****************************************************************/
status ListDelete(LinkList *L, int i,ElemType *e)
{
	int j = 1;
	LinkList p ,q;
	p = *L;//p指向表头节点
	while(p->next && j < i)
	{
		p = p->next;//找到删除位置，同时p的下一个节点不为空
		++j;
	}
	if(!(p->next) || j>i)//此时，根据p指针的情况 或者 j 的值判断
	return ERROR;//即要么是空表，要么是删除位置不对。两者均可认为是删除位置不对

	q = p->next;
	p->next = q->next;//这四行代码是删除节点，并用e取出删除节点的值
	*e = q->data;
	free(q);

	return OK;
}

/***************************************************************
*函数名称：ListTrabverse
*函数功能：遍历顺序表
*注释：输出顺序表的值
*返回值类型：status类型
****************************************************************/
status ListTrabverse(LinkList L)
{
   LinkList p = L->next;
   if(!p)//此时为空表
   return ERROR;
   printf("\n---------------------------- all elements -------------------------------------\n");
   while(p)
   {
   	printf("%d ",p->data);//循环，输出值
   	p = p->next;
   }
   printf("\n--------------------------------- end -----------------------------------------\n");
   return OK;
}

/***************************************************************
*函数名称：SaveList
*函数功能：保存线性表
*注释：将线性表保存，参考附录B，其中关于写入元素个数和长度的问题理解不够清楚
*返回值类型：
****************************************************************/
status SaveList(LinkList L, char* filename)
{
	LinkList p = L->next;//指针指向第一个节点
	int listsize=LIST_INIT_SIZE;//最大表长
	if ((fp = fopen(filename, "w")) == NULL)
	{
		printf("\t\t\t文件保存失败！");//文件保存失败
		return ERROR;
	}
	fprintf(fp, "%d ", ListLength(L));//将元素个数写入
	fprintf(fp, "%d ", listsize);//将最大长度写入
	while(p)
    {
	fprintf(fp, "%d ", p->data);//指针op不指向空，依次写入文件
	p = p->next;
    }
	fclose(fp);
	return OK;
}

/***************************************************************
*函数名称：LoadList
*函数功能：加载文件
*注释：加载文件，以便功能的测试，文件名要正确
*返回值类型：status类型
****************************************************************/
status LoadList(LinkList *L, char *filename)
{
	int i = 1,length = 0,listsize;
	ElemType e;
	if ((fp = fopen(filename, "r")) == NULL)
	{
		printf("文件加载失败!");
		return ERROR;
	}
	fscanf(fp, "%d ", &length);
	fscanf(fp, "%d ", &listsize);
	fscanf(fp, "%d ", &e);
	while(i<=length)
    {
	ListInsert(L,i,e);
	fscanf(fp, "%d ", &e);
	i++;
    }
	fclose(fp);
	return OK;
}
