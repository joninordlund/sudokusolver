#include <iostream>
#include <random>
#include <algorithm>

// Luo satunnainen exact cover -matriisi, jossa on uniikki ratkaisu
std::vector<std::vector<int>> makeRandomExactCover(int N, int extraRows, unsigned seed)
{
    std::vector<std::vector<int>> mat;

    // 1) Pohjana identiteettimatriisi (uniikki ratkaisu)
    for (int i = 0; i < N; i++)
    {
        std::vector<int> row(N, 0);
        row[i] = 1;
        mat.push_back(std::move(row));
    }

    // 2) Lisää ylimääräisiä "häiriörivejä"
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(1, N - 1); // kuinka monta ykköstä rivillä

    for (int k = 0; k < extraRows; k++)
    {
        std::vector<int> row(N, 0);
        int ones = dist(rng);
        for (int j = 0; j < ones; j++)
        {
            int col = rng() % N;
            row[col] = 1;
        }
        mat.push_back(std::move(row));
    }

    // 3) Sekoita rivit
    std::shuffle(mat.begin(), mat.end(), rng);

    // 4) Sekoita sarakkeet
    std::vector<int> perm(N);
    for (int i = 0; i < N; i++)
        perm[i] = i;
    std::shuffle(perm.begin(), perm.end(), rng);

    for (auto &row : mat)
    {
        std::vector<int> newRow(N);
        for (int j = 0; j < N; j++)
        {
            newRow[j] = row[perm[j]];
        }
        row = std::move(newRow);
    }

    return mat;
}

int main()
{
    int N = 10;           // sarakkeiden määrä
    int extraRows = 6;    // ylimääräiset rivit
    unsigned seed = 1234; // voit vaihtaa tähän minkä tahansa luvun

    auto mat = makeRandomExactCover(N, extraRows, seed);

    std::cout << "Matrix size: " << mat.size() << " x " << N << "\n";
    std::cout << "Matrix sudoku1 = {" << std::endl;
    for (auto &row : mat)
    {
        std::cout << "{";
        for (auto v : row)
        {
            std::cout << v << ",";
        }
        std::cout << "},\n";
    }
    std::cout << "};";
}