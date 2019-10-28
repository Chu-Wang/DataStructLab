/* Linear Table On Sequence Structure */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stack>
#include <queue>

/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2
#define MAX_NUM 10

typedef int status;
typedef char TElemType;
//数据元素类型定义
/*-------page 22 on textbook -------*/
#define LIST_INIT_SIZE 100
#define LISTINCRE MENT  10

/*-----page 19 on textbook ---------*/
typedef struct BiTNode{
	int key;//用key作为标记，便于查找节点
	TElemType data;//char类型数据域
	struct BiTNode *lchild, *rchild;//定义二叉链表的左孩子指针与右孩子指针
}BiTNode, *BiTree;//BiTNode类型指针BiTree

typedef struct {
	BiTree T;//创建二叉树用的指针T
	char name[20];//用于保存树的名称
}LElemType;

typedef struct {
	LElemType tree[20];//多个树进行操作
	int length;
	int listsize;
}SqList;

status InitBiTree(BiTree *T);
status DestroyBiTree(BiTree *T);
status CreateBiTree(BiTree *T);
status ClearBiTree(BiTree T);
status BiTreeEmpty(BiTree T);
status BiTreeDepth(BiTree T);
char Root(BiTree T);
char Value(BiTree T, int e);
status Assign(BiTree T, int e, char ch);
BiTNode* Parent(BiTree T, int e);
BiTNode* LeftChild(BiTree T, int e);
BiTNode* RightChild(BiTree T, int e);
BiTNode* RightSibling(BiTree T, int e);
BiTNode* LeftSibling(BiTree T, int e);
BiTree Find(BiTree T, int key);
status InsertChild(BiTree T, BiTree p, int LR, BiTree c);
status DeleteChild(BiTree T, BiTree p, int LR);
status visit(char e);
status PreOrderTraverse(BiTree T, status (* visit)(char e));
status InOrderTraverse(BiTree T, status (* visit)(char e));
status PostOrderTraverse(BiTree T, status (* visit)(char e));
status LevelOrderTraverse(BiTree T, status (* visit)(char e));
status TreeDisplay(BiTree T,int depth,status (* visit)(char e));
status Save(BiTree T,FILE *fp);
status Load(BiTree *T,FILE *fp);

int main(){
	FILE *fp;
	char filename[30];
	SqList L;//相当于用数组构建多树操作的框架
	BiTree T1, T2;//两个用BiTree构建的BiTNode指针
	int op=1, key;//op用来case,便于用户操作;key是用来标记节点
    int i, num=1, LR;//num默认为1，即默认在第一棵树进行操作，LR用来表示方向
    char ch;//用来接收data域的值
    for(i=0;i<20;i++)
    {
        L.tree[i].T = NULL;//相当于创建20个定义中类型的树的指针，同时指空，即20个树；
    }

    while(op){
	system("cls");//系统函数，用于清屏
	system("color F");
    printf("\n\n");
	printf("\t\t      Menu for Binary Tree On Binary Linked List \n");
	printf("\t*****************************************************************************\n");
	printf("\t\t\t1.  InitBiTree       \t  2.  DestroyBiTree\n");
	printf("\t\t\t3.  CreateBiTree     \t  4.  ClearBiTree\n");
	printf("\t\t\t5.  BiTreeEmpty      \t  6.  BiTreeDepth\n");
	printf("\t\t\t7.  Root             \t  8.  Value\n");
	printf("\t\t\t9.  Assign           \t  10. Parent\n");
	printf("\t\t\t11. LeftChild        \t  12. RightChild\n");
	printf("\t\t\t13. LeftSibling	   \t  14. RightSibling\n");
	printf("\t\t\t15. InsertChild	   \t  16. DeleteChild\n");
	printf("\t\t\t17. PreOrderTraverse      18. InOrderTraverse\n");
	printf("\t\t\t19. PostOrderTraverse     20. LevelOrderTraverse\n");
	printf("\t\t\t21. Choose(多树操作)      22. Save(保存文件)\n");
	printf("\t\t\t23. Load(加载文件)        0.  Exit\n");
	printf("\t\t\t24. TreeDisplay           Powered By @_@||王明明\n");
	printf("\t****************************************************************************\n");
	printf("\t\t\t请选择你的操作[0~24]:");
	scanf("%d",&op);
    switch(op)
	{

	   case 1:
        system("color 3");
	    InitBiTree(&(L.tree[num-1].T));
	    printf("\t\t\t二叉树创建成功！\n");
		getchar();getchar();
		break;

	   case 2:
        system("color 2");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	    DestroyBiTree(&(L.tree[num-1].T));
	    printf("\t\t\t二叉树销毁成功！\n");
		getchar();getchar();
		break;

	   case 3:
        system("color F");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	   	getchar();
	   	printf("\t\t\t请用前序方式构建二叉树，#表示空结点！\n");
	    CreateBiTree(&L.tree[num-1].T);
	    printf("\t\t\t二叉树构造成功！\n");
		getchar();getchar();
		break;

	   case 4:
        system("color D");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	    if(ClearBiTree(L.tree[num-1].T) == 1)
	    printf("\t\t\t叉树清空成功！\n");
	    else
	    printf("\t\t\t二叉树清空失败！\n");
		getchar();getchar();
		break;

	   case 5:
        system("color 8");
	   	if (L.tree[num-1].T==NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	    if(BiTreeEmpty(L.tree[num-1].T) == OK)
	    printf("\t\t\t二叉树为空树！\n");
	    else
	    printf("\t\t\t二叉树不为空树！\n");
		getchar();getchar();
		break;

	   case 6:
        system("color A");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
		if(BiTreeEmpty(L.tree[num-1].T) == OK)
	    printf("\t\t\t二叉树为空树！\n");
	    else
	    printf("\t\t\t二叉树的深度为:%d\n", BiTreeDepth(L.tree[num-1].T));
		getchar();getchar();
		break;

	   case 7:
        system("color C");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	    printf("\t\t\t二叉树的根为:%c\n", Root(L.tree[num-1].T));
		getchar();getchar();
		break;

	   case 8:
        system("color D");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	    printf("\t\t\t请输入你要查找的key:");
		scanf("%d", &key);
		if (Value(L.tree[num-1].T, key) != ERROR)
        {
            printf("\t\t\t该key的data为：%c\n", Value(L.tree[num-1].T, key));
            if (Parent(L.tree[num-1].T, key) != NULL)
            {
            printf("\t\t\t该key双亲的data为：%c\n", Parent(L.tree[num-1].T, key)->data);
		    printf("\t\t\t该key双亲的key为：%d\n", Parent(L.tree[num-1].T, key)->key);
            }
		   else{
                  printf("\t\t\t该key双亲不存在！\n");
		       }

            if (LeftChild(L.tree[num-1].T, key) != NULL)
            {
            printf("\t\t\t该key左孩子对应的data为：%c\n", LeftChild(L.tree[num-1].T, key)->data);
		    printf("\t\t\t该key左孩子对应的key为：%d\n", LeftChild(L.tree[num-1].T, key)->key);
            }
		   else{
                  printf("\t\t\t该key左孩子不存在！\n");
               }

            if (RightChild(L.tree[num-1].T, key) != NULL)
               {
                 printf("\t\t\t该key右孩子对应的data为：%c\n", RightChild(L.tree[num-1].T, key)->data);
                 printf("\t\t\t该key右孩子对应的key为：%d\n",  RightChild(L.tree[num-1].T, key)->key);
               }
           else{
                 printf("\t\t\t该key右孩子不存在！\n");
           }

             if (LeftSibling(L.tree[num-1].T, key) != NULL)
              {
            printf("\t\t\t该key左兄弟对应的data为：%c\n", LeftSibling(L.tree[num-1].T, key)->data);
            printf("\t\t\t该key左兄弟对应的key为：%d\n", LeftSibling(L.tree[num-1].T, key)->key);
              }

		   else{
            printf("\t\t\t该key左兄弟不存在！\n");
		   }
		   if (RightSibling(L.tree[num-1].T, key) != NULL)
            {
            printf("\t\t\t该key右兄弟对应的data为：%c\n", RightSibling(L.tree[num-1].T, key)->data);
            printf("\t\t\t该key右兄弟对应的key为：%d\n", RightSibling(L.tree[num-1].T, key)->key);
            }

		else{
            printf("\t\t\t该key右兄弟不存在！\n");
		    }
        }

		else
		printf("\t\t\t输入的key在树中不存在！\n");
		getchar();getchar();
		break;

	   case 9:
        system("color A");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	    printf("\t\t\t请输入你要查找的key:");
		scanf("%d", &key);
		getchar();
		printf("\t\t\t请输入你要重新赋值的data:");
		scanf("%c", &ch);
		if (Assign(L.tree[num-1].T, key, ch) != ERROR)
		printf("\t\t\t该key对应的data改为了：%c\n", ch);
		else
		printf("\t\t\t输入的key在树种不存在！\n");
		getchar();getchar();
		break;

	   case 10:
        system("color B");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	    printf("\t\t\t请输入你要查找父母的key:");
		scanf("%d", &key);
		if (Parent(L.tree[num-1].T, key) != NULL)
           {
            printf("\t\t\t双亲对应的data为：%c\n", Parent(L.tree[num-1].T, key)->data);
		    printf("\t\t\t双亲对应的key为：%d\n", Parent(L.tree[num-1].T, key)->key);
           }

		else
		printf("\t\t\t双亲不存在！\n");
		getchar();getchar();
		break;

	   case 11:
        system("color C");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	    printf("\t\t\t请输入你要查找左孩子的key: ");
		scanf("%d", &key);
		if (LeftChild(L.tree[num-1].T, key) != NULL)
          {
            printf("\t\t\t左孩子对应的data为：%c\n", LeftChild(L.tree[num-1].T, key)->data);
		    printf("\t\t\t左孩子对应的key为：%d\n", LeftChild(L.tree[num-1].T, key)->key);
          }

		else
		printf("\t\t\t左孩子不存在！\n");
		getchar();getchar();
		break;

	   case 12:
        system("color D");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	    printf("\t\t\t请输入你要查找右孩子的key: ");
		scanf("%d", &key);
		if (RightChild(L.tree[num-1].T, key) != NULL)
        {
            printf("\t\t\t右孩子对应的data为：%c\n", RightChild(L.tree[num-1].T, key)->data);
            printf("\t\t\t右孩子对应的key为：%d\n",  RightChild(L.tree[num-1].T, key)->key);
        }

		else
		printf("\t\t\t右孩子不存在！\n");
		getchar();getchar();
		break;

	   case 13:
        system("color E");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	    printf("\t\t\t请输入你要查找左兄弟的key: ");
		scanf("%d", &key);
		if (LeftSibling(L.tree[num-1].T, key) != NULL)
        {
            printf("\t\t\t左兄弟对应的data为：%c\n", LeftSibling(L.tree[num-1].T, key)->data);
            printf("\t\t\t左兄弟对应的key为：%d\n", LeftSibling(L.tree[num-1].T, key)->key);
        }

		else
		printf("\t\t\t左兄弟不存在！\n");
		getchar();getchar();
		break;

	   case 14:
        system("color F");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
	    printf("\t\t\t请输入你要查找右兄弟的key: ");
		scanf("%d", &key);
		if (RightSibling(L.tree[num-1].T, key) != NULL)
        {
            printf("\t\t\t右兄弟对应的data为：%c\n", RightSibling(L.tree[num-1].T, key)->data);
            printf("\t\t\t右兄弟对应的key为：%d\n", RightSibling(L.tree[num-1].T, key)->key);
        }

		else
		printf("\t\t\t右兄弟不存在！\n");
		getchar();getchar();
		break;

	   case 15:
        system("color D");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
		printf("\t\t\t请输入你要插入位置的key: ");
		scanf("%d", &key);
		getchar();
		T1 = Find(L.tree[num-1].T, key);
		printf("\t\t\t请输入你想要插入的方向,0表示左边,1表示右边: ");
		scanf("%d", &LR);
		getchar();
		CreateBiTree(&T2);
		if (InsertChild(L.tree[num-1].T, T1, LR, T2) == OK)
		{
		    printf("\t\t\t插入成功!\n");
			getchar(); getchar();
		}
		else
		{
		    printf("\t\t\t插入失败!\n");
			getchar(); getchar();
		}
		break;

	   case 16:
        system("color A");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
		printf("\t\t\t请输入你要删除位置的key: ");
		scanf("%d", &key);
		getchar();
		T1 = Find(L.tree[num-1].T, key);
		printf("\t\t\t请输入你想要删除的方向,0表示左边,1表示右边: ");
		scanf("%d", &LR);
		getchar();
		if (DeleteChild(L.tree[num-1].T, T1, LR) == OK)
		{
		    printf("\t\t\t删除成功!\n");
			getchar(); getchar();
		}
		else
		{
		    printf("\t\t\t删除失败!\n");
			getchar(); getchar();
		}
		break;

	   case 17:
        system("color C");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
		printf("\t\t\t该二叉树的前序遍历为：\n");
	   	PreOrderTraverse(L.tree[num-1].T, visit);
		getchar();getchar();
		break;

	   case 18:
        system("color E");
	   	if (!L.tree[num-1].T)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
		printf("\t\t\t该二叉树的中序遍历为：\n");
	   	InOrderTraverse(L.tree[num-1].T, visit);
		getchar();getchar();
		break;

	   case 19:
        system("color F");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
		printf("\t\t\t该二叉树的后序遍历为：\n");
	   	PostOrderTraverse(L.tree[num-1].T, visit);
		getchar();getchar();
		break;

	   case 20:
        system("color D");
	   	if (L.tree[num-1].T == NULL)
	   	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		}
		printf("\t\t\t该二叉树的层序遍历为：\n");
	   	LevelOrderTraverse(L.tree[num-1].T, visit);
		getchar();getchar();
		break;

	   case 21:
        system("color 2");
	   	printf("\t\t\t在第几个树操作?只支持20个树进行操作: \n");
		scanf("%d",&num);
		if(num<1||num>20)
		{
			printf("\t\t\t请选择正确范围！\n");
			num=1;
		}
		getchar(); getchar();
		break;

	   case 22:
        system("color D");
	   	printf("\t\t\t请输入要保存的文件名: ");
		scanf("%s", filename);
	    if((fp=fopen(filename,"w"))==NULL) printf("\t\t\t打开文件失败！\n");
       	else
		{
       		if(Save(L.tree[num-1].T,fp)==OK)
			   printf("\t\t\t保存文件成功！\n");
       		else printf("\t\t\t保存文件失败！\n");
		}
		fclose(fp);
		getchar();getchar();
		break;

	   case 23:
        system("color B");
	   printf("\t\t\t请输入要加载的文件名: ");
       	scanf("%s",filename);
       	if((fp=fopen(filename,"r"))==NULL)
		   printf("\t\t\t加载失败！\n");
       	else
		{
       		if(Load(&L.tree[num-1].T,fp)==OK)
			printf("\t\t\t加载成功！\n");
       		else printf("\t\t\t加载失败！\n");
		}
		fclose(fp);
		getchar();getchar();
		break;

		case 24:
		    system("color A");
            if (!L.tree[num-1].T)
	    	{
	   		printf("\t\t\t二叉树不存在！\n");
	   		getchar();getchar();
	   		break;
		    }
            if(BiTreeEmpty(L.tree[num-1].T) == OK)
	           printf("\t\t\t二叉树为空树！\n");
            else{
               printf("\t\t\t该二叉树为:\n");
               TreeDisplay((L.tree[num-1].T),1, visit);
                }
               getchar();getchar();
        break;

	   case 0:
        break;
	}//end of switch
  }//end of while
    printf("\n");
	printf("\t\t\t欢迎下次再使用本系统！\n\n");
	system("color 9");
	printf("\t\t\tPowered By@_@||王明明\n\n");
	//system("pause");
}//end of main()

/*--------page 23 on textbook --------------------*/
/**
 * 函数名称: InitBiTree
 * 初始条件：二叉树T不存在
 * 操作结果: 构造空树二叉树T
 *
 */
status InitBiTree(BiTree *T) {
	*T = (BiTree)malloc(sizeof(BiTNode));
	(*T)->lchild = NULL;
	(*T)->rchild = NULL;
	(*T)->data='#';//初始化二叉树,将左右指针指向空,data域设为#
	return OK;
}

/**
 * 函数名称: DestroyBiTree
 * 初始条件：二叉树T已存在
 * 操作结果: 销毁二叉树T
 *
 */
status DestroyBiTree(BiTree *T)
{
	if (*T) {
		if ((*T)->lchild)
			DestroyBiTree(&((*T)->lchild));
		if ((*T)->rchild)
			DestroyBiTree(&((*T)->rchild));
		free(*T);
		(*T) = NULL;//使用递归依次释放左子树、右子树、根节点指针
	}
	return OK;
}

/**
 * 函数名称: CreateBiTree
 * 初始条件：二叉树T已存在
 * 操作结果: 创建二叉树
 *
 */
status CreateBiTree(BiTree *T)
{
	char ch;
	printf("\t\t\t请输入data: ");
	scanf("%c", &ch);
	getchar();
	if(ch == '#')
	{
		*T = NULL;
		return 0;
	}
	else
	{
		*T = (BiTree)malloc(sizeof(BiTNode));
		(*T)->data = ch;
		printf("\t\t\t请输入key:");
		scanf("%d", &(*T)->key);
		getchar();
		CreateBiTree(&((*T)->lchild));
		CreateBiTree(&((*T)->rchild));
	}
	return OK;
}
//取材于课本,依次输入节点值，并用key标记节点

/**
 * 函数名称: ClearBiTree
 * 初始条件：二叉树T已初始化
 * 操作结果: 构造二叉树
 *
 */
status ClearBiTree(BiTree T)
{
	if(T)
	{
		T->lchild = NULL;
		T->rchild = NULL;
		T->data = '#';//清空二叉树,左右子树指针指向空,data域设置为#
	}
	return OK;
}

/**
 * 函数名称: BiTreeEmpty
 * 初始条件：二叉树T已存在
 * 操作结果: 若T为空二叉树则返回TRUE，否则返回FALSE
 *
 */
status BiTreeEmpty(BiTree T)
{
	if(T->data=='#')//判断根节点的data域值是否为空
	{
		return OK;
	}
	else
	{
		return ERROR;
	}
}

/**
 * 函数名称: BiTreeDepth
 * 初始条件：二叉树T已存在
 * 操作结果: 返回T的深度
 *
 */
status BiTreeDepth(BiTree T)
{
	int depth = 0;
	if(T)
	{
		int lchilddepth = BiTreeDepth(T->lchild);
		int rchilddepth = BiTreeDepth(T->rchild);
		depth = (lchilddepth>=rchilddepth?(lchilddepth+1):(rchilddepth+1));
	}//使用递归,得到左右子树的深度,并比较大小,之后返回最大的深度
	return depth;
}

/**
 * 函数名称: Root
 * 初始条件：二叉树T已存在
 * 操作结果: 返回T的根
 *
 */
char Root(BiTree T)
{
	return T->data;//T的data域的值即根节点
}

/**
 * 函数名称: visit
 * 操作结果: 打印字符e
 */
status visit(char e)
{
	printf("%c",e);//依次调用该函数，用来打印
}

/**
 * 函数名称: Value
 * 初始条件：二叉树T已存在, e是T中的某个结点
 * 操作结果: 返回e的值
 *
 */
char Value(BiTree T, int e)
{
	if (!T) return ERROR;   //若二叉树为空,返回ERROR
	BiTNode *st[10], *p;
	int top = 0;  //置空栈
	st[top++] = T;
	while (top)
	{
		p = st[--top]; //先序遍历,弹出栈顶元素,判断是否有key的值与e相等
		if (p->key == e)
        {
            return p->data;
        }

		else {
			if(p->rchild!=NULL)
				st[top++] = p->rchild;
			if(p->lchild!=NULL)
				st[top++] = p->lchild;
		}
	}
	return ERROR;
}

/**
 * 函数名称: Assign
 * 初始条件：二叉树T已存在, e是T中的某个结点
 * 操作结果: 结点e赋值为value
 *
 */
status Assign(BiTree T, int e, char ch)
{
	if (!T) return ERROR;
    BiTNode *st[10], *p;
	int top = 0;
	st[top++] = T;
	while (top)
	{
		p = st[--top];
		if (p->key == e)
		{
			p->data =ch;//找到后进行复制,与Value函数相似
			return OK;
		}
		else
		{
			if (p->rchild != NULL)
				st[top++] = p->rchild;
			if (p->lchild != NULL)
				st[top++] = p->lchild;
		}
	}
	return ERROR;
}

/**
 * 函数名称: Parent
 * 初始条件：二叉树T已存在, e是T中的某个结点
 * 操作结果: 若e是T的非根结点，则返回它的双亲结点指针，否则返回NULL
 *
 */
BiTNode* Parent(BiTree T, int e)
{
	BiTree T1;//利用遍历和递归依次寻找左孩子右孩子对应的e值,符合条件便返回指针
	if (T)
	{
		if ((T->lchild!=NULL&&T->lchild->key == e) ||(T->rchild!=NULL&& T->rchild->key == e)) return T;
		T1 = Parent(T->lchild, e);
		if (T1 != NULL) return T1;
		T1 = Parent(T->rchild, e);
		if (T1 != NULL) return T1;
	}
	return NULL;
}

/**
 * 函数名称: LeftChild
 * 初始条件：二叉树T已存在, e是T中的某个结点
 * 操作结果: 返回e的左孩子结点指针。若e无左孩子，则返回NULL
 *
 */
BiTNode* LeftChild(BiTree T, int e)
{
	BiTree p;//利用递归找左孩子,思路简单
	if (T)
	{
		if (T->key == e) return T->lchild;
		p = LeftChild(T->lchild, e);
		if (p != NULL) return p;
		p = LeftChild(T->rchild, e);
		if (p != NULL) return p;
	}
	return NULL;
}

/**
 * 函数名称: RightChild
 * 初始条件：二叉树T已存在, e是T中的某个结点
 * 操作结果: 返回e的右孩子结点指针。若e无右孩子，则返回NULL
 *
 */
BiTNode* RightChild(BiTree T, int e)
{
	BiTree p;//与上一个同理
	if (T)
	{
		if (T->key == e) return T->rchild;
		p = RightChild(T->lchild, e);
		if (p != NULL) return p;
		p = RightChild(T->rchild, e);
		if (p != NULL) return p;
	}
	return NULL;
}

/**
 * 函数名称: LeftSibling
 * 初始条件：二叉树T已存在, e是T中的某个结点
 * 操作结果: 返回e的左兄弟结点指针。若e是T的左孩子或者无左兄弟，则返回NULL
 *
 */
BiTNode* LeftSibling(BiTree T, int e)
{
	BiTree p=NULL;
	if (T)
	{
		if (T->rchild!=NULL&&T->rchild->key == e)//如果右孩子不为空并且节点的值符合,那么便返回左孩子的指针
		return T->lchild;
		p = LeftSibling(T->lchild, e);
		if (p != NULL) return p;
		p = LeftSibling(T->rchild, e);
		if (p != NULL) return p;
	}
	return NULL;
}

/**
 * 函数名称: RightSibling
 * 初始条件：二叉树T已存在, e是T中的某个结点
 * 操作结果: 返回e的左兄弟结点指针。若e是T的左孩子或者无左兄弟，则返回NULL回e的右兄弟结点指针。若e是T的右孩子或者无有兄弟，则返回NULL
 *
 */
BiTNode* RightSibling(BiTree T, int e)
{
	BiTree p=NULL;
	if (T)
	{
		if (T->lchild!=NULL&&T->lchild->key == e)//与上一个函数类似,最后依次递归遍历左子树右子树
		return T->rchild;
		p = RightSibling(T->lchild, e);
		if (p != NULL) return p;
		p = RightSibling(T->rchild, e);
		if (p != NULL) return p;
	}
	return NULL;
}

BiTree Find(BiTree T, int key)
{
	BiTree T1;
	if (T == NULL) return NULL;
	if (T->key == key) return T;
	else
	{
		T1 = Find(T->lchild, key);
		if (T1 != NULL) return T1;
		T1 = Find(T->rchild, key);
		if (T1 != NULL) return T1;
	}
	return NULL;
}

/**
 * 函数名称: InsertChild
 * 初始条件：二叉树T存在，p指向T中的某个结点，LR为0或1
 * 操作结果: 根据LR为0或者1，插入c为T中p所指结点的左或右子树，p	所指结点的原有左子树或右子树则为c的右子树
 *
 */
status InsertChild(BiTree T, BiTree p, int LR, BiTree c)
{
	if (!T)
	{
		printf("\t\t\t二叉树不存在！");
		return ERROR;
	}
	if (c->rchild != NULL)
	{
		printf("\t\t\t待插入二叉树的右子树不为空！");
		return ERROR;
	}
	if (LR == 0)
	{
		c->rchild = p->lchild;//插入方向为左的情况
		p->lchild = c;
	}
	else
	{
		c->rchild = p->rchild;//插入方向为右的情况
		p->rchild = c;
    }
		return OK;
}

/**
 * 函数名称: DeleteChild
 * 初始条件：二叉树T存在，p指向T中的某个结点，LR为0或1
 * 操作结果: 根据LR为0或者1，删除c为T中p所指结点的左或右子树
 *
 */
status DeleteChild(BiTree T, BiTree p, int LR)
{
	BiTree T1;
	if (T == NULL)
	{
		printf("\t\t\t二叉树不存在！\n");
		return ERROR;
	}
	if (LR == 0)//删除左子树
	{
		T1 = p->lchild;
		p->lchild = NULL;
		if(!DestroyBiTree(&T1)) return ERROR;
	}
	else//删除右子树
	{
		T1 = p->rchild;
		p->rchild = NULL;
		if (!DestroyBiTree(&T1)) return ERROR;
	}
	return OK;
}

/**
 * 函数名称: PreOrderTraverse
 * 初始条件：二叉树T已存在
 * 操作结果: 先序遍历t，对每个结点调用函数Visit一次且一次，一旦调用失败，则操作失败
 *
 */
status PreOrderTraverse(BiTree T, status (* visit)(char e))
{
	if(T)
	{
		if(visit(T->data))//先序遍历，利用递归方式
		if(PreOrderTraverse(T->lchild, visit))
		if(PreOrderTraverse(T->rchild, visit))
		return 1;
		return 0;
	}
	else return 1;
}

/**
 * 函数名称: InOrderTraverse
 * 初始条件：二叉树T已存在
 * 操作结果: 中序遍历t，对每个结点调用函数Visit一次且一次，一旦调用失败，则操作失败
 *
 */
status InOrderTraverse(BiTree T, status (* visit)(char e))
{
	if(T)
	{
		if(InOrderTraverse(T->lchild, visit))
		if(visit(T->data))//中序遍历，利用递归方式
		if(InOrderTraverse(T->rchild, visit))
		return 1;
		return 0;
	}
	else return 1;
}

/**
 * 函数名称: PostOrderTraverse
 * 初始条件：二叉树T已存在
 * 操作结果: 后序遍历t，对每个结点调用函数Visit一次且一次，一旦调用失败，则操作失败
 *
 */
status PostOrderTraverse(BiTree T, status (* visit)(char e))
{
	if(T)
	{
		if(PostOrderTraverse(T->lchild, visit))
		if(PostOrderTraverse(T->rchild, visit))
		if(visit(T->data))//后序遍历，利用递归方式
		return 1;
		return 0;
	}
	else return 1;
}

void level(BiTree T, int i)
{
	if(T)
	{
		if(i == 1)
		visit(T->data);//该函数用来辅助层序遍历
		else
		{
			level(T->lchild, i-1);
			level(T->rchild, i-1);
		}
	}
}

/**
 * 函数名称: LevelOrderTraverse
 * 初始条件：二叉树T已存在
 * 操作结果: 层序遍历t，对每个结点调用函数Visit一次且一次，一旦调用失败，则操作失败
 *
 */
status LevelOrderTraverse(BiTree T, status (* visit)(char e))
{
	if(T)
	{
		int h = BiTreeDepth(T);//调用函数,得到深度
		int i;
		for(i=1; i<=h; i++)
		{
			level(T, i);//对每一层进行访问
		}
		return OK;
	}
	else
	return OK;
}

/**
 * 函数名称: Save
 * 初始条件：二叉树T已存在
 * 操作结果: 保存二叉树为文件
 *
 */
status Save(BiTree T,FILE *fp)
{
	int i = 0;
	char ch = '#';
	if(T)
	{
        fprintf(fp,"%d%c",T->key,T->data);//将key和data依次写入文件
        if(Save(T->lchild,fp))
        if(Save(T->rchild,fp)) return OK;
        else return ERROR;
	}
	else
	{
	fprintf(fp,"%d%c",i,ch);//i和ch初始设置,意味着空节点的key为0,#代表空节点.
	return OK;
	}
}

/**
 * 函数名称: Load
 * 操作结果: 从文件中读取二叉树
 *
 */
status Load(BiTree *T,FILE *fp)
{
	int i;//用i来表示key
	char ch;//用ch来读取data域
	if(feof(fp))
	{
		(*T)=NULL;
		return OK;
	}
	fscanf(fp,"%d",&i);
	fscanf(fp,"%c",&ch);
	if(ch=='#')
	{
		(*T)=NULL;
	}
	else
	{
		(*T)=(BiTree)malloc(sizeof(BiTNode));
		(*T)->key=i;
		(*T)->data=ch;
		Load(&((*T)->lchild),fp);
		Load(&((*T)->rchild),fp);
	}//相当于依次读取之后创建为二叉树
	return OK;
}

status TreeDisplay(BiTree T,int depth,status (* visit)(char e))
{
    if(!T)
    {
        printf("\n");
        return OK;
    }
    int i=0;
    for(; i<depth; i++)
        printf(" ");
    visit(T->data);
    printf("\n");
    if(T->lchild||T->rchild)
    {
	TreeDisplay(T->lchild,depth+1,visit);
    TreeDisplay(T->rchild,depth+1,visit);
    }
    return OK;
}

