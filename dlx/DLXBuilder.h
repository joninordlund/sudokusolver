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
        std::cout << "idx: " << idx << ", row " << row << ", up " << up << ", down " << down << ", left " << left << ", right " << right << str << x << std::endl;
    }
    int up, down, left, right, top, len, row;
    Node() : up(-1), down(-1), left(-1), right(-1), top(-1), len(-1), row(-1) {}
};

class DLXBuilder
{
private:
    std::vector<Node> nodes;
    std::vector<std::vector<int>> matrix;
    std::vector<int> lenVec;
    std::vector<int> solution;
    int N, M;

    void cover(int col);
    void uncover(int col);
    void hide(int i);
    void unhide(int i);
    int chooseCol();

public:
    DLXBuilder(std::vector<std::vector<int>> mat) : matrix(mat)
    {
        M = matrix.size();
        N = matrix[0].size();
        solution.resize(M);
    };
    void build();
    void search(int x);
};