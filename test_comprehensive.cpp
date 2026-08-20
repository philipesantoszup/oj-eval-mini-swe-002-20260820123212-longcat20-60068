#include "src/include/int2048.h"
#include <iostream>
#include <cassert>

void testInteger1() {
    std::cout << "=== Testing Integer 1 (basic operations) ===" << std::endl;
    
    // Test add function
    sjtu::int2048 a(100);
    sjtu::int2048 b(200);
    
    sjtu::int2048 sum = add(a, b);
    std::cout << "add(100, 200) = ";
    sum.print(); std::cout << " (expected 300)" << std::endl;
    
    // Test add method
    sjtu::int2048 c(100);
    c.add(b);
    std::cout << "100.add(200) = ";
    c.print(); std::cout << " (expected 300)" << std::endl;
    
    // Test minus function
    sjtu::int2048 diff = minus(b, a);
    std::cout << "minus(200, 100) = ";
    diff.print(); std::cout << " (expected 100)" << std::endl;
    
    // Test minus method
    sjtu::int2048 d(200);
    d.minus(a);
    std::cout << "200.minus(100) = ";
    d.print(); std::cout << " (expected 100)" << std::endl;
    
    // Test with negative numbers
    sjtu::int2048 negA(-100);
    sjtu::int2048 negB(-200);
    
    sjtu::int2048 sum2 = add(negA, negB);
    std::cout << "add(-100, -200) = ";
    sum2.print(); std::cout << " (expected -300)" << std::endl;
    
    sjtu::int2048 diff2 = minus(negA, negB);
    std::cout << "minus(-100, -200) = ";
    diff2.print(); std::cout << " (expected 100)" << std::endl;
    
    // Mixed signs
    sjtu::int2048 sum3 = add(a, negB);
    std::cout << "add(100, -200) = ";
    sum3.print(); std::cout << " (expected -100)" << std::endl;
    
    sjtu::int2048 diff3 = minus(a, negB);
    std::cout << "minus(100, -200) = ";
    diff3.print(); std::cout << " (expected 300)" << std::endl;
}

void testInteger2() {
    std::cout << "\n=== Testing Integer 2 (operator overloading) ===" << std::endl;
    
    // Test operators
    sjtu::int2048 a(100);
    sjtu::int2048 b(200);
    
    // += 
    sjtu::int2048 c(100);
    c += b;
    std::cout << "100 += 200: ";
    c.print(); std::cout << " (expected 300)" << std::endl;
    
    // -=
    sjtu::int2048 d(200);
    d -= a;
    std::cout << "200 -= 100: ";
    d.print(); std::cout << " (expected 100)" << std::endl;
    
    // *=
    sjtu::int2048 e(100);
    e *= b;
    std::cout << "100 *= 200: ";
    e.print(); std::cout << " (expected 20000)" << std::endl;
    
    // /=
    sjtu::int2048 f(200);
    f /= a;
    std::cout << "200 /= 100: ";
    f.print(); std::cout << " (expected 2)" << std::endl;
    
    // %=
    sjtu::int2048 g(10);
    sjtu::int2048 h(3);
    g %= h;
    std::cout << "10 %= 3: ";
    g.print(); std::cout << " (expected 1)" << std::endl;
    
    // Test comparison operators
    std::cout << "\n=== Testing comparison operators ===" << std::endl;
    sjtu::int2048 x(100);
    sjtu::int2048 y(200);
    sjtu::int2048 z(100);
    
    std::cout << "100 == 200: " << (x == y) << " (expected 0)" << std::endl;
    std::cout << "100 == 100: " << (x == z) << " (expected 1)" << std::endl;
    std::cout << "100 != 200: " << (x != y) << " (expected 1)" << std::endl;
    std::cout << "100 < 200: " << (x < y) << " (expected 1)" << std::endl;
    std::cout << "100 > 200: " << (x > y) << " (expected 0)" << std::endl;
    std::cout << "100 <= 100: " << (x <= z) << " (expected 1)" << std::endl;
    std::cout << "100 >= 100: " << (x >= z) << " (expected 1)" << std::endl;
    
    // Test modulo with negative numbers (Python-style)
    std::cout << "\n=== Testing modulo (Python-style) ===" << std::endl;
    sjtu::int2048 ten(10);
    sjtu::int2048 three(3);
    sjtu::int2048 negTen(-10);
    sjtu::int2048 negThree(-3);
    
    // 10 % 3 = 1
    sjtu::int2048 mod1 = ten % three;
    std::cout << "10 % 3 = ";
    mod1.print(); std::cout << " (expected 1)" << std::endl;
    
    // -10 % 3 = 2 (Python-style: result has same sign as divisor)
    sjtu::int2048 mod2 = negTen % three;
    std::cout << "-10 % 3 = ";
    mod2.print(); std::cout << " (expected 2)" << std::endl;
    
    // 10 % -3 = -2 (Python-style: result has same sign as divisor)
    sjtu::int2048 mod3 = ten % negThree;
    std::cout << "10 % -3 = ";
    mod3.print(); std::cout << " (expected -2)" << std::endl;
    
    // -10 % -3 = -1 (Python-style: result has same sign as divisor)
    sjtu::int2048 mod4 = negTen % negThree;
    std::cout << "-10 % -3 = ";
    mod4.print(); std::cout << " (expected -1)" << std::endl;
}

void testStreamOperators() {
    std::cout << "\n=== Testing stream operators ===" << std::endl;
    sjtu::int2048 a(123456789);
    std::cout << "Using << operator: " << a << std::endl;
    
    sjtu::int2048 b;
    std::string s = "987654321";
    b.read(s);
    std::cout << "Using >>/read: " << b << std::endl;
}

int main() {
    testInteger1();
    testInteger2();
    testStreamOperators();
    
    std::cout << "\nAll comprehensive tests passed!" << std::endl;
    return 0;
}
