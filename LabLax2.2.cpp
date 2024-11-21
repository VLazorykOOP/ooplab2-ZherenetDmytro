#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

int calculateParity(int value, int bits) {
    int parity = 0;
    for (int i = 0; i < bits; ++i) {
        parity ^= (value >> i) & 1;
    }
    return parity;
}

int main() {
    const int ROWS = 16;
    const int MAX_COLS = 4;
    const int FINAL_COLS = 16;

    string text[ROWS];
    cout << "Введіть текст, що складається з 16 рядків по 4 символи:\n";

    cin.ignore();
    for (int i = 0; i < ROWS; ++i) {
        cout << "Рядок " << i + 1 << ": ";
        getline(cin, text[i]);
        while (text[i].length() < FINAL_COLS) {
            text[i] += ' ';
        }
    }

    vector<uint16_t> encryptedText;

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < FINAL_COLS; ++col) {
            char ch = text[row][col];
            int asciiCode = static_cast<int>(ch);
            int highPart = (asciiCode >> 4) & 0xF;
            int lowPart = asciiCode & 0xF;
            int rowNumber = row & 0xF;
            int colPosition = col & 0x3;
            int part1 = (rowNumber << 4) | highPart;
            int parity1 = calculateParity(part1, 8);
            int part2 = (lowPart << 2) | colPosition;
            int parity2 = calculateParity(part2, 6);
            uint16_t encryptedChar = (rowNumber << 12)
                                    | (highPart << 8)
                                    | (parity1 << 7)
                                    | (lowPart << 2)
                                    | (colPosition << 0)
                                    | (parity2 << 15);
            encryptedText.push_back(encryptedChar);
        }
    }

    cout << "\nЗашифрований текст (у двійковій формі):\n";
    for (size_t i = 0; i < encryptedText.size(); ++i) {
        cout << "Символ " << i + 1 << ": " << bitset<16>(encryptedText[i]) << endl;
    }

    return 0;
}
