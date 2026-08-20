#include "src/include/int2048.h"

sjtu::int2048 a, b;

int main()
{
    int T;
    std::cin >> T;
    while (T--)
    {
        std::cin >> a >> b;
        std::cout << a * b << '\n';
        std::cout << (a *= b) << '\n';
        a *= b;
        a *= a;
        a *= b;
        std::cout << (b *= a) << '\n';
    }
}
