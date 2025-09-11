#include "sudokusolver.h"
#include <vector>
#include <iostream>

using namespace std;
using Matrix = vector<vector<int>>;

Matrix SudokuSolver::makeSudokuMatrix(Matrix sudoku)
{
    const int M = 9;

    Matrix mat;
    mat.reserve(M * M * M);

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            for (int n = 0; n < M; n++)
            {
                vector<int> row;
                row.resize(4 * M * M, 0);

                if (sudoku[i][j] == 0 || sudoku[i][j] == n + 1)
                {
                    // constraints
                    int box = (i / 3) * 3 + (j / 3);
                    row[i * M + j] = 1;               // cell
                    row[M * M + j * M + n] = 1;       // row
                    row[2 * M * M + i * M + n] = 1;   // column
                    row[3 * M * M + box * M + n] = 1; // box

                    mat.push_back(std::move(row));
                }
            }
        }
    }
    return mat;
}
