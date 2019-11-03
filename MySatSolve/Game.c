#ifndef GAME_C_INCLUDED
#define GAME_C_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Sudoku.h"
#include "SAT.h"
/***********************************
*文件名称:Game.c
*文件描述:数独游戏部分的代码实现
*日    期:2019-2-21
*版    本：V1.0
*@_@|||||||||||
***********************************/


int Sudoku() {
    Clause *S = NULL;
    SatAnswer *answer;
    FILE *fp;
    char filename[100];
    Var_watch var_watch[MaxNumVar + 1];
    int branchDecision[2 * MaxNumVar + 1];
    int sudokuTable[9][9];
    int op = 1;
    clock_t start, finish;
    double duration;
    srand((unsigned) time(NULL));
    while (op) {
        printf("\n\n");
        printf("\t\t\t       Sudoku\n");
        printf("\t\t************************************\n");
        printf("\t\t\t1.  Generate Sudoku   2.  Solve Sudoku\n");
        printf("\t\t\t0.  Back\n");
        printf("\t\t************************************\n");
        printf("\t\t\tPlease choose your operation[0--2]:");
        scanf("%d", &op);
        system("cls");
        switch (op) {
            case 1:
                start = clock();
                NewSudoku(&S, &answer, var_watch, branchDecision, sudokuTable);
                finish = clock();
                duration = (double) (finish - start) / 1000.0;
                printf("The time to generate Sudoku is %.3f s\n", duration);
                break;
            case 2:
                InitSat(&S, &answer, var_watch, branchDecision);
                /*printf("Input the file path:\n");
                scanf("%s", filename);
                fp = fopen(filename, "r");*/
                fp = fopen("D:\\sudoku_rule.txt", "r");
                LoadCnf(&S, answer, var_watch, fp);
                fclose(fp);
                start = clock();
                SolveSudoku(answer, var_watch, branchDecision, sudokuTable);
                finish = clock();
                duration = (double) (finish - start) / 1000.0;
                printf("The time to solve Sudoku is %.3f s\n", duration);
                break;
            case 0:
                return 0;
            default:
                printf("\t\t\tPlease choose your operation again[0~2]:");
                scanf("%d", &op);
        }
    }
}

int NewSudoku(Clause **S, SatAnswer **answer, Var_watch var_watch[], int branchDecision[], int sudokuTable[9][9]) {
    clock_t a, b;
    double d;
    a = clock();
    if (GenerateSudoku(S, answer, var_watch, branchDecision) == 0)
        return 0;
    b = clock();
    d = (double) (b - a) / 1000.0;
    printf("The time to generate the final Sudoku is %.3f s\n\n", d);
    DigHole(*answer, var_watch, branchDecision, sudokuTable);
}

int GenerateSudoku(Clause **S, SatAnswer **answer, Var_watch var_watch[], int branchDecision[]) {
    int x, y, z, i, j, k, l;
    int dig_order[82], index;
    FILE *fp;
    fp = fopen("D:\\sudoku_rule.txt", "w");
    fprintf(fp, "p cnf 729 10287\r\n");
    for (x = 0; x < 9; ++x)
        for (y = 0; y < 9; ++y)
            for (z = 1; z <= 8; ++z) {
                for (i = z + 1; i <= 9; ++i)
                    fprintf(fp, "%d %d 0\r\n", -(81 * x + 9 * y + z), -(81 * x + 9 * y + i));//每个位置，数字1~9至多出现一次
            }
    for (x = 0; x < 9; ++x)
        for (z = 1; z <= 9; ++z)
            for (y = 0; y < 8; ++y) {
                for (i = y + 1; i < 9; ++i)
                    fprintf(fp, "%d %d 0\r\n", -(81 * x + 9 * y + z), -(81 * x + 9 * i + z));//每一行，数字1~9至多出现一次
            }
    for (y = 0; y < 9; ++y)
        for (z = 1; z <= 9; ++z)
            for (x = 0; x < 8; ++x) {
                for (i = x + 1; i < 9; ++i)
                    fprintf(fp, "%d %d 0\r\n", -(81 * x + 9 * y + z), -(81 * i + 9 * y + z));//每一列，数字1~9至多出现一次
            }
    for (z = 1; z <= 9; ++z)
        for (i = 0; i < 3; ++i)
            for (j = 0; j < 3; ++j) {
                for (x = 0; x < 3; ++x)
                    for (y = 0; y < 3; ++y)
                        fprintf(fp, "%d ", 81 * (3 * i + x) + 9 * (3 * j + y) + z);//数字1~9在每个3×3数独中至少出现一次
                fprintf(fp, "0\r\n");
                for (x = 0; x < 3; ++x) {
                    for (y = 0; y < 3; ++y) {
                        for (k = x + 1; k < 3; ++k)
                            for (l = 0; l < 3; ++l)
                                if (l != y)
                                    fprintf(fp, "%d %d 0\r\n", -(81 * (3 * i + x) + 9 * (3 * j + y) + z),
                                            -(81 * (3 * i + k) + 9 * (3 * j + l) + z));//数字1~9在每个3×3数独中至多出现一次
                    }
                }
            }
    fclose(fp);
    do {
        fp = fopen("D:\\sudoku_rule.txt", "r");
//        fp = fopen("C:\\Users\\dell\\Desktop\\base.txt", "r");
        if (fp == NULL) {
            printf("Opening \"D:\\sudoku_rule.txt\" failed.\n ");
            return 0;
        }
        InitSat(S, answer, var_watch, branchDecision);
        LoadCnf(S, *answer, var_watch, fp);
        fclose(fp);
        for (j = 1; j <= 81; ++j)
            dig_order[j] = j;
        for (j = 81; j > 1; --j) {         //随机生成初始化顺序
            index = rand() % j + 1;
            if (j != index) {
                dig_order[j] = dig_order[j] ^ dig_order[index];
                dig_order[index] = dig_order[index] ^ dig_order[j];
                dig_order[j] = dig_order[j] ^ dig_order[index];
            }
        }
        for (k = 0; k < 11;) {                 //在棋盘中随机选11个格子随机填入1~9
            x = (dig_order[j] - 1) / 9;
            y = (dig_order[j] - 1) % 9;
            z = rand() % 9 + 1;
            for (l = 1; l <= 9; ++l)
                if (l == z)
                    (*answer)->value[81 * x + 9 * y + l] = TRUE;
                else
                    (*answer)->value[81 * x + 9 * y + l] = FALSE;
            ++k;
        }
        knownVar = k;
    } while (DPLL(*answer, var_watch, branchDecision, 2, -(rand() % 729 + 1)) == UNSATISFIABLE);
    return 1;
}

int DigHole(SatAnswer *answer, Var_watch var_watch[], int branchDecision[], int sudokuTable[9][9]) {
    int x, y, z, i, j, k;
    int dig_order[82], index, dig = 1;
    int firstBranch;
    FILE *fp;
    clock_t a, b;
    a = clock();
    for (i = 1; i <= numVar; ++i) {
        answer->branchLevel[i] = 0;
        answer->searched[i] = 0;
        answer->singleClause[i] = 0;
    }
    for (x = 0; x < 9; ++x) {                        //得到终盘结果
        for (y = 0; y < 9; ++y) {
            for (z = 1; z <= 9; ++z)
                if (answer->value[81 * x + 9 * y + z] == TRUE) {
                    sudokuTable[x][y] = z;
                    break;
                }
        }
    }
    for (j = 1; j <= 81; ++j)
        dig_order[j] = j;
    for (j = 81; j > 1; --j) {         //随机生成挖洞顺序
        index = rand() % j + 1;
        if (j != index) {
            dig_order[j] = dig_order[j] ^ dig_order[index];
            dig_order[index] = dig_order[index] ^ dig_order[j];
            dig_order[j] = dig_order[j] ^ dig_order[index];
        }
    }
    for (j = 1; j <= 81 - PreassignVar && dig <= 81;) {
        b = clock();
        printf("digged = %d digging = %d time = %.3f s\n", j - 1, dig, (double) (b - a) / 1000.0);
        dig_watch(sudokuTable);
        x = (dig_order[dig] - 1) / 9;
        y = (dig_order[dig++] - 1) % 9;
        z = sudokuTable[x][y];
        if (z <= 0)          //该位置不可挖，寻找下一个位置
            continue;
        knownVar = 9 * (81 - j);         //已经挖掉j个洞
        numBranch = 0;
        for (i = 1; i <= 9; ++i)           //挖去该位置
            answer->value[81 * x + 9 * y + i] = UNKNOWN;
        if (j < 4) {             //挖去个数小于4，解必定唯一
            ++j;
            sudokuTable[x][y] = 0;
            for (k = 1; k <= 9; ++k)           //挖去该位置
                answer->value[81 * x + 9 * y + k] = UNKNOWN;
            continue;
        }
        for (i = 1; i <= 9; ++i) {            //检测挖去该位置解是否唯一
            if (i == z)
                continue;
            firstBranch = 81 * x + 9 * y + i;
            answer->searched[81 * x + 9 * y + i] = 1;       //锁定i的另一分支
            if (DPLL(answer, var_watch, branchDecision, 2, firstBranch) == SATISFIABLE)     //挖去该位置有其他解
                break;
            knownVar = 9 * (81 - j);             //已经挖掉j个洞
            numBranch = 0;
            for (k = 1; k <= numVar; ++k) {        //重置终盘
                if (!answer->branchLevel[k])       //决策级为0，为初始化条件，不重置
                    continue;
                answer->value[k] = UNKNOWN;
                answer->branchLevel[k] = 0;
                answer->searched[k] = 0;
                answer->singleClause[k] = 0;
            }
        }
        if (i == 10) {       //挖去该位置解仍唯一
            ++j;
            sudokuTable[x][y] = 0;
        } else {            //挖去该位置解不唯一
            if (dig > 81)
                break;
            sudokuTable[x][y] = -sudokuTable[x][y];         //该位置不可挖去
            for (k = 1; k <= numVar; ++k) {        //重置终盘
                if (!answer->branchLevel[k])       //决策级为0，为初始化条件，不重置
                    continue;
                answer->value[k] = UNKNOWN;
                answer->branchLevel[k] = 0;
                answer->searched[k] = 0;
                answer->singleClause[k] = 0;
            }
            for (k = 1; k <= 9; ++k)            //填入原来的数
                if (k == z)
                    answer->value[81 * x + 9 * y + k] = TRUE;
                else
                    answer->value[81 * x + 9 * y + k] = FALSE;
        }
    }
    fp = fopen("D:\\sudokuTable.txt", "a+");
    printf("The new Sudoku was generated at \"D:\\sudokuTable.txt\"\n");
    printf("%d known positions:\n", 81 - j + 1);
    for (x = 0; x < 9; ++x) {                    //得到生成数独
        for (y = 0; y < 9; ++y) {
            sudokuTable[x][y] = abs(sudokuTable[x][y]);
            if (sudokuTable[x][y] != 0) {
                for (i = 1; i <= 9; ++i) {
                    if (i != sudokuTable[x][y])
                        fprintf(fp, "%d 0\r\n", -(81 * x + 9 * y + i));
                    else
                        fprintf(fp, "%d 0\r\n", 81 * x + 9 * y + i);
                }
            }
            if (y != 0 && y % 3 == 0)
                printf("| ");
            printf("%d ", sudokuTable[x][y]);
        }
        printf("\n");
        if (x != 8 && x % 3 == 2)
            printf("---------------------\n");
    }
    fclose(fp);
}

int SolveSudoku(SatAnswer *answer, Var_watch var_watch[], int branchDecision[], int sudokuTable[9][9]) {
    int x, y, z;
    FILE *fp;
    DPLL(answer, var_watch, branchDecision, 1, 1);
    for (x = 0; x < 9; ++x) {                        //得到终盘结果
        for (y = 0; y < 9; ++y) {
            for (z = 1; z <= 9; ++z)
                if (answer->value[81 * x + 9 * y + z] == TRUE) {
                    sudokuTable[x][y] = z;
                    break;
                }
        }
    }
    fp = fopen("D:\\sudokuAnswer.txt", "w");
    printf("The answer to Sudoku was saved at \"D:\\sudokuAnswer.txt\"\n");
    for (x = 0; x < 9; ++x) {                    //保存并打印数独答案
        for (y = 0; y < 9; ++y) {
            sudokuTable[x][y] = abs(sudokuTable[x][y]);
            if (sudokuTable[x][y] != 0) {
                for (int i = 1; i <= 9; ++i) {
                    if (i != sudokuTable[x][y])
                        fprintf(fp, "%d 0\r\n", -(81 * x + 9 * y + i));
                    else
                        fprintf(fp, "%d 0\r\n", 81 * x + 9 * y + i);
                }
            }
            if (y != 0 && y % 3 == 0)
                printf("| ");
            printf("%d ", sudokuTable[x][y]);
        }
        printf("\n");
        if (x != 8 && x % 3 == 2)
            printf("---------------------\n");
    }
    fclose(fp);
}
int dig_watch(int sudokuTable[9][9]) {
    int x, y;
    for (x = 0; x < 9; ++x) {                    //保存并打印数独答案
        for (y = 0; y < 9; ++y) {
            printf("%2d ", sudokuTable[x][y]);
        }
        printf("\n");
    }
    printf("\n\n");
}

int dig_test(Clause **S, SatAnswer **answer, Var_watch *var_watch, int *branchDecision) {
    FILE *fp;
    Clause *S1 = NULL;
    Var_watch var_watch1[MaxNumVar + 1];
    int branchDecision1[2 * MaxNumVar + 1];
    int sudokuTable[9][9];
    InitSat(S, answer, var_watch, branchDecision);
    fp = fopen("C:\\Users\\dell\\Desktop\\sudoku_rule.txt", "r");      //初始棋盘
    LoadCnf(S, *answer, var_watch, fp);
    fclose(fp);
    InitSat(&S1, answer, var_watch1, branchDecision1);
    fp = fopen("D:\\sudoku_answer.txt", "r");     //测试终盘
    LoadCnf(&S1, *answer, var_watch1, fp);
    fclose(fp);
    DigHole(*answer, var_watch, branchDecision, sudokuTable);
}


#endif // GAME_C_INCLUDED
