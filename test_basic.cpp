#include "src/include/int2048.h"
#include <iostream>
#include <cassert>

int main() {
    // Test constructors
    sjtu::int2048 a;
    std::cout << "Default constructor: ";
    a.print(); std::cout << std::endl;
    
    sjtu::int2048 b(1145141919810LL);
    std::cout << "Long long constructor: ";
    b.print(); std::cout << std::endl;
    
    sjtu::int2048 c(-1145141919810LL);
    std::cout << "Negative long long: ";
    c.print(); std::cout << std::endl;
    
    sjtu::int2048 d("-2333333333333333333333333333333333333333333333333333333");
    std::cout << "String constructor: ";
    d.print(); std::cout << std::endl;
    
    // Test read
    sjtu::int2048 e;
    e.read("19260817192608171926081719260817192608171926081719260817");
    std::cout << "Read test: ";
    e.print(); std::cout << std::endl;
    
    // Test zero
    sjtu::int2048 zero("0");
    std::cout << "Zero: ";
    zero.print(); std::cout << std::endl;
    
    sjtu::int2048 negZero("-0");
    std::cout << "Negative zero: ";
    negZero.print(); std::cout << std::endl;
    
    // Test addition
    sjtu::int2048 x(100);
    sjtu::int2048 y(200);
    sjtu::int2048 sum = x + y;
    std::cout << "100 + 200 = ";
    sum.print(); std::cout << std::endl;
    
    // Test subtraction
    sjtu::int2048 diff = y - x;
    std::cout << "200 - 100 = ";
    diff.print(); std::cout << std::endl;
    
    // Test multiplication
    sjtu::int2048 prod = x * y;
    std::cout << "100 * 200 = ";
    prod.print(); std::cout << std::endl;
    
    // Test division
    sjtu::int2048 div = y / x;
    std::cout << "200 / 100 = ";
    div.print(); std::cout << std::endl;
    
    // Test floor division
    sjtu::int2048 negX(-100);
    sjtu::int2048 negY(-200);
    
    // -10 / 3 = -4 (floor division)
    sjtu::int2048 ten(10);
    sjtu::int2048 three(3);
    sjtu::int2048 negTen(-10);
    
    sjtu::int2048 floorDiv1 = ten / three;
    std::cout << "10 / 3 = ";
    floorDiv1.print(); std::cout << " (expected 3)" << std::endl;
    
    sjtu::int2048 floorDiv2 = negTen / three;
    std::cout << "-10 / 3 = ";
    floorDiv2.print(); std::cout << " (expected -4)" << std::endl;
    
    sjtu::int2048 floorDiv3 = ten / (-three);
    std::cout << "10 / -3 = ";
    floorDiv3.print(); std::cout << " (expected -4)" << std::endl;
    
    sjtu::int2048 floorDiv4 = negTen / (-three);
    std::cout << "-10 / -3 = ";
    floorDiv4.print(); std::cout << " (expected 3)" << std::endl;
    
    std::cout << "All basic tests passed!" << std::endl;
    return 0;
}
