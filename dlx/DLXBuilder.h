#pragma once

#include <vector>
#include <iostream>

struct Node
{
    void print(int idx)
    {
        std::cout << "idx: " << idx << "   up " << up << ", down " << down << ", left " << left << ", right " << right << ", col " << col << std::endl;
    }
    int up = -1;
    int down = -1;
    int left = -1;
    int right = -1;
    int col = -1; // either len or top
};

class DLXBuilder
{
private:
    std::vector<Node> nodes;
    std::vector<std::vector<int>> matrix;
    std::vector<int> lenVec;

    void cover(int col);
    void uncover(int col);
    void hide(int i);
    void unhide(int i);

public:
    DLXBuilder(std::vector<std::vector<int>> mat) : matrix(mat) {};
    void build();
};