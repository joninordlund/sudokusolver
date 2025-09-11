#pragma once

#include <vector>
using namespace std;
using Matrix = vector<vector<int>>;
class SudokuSolver
{

public:
    SudokuSolver() {}
    Matrix makeSudokuMatrix(Matrix sudoku);
};