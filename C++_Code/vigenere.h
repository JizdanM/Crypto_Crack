#ifndef VIGENERE_H
#define VIGENERE_H

using namespace std;

const int MAX_LENGTH = 1000;

string translateMessage(const string& message, const string& key, const string& mode) {
    string translated;

    size_t* keyIndex = new size_t(0);
    string* uppercaseKey = new string(key);
    transform(uppercaseKey->begin(), uppercaseKey->end(), uppercaseKey->begin(), ::toupper);

    size_t i = 0;
    while (i < message.length()) {
        char symbol = message[i];
        size_t num = LETTERS.find(toupper(symbol));
        if (num != string::npos) {
            int keyNum = LETTERS.find((*uppercaseKey)[*keyIndex]);

            if (mode == "encrypt") {
                num += keyNum;
            } else if (mode == "decrypt") {
                num -= keyNum;
                num = (num + LETTERS.length()) % LETTERS.length();
            }

            if (num < 0) {
                num = static_cast<int>((num + LETTERS.length()) % LETTERS.length());
            } else {
                num %= LETTERS.length();
            }

            if (isupper(symbol)) {
                translated += LETTERS[num];
            } else if (islower(symbol)) {
                translated += tolower(LETTERS[num]);
            } else {
                translated += symbol;
            }

            (*keyIndex)++;
            if (*keyIndex == uppercaseKey->length()) {
                *keyIndex = 0;
            }
        } else {
            translated += symbol;
        }

        i++;
    }

    delete keyIndex;
    delete uppercaseKey;

    return translated;
}

#endif

