#include <vector>
#include "DLXBuilder.h"
#include "sudokusolver.h"
#include <algorithm>

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

Matrix matrix{
    {0, 0, 1, 0, 1, 0, 0},
    {1, 0, 0, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1},
    {0, 0, 0, 1, 1, 0, 1}};

// sivu 298

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

//     {0, 0, 1, 0},
//     {1, 0, 1, 1},
//     {0, 1, 0, 0}};
Matrix mat6 = {
    {0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0},
    {1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0}};

Matrix matid = {{1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 1}};
Matrix mat7 = {{1, 0, 0, 0},
               {0, 1, 0, 0},
               {0, 1, 1, 0},
               {0, 0, 0, 1}};

bool validateExactCover(const Matrix &mat, const std::vector<int> &chosenRows)
{
    if (chosenRows.empty())
        return false;
    int nCols = mat[0].size();
    std::vector<int> coverCount(nCols, 0);

    for (int row : chosenRows)
    {
        for (int c = 0; c < nCols; ++c)
        {
            if (mat[row - 1][c] == 1)
            {
                coverCount[c]++;
            }
        }
    }

    for (int c = 0; c < nCols; ++c)
    {
        if (coverCount[c] != 1)
        {
            return false;
        }
    }
    return true;
}

// ======== Testit ========
void validateRowMapping()
{
    int rowId = 0;
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            for (int d = 1; d <= 9; d++)
            {
                int rr = rowId / 81;
                int cc = (rowId / 9) % 9;
                int dd = rowId % 9 + 1;

                if (rr != r || cc != c || dd != d)
                {
                    cout << "Mismatch! rowId=" << rowId
                         << " expected (" << r << "," << c << "," << d
                         << ") got (" << rr << "," << cc << "," << dd << ")\n";
                }
                rowId++;
            }
        }
    }
    cout << "Validation done.\n";
}
void runTests()
{
    std::vector<Matrix> tests = {
        // 1) Identiteetti (pitäisi onnistua)
        {{1, 0, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 1, 0},
         {0, 0, 0, 1}},

        // 2) Rivi puuttuu → ei ratkaisua
        {{1, 0, 0},
         {0, 1, 0}},

        // 3) Päällekkäiset rivit (vain toinen voi olla mukana)
        {{1, 1, 0},
         {1, 1, 0},
         {0, 0, 1}},

        // 5) Triviaalisti tyhjä (ei ratkaisua)
        {{0, 0, 0}}};

    int testNum = 1;
    // for (const auto &mat : tests)
    // {
    SudokuSolver solver;
    Matrix mat1 = solver.makeSudokuMatrix(sudoku);

    DLXBuilder dlx(mat1);
    std::cout << "=== Testi " << testNum++ << " ===\n";
    dlx.findSolution();
    std::cout << "DLX solutionFound = " << (dlx.solutionFound ? "YES" : "NO") << "\n";

    if (dlx.solutionFound)
    {
        bool ok = validateExactCover(mat1, dlx.solution);
        cout << "Rows: \n";
        for (int f = 0; f < dlx.solutionSize; f++)
        {
            cout << dlx.solution[f] << " ";
            // for (int g = 0; g < 90; g++)
            // {
            //     int e = dlx.solution[f];
            //     cout << mat1[e - 1][g];
            // }
            // cout << endl;
        }
        cout << endl;
        std::cout << "Validation = " << (ok ? "VALID ✅" : "INVALID ❌") << "\n";
    }
    std::cout << "\n";
    // }
}

int main()
{
    SudokuSolver solver;

    Matrix mat = solver.makeSudokuMatrix(sudoku);
    DLXBuilder builder(mat);
    validateRowMapping();
    // vector<int> solution = builder.findSolution();
    runTests();
    // builder.printSudoku(sudoku);

    // cout << "Size: " << mat.size() << " * " << mat[0].size() << endl;
    return 0;
}