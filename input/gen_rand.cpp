#include <bits/stdc++.h>

int main()
{
    srand(time(NULL));
    for (int i = 0; i < 100000; ++i)
        std::cout << rand() % 1000 << " " << rand() % 1000 << std::endl;
}

