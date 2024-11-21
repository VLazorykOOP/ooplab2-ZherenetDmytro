#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

// Структура для шифрування символу
struct EncodedChar {
    uint16_t row : 4;          // Біти 0-3: номер рядка (4 біти)
    uint16_t highPart : 4;     // Біти 4-7: старша частина ASCII-коду (4 біти)
    uint16_t parity1 : 1;      // Біт 8: біт парності для перших двох полів
    uint16_t lowPart : 4;      // Біти 9-12: молодша частина ASCII-коду (4 біти)
    uint16_t position : 2;     // Біти 13-14: позиція символу в рядку (2 біти)
    uint16_t parity2 : 1;      // Біт 15: біт парності для останніх двох полів
};

// Об’єднання для зручності роботи з зашифрованими символами
union EncodedData {
    uint16_t value;
    EncodedChar fields;
};

// Функція для обчислення біта парності
int calculateParity(uint16_t value, int bits) {
    int parity = 0;
    for (int i = 0; i < bits; ++i) {
        parity ^= (value >> i) & 1;
    }
    return parity;
}

int main() {
    const int ROWS = 16;
    const int FINAL_COLS = 16; // Довжина рядка після доповнення
    string inputFileName = "input.txt";
    string outputFileName = "encrypted.bin";

    // Читання тексту з текстового файлу
    ifstream inputFile(inputFileName);
    if (!inputFile) {
        cerr << "Не вдалося відкрити файл: " << inputFileName << endl;
        return 1;
    }

    string text[ROWS];
    for (int i = 0; i < ROWS && getline(inputFile, text[i]); ++i) {
        // Доповнюємо кожен рядок пробілами до 16 символів
        while (text[i].length() < FINAL_COLS) {
            text[i] += ' ';
        }
    }
    inputFile.close();

    // Шифрування
    vector<uint16_t> encryptedData;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < FINAL_COLS; ++col) {
            char ch = text[row][col];
            int asciiCode = static_cast<int>(ch);

            EncodedData encoded;
            encoded.fields.row = row & 0xF;
            encoded.fields.highPart = (asciiCode >> 4) & 0xF;
            encoded.fields.lowPart = asciiCode & 0xF;
            encoded.fields.position = col & 0x3;

            // Розрахунок біта парності
            encoded.fields.parity1 = calculateParity((encoded.fields.row << 4) | encoded.fields.highPart, 8);
            encoded.fields.parity2 = calculateParity((encoded.fields.lowPart << 2) | encoded.fields.position, 6);

            encryptedData.push_back(encoded.value);
        }
    }

    // Запис у бінарний файл
    ofstream outputFile(outputFileName, ios::binary);
    if (!outputFile) {
        cerr << "Не вдалося створити файл: " << outputFileName << endl;
        return 1;
    }
    outputFile.write(reinterpret_cast<char*>(&encryptedData[0]),
                     encryptedData.size() * sizeof(uint16_t));
    outputFile.close();

    cout << "Шифрування завершено. Результат записано у файл: " << outputFileName << endl;
    return 0;
}
