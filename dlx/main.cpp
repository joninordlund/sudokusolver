#include <vector>
#include "DLXBuilder.h"

std::vector<std::vector<int>> matrix{
    {0, 0, 1, 0, 1, 0, 0},
    {1, 0, 0, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1},
    {0, 0, 0, 1, 1, 0, 1}};

// std::vector<std::vector<int>> matrix{
//     {0, 0, 1, 0},
//     {1, 0, 1, 1},
//     {0, 1, 0, 0}};

int main()
{
    DLXBuilder builder(matrix);
    builder.build();
    builder.search(0);
    return 0;
}