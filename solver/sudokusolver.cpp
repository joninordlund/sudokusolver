#include "sudokusolver.h"
#include <vector>
#include <iostream>

vector<vector<int>> SudokuSolver::makeSudokuMatrix()
{
    const int M = 9;

    vector<vector<int>> mat;
    mat.reserve(M * M * M);

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            for (int n = 1; n <= M; n++)
            {
                vector<int> row;
                row.resize(4 * M * M);

                auto box = [](int r, int c)
                { return (r / 3) * 3 + (c / 3); };

                // constraints
                row[i * M + j] = 1;                     // cell
                row[M * M + j * M + n] = 1;             // row
                row[2 * M * M + i * M + n] = 1;         // column
                row[3 * M * M + box(i, j) * M + n] = 1; // box
                cout << i << " " << j << " " << n << endl;

                mat.push_back(std::move(row));
            }
        }
    }
    return mat;
}
