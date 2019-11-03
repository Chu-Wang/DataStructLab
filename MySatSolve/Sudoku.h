#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED
#include "SAT.h"
/***********************************
*文件名称:Sudoku.h
*文件描述:用来定义数独游戏相关的函数
*日    期:2019-2-21
*版    本：V1.0
*@_@|||||||||||
***********************************/


int Sudoku();

int NewSudoku(Clause **S, SatAnswer **answer, Var_watch var_watch[], int branchDecision[], int sudokuTable[9][9]);

int GenerateSudoku(Clause **S, SatAnswer **answer, Var_watch var_watch[], int branchDecision[]);

int DigHole(SatAnswer *answer, Var_watch var_watch[], int branchDecision[], int sudokuTable[9][9]);

int SolveSudoku(SatAnswer *answer, Var_watch var_watch[], int branchDecision[], int sudokuTable[9][9]);
int dig_watch(int sudokuTable[9][9]);
int dig_test(Clause **S, SatAnswer **answer, Var_watch *var_watch, int *branchDecision);


#endif // SUDOKU_H_INCLUDED
