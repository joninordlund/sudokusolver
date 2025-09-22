#include "DLXBuilder.h"
#include <limits>
#include <algorithm>
#include <cassert>
#include <unordered_set>

vector<int> DLXBuilder::findSolution()
{
    build();
    search(0);

    // solutionSize++;
    // if (allSolutions.size() > 1)
    // {
    //     std::cout << "Sudoku has multiple solutions: " << allSolutions.size() << "\n";
    // }
    // else
    // {
    std::cout << "Sol size: " << solutionSize << endl;
    // std::cout << "Sudoku has unique solution\n";
    solution.resize(solutionSize);
    validateSolutionDebug();
    return solution;
}

void DLXBuilder::print(vector<Choice> rowMapping)
{
    int solNum = 0;
    solution = allSolutions[solNum];
    int count = 0;
    cout << "!!!!!!!!!!!!!!   " << allSolutions[solNum].size() << endl;
    std::sort(allSolutions[solNum].begin(), allSolutions[solNum].end());
    for (int k = 0; k < allSolutions[solNum].size(); k++)
    {
        int nodeIdx = allSolutions[solNum][k];
        int rowIdx = nodes[nodeIdx].row;
        cout << "Row: " << rowIdx << " .... Node: " << nodeIdx << endl;
        // if (rowIdx != -1)
        count++;

        // auto choice = rowMapping[rowIdx];
        // std::cout << "(" << choice.row << "," << choice.col << ")=" << choice.digit << "\n";
    }
    cout << "Count: " << count << endl;
}
void DLXBuilder::printSudoku()
{
    std::sort(solution.begin(), solution.end());
    int grid[9][9] = {0};

    for (int row : solution)
    {
        int rowIndex = nodes[row].row;
        cout << rowIndex << endl;
        if (rowIndex == 0)
            continue; // ohita tyhjät

        int mapIndex = nodes[row].row; // vähennä header
        int r = mapIndex / 81;
        int c = (mapIndex / 9) % 9;
        int d = (mapIndex % 9) + 1;

        grid[r][c] = d;
    }

    // Tulostetaan nätisti
    for (int r = 0; r < 9; r++)
    {
        if (r % 3 == 0 && r != 0)
        {
            std::cout << "------+-------+------\n";
        }
        for (int c = 0; c < 9; c++)
        {
            if (c % 3 == 0 && c != 0)
                std::cout << "| ";
            std::cout << grid[r][c] << " ";
        }
        std::cout << "\n";
    }
}

void DLXBuilder::build()
{
    int M = matrix.size();
    int N = matrix[0].size();
    lenVec.resize(N);

    int idx = N + 2;
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (matrix[i][j] > 0)
            {
                matrix[i][j] = idx++;
                lenVec[j]++;
            }
        }
        idx++;
    }

    vector<int> head;
    for (int i = 1; i <= N; i++)
    {
        head.push_back(i);
    }
    int maxSize = N * M + N + (N + 1); // matrix + header + spacers
    nodes.clear();
    nodes.resize(maxSize);

    nodes[0].up = -1;
    nodes[0].down = -1;
    nodes[0].left = N;
    nodes[0].right = 1;
    nodes[0].top = 0;
    nodes[0].row = 0;
    nodes[0].len = -1;

    matrix.insert(matrix.begin(), head);

    auto posMod = [](int a, int b)
    {
        return ((a % b) + b) % b;
    };

    int nodeCount = 0;
    for (int i = 0; i < M + 1; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (matrix[i][j] > 0)
            {
                int f = posMod(i - 1, M + 1);
                while (true)
                {
                    if (matrix[f][j] > 0)
                        break;
                    f = posMod(f - 1, M + 1);
                }
                int up = matrix[f][j];
                f = posMod(i + 1, M + 1);
                while (true)
                {
                    if (matrix[f][j] > 0)
                        break;
                    f = posMod(f + 1, M + 1);
                }
                int down = matrix[f][j];
                Node node;
                node.up = up;
                node.down = down;
                node.left = -1;
                node.right = -1;
                node.row = i;
                node.top = j + 1;
                if (i == 0)
                {
                    node.left = posMod(j, N + 1);
                    node.right = posMod(j + 2, N + 1);
                    node.len = lenVec[j];
                    // node.top = -1;
                }
                else
                {
                    // node.top = j + 1;
                    node.len = -1;
                }
                nodes[matrix[i][j]] = node;
                nodeCount++;
            }
        }
    }
    nodes.resize(nodeCount + M + 2);

    int prev = -1;
    for (int i = N + 1; i < nodes.size(); i++)
    {
        if (nodes[i].top < 0)
        {
            if (prev >= 0)
            {
                nodes[i].up = prev + 1;
                nodes[prev].down = i - 1;
            }
            prev = i;
        }
    }
    // for (int i = 0; i < nodes.size(); i++)
    // {
    //     nodes[i].print(i, N);
    // }
    // checkSpacers();
    // printRows();
    // validateRows5C();
}

void DLXBuilder::debugStructure()
{
    std::cout << "=== Spacer Check ===\n";
    for (size_t i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].top == -1) // spacer
        {
            int up = nodes[i].up;
            int down = nodes[i].down;
            std::cout << "Spacer idx " << i
                      << " up=" << up
                      << " down=" << down;

            if (up != -1 && nodes[up].down != i)
                std::cout << "  [mismatch: up->down=" << nodes[up].down << "]";
            if (down != -1 && nodes[down].up != i)
                std::cout << "  [mismatch: down->up=" << nodes[down].up << "]";
            std::cout << "\n";
        }
    }

    std::cout << "\n=== Row Traversal ===\n";
    for (size_t i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].top == -1) // spacer
        {
            std::cout << "Row spacer idx " << i << ":\n";

            int q = nodes[i].up; // rivin ensimmäinen node
            if (q == -1)
            {
                std::cout << "  (empty row)\n";
                continue;
            }

            int start = q;
            do
            {
                std::cout << "  idx " << q
                          << " row=" << nodes[q].row
                          << " top=" << nodes[q].top
                          << " up=" << nodes[q].up
                          << " down=" << nodes[q].down
                          << "\n";

                q = nodes[q].down;
            } while (q != start && q != -1);

            std::cout << "  -> back to spacer " << i << "\n";
        }
    }
}
void DLXBuilder::checkSpacers()
{
    std::cout << "=== Spacer Debug ===\n";
    for (size_t i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].top == -1) // tämä on spacer
        {
            int up = nodes[i].up;
            int down = nodes[i].down;

            std::cout << "Spacer idx " << i
                      << " | up=" << up
                      << " (row=" << (up >= 0 ? nodes[up].row : -1) << ")"
                      << " down=" << down
                      << " (row=" << (down >= 0 ? nodes[down].row : -1) << ")";

            if (up >= 0 && nodes[up].top >= 0)
                std::cout << " ✅ up ok";
            else if (up != -1)
                std::cout << " ❌ up epäilyttävä";

            if (down >= 0 && nodes[down].top >= 0)
                std::cout << " ✅ down ok";
            else if (down != -1)
                std::cout << " ❌ down epäilyttävä";

            std::cout << "\n";
        }
    }
}
void DLXBuilder::printRows()
{
    std::cout << "=== Row Traversal Debug ===\n";
    for (size_t i = N + 2; i < nodes.size(); i++)
    {
        if (nodes[i].top == -1) // spacer
        {
            std::cout << "Row spacer idx " << i << ":\n";

            int q = nodes[i].up; // rivin ensimmäinen node
            if (q == -1)
            {
                std::cout << "  (empty row)\n";
                continue;
            }

            int start = q;
            do
            {
                std::cout << "  idx " << q
                          << " row=" << nodes[q].row
                          << " top=" << nodes[q].top
                          << " up=" << nodes[q].up
                          << " down=" << nodes[q].down
                          << "\n";

                q = nodes[q].down; // etene rivissä alaspäin
            } while (q != start && q != -1);

            std::cout << "  -> back to spacer " << i << "\n";
        }
    }
}

void DLXBuilder::validateRows5C()
{
    std::cout << "=== 5C Row Validation ===\n";

    for (int i = 0; i < (int)nodes.size(); i++)
    {
        if (nodes[i].top == -1 && i != 0)
        { // spacer (paitsi header 0)
            int spacer = i;
            std::cout << "Row starting at spacer " << spacer << ":\n";

            int q = nodes[spacer].up; // spacer.up = rivin alku
            int start = q;
            bool ok = true;

            while (true)
            {
                std::cout << "  idx " << q
                          << " row=" << nodes[q].row
                          << " top=" << nodes[q].top
                          << " up=" << nodes[q].up
                          << " down=" << nodes[q].down
                          << "\n";

                // turvacheck: ei saa jäädä ikilenkkiin
                if (nodes[q].top == -1 && q != spacer)
                {
                    std::cout << "   ⚠ Virhe: spacer keskellä riviä!\n";
                    ok = false;
                    break;
                }

                // jos tullaan takaisin spacerille, lopetetaan
                if (q == spacer)
                    break;

                q++;
                if (q >= (int)nodes.size())
                {
                    std::cout << "   ⚠ Virhe: indeksi karkasi ulos!\n";
                    ok = false;
                    break;
                }
            }

            if (ok)
            {
                std::cout << "  ✅ Rivi on validi renkaana spacer " << spacer << " ympärillä.\n";
            }
        }
    }
}
std::string constraintName(int col)
{
    if (col < 81)
    {
        int i = col / 9;
        int j = col % 9;
        return "Cell(" + std::to_string(i) + "," + std::to_string(j) + ")";
    }
    else if (col < 162)
    {
        int idx = col - 81;
        int i = idx / 9;
        int n = (idx % 9) + 1;
        return "Row(" + std::to_string(i) + ") num " + std::to_string(n);
    }
    else if (col < 243)
    {
        int idx = col - 162;
        int j = idx / 9;
        int n = (idx % 9) + 1;
        return "Col(" + std::to_string(j) + ") num " + std::to_string(n);
    }
    else
    {
        int idx = col - 243;
        int b = idx / 9;
        int n = (idx % 9) + 1;
        return "Box(" + std::to_string(b) + ") num " + std::to_string(n);
    }
}
bool DLXBuilder::validateSolutionDebug()
{
    if (!solutionFound)
    {
        std::cout << "Ei ratkaisua, joten ei voi validoida.\n";
        return false;
    }

    int cols = matrix[0].size();
    std::vector<int> coverage(cols, 0);
    std::vector<std::vector<int>> byRow(cols);
    std::sort(solution.begin(), solution.end());
    for (int i = 0; i < solutionSize; i++)
    {
        int rowIdx = solution[i];
        if (rowIdx < 0 || rowIdx >= (int)matrix.size())
        {
            std::cout << "Virhe: solution sisältää rivin " << rowIdx << "\n";
            return false;
        }

        const auto &row = matrix[rowIdx];
        for (int j = 0; j < cols; j++)
        {
            if (row[j] > 0)
            {
                coverage[j]++;
                byRow[j].push_back(rowIdx);
            }
        }
    }

    bool ok = true;
    for (int j = 0; j < cols; j++)
    {
        if (coverage[j] != 1)
        {
            ok = false;
            if (coverage[j] == 0)
            {
                std::cout << "❌ Sarake " << j + 1
                          //   << " (" << constraintName(j)
                          << ") jäi kattamatta!\n";
            }
            else
            {
                std::cout << "❌ Sarake " << j + 1
                          //   << " (" << constraintName(j)
                          << " peitetty " << coverage[j] << " kertaa.\n";
                std::cout << "   Mukana rivit: ";
                for (int r : byRow[j])
                    std::cout << r << " ";
                std::cout << "\n";
            }
        }
    }

    if (ok)
    {
        std::cout << "✅ Sudoku-ratkaisu on validi exact cover!\n";
    }
    else
    {
        std::cout << "❌ Sudoku-ratkaisussa on ongelmia.\n";
    }

    return ok;
}
void DLXBuilder::cover(int col)
{
    std::cout << "Covering col " << col << "\n";
    int left = nodes[col].left;
    int right = nodes[col].right;
    nodes[right].left = left;
    nodes[left].right = right;

    int p = nodes[col].down;
    while (p != col)
    {
        hide(p);
        p = nodes[p].down;
    }
}

void DLXBuilder::hide(int p)
{
    int q = p + 1;
    while (q != p)
    {
        int x = nodes[q].top;
        if (x < 0) // q is a spacer
        {
            q = nodes[q].up;
            continue;
        }
        int up = nodes[q].up;
        int down = nodes[q].down;

        nodes[up].down = down;
        nodes[down].up = up;
        nodes[x].len = nodes[x].len - 1;
        q++;
    }
}

void DLXBuilder::uncover(int col)
{
    std::cout << "Uncovering col " << col << "\n";
    int p = nodes[col].up;
    while (p != col)
    {
        unhide(p);
        p = nodes[p].up;
    }
    int left = nodes[col].left;
    int right = nodes[col].right;
    nodes[left].right = col;
    nodes[right].left = col;
}

void DLXBuilder::unhide(int p)
{
    int q = p - 1;
    while (q != p)
    {
        int x = nodes[q].top;
        if (x < 0) // q is a spacer
        {
            q = nodes[q].down;
            continue;
        }
        int up = nodes[q].up;
        int down = nodes[q].down;

        nodes[up].down = q;
        nodes[down].up = q;
        nodes[x].len = nodes[x].len + 1;
        q--;
    }
}
void DLXBuilder::search(int x)
{
    if (solutionFound)
        return;

    if (nodes[0].right == 0)
    {
        solutionFound = true;
        solutionSize = x;
        std::cout << "\n✅ Ratkaisu löytyi! Solution size=" << x << "\nRows: ";
        for (int i = 0; i < x; i++)
            std::cout << solution[i] << " ";
        std::cout << "\n";
        return;
    }

    int col = chooseCol();
    std::cout << "\n==> Covering col " << col
              << " | Current stack: [ ";
    for (int i = 0; i < x; i++)
        std::cout << solution[i] << " ";
    std::cout << "]\n";

    cover(col);

    int r = nodes[col].down;
    while (r != col)
    {
        // solution[x] = r;
        solution[x] = nodes[r].row;
        std::cout << "Pick row " << nodes[r].row
                  << " (node " << r << ") | Stack: [ ";
        for (int i = 0; i <= x; i++)
            std::cout << solution[i] << " ";
        std::cout << "]\n";

        // Mitä sarakkeita tämä rivi peittää
        int j = r + 1;
        while (j != r)
        {
            if (nodes[j].top < 0)
            {
                j = nodes[j].up;
                continue;
            }
            std::cout << "   -> Row " << nodes[r].row
                      << " covers col " << nodes[j].top
                      << " via node " << j << "\n";
            cover(nodes[j].top);
            j++;
        }

        search(x + 1);

        // Backtrack
        int t = r - 1;
        while (t != r)
        {
            if (nodes[t].top < 0)
            {
                t = nodes[t].down;
                continue;
            }
            std::cout << "   <- Uncover col " << nodes[t].top
                      << " from row " << nodes[r].row
                      << " via node " << t << "\n";
            uncover(nodes[t].top);
            t--;
        }

        std::cout << "Backtracking from row " << nodes[r].row
                  << " | Stack before pop: [ ";
        for (int i = 0; i <= x; i++)
            std::cout << solution[i] << " ";
        std::cout << "]\n";

        r = nodes[r].down;
        if (solutionFound)
            return;
    }

    uncover(col);
    std::cout << "<== Uncovered col " << col
              << " | Returning to depth " << x << "\n";
}

// void DLXBuilder::search(int x)
// {
//     if (solutionFound)
//     {
//         return;
//     }
//     if (nodes[0].right == 0)
//     {
//         solutionFound = true;
//         // solution.resize(solutionSize + 1);
//         // allSolutions.push_back(solution);
//         // solution.clear();
//         return;
//     }

//     int col = chooseCol();
//     cover(col);
//     int r = nodes[col].down;
//     while (r != col)
//     {
//         // std::cout << "Trying row " << nodes[r].row << " via node " << r << "\n";
//         if (x > solutionSize)
//         {
//             solutionSize = x;
//         }
//         // solution[x] = r;
//         solution[x] = nodes[r].row;
//         std::cout << "Pick row " << nodes[r].row << "\n";
//         int j = r + 1;
//         while (j != r)
//         {
//             if (nodes[j].top < 0) // spacer
//             {
//                 j = nodes[j].up;
//                 continue;
//             }
//             std::cout << "Row " << nodes[r].row << " covers col " << nodes[j].top << "\n";
//             cover(nodes[j].top);
//             j++;
//         }
//         search(x + 1);
//         int t = r - 1;
//         while (t != r)
//         {
//             if (nodes[t].top < 0) // spacer
//             {
//                 t = nodes[t].down;
//                 continue;
//             }
//             uncover(nodes[t].top);
//             t--;
//         }
//         r = nodes[r].down;
//     }
//     uncover(col);
// }
int DLXBuilder::chooseCol()
{
    int best = -1;
    int bestLen = numeric_limits<int>::max();

    for (int c = nodes[0].right; c != 0; c = nodes[c].right)
    {
        if (nodes[c].len < bestLen)
        {
            bestLen = nodes[c].len;
            best = c;
        }
    }
    return best;
}
