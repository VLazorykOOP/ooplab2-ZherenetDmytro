#include <iostream>
using namespace std;

unsigned int multiply(unsigned int a, unsigned int b) {
    unsigned int result = 0;
    
    
    while (b > 0) {
    
        if (b & 1) {
            result += a;
        }
        
        // Зсув a вліво (це еквівалентно множенню на 2)
        a <<= 1;
        
        // Зсув b вправо (це еквівалентно діленню на 2)
        b >>= 1;
    }
    
    return result;
}

int main() {
    unsigned int a, b;
    cout << "Enter two unsigned integers: ";
    cin >> a >> b;
    
    unsigned int product = multiply(a, b);
    
    cout << "Product of " << a << " and " << b << " is: " << product << endl;
    
    return 0;
}
