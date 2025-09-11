#include <vector>
#include "DLXBuilder.h"
#include "sudokusolver.h"

Matrix matrix{
    {0, 0, 1, 0, 1, 0, 0},
    {1, 0, 0, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1},
    {0, 0, 0, 1, 1, 0, 1}};
// sivu 298
const Matrix sudoku{
    {0, 0, 0, 2, 0, 5, 0, 0, 0},
    {0, 0, 8, 0, 0, 0, 2, 0, 0},
    {0, 5, 0, 8, 0, 9, 0, 1, 0},
    {9, 0, 7, 0, 0, 0, 8, 0, 6},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {6, 0, 2, 0, 0, 0, 3, 0, 0},
    {0, 4, 0, 1, 0, 8, 0, 3, 0},
    {0, 0, 6, 0, 0, 0, 5, 0, 0},
    {0, 0, 0, 5, 0, 4, 0, 0, 0}};
// std::, <std::vector<int>> matrix{
//     {0, 0, 1, 0},
//     {1, 0, 1, 1},
//     {0, 1, 0, 0}};

int main()
{
    SudokuSolver solver;
    Matrix mat = solver.makeSudokuMatrix(sudoku);
    DLXBuilder builder(mat);
    Matrix solution = builder.findSolution();
    // builder.build();
    // builder.search(0);
    cout << "Size: " << mat.size() << " * " << mat[0].size() << endl;
    return 0;
}