#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "SAT.h"
#include "Sudoku.h"
/***********************************
*文件名称:main.c
*文件描述:主界面的显示、函数调用
*日    期:2019-2-21
*版    本：V1.0
*@_@|||||||||||
***********************************/


int main() {
    int op = 1;
    while (op) {
        printf("\n\n");
        printf("\t\t\t      Main Menu \n");
        printf("\t\t************************************\n");
        printf("\t\t\t1.  Sudoku   2.  SAT\n");
        printf("\t\t\t0.  Exit\n");
        printf("\t\t************************************\n");
        printf("\t\t\tPlease choose your operation[0--2]:");
        scanf("%d", &op);
        system("cls");
        switch (op) {
            case 1:
                Sudoku();
                break;
            case 2:
                Sat();
                break;
            case 0:
                exit(0);
            default:
                printf("\t\t\tPlease choose your operation again[0--2]:");
                scanf("%d", &op);
        }
    }
    return 0;
}
