#ifndef SAT_H_INCLUDED
#define SAT_H_INCLUDED

#define MaxNumVar 4000
#define PreassignVar 18
//预先分配的变元数目
#define CONFLICT 0
#define SATISFIABLE 1
#define UNSATISFIABLE 0
#define OTHERS 2

#define SINGLE -1

//UNKNOWN表示该变元未知，NONE表示该变元不存在，FALSE表示该变元为假，TRUE表示该变元为真
#define FALSE -1
#define TRUE 1
#define UNKNOWN 0
#define NONE 2
/***********************************
*文件名称:SAT.h
*文件描述:关于求解SAT问题的函数声明、数据结构定义
*日    期:2019-2-21
*版    本：V1.0
*@_@|||||||||||
***********************************/

//该结构参考双文字监视方法，可看作两张表格
typedef struct varWatch {
    struct varList *pos;       //文字邻接表，正文字邻接表，负文字邻接表，
    struct varList *neg;
} Var_watch;

typedef struct varList {
    struct clause *p;     //指向一个子句
    struct varList *next; //指向下一个包含该文字的子句
} VarList;

typedef struct clause {
    struct clauseLiteral *p;//指向一个子句中的下一个文字
    struct clause *nextClause;//这个在邻接表中无意义，要从读取CNF文件时看出效果
} Clause;

typedef struct clauseLiteral {
    int data;//文字的值
    struct clauseLiteral *next;        //指向子句中的下一个文字
} ClauseLiteral;

typedef struct satAnswer {
    int branchLevel[MaxNumVar + 1];   //赋值时的决策树高度
    int value[MaxNumVar + 1];          //TRUE or FALSE or UNKNOWN or NONE
    int searched[MaxNumVar + 1];       //已被搜索的情况数
    int singleClause[MaxNumVar + 1];  //标记是否存在该变量的单子句
} SatAnswer;

int InitSat(Clause **S, SatAnswer **answer, Var_watch *var_watch, int *branchDecision);

int Sat();

int LoadCnf(Clause **S, SatAnswer *answer, Var_watch var_watch[], FILE *fp);

int GetNum(FILE *fp);

int DPLL(SatAnswer *answer, Var_watch var_watch[], int branchDecision[], int op, int firstBranch);

int PrintAnswer(SatAnswer *answer, int result, char filename[100], int duration);

int PutClause(Clause *ctemp, int var, Var_watch var_watch[]);

int Deduce(int blevel, SatAnswer *answer, Var_watch var_watch[], int branchDecision[], VarList *root);

int SingleClauseDeduce(int blevel, SatAnswer *answer, Var_watch var_watch[], int branchDecision[], VarList **vp);

int NextBranch(int branchDecision[], SatAnswer *answer);

int Analyse_conflict(int *blevel, int var, SatAnswer *answer);

int ChectAnswer(Clause *S, SatAnswer *answer);

int numVar;
int knownVar;
int numBranch;
int firstBranch[MaxNumVar];

#endif // SAT_H_INCLUDED
