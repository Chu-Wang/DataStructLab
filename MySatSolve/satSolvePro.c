#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>


#define TRUETRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define DECIDE 1
#define UNDECIDE 3
#define SUDUPTN 729
#define SUDUCTN 10530


typedef int status;
typedef struct LiteralsNode
{
    int literals;
    int flag;
    struct LiteralsNode *next;
} LiteralsNode;   //文字结点

typedef struct ClauseNode
{
    int clause;
    int trueValue;
    int flag;
    struct ClauseNode *next;
} ClauseNode;      //子句结点

typedef struct
{
    int lNumber;
    LiteralsNode *head;
    LiteralsNode *Watch1;
    LiteralsNode *Watch2;
} ClauseList;      //子句邻接表

typedef struct
{
    int cNumber;
    ClauseNode *head;
    ClauseNode *pos_watched;
    ClauseNode *neg_watched;
} LiteralsList;    //文字邻接表

typedef struct
{
    LiteralsNode *head;
} Backtree;
typedef struct
{
    int totalpNumber;
    int totalCNumber;
} TotalNumber;

typedef struct
{
    int *pTV;
    int num;
} TrueValue;

typedef struct
{
    status SAT;
    int *pTV;
    clock_t time;
} Result;

typedef struct
{
    int pos;
    int neg;
} Counter;
int SatPro();
status Save (ClauseList *CL,TotalNumber TN);
int createchain(int pn,int flag,int cn,LiteralsList **LL, ClauseList **CL,Counter **counter);
void sudurestraint (LiteralsList **LL, ClauseList **CL,TotalNumber *TN,Counter **counter);
status Load (LiteralsList **LL, ClauseList **CL,TotalNumber *TN,Counter **counter);
Result DPLLPro (LiteralsList *LL, ClauseList *CL, TotalNumber TN,Counter *counter);
void destroy (TrueValue*);
Result UNSAT (clock_t start);
Result SAT(clock_t start,int *pTV);
void ordercounter(int *order,Counter *counter,int n);
void showCL(ClauseList *CL,int begin,int cn);
void showLL(LiteralsList *LL,int begin,int pn);
void deleteclause(int cn,LiteralsList **LL, ClauseList **CL,TotalNumber *TN,Counter **counter);


int SatPro()
{
    int i,*pTV,op=1,op1;
    LiteralsList *LL=NULL;
    ClauseList *CL=NULL;
    TotalNumber TN;
    LiteralsNode *lp;
    ClauseNode *cp;
    Counter *counter;
    Result result;
    while(op)
    {
        system("cls");
        printf("\n\n");
        printf("\t           SUDU & SAT问题求解  \n");
        printf("-----------------------------------------------------\n");
        printf("\tSAT :1  SUDU:2\n");
        printf("\t退出：0\n");
        printf("-----------------------------------------------------");
        printf("\n请选择你要执行的操作:\n");

        scanf("%d",&op);
        if(op==1)
        {

            Load(&LL,&CL,&TN,&counter);
            result= DPLLPro (LL, CL, TN, counter);
            printf("DPLLPro\n");
            if(result.SAT==TRUETRUE)
            {
                printf("s 1\n");
                for(i=1; i<=TN.totalpNumber; i++)
                {
                    if(result.pTV[i]==FALSE)
                        printf("-%d ",i);
                    else
                        printf("%d ",i);
                }
                printf("\nt %ld",result.time);
            }
            else
            {
                printf("s 0\n");
                printf("t %ld",result.time);
            }
        }
    }
    return 0;
}




status Load (LiteralsList **LL, ClauseList **CL,TotalNumber *TN,Counter **counter)
{
    FILE *fp;
    char filename[100],s[100],c;
    int n,i=1,j,flag,pn;
    LiteralsNode *lp,*lp1;
    ClauseNode *cp,*cp1;
    printf("请输入要载入的文件:");
    scanf("%s",filename);
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
        printf("文件打开失败！");
        return ERROR;
    }
    fscanf(fp,"%c",&c);
    while(c!='p')
    {
        fgets(s,100,fp);
        fscanf(fp,"%c",&c);
    }
    do
    {
        fscanf(fp,"%s",s);
    }
    while(strcmp(s,"cnf"));
    fscanf(fp,"%d",&n);
    TN->totalpNumber=n;
    *LL=(LiteralsList *)malloc((n+1)*sizeof(LiteralsList));
    *counter=(Counter *)malloc((n+1)*sizeof(Counter));
    for(i=0; i<=n; i++)
    {
        (*LL)[i].cNumber=0;
        (*LL)[i].head=NULL;
        (*LL)[i].neg_watched=(*LL)[i].pos_watched=NULL;
        (*counter)[i].neg=(*counter)[i].pos=0;
    }
    fscanf(fp,"%d",&n);
    TN->totalCNumber=n;
    *CL=(ClauseList *)malloc((n+1)*sizeof(ClauseList));
    for(i=0; i<=n; i++)
    {
        (*CL)[i].lNumber=0;
        (*CL)[i].head=NULL;
    }
    for(i=1; i<=TN->totalCNumber; i++)
    {
        lp=(*CL)[i].head;
        fscanf(fp,"%d",&n);
        while(n)
        {
            flag=n>0?1:0;
            pn=n>0?n:-n;
            if(flag==1)
                ((*counter)[pn]).pos++;
            else ((*counter)[pn]).neg++;
            lp1=(LiteralsNode *)malloc(sizeof(LiteralsNode));
            lp1->literals=pn;
            lp1->flag=flag;
            lp1->next=NULL;
            if(!lp)
                (*CL)[i].head=lp1;
            else
                lp->next=lp1;
            lp=lp1;
            (*CL)[i].lNumber++;
            cp1=(ClauseNode *)malloc(sizeof(ClauseNode));
            cp1->clause=i;
            cp1->trueValue=UNDECIDE;
            cp1->flag=flag;
            cp1->next=NULL;
            if(flag==TRUETRUE)
            {
                if((*LL)[pn].pos_watched==NULL)
                    (*LL)[pn].pos_watched=cp1;
                else
                {
                    cp=(*LL)[pn].pos_watched;
                    while(cp->next)
                        cp=cp->next;
                    cp->next=cp1;
                }
                (*counter)[pn].pos++;
            }
            else
            {
                if((*LL)[pn].neg_watched==NULL)
                    (*LL)[pn].neg_watched=cp1;
                else
                {
                    cp=(*LL)[pn].neg_watched;
                    while(cp->next)
                        cp=cp->next;
                    cp->next=cp1;
                }
                (*counter)[pn].neg++;
            }

            (*LL)[pn].cNumber++;
            fscanf(fp,"%d",&n);
        }

        (*CL)[i].Watch1=(*CL)[i].head;
        if((*CL)[i].head)
            (*CL)[i].Watch2=(*CL)[i].head->next;
        else (*CL)[i].Watch2=NULL;
    }
    fclose(fp);
    return OK;
}

Result DPLLPro (LiteralsList *LL, ClauseList *CL, TotalNumber TN,Counter *counter)
{
    TrueValue stack[TN.totalpNumber];
    Result result;
    ClauseNode *Cp;
    int top=0,num=1,Cnum,pnum,i,j=1,queue[TN.totalpNumber*2];
    int rear=0,head=0,m=1,backtrack=FALSE,order[TN.totalpNumber*2];
    clock_t start,over;
    start=clock();
    stack[top].pTV=(int *)malloc((TN.totalpNumber+1)*sizeof(int));
    for(i=0; i<=TN.totalpNumber; i++)
    {
        stack[top].pTV[i]=UNDECIDE;
    }
    ordercounter(order,counter,TN.totalpNumber);
    num=order[0];
    while(1)
    {
        if(m%10==0)
        {
            for(int k=1; k<=TN.totalpNumber; k++)
            {
                counter[k].pos/=2;
                counter[k].neg/=2;
            }
            m=1;
        }
        LiteralsNode *lp2;
        backtrack=FALSE;
        if(stack[top].pTV[num]==UNDECIDE)
        {
            top++;
            stack[top].pTV=(int *)malloc((TN.totalpNumber+1)*sizeof(int));
            for(i=1; i<=TN.totalpNumber; i++)
                stack[top].pTV[i]=stack[top-1].pTV[i];
            stack[top].pTV[num]=DECIDE;
            stack[top].num=num;
            rear=head=0;
        }
        if(stack[top].pTV[num]==FALSE)
            Cp=LL[num].pos_watched;
        else
            Cp=LL[num].neg_watched;
        while(Cp)
        {
            Cnum=Cp->clause;
            int watch1,watch2,flag2=FALSE;
            watch1=CL[Cnum].Watch1->literals;
            if(CL[Cnum].Watch2==NULL)
            {
                if(top==0)
                {
                    return UNSAT(start);
                }
                else
                {
                    if(CL[Cnum].Watch1->flag==TRUETRUE)
                    {
                        counter[watch1].pos++;
                    }
                    else
                        counter[watch1].neg++;
                    num=stack[top].num;
                    destroy(&stack[top]);
                    top--;
                    stack[top].pTV[num]=FALSE;
                    backtrack=TRUETRUE;
                    rear=head=0;
                    break;
                }
            }
            watch2=CL[Cnum].Watch2->literals;
            if(watch1==num||watch2==num)
            {
                if(watch2==num)
                {
                    int temp;
                    temp=watch1;
                    watch1=watch2;
                    watch2=temp;
                    flag2=TRUETRUE;
                }
                int pnum1,flag1=4;
                LiteralsNode *lp1;
                lp1=CL[Cnum].head;
                while(lp1)
                {
                    pnum1=lp1->literals;
                    if((stack[top].pTV[pnum1]==UNDECIDE)||(stack[top].pTV[pnum1]==lp1->flag))
                    {
                        if(pnum1!=watch2)
                        {
                            if(flag2==FALSE)
                                CL[Cnum].Watch1=lp1;
                            else
                                CL[Cnum].Watch2=lp1;
                            break;
                        }
                        else
                        {
                            flag1=lp1->flag;
                        }
                    }
                    lp1=lp1->next;
                }
                if(lp1)
                {
                    Cp=Cp->next;
                    continue;
                }
                else
                {
                    if(flag1!=4)
                    {
                        if(stack[top].pTV[watch2]!=UNDECIDE)
                        {
                            Cp=Cp->next;
                            continue;
                        }
                        else
                        {
                            stack[top].pTV[watch2]=flag1;
                            queue[rear++]=watch2;
                        }
                    }
                    else
                    {
                        if(top==0)
                        {
                            return UNSAT(start);
                        }
                        else
                        {
                            lp2=CL[Cnum].head;
                            while(lp2)
                            {
                                if(lp2->flag)
                                {
                                    counter[lp2->literals].pos++;
                                }
                                else
                                    counter[lp2->literals].neg++;
                                lp2=lp2->next;
                            }
                            num=stack[top].num;
                            destroy(&stack[top]);
                            top--;
                            stack[top].pTV[num]=FALSE;
                            backtrack=TRUETRUE;
                            rear=head=0;
                            break;
                        }
                    }
                }
            }
            Cp=Cp->next;
        }
        if(backtrack==TRUETRUE)
            continue;
        if(head<rear)
            num=queue[head++];
        else
        {
            rear=head=0;
            int nextp;
            ordercounter(order,counter,TN.totalpNumber);
            m++;
            for(j=0; j<TN.totalpNumber*2; j++)
            {
                nextp=order[j];
                if(stack[top].pTV[nextp]==UNDECIDE)
                {
                    num=nextp;
                    break;
                }
            }
            if(j>=TN.totalpNumber*2)
            {
                return SAT(start,stack[top].pTV);
            }
        }
    }

}
void destroy (TrueValue* TV)
{
    free(TV->pTV);

}
Result UNSAT (clock_t start)
{
    Result result;
    clock_t over;
    over=clock();
    result.SAT=FALSE;
    result.time=over-start;
    return result;
}
Result SAT(clock_t start,int *pTV)
{
    Result result;
    clock_t over;
    over=clock();
    result.SAT=TRUETRUE;
    result.time=over-start;
    result.pTV=pTV;
    return result;
}
void ordercounter(int *order,Counter *counter,int n)
{
    int i,j,t,a[n*2];
    for(i=0; i<n; i++)
    {
        a[i*2]=counter[i+1].pos;
        a[i*2+1]=counter[i+1].neg;
        order[i*2]=order[i*2+1]=i+1;
    }
    for(i=1; i<2*n-1; i++)
    {
        for(j=i; j; j--)
        {
            if(a[j]>a[j-1])
            {
                t=a[j];
                a[j]=a[j-1];
                a[j-1]=t;
                t=order[j],order[j]=order[j-1],order[j-1]=t;
            }
            else
                break;
        }
    }
}

status Save (ClauseList *CL,TotalNumber TN)
{
    int i;
    FILE *fp;
    char filename[100];
    printf("请输入要保存的文件:");
    scanf("%s",filename);
    fp=fopen(filename,"w");
    if(!fp)
    {
        printf("文件打开失败！");
        return ERROR;
    }
    fprintf(fp,"p cnf %d %d\n",TN.totalpNumber,TN.totalCNumber);
    LiteralsNode *lp;
    for( i=1; i<=TN.totalCNumber; i++)
    {
        lp=CL[i].head;
        while(lp)
        {
            if(lp->flag)
                fprintf(fp,"%d ",lp->literals);
            else
                fprintf(fp,"%d ",-lp->literals);
            lp=lp->next;
        }
        fprintf(fp,"0\n");
    }
    fclose(fp);
}


void showCL(ClauseList *CL,int begin,int cn)
{
    LiteralsNode *lp;
    for(int i=begin; i<=cn; i++)
    {
        lp=CL[i].head;
        printf("C%d:",i);
        while(lp)
        {
            if(lp->flag)
                printf("%d ",lp->literals);
            else
                printf("%d ",-lp->literals);
            lp=lp->next;
        }
        printf("\n");
    }
    printf("\n");
}
void showLL(LiteralsList *LL,int begin,int pn)
{
    ClauseNode *cp;
    for(int i=begin; i<=pn; i++)
    {
        printf("L%d:",i);
        cp=LL[i].pos_watched;
        printf("pos_watched: ");
        while(cp)
        {
            printf("%d ",cp->clause);
            cp=cp->next;
        }
        printf("\n");
        printf("neg_watched: ");
        cp=LL[i].neg_watched;
        while(cp)
        {
            printf("%d ",cp->clause);
            cp=cp->next;
        }
        printf("\n");
    }
    printf("\n");
}


void deleteclause(int cn,LiteralsList **LL, ClauseList **CL,TotalNumber *TN,Counter **counter)
{
    LiteralsNode *lp,*lp1;
    ClauseNode *cp,*cp1;
    int dpn,dcn,flag,isdelete=FALSE;
    lp=(*CL)[cn].head;
    while(lp)
    {
        isdelete=FALSE;
        lp1=lp;
        lp=lp->next;
        dpn=lp1->literals;
        flag=lp1->flag;
        if(flag==TRUETRUE)
        {
            (*counter)[dpn].pos--;
            cp=(*LL)[dpn].pos_watched;
            if(cp->clause==cn)
            {
                free(cp);
                (*LL)[dpn].pos_watched=cp->next;
                isdelete=TRUETRUE;
            }
        }
        else
        {
            (*counter)[dpn].neg--;
            cp=(*LL)[dpn].neg_watched;
            if(cp->clause==cn)
            {
                free(cp);
                (*LL)[dpn].neg_watched=cp->next;
                isdelete=TRUETRUE;

            }
        }
        if(isdelete==FALSE)
        {
            cp1=cp;
            cp=cp->next;
            while(cp)
            {
                if(cp->clause==cn)
                {
                    cp1->next=cp->next;
                    free(cp);
                    break;
                }
                cp=cp->next;
            }

        }
        free(lp1);
    }
    (*CL)[cn].lNumber=0;
    (*CL)[cn].Watch2=(*CL)[cn].Watch1=(*CL)[cn].head=NULL;
}
