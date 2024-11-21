#include <iostream>
using namespace std;

int main() {
    int a, b, c, d;
    cout << "Введіть a, b, c, d: ";
    cin >> a >> b >> c >> d;

    // 2049 * b = (2048 + 1) * b
    int term1 = (b << 11) + b;

    // d * 15 = (d * 8 + d * 4 + d * 2 + d)
    int d_mult_15 = (d << 3) + (d << 2) + (d << 1) + d;

    // 12 * a = (8 * a + 4 * a)
    int a_mult_12 = (a << 3) + (a << 2);

    // (d * 15 + 12 * a) / 2048
    int term2 = (d_mult_15 + a_mult_12) >> 11;

    // c * 100 = (c * 64 + c * 32 + c * 4)
    int c_mult_100 = (c << 6) + (c << 5) + (c << 2);

    // d * 104 = (d * 64 + d * 32 + d * 8)
    int d_mult_104 = (d << 6) + (d << 5) + (d << 3);

    // Підсумковий вираз
    int result = term1 + term2 - c_mult_100 + d_mult_104;

    cout << "Результат обчислення: " << result << endl;

    return 0;
}
