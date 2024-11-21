#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

// Структура для розшифрування символу
struct EncodedChar {
    uint16_t row : 4;          // Біти 0-3: номер рядка (4 біти)
    uint16_t highPart : 4;     // Біти 4-7: старша частина ASCII-коду (4 біти)
    uint16_t parity1 : 1;      // Біт 8: біт парності для перших двох полів
    uint16_t lowPart : 4;      // Біти 9-12: молодша частина ASCII-коду (4 біти)
    uint16_t position : 2;     // Біти 13-14: позиція символу в рядку (2 біти)
    uint16_t parity2 : 1;      // Біт 15: біт парності для останніх двох полів
};

// Об’єднання для роботи з зашифрованими даними
union EncodedData {
    uint16_t value;
    EncodedChar fields;
};

int main() {
    const int ROWS = 16;
    const int FINAL_COLS = 16;
    string inputFileName = "encrypted.bin";
    string outputFileName = "output.txt";

    // Читання зашифрованих даних з бінарного файлу
    ifstream inputFile(inputFileName, ios::binary);
    if (!inputFile) {
        cerr << "Не вдалося відкрити файл: " << inputFileName << endl;
        return 1;
    }

    vector<uint16_t> encryptedData;
    uint16_t temp;
    while (inputFile.read(reinterpret_cast<char*>(&temp), sizeof(uint16_t))) {
        encryptedData.push_back(temp);
    }
    inputFile.close();

    // Розшифрування
    string decryptedText[ROWS];
    for (size_t i = 0; i < encryptedData.size(); ++i) {
        EncodedData encoded;
        encoded.value = encryptedData[i];

        int asciiCode = (encoded.fields.highPart << 4) | encoded.fields.lowPart;
        char ch = static_cast<char>(asciiCode);

        if (encoded.fields.row < ROWS && encoded.fields.position < FINAL_COLS) {
            while (decryptedText[encoded.fields.row].length() <= encoded.fields.position) {
                decryptedText[encoded.fields.row] += ' ';
            }
            decryptedText[encoded.fields.row][encoded.fields.position] = ch;
        }
    }

    // Запис результату у текстовий файл
    ofstream outputFile(outputFileName);
    if (!outputFile) {
        cerr << "Не вдалося створити файл: " << outputFileName << endl;
        return 1;
    }

    for (int i = 0; i < ROWS; ++i) {
        outputFile << decryptedText[i] << endl;
    }
    outputFile.close();

    cout << "Розшифрування завершено. Результат записано у файл: " << outputFileName << endl;
    return 0;
}
