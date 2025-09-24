#include "sudokusolver.h"
#include <vector>
#include <iostream>

using namespace std;
using Matrix = vector<vector<int>>;

Matrix SudokuSolver::makeSudokuMatrix(const Matrix &sudoku)
{
    const int M = 9;

    Matrix mat;
    mat.reserve(M * M * M);

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            for (int n = 1; n <= 9; n++)
            {
                vector<int> row;
                row.resize(4 * 81);
                if (sudoku[i][j] == 0 || sudoku[i][j] == n)
                {
                    // constraints
                    int box = (i / 3) * 3 + (j / 3);
                    row[i * 9 + j] = 1;               // cell
                    row[81 + i * 9 + (n - 1)] = 1;    // row
                    row[162 + j * 9 + (n - 1)] = 1;   // column
                    row[243 + box * 9 + (n - 1)] = 1; // box

                    mat.push_back(std::move(row));
                }
            }
        }
    }
    return mat;
}