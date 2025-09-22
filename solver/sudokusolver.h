#pragma once

#include <vector>
using namespace std;
using Matrix = vector<vector<int>>;

struct Choice
{
    int row, col, digit;
};

class SudokuSolver
{
    std::vector<Choice> rowMapping;

public:
    SudokuSolver() {}
    Matrix makeSudokuMatrix(const Matrix &sudoku);
    std::vector<Choice> getRowMapping() { return rowMapping; };
};