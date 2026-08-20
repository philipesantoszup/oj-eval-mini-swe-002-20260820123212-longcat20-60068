#include "src/include/int2048.h"

sjtu::int2048 a, b;

int main()
{
    int T;
    std::cin >> T;
    while (T--)
    {
        std::cin >> a >> b;
        std::cout << a / b << '\n';
        std::cout << (a /= b) << '\n';

        sjtu::int2048 tmp;
        tmp = a * b;
        a *= b;
        a /= a;
        a *= b;
        tmp /= a;
        std::cout << tmp << '\n';
    }
}
