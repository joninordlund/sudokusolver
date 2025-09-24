#include <vector>
#include "DLXBuilder.h"
#include "sudokusolver.h"
#include <algorithm>

// sivu 298
const Matrix sudoku{
    {0, 0, 0, 2, 0, 5, 0, 0, 0},
    {0, 0, 8, 0, 0, 0, 2, 0, 0},
    {0, 5, 0, 8, 0, 9, 0, 1, 0},
    {9, 0, 7, 0, 0, 0, 8, 0, 6},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {6, 0, 2, 0, 0, 0, 3, 0, 9},
    {0, 4, 0, 1, 0, 8, 0, 3, 0},
    {0, 0, 6, 0, 0, 0, 5, 0, 0},
    {0, 0, 0, 5, 0, 4, 0, 0, 0}};

const Matrix sudoku1{
    {1, 0, 0, 0, 0, 0, 0, 4, 5},
    {4, 0, 0, 7, 0, 0, 0, 0, 0},
    {0, 5, 7, 0, 0, 0, 3, 8, 0},
    {8, 0, 0, 5, 9, 0, 4, 0, 0},
    {0, 0, 6, 0, 2, 0, 5, 0, 0},
    {0, 0, 9, 0, 7, 3, 0, 0, 8},
    {0, 1, 5, 0, 0, 0, 8, 6, 0},
    {0, 0, 0, 0, 0, 8, 0, 0, 7},
    {9, 7, 0, 0, 0, 0, 0, 0, 4}};

const Matrix sudoku2{
    {0, 3, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 4, 0, 0, 1, 0, 0},
    {0, 5, 0, 0, 0, 0, 0, 9, 0},
    {2, 0, 0, 0, 0, 0, 6, 0, 4},
    {0, 0, 0, 0, 3, 5, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0},
    {4, 0, 0, 6, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 5, 0},
    {0, 9, 0, 0, 0, 0, 0, 0, 0}};

const Matrix sudoku0{
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0}};

void runTests()
{
    SudokuSolver solver;
    std::pair<Matrix, std::vector<Cell>> res = solver.makeSudokuMatrix(sudoku2);
    DLXBuilder dlx(res.first);
    auto solution = dlx.findSolution();
    std::cout << "DLX solutionFound = " << (dlx.solutionFound ? "YES" : "NO") << "\n";
    std::cout << "Rows after removing clues: " << res.second.size() << "/729" << std::endl;
    Matrix solved(9, std::vector<int>(9, 0));

    for (int rowID : solution)
    {
        auto [i, j, n] = res.second[rowID - 1];
        solved[i][j] = n;
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            std::cout << solved[i][j] << " ";
            if ((j + 1) % 3 == 0 && j < 8)
                std::cout << "| ";
        }
        std::cout << "\n";
        if ((i + 1) % 3 == 0 && i < 8)
            std::cout << "------+-------+------\n";
    }
}

int main()
{
    SudokuSolver solver;
    runTests();
    return 0;
}