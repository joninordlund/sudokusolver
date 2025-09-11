#include "DLXBuilder.h"
#include <limits>
#include <cassert>

using namespace std;

Matrix DLXBuilder::findSolution()
{
    build();
    search(0);
    solutionSize++;
    if (!solutionFound)
    {
        std::cout << "No solution found!\n";
        return {};
    }
    // in a normal 9x9 sudoku, solution size should always be 81:
    assert(solutionSize == 81);
    cout << "Solution found!\n";
    solution.resize(solutionSize);

    int count = 0;
    for (int i : solution)
    {
        for (int j = 0; j < 81; j++)
        {
            if (matrix[i][j] > 0)
            {
                count++;
                cout << matrix[i][j] % 9;
            }
        }
        // cout << matrix[i][j] << ",";
    }
    cout << endl;
}
cout << "Count: " << count << endl;
return {};
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
    nodes[0].top = -1;
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

                if (i == 0)
                {
                    node.left = posMod(j, N + 1);
                    node.right = posMod(j + 2, N + 1);
                    node.len = lenVec[j];
                    node.top = -1;
                }
                else
                {
                    node.top = j + 1;
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
    for (int i = 0; i < nodes.size(); i++)
    {
        nodes[i].print(i, N);
    }
}

void DLXBuilder::cover(int col)
{
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
    if (nodes[0].right == 0)
    {
        solutionFound = true;
        return;
    }

    int col = chooseCol();
    cover(col);
    int r = nodes[col].down;
    while (r != col)
    {
        if (x > solutionSize)
        {
            solutionSize = x;
        }
        solution[x] = nodes[r].row;

        int j = r + 1;
        while (j != r)
        {
            if (nodes[j].top < 0) // spacer
            {
                j = nodes[j].up;
                continue;
            }
            cover(nodes[j].top);
            j++;
        }
        search(x + 1);
        j = r - 1;
        while (j != r)
        {
            if (nodes[j].top < 0) // spacer
            {
                j = nodes[j].down;
                continue;
            }
            uncover(nodes[j].top);
            j--;
        }
        r = nodes[r].down;
    }
    uncover(col);
}
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
