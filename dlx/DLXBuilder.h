#pragma once

#include <vector>
#include <iostream>
#include <string>

struct Node
{
    void print(int idx, int N)
    {
        int x = top;
        std::string str = " ,top ";
        if (idx <= N)
        {
            x = len;
            str = " ,len ";
        }
        std::cout << "idx: " << idx << "   up " << up << ", down " << down << ", left " << left << ", right " << right << str << x << std::endl;
    }
    int up = -1;
    int down = -1;
    int left = -1;
    int right = -1;
    int top = -1;
    int len = -1;
};

class DLXBuilder
{
private:
    std::vector<Node> nodes;
    std::vector<std::vector<int>> matrix;
    std::vector<int> lenVec;
    std::vector<int> solution;

    void cover(int col);
    void uncover(int col);
    void hide(int i);
    void unhide(int i);
    int chooseCol();

public:
    DLXBuilder(std::vector<std::vector<int>> mat) : matrix(mat) { solution.resize(matrix.size()); };
    void build();
    void search(int x);
};