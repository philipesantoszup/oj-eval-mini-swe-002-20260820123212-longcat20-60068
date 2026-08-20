#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// Implement a signed big integer class that only needs to support simple addition and subtraction

// Integer 2:
// Implement a signed big integer class that supports addition, subtraction, multiplication, and division, and overload related operators

// Do not use any header files other than the following
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

// Do not use "using namespace std;"

namespace sjtu {
class int2048 {
  // todo
private:
    static const int BASE = 1000;  // 10^3 for FFT compatibility
    static const int BASE_DIGITS = 3;
    static constexpr double PI = 3.14159265358979323846;
    
    std::vector<int> digits;  // digits stored in little-endian order
    bool negative;  // true if number is negative
    
    // FFT for polynomial multiplication
    using cd = std::complex<double>;
    
    static void fft(std::vector<cd> & a, bool invert) {
        int n = a.size();
        if (n <= 1) return;
        
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1)
                j ^= bit;
            j ^= bit;
            if (i < j)
                std::swap(a[i], a[j]);
        }
        
        for (int len = 2; len <= n; len <<= 1) {
            double ang = 2 * PI / len * (invert ? -1 : 1);
            cd wlen(cos(ang), sin(ang));
            for (int i = 0; i < n; i += len) {
                cd w(1);
                for (int j = 0; j < len / 2; j++) {
                    cd u = a[i + j];
                    cd v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }
        
        if (invert) {
            for (cd & x : a)
                x /= n;
        }
    }
    
    // Remove leading zeros
    void trim() {
        while (!digits.empty() && digits.back() == 0) {
            digits.pop_back();
        }
        if (digits.empty()) {
            negative = false;
        }
    }
    
    // Check if zero
    bool isZero() const {
        return digits.empty();
    }
    
    // Get absolute value
    int2048 absValue() const {
        int2048 res = *this;
        res.negative = false;
        return res;
    }
    
    // Compare absolute values: -1 if |this| < |other|, 0 if equal, 1 if |this| > |other|
    int absCompare(const int2048 &other) const {
        if (digits.size() != other.digits.size()) {
            return digits.size() < other.digits.size() ? -1 : 1;
        }
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i]) {
                return digits[i] < other.digits[i] ? -1 : 1;
            }
        }
        return 0;
    }
    
    // Add absolute values (both must be non-negative)
    static int2048 absAdd(const int2048 &a, const int2048 &b) {
        int2048 res;
        res.negative = false;
        long long carry = 0;
        size_t n = std::max(a.digits.size(), b.digits.size());
        for (size_t i = 0; i < n || carry; ++i) {
            long long sum = carry;
            if (i < a.digits.size()) sum += a.digits[i];
            if (i < b.digits.size()) sum += b.digits[i];
            res.digits.push_back(sum % BASE);
            carry = sum / BASE;
        }
        return res;
    }
    
    // Subtract absolute values (|a| >= |b|, both non-negative)
    static int2048 absSub(const int2048 &a, const int2048 &b) {
        int2048 res;
        res.negative = false;
        int borrow = 0;
        for (size_t i = 0; i < a.digits.size(); ++i) {
            int diff = a.digits[i] - borrow;
            if (i < b.digits.size()) diff -= b.digits[i];
            borrow = 0;
            if (diff < 0) {
                diff += BASE;
                borrow = 1;
            }
            res.digits.push_back(diff);
        }
        res.trim();
        return res;
    }
    
    // FFT-based multiplication for large numbers
    static int2048 absMulFFT(const int2048 &a, const int2048 &b) {
        if (a.isZero() || b.isZero()) return int2048();
        
        // Use simple multiplication for small numbers
        if (std::min(a.digits.size(), b.digits.size()) < 64) {
            int2048 res;
            res.digits.resize(a.digits.size() + b.digits.size(), 0);
            for (size_t i = 0; i < a.digits.size(); ++i) {
                long long carry = 0;
                for (size_t j = 0; j < b.digits.size() || carry; ++j) {
                    long long cur = res.digits[i + j] + 
                        (long long)a.digits[i] * (j < b.digits.size() ? b.digits[j] : 0) + carry;
                    res.digits[i + j] = cur % BASE;
                    carry = cur / BASE;
                }
            }
            res.negative = false;
            res.trim();
            return res;
        }
        
        // FFT multiplication
        std::vector<cd> fa(a.digits.begin(), a.digits.end());
        std::vector<cd> fb(b.digits.begin(), b.digits.end());
        
        size_t n = 1;
        while (n < a.digits.size() + b.digits.size()) 
            n <<= 1;
        
        fa.resize(n);
        fb.resize(n);
        
        fft(fa, false);
        fft(fb, false);
        
        for (size_t i = 0; i < n; ++i)
            fa[i] *= fb[i];
        
        fft(fa, true);
        
        int2048 res;
        res.negative = false;
        res.digits.resize(n);
        
        long long carry = 0;
        for (size_t i = 0; i < n; ++i) {
            long long cur = carry + (long long)(fa[i].real() + 0.5);
            res.digits[i] = cur % BASE;
            carry = cur / BASE;
        }
        
        while (carry) {
            res.digits.push_back(carry % BASE);
            carry /= BASE;
        }
        
        res.trim();
        return res;
    }
    
    // Multiply absolute values
    static int2048 absMul(const int2048 &a, const int2048 &b) {
        return absMulFFT(a, b);
    }
    
    // Multiply by single digit (for division algorithm)
    static int2048 absMulDigit(const int2048 &a, int digit) {
        if (digit == 0 || a.isZero()) return int2048();
        if (digit == 1) return a;
        
        int2048 res;
        res.negative = false;
        long long carry = 0;
        for (size_t i = 0; i < a.digits.size() || carry; ++i) {
            long long prod = carry;
            if (i < a.digits.size()) prod += (long long)a.digits[i] * digit;
            res.digits.push_back(prod % BASE);
            carry = prod / BASE;
        }
        return res;
    }
    
public:
  // Constructors
  int2048() : negative(false) {}
  int2048(long long x) : negative(false) {
      if (x < 0) {
          negative = true;
          x = -x;
      }
      if (x > 0) {
          digits.push_back(x % BASE);
          if (x >= BASE) {
              x /= BASE;
              while (x > 0) {
                  digits.push_back(x % BASE);
                  x /= BASE;
              }
          }
      }
  }
  int2048(const std::string &s) : negative(false) {
      read(s);
  }
  int2048(const int2048 &other) : digits(other.digits), negative(other.negative) {}

  // The parameter types of the following functions are for reference only, you can choose to use constant references or not
  // If needed, you can add other required functions yourself
  // ===================================
  // Integer1
  // ===================================

  // Read a big integer
  void read(const std::string &s) {
      digits.clear();
      negative = false;
      size_t pos = 0;
      
      if (pos < s.size() && s[pos] == '-') {
          negative = true;
          pos++;
      } else if (pos < s.size() && s[pos] == '+') {
          pos++;
      }
      
      // Skip leading zeros
      while (pos < s.size() && s[pos] == '0') pos++;
      
      if (pos >= s.size()) {
          // Number is zero
          negative = false;
          return;
      }
      
      // Process digits from right to left in chunks of BASE_DIGITS
      for (int i = (int)s.size(); i > (int)pos; i -= BASE_DIGITS) {
          int start = std::max((int)pos, i - BASE_DIGITS);
          int len = i - start;
          digits.push_back(std::stoi(s.substr(start, len)));
      }
  }
  
  // Output the stored big integer, no need for newline
  void print() {
      if (isZero()) {
          std::cout << 0;
          return;
      }
      
      if (negative) {
          std::cout << '-';
      }
      
      // Print most significant digit without leading zeros
      std::cout << digits.back();
      
      // Print remaining digits with leading zeros
      char buf[10];
      for (int i = (int)digits.size() - 2; i >= 0; --i) {
          snprintf(buf, sizeof(buf), "%03d", digits[i]);
          std::cout << buf;
      }
  }

  // Add a big integer
  int2048 &add(const int2048 &other) {
      *this = *this + other;
      return *this;
  }
  
  // Return the sum of two big integers
  friend int2048 add(int2048 a, const int2048 &b) {
      return a + b;
  }

  // Subtract a big integer
  int2048 &minus(const int2048 &other) {
      *this = *this - other;
      return *this;
  }
  
  // Return the difference of two big integers
  friend int2048 minus(int2048 a, const int2048 &b) {
      return a - b;
  }

  // ===================================
  // Integer2
  // ===================================

  int2048 operator+() const {
      return *this;
  }
  
  int2048 operator-() const {
      int2048 res = *this;
      if (!isZero()) {
          res.negative = !res.negative;
      }
      return res;
  }

  int2048 &operator=(const int2048 &other) {
      if (this != &other) {
          digits = other.digits;
          negative = other.negative;
      }
      return *this;
  }

  int2048 &operator+=(const int2048 &other) {
      *this = *this + other;
      return *this;
  }
  
  friend int2048 operator+(int2048 a, const int2048 &b) {
      if (a.isZero()) return b;
      if (b.isZero()) return a;
      
      if (a.negative == b.negative) {
          int2048 res = absAdd(a, b);
          res.negative = a.negative;
          return res;
      }
      
      int cmp = a.absCompare(b);
      if (cmp == 0) return int2048();
      if (cmp > 0) {
          // |a| > |b|
          int2048 res = absSub(a, b);
          res.negative = a.negative;
          return res;
      } else {
          // |b| > |a|
          int2048 res = absSub(b, a);
          res.negative = b.negative;
          return res;
      }
  }

  int2048 &operator-=(const int2048 &other) {
      *this = *this - other;
      return *this;
  }
  
  friend int2048 operator-(int2048 a, const int2048 &b) {
      if (b.isZero()) return a;
      if (a.isZero()) {
          int2048 res = b;
          res.negative = !res.negative;
          return res;
      }
      
      if (a.negative != b.negative) {
          // a - b = a + (-b), and they have different signs
          int2048 res = absAdd(a, b);
          res.negative = a.negative;
          return res;
      }
      
      // Same sign
      int cmp = a.absCompare(b);
      if (cmp == 0) return int2048();
      if (cmp > 0) {
          // |a| > |b|
          int2048 res = absSub(a, b);
          res.negative = a.negative;
          return res;
      } else {
          // |b| > |a|
          int2048 res = absSub(b, a);
          res.negative = !a.negative;
          return res;
      }
  }

  int2048 &operator*=(const int2048 &other) {
      *this = *this * other;
      return *this;
  }
  
  friend int2048 operator*(int2048 a, const int2048 &b) {
      int2048 res = absMul(a, b);
      res.negative = a.negative != b.negative && !res.isZero();
      return res;
  }

  int2048 &operator/=(const int2048 &other) {
      *this = *this / other;
      return *this;
  }
  
  friend int2048 operator/(int2048 a, const int2048 &b) {
      // Floor division (Python-style): rounds toward negative infinity
      // Examples: 10/3=3, -10/3=-4, 10/-3=-4, -10/-3=3
      if (b.isZero()) {
          // Undefined behavior - not expected per problem statement
          return int2048();
      }
      
      if (a.isZero()) return int2048();
      
      // Determine if result should be negative
      bool resultNegative = (a.negative != b.negative);
      
      int cmp = a.absCompare(b);
      if (cmp < 0) {
          // |a| < |b|
          if (resultNegative) {
              // Result is -1 (floor division)
              int2048 res;
              res.negative = true;
              res.digits.push_back(1);
              return res;
          }
          return int2048(); // 0
      }
      
      if (cmp == 0) {
          // |a| == |b|
          int2048 res;
          res.digits.push_back(1);
          res.negative = resultNegative;
          return res;
      }
      
      // Long division for |a| / |b|
      int2048 absA = a.absValue();
      int2048 absB = b.absValue();
      
      // Long division algorithm
      int2048 q;
      int2048 r;
      
      q.digits.resize(absA.digits.size(), 0);
      
      for (int i = (int)absA.digits.size() - 1; i >= 0; --i) {
          // Shift r by BASE and add current digit
          r.digits.insert(r.digits.begin(), absA.digits[i]);
          r.trim();
          
          // Binary search for the digit
          int lo = 0, hi = BASE - 1, digit = 0;
          while (lo <= hi) {
              int mid = (lo + hi) / 2;
              int2048 prod = absMulDigit(absB, mid);
              if (prod.absCompare(r) <= 0) {
                  digit = mid;
                  lo = mid + 1;
              } else {
                  hi = mid - 1;
              }
          }
          
          q.digits[i] = digit;
          if (digit > 0) {
              int2048 prod = absMulDigit(absB, digit);
              r = absSub(r, prod);
          }
      }
      
      q.trim();
      
      // Apply sign and adjust for floor division
      if (resultNegative) {
          // Result should be negative
          if (!r.isZero()) {
              // There's a remainder, so we need to round down (more negative)
              // e.g., -10/3: |10|/|3| = 3 remainder 1, so result is -4
              int2048 one;
              one.digits.push_back(1);
              q = absAdd(q, one);
          }
          q.negative = true;
      } else {
          // Result should be positive
          q.negative = false;
      }
      
      return q;
  }

  int2048 &operator%=(const int2048 &other) {
      *this = *this % other;
      return *this;
  }
  
  friend int2048 operator%(int2048 a, const int2048 &b) {
      // a % b = a - (a / b) * b
      // For Python-style: result has same sign as b (or is zero)
      int2048 q = a / b;
      int2048 res = a - q * b;
      return res;
  }

  friend std::istream &operator>>(std::istream &is, int2048 &x) {
      std::string s;
      is >> s;
      x.read(s);
      return is;
  }
  
  friend std::ostream &operator<<(std::ostream &os, const int2048 &x) {
      if (x.isZero()) {
          os << 0;
          return os;
      }
      
      if (x.negative) {
          os << '-';
      }
      
      // Print most significant digit without leading zeros
      os << x.digits.back();
      
      // Print remaining digits with leading zeros
      char buf[10];
      for (int i = (int)x.digits.size() - 2; i >= 0; --i) {
          snprintf(buf, sizeof(buf), "%03d", x.digits[i]);
          os << buf;
      }
      
      return os;
  }

  friend bool operator==(const int2048 &a, const int2048 &b) {
      if (a.isZero() && b.isZero()) return true;
      if (a.negative != b.negative) return false;
      if (a.digits.size() != b.digits.size()) return false;
      for (size_t i = 0; i < a.digits.size(); ++i) {
          if (a.digits[i] != b.digits[i]) return false;
      }
      return true;
  }
  
  friend bool operator!=(const int2048 &a, const int2048 &b) {
      return !(a == b);
  }
  
  friend bool operator<(const int2048 &a, const int2048 &b) {
      if (a.isZero() && b.isZero()) return false;
      if (a.negative && !b.negative) return true;
      if (!a.negative && b.negative) return false;
      
      int cmp = a.absCompare(b);
      if (a.negative) cmp = -cmp;
      return cmp < 0;
  }
  
  friend bool operator>(const int2048 &a, const int2048 &b) {
      return b < a;
  }
  
  friend bool operator<=(const int2048 &a, const int2048 &b) {
      return !(b < a);
  }
  
  friend bool operator>=(const int2048 &a, const int2048 &b) {
      return !(a < b);
  }
};
} // namespace sjtu

#endif
