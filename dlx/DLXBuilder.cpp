#include "DLXBuilder.h"

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

    std::vector<int> head;
    for (int i = 1; i <= N; i++)
    {
        head.push_back(i);
    }
    int maxSize = N * M + N + (N + 1); // matrix + header + spacers
    nodes.resize(maxSize);
    nodes[0] = {-1, -1, N, 1, -1}; // root node
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
                Node node{up, down, -1, -1, j};

                if (i == 0)
                {
                    node.left = posMod(j, N + 1);
                    node.right = posMod(j + 2, N + 1);
                    node.col = lenVec[j];
                }
                nodes[matrix[i][j]] = node;
                nodeCount++;
            }
        }
    }
    nodes.resize(nodeCount + N + 1);

    // set up spacer nodes
    int prev = -1;
    for (int i = N + 1; i < nodes.size(); i++)
    {
        if (nodes[i].col < 0)
        {
            if (prev >= 0)
                nodes[i].up = prev + 1;
            nodes[prev].down = i - 1;
            prev = i;
        }
    }
    for (int i = 0; i < nodes.size(); i++)
    {
        nodes[i].print(i);
    }
}

void DLXBuilder::cover(int col)
{
    int p = nodes[col].down;
    while (p != col)
    {
        hide(p);
        p = nodes[p].down;
    }
    int left = nodes[col].left;
    int right = nodes[col].right;
    nodes[left].right = nodes[right].left;
}

void DLXBuilder::hide(int p)
{
    int q = p + 1;
    while (q != p)
    {
        int x = nodes[q].col;
        int up = nodes[q].up;
        int down = nodes[q].down;
        if (x < 0) // q is a spacer
        {
            q = up;
        }
        else
        {
            nodes[up].down = down;
            nodes[down].up = up;
            nodes[x].col = nodes[x].col - 1; // col is here col length
            q++;
        }
    }
}

void DLXBuilder::uncover(int col)
{
    int left = nodes[col].left;
    int right = nodes[col].right;
    nodes[left].right = col;
    nodes[right].left = col;
    int p = nodes[col].up;
    while (p != col)
    {
        unhide(p);
        p = nodes[p].up;
    }
}

void DLXBuilder::unhide(int p)
{
    int q = p - 1;
    while (q != p)
    {
        int x = nodes[q].col;
        int up = nodes[q].up;
        int down = nodes[q].down;
        if (x < 0) // q is a spacer
        {
            q = down;
        }
        else
        {
            nodes[up].down = q;
            nodes[down].up = q;
            nodes[x].col = nodes[x].col + 1; // col is here col length
            q--;
        }
    }
}
