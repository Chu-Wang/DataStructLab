#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SAT.h"
/***********************************
*文件名称:satSolve.c
*文件描述:解决SAT问题的函数实现
*日    期:2019-2-21
*版    本：V1.0
*@_@|||||||||||
***********************************/

int InitSat(Clause **S, SatAnswer **answer, Var_watch var_watch[], int branchDecision[]) {
    Clause *cfront = *S, *crear;                                   //子句的前后指针，用于清空
    ClauseLiteral *lfront, *lrear;                                 //文字的前后指针
    //子句初始化
    while (cfront) {
        crear = cfront->nextClause;                                //这两行可以理解为一个表格从左上角出发
        lfront = cfront->p;
        while (lfront) {
            lrear = lfront->next;
            free(lfront);
            lfront = lrear;                                        //逐一清空子句中的文字，清空表格中的一行
        }
        free(cfront);
        cfront = crear;                                            //清除完毕之后，由于free了前指针。。。
    }
    *S = NULL;
    //解初始化
    numVar = 0;                                                   //变元数目为0
    knownVar = 0;                                                 //已知变元数目为0
    numBranch = 0;                                                //分支数目为0
    *answer = (SatAnswer *) malloc(sizeof(SatAnswer));            //给answer结构分配空间
    for (int i = 1; i <= MaxNumVar; ++i) {
        //解初始化
        (*answer)->value[i] = NONE;                               //表示变元不存在
        (*answer)->branchLevel[i] = 0;                           //赋值时决策树高度为0
        (*answer)->searched[i] = 0;                              //已被搜索的情况数是0
        (*answer)->singleClause[i] = 0;                          //该变量的单子句数目为0
        //文字邻接表初始化
        var_watch[i].pos = (VarList *) malloc(sizeof(VarList));  //为正文字邻接表指针分配空间
        var_watch[i].pos->next = NULL;                           //正文字邻接表指针指向空
        var_watch[i].neg = (VarList *) malloc(sizeof(VarList));  //为负文字邻接表指针分配空间
        var_watch[i].neg->next = NULL;                           //负文字邻接表指针指向空
    }
    //分支决策计数器初始化
    for (int j = 1; j <= 2 * MaxNumVar; ++j)
        branchDecision[j] = 0;                                  //分支决策计数器初始化为0
}

int Sat() {
    Clause *S = NULL;                           //子句指针
    SatAnswer *answer;                          //SAT解的指针
    Var_watch var_watch[MaxNumVar + 1];         //最大变元数目加一个空间
    FILE *fp;								    //文件指针
    char filename[100];						    //文件名
    int branchDecision[2 * MaxNumVar + 1];      //决策树最大分支 应该是变元数目的二倍加一（一个变元有两种可能）
    int op = 1,result;
    clock_t start, finish;                      //调用的time.h头文件
    int duration;                               //用于表示耗时
    while (op) {
        printf("\n\n");
        printf("\t\t\t\t      SAT\n");
        printf("\t\t\t******************************************\n");
        printf("\t\t\t1.  New SAT  2.  Check Answer\n");
        printf("\t\t\t0.  Back\n");
        printf("\t\t\t******************************************\n");
        printf("\t\t\tPlease choose your operation[0--2]:");
        scanf("%d", &op);
        system("cls");
        switch (op) {
            case 1://case1意味着要读取cnf文件，首先要初始化，因此有下一行
                InitSat(&S, &answer, var_watch, branchDecision);
                printf("Please input the file path:\n");
                scanf("%s", filename);
                fp = fopen(filename, "r");

                if (fp == NULL) {
                    printf("fail to open the file!\n ");
                    break;
                } else LoadCnf(&S, answer, var_watch, fp);      //调用LoadCnf 函数，读取文件
                start = clock();                              //调用time.h，记录开始时间
                result = DPLL(answer, var_watch, branchDecision, 1, 1);
                finish = clock();                             //调用time.h，记录结束时间
                duration = (finish - start);//得到解决问题的耗时
                if (result == SATISFIABLE)
				{
					printf("Time to solve the SAT problem %d ms\n", duration);
					PrintAnswer(answer, 1, filename, duration);
				}
                else
				{
					printf("No solution!\n");
					PrintAnswer(answer, 0, filename, duration);
				}
                break;
            case 2:
                ChectAnswer(S, answer);
                break;
            case 0:
                return 0;
            default:
                printf("\t\t\tPlease choose your operation again[0~2]:");
                scanf("%d", &op);
        }
    }
}

int LoadCnf(Clause **S, SatAnswer *answer, Var_watch var_watch[], FILE *fp) {//加载CNF文件
    char c;
    Clause *ctemp, *cp = NULL;          //两个指针，加载CNF文件中的子句会用到
    ClauseLiteral *lp, *ltemp;         //两个指针，加载CNF文件中的文字会用到
    int var;                            //变量的值
    int numClauseVar;                   //每个子句的变元数
    fscanf(fp, "%c", &c);
    while (c == 'c') {                  //注释段
        while (c != '\n' && c != '\r')  //
            fscanf(fp, "%c", &c);       //
        fscanf(fp, "%c", &c);
        if (c == '\n')                  //如果读取到换行符，意味着要再重复一次
            fscanf(fp, "%c", &c);
    }
    fscanf(fp, " cnf ");                //开始读到文件的信息段
    numVar = GetNum(fp);              //读取变量数，并赋值给numVar
    GetNum(fp);                        //读取子句数
    var = GetNum(fp);                  //再次调用，读取下一行第一个变量值，把值赋给var
    while (1) {
        numClauseVar = 0;
        ctemp = (Clause *) malloc(sizeof(Clause));      //为ctemp指针分配空间
        lp = ctemp->p;                                  //lp为文字指针，，可以想象一下表格长什么样
        while (var) {
            ++numClauseVar;                             //计数器功能，统计每个子句的变元数目
            if (answer->value[abs(var)] == NONE)
                answer->value[abs(var)] = UNKNOWN;
            ltemp = (ClauseLiteral *) malloc(sizeof(ClauseLiteral));
            ltemp->data = var;                          //文字的值域改成var，赋值
            ltemp->next = NULL;                         //
            if (numClauseVar == 1) {                    //储存子句中首个变量
                ctemp->p = lp = ltemp;
            } else {                                    //储存子句中非首个变量
                lp->next = ltemp;
                lp = lp->next;
            }
            if (var > 0)
                ++firstBranch[var];                    //初始分支决策计数增加
            else
                ++firstBranch[numVar - var];
            PutClause(ctemp, var, var_watch);             //储存各变量的子句地址
            var = GetNum(fp);
        }
        if (numClauseVar == 1) {                        //输入单子句，则该子句必须满足，无需存入
            answer->value[abs(lp->data)] = lp->data / abs(lp->data);
            ++knownVar;                                //已知变元数目加1
        } else if (*S == NULL) {
            *S = cp = ctemp;
            cp->nextClause = NULL;
        } else {                                        //想想表格的形式，是列的头
            cp->nextClause = ctemp;
            cp = cp->nextClause;
            cp->nextClause = NULL;
        }
        var = GetNum(fp);                             //若到达文件尾，再执行一次z'z 读文件操作时，设置文件结束标志
        if (feof(fp))
            break;
    }
}

int GetNum(FILE *fp) {//这个函数用来读取cnf文件时得到变量数、子句数、后面的内容
    char c;
    int sign = 1, num = 0;        //num 用来得到文字的值,sign用来标记文字是正或者负
    fscanf(fp, "%c", &c);
    if (c == '-') {
        sign = -1;                //sign变为-1，表示为负文字
        fscanf(fp, "%c", &c);
    } else if (c == '0') {        //表示该条子句结束
        fscanf(fp, "%c", &c);
        if (c == '\r')            //表示换行
            fscanf(fp, "%c", &c);
        return num;               //
    } else if (feof(fp))          //如果是结束标记
        return 0;
    while (c != ' ' && c != '\n' && c != '\r') {
        num = num * 10 + c - '0'; //得到文字的值
        fscanf(fp, "%c", &c);
    }
    if (c == '\r')
        fscanf(fp, "%c", &c);
    return sign * num;                      //用来得到文字（包括正负和值）
}

int DPLL(SatAnswer *answer, Var_watch var_watch[], int branchDecision[], int op, int firstBranch) {
    int status, var, blevel = 0;            //初始判定级为0
    VarList *vp;                            //邻接表指针，用于
    while (1) {
        if (numBranch++ == 0) {             //第一次分支决策
            if (op == 1)                    //1:SAT求解、2:生成数独的检验求解
                var = NextBranch(branchDecision, answer);
            else
                var = firstBranch;
        } else
            var = NextBranch(branchDecision, answer);        //下一分支决策
        ++blevel;                                            //判定级加1
        answer->value[abs(var)] = var / abs(var);            //进入一分支
        answer->branchLevel[abs(var)] = blevel;
        ++answer->searched[abs(var)];                        //已被搜索情况数加1
        ++knownVar;                                          //已知变元数目加1
        while (1) {
            if (var > 0)
                vp = var_watch[var].neg->next;               //var为TRUE，则搜索var为FALSE的子句
            else
                vp = var_watch[-var].pos->next;              //var为FALSE，则搜索var为TRUE的子句
            status = Deduce(blevel, answer, var_watch, branchDecision, vp);//单子句传播，返回子句的状态
            if (status == SATISFIABLE)                       //满足的情况
                return SATISFIABLE;
            else if (status == CONFLICT) {
                var = Analyse_conflict(&blevel, var, answer);//var > 0，矛盾，开始回溯
                if (blevel == 0)
                    return UNSATISFIABLE;
                else {                                       //进入另一分支，不满足
                    answer->value[var] = -answer->value[var];//则值进行反转，正变成负，负变成正
                    ++answer->searched[var];                 //被搜索情况数目加1
                    if (answer->value[var] < 0)
                        var = -var;
                }
            } else if (status == OTHERS) break;              //已知条件不足，进入下一层
        }
    }
}

int PrintAnswer(SatAnswer *answer,int result, char filename[100], int duration) {
    FILE *fp;
    fp = filename;
	int p = 0;
	while (filename[p] != 0) p++;
	while (filename[p] != '.') p--;
	p++;
	filename[p] = 'r';
	p++;
	filename[p] = 'e';
	p++;
	filename[p] = 's';
	p++;
	filename[p] = 0;
	fp = fopen(fp, "w");
	if(result == 1)
		fprintf(fp, "s 1\r\n");
	else
		fprintf(fp, "s 0\r\n");
	fprintf(fp, "v ");
	for(int i = 1; i < MaxNumVar; i++)
	{
		if(answer->value == TRUE)
			fprintf(fp, "%d ", i);
		else
			fprintf(fp, "-%d ", i);
	}
	fprintf(fp, "\r\n");
	fprintf(fp, "t %d\r\n", duration);
    fclose(fp);
    printf("The answer has been saved\n");
    printf("One solution of SAT is:\n");
    for (int i = 1; i <= MaxNumVar; ++i)
        if (answer->value[i] == TRUE)
		{
            printf("%d ", i);
			//fprintf(file, "%d ", i);
		}
        else if (answer->value[i] == FALSE)
            printf("-%d ", i);
    printf("\n");

	//fflush(file);

}

int PutClause(Clause *ctemp, int var, Var_watch var_watch[]) {
    VarList *wp;
    if (var > 0)                                     //判断var是否大于零，从而归纳到相应的表格中
        wp = var_watch[var].pos;
    else
        wp = var_watch[-var].neg;
    while (wp->next)
        wp = wp->next;                               //循环，找到VarList的尾部，将var添加到尾部
    wp->next = (VarList *) malloc(sizeof(VarList));//分配空间
    wp = wp->next;                                   //
    wp->p = ctemp;                                   //将子句放在末尾
    wp->next = NULL;
}

int Deduce(int blevel, SatAnswer *answer, Var_watch var_watch[], int branchDecision[], VarList *root) {
    int top = 0, status;                               //status表示状态
    VarList *stack[MaxNumVar], *vp = root;             //栈，栈的最大长度为最大变元数目
    stack[top++] = vp;
    while (top) {
        vp = stack[top - 1];                           //访问栈顶元素
        status = SINGLE;
        while (status == SINGLE && vp) {               //左子树搜索
            status = SingleClauseDeduce(blevel, answer, var_watch, branchDecision, &vp);
            stack[top++] = vp;                         //左孩子入栈
        }
        --top;                                         //空指针退栈
        if (status == CONFLICT)
            return CONFLICT;
        if (top) {                                     //右子树搜索
            vp = stack[--top];                         //根节点出栈
            if (vp->next)
                stack[top++] = vp->next;               //右孩子入栈
        }
    }
    if (knownVar < numVar)
        return OTHERS;
    else return SATISFIABLE;
}

int SingleClauseDeduce(int blevel, SatAnswer *answer, Var_watch var_watch[], int branchDecision[], VarList **vp) {
    Clause *cp;                                      //子句指针
    ClauseLiteral *lp;                               //文字指针
    int unknownNum, firstUnknown, satisfiable;       //
	//初始化
    unknownNum = 0;
    firstUnknown = 0;
    satisfiable = 0;
    cp = (*vp)->p;                                   //将cp指向vp所指向的子句
    lp = cp->p;                                      //文字指针指向cp所指向的文字
    if (lp == NULL)
        return OTHERS;
    while (lp) {
        if (lp->data > 0)
            ++branchDecision[lp->data];                         //分支决策计数增加
        else
            ++branchDecision[numVar - lp->data];
        if (answer->value[abs(lp->data)] * lp->data > 0) {      //子句中存在值为TRUE的文字，子句成立
            satisfiable = 1;
            break;
        }
        if (answer->value[abs(lp->data)] == UNKNOWN) {
            ++unknownNum;                         //计数子句中未被赋值的文字，
            if (firstUnknown == 0)
                firstUnknown = lp->data;          //记录第一个未知的文字
        }
        lp = lp->next;
    }
    if (unknownNum == 0 && satisfiable == 0)       //该子句文字均已知并且都为FALSE，为矛盾句
        return CONFLICT;
    else if (unknownNum == 1 && satisfiable == 0) {  //该子句无值为TRUE的文字，且只有一个未知文字，为单子句
        answer->singleClause[abs(firstUnknown)] = 1; //标记，单子句出现的位置
        answer->value[abs(firstUnknown)] = firstUnknown / abs(firstUnknown);
        answer->branchLevel[abs(firstUnknown)] = blevel;
        ++knownVar;                                 //已被赋值的变元数目加1,即确定变元数目加1
        if (firstUnknown > 0)
            *vp = var_watch[firstUnknown].neg->next;  //var为TRUE，则检索var为FALSE的子句
        else
            *vp = var_watch[-firstUnknown].pos->next; //var为FALSE，则检索var为TRUE的子句
        return SINGLE;
    } else if (knownVar < numVar) {
        *vp = NULL;
        return OTHERS;                                //判断条件不足，返回OTHERS
    } else return SATISFIABLE;
}

int NextBranch(int branchDecision[], SatAnswer *answer) {//下一分支函数
    int maxVar = numVar, maxCount = 0;
    int *branch;
    ++numBranch;
    branch = numBranch == 1 ? firstBranch : branchDecision;
    for (int i = 1; i <= 2 * numVar; ++i) {
        if (i <= numVar && answer->value[i] != UNKNOWN)
            continue;
        if (i > numVar && answer->value[i - numVar] != UNKNOWN)
            continue;
        if (maxCount <= *(branch + i)) {
            maxVar = i;
            maxCount = *(branch + i);

        }
       // maxVar = i;
    }
    return maxVar > numVar ? numVar - maxVar : maxVar;
}

int Analyse_conflict(int *blevel, int var, SatAnswer *answer) {//回溯函数
    int fore = abs(var);
    while (*blevel != 0) {
        for (int j = 1; j <= numVar; ++j)
            if (j != fore && answer->branchLevel[j] == *blevel) {      //将由var赋值产生的单子句重置
                answer->value[j] = UNKNOWN;
                answer->branchLevel[j] = 0;
                answer->searched[j] = 0;
                answer->singleClause[j] = 0;
                --knownVar;
            }
        if (*blevel != 1) {
            if (answer->searched[fore] == 2) {       //var的TRUE和FALSE分支均搜索过，进行回溯
                --(*blevel);
                answer->value[fore] = UNKNOWN;
                answer->branchLevel[fore] = 0;
                answer->searched[fore] = 0;
                --knownVar;
                for (int i = 1; i <= numVar; ++i)
                    if (answer->branchLevel[i] == *blevel && answer->singleClause[i] == 0) {
                        fore = i;
                        break;
                    }
            } else break;           //搜索另一分支
        } else if (answer->searched[abs(fore)] == 2)//blevel1全部搜索完
            --(*blevel);
        else break;                 //搜索blevel1的另一分支
    }
    return fore;
}

int ChectAnswer(Clause *S, SatAnswer *answer) {//解的正误检查
    FILE *fp;
    Clause *cp = S;
    VarList *error = NULL, *ep;
    ClauseLiteral *lp;
    int ok = 0, s = 1, num = 0;
    fp = fopen("D:\\Satcheck.txt", "w");//在D盘建立一个txt文件
    error = ep = (VarList *) malloc(sizeof(VarList));//分配空间
    error->next = NULL;
    fprintf(fp, "Answer:\r\n");
    for (int i = 1; i <= MaxNumVar; ++i)
        if (answer->value[i] == TRUE)//标记的第i个值为正文字
            fprintf(fp, "%d ", i);
        else if (answer->value[i] == FALSE)//标记的第i个值为负文字
            fprintf(fp, "-%d ", i);
    fprintf(fp, "\r\n\r\n");
    while (cp) {
        ok = 0;
        lp = cp->p;
        fprintf(fp, "C%-4d ", ++num);
        while (lp) {
            fprintf(fp, "%4d ", lp->data);
            if (answer->value[abs(lp->data)] * lp->data > 0)
                ok = 1;
            lp = lp->next;
        }
        if (ok == 1)
            fprintf(fp, "T\r\n");
        else {
            s = 0;
            fprintf(fp, "F\r\n");
            ep->next = (VarList *) malloc(sizeof(VarList));
            ep = ep->next;
            ep->p = cp;
            ep->next = NULL;
        }
        cp = cp->nextClause;
    }
    if (s == 1)
        printf("Check: Satisfiable!\n");
    else printf("Check: Unsatisfiable!\n");//检查的结果
    printf("The detail was saved at \"D:\\Satcheck.txt\"\n");//保存在哪里了
    ep = error->next;
    if (ep) {
        fprintf(fp, "\r\n\r\nError:\r\n");
        num = 0;
    }
    while (ep) {
        fprintf(fp, "C%-4d ", ++num);
        lp = ep->p->p;
        while (lp) {
            fprintf(fp, "%4d ", lp->data);
            lp = lp->next;
        }
        fprintf(fp, "F\r\n");
        ep = ep->next;
    }
    fclose(fp);
}
