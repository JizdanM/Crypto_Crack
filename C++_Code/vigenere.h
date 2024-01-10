#ifndef VIGENERE_H
#define VIGENERE_H

using namespace std;

const int MAX_LENGTH = 1000;

char* translateMessage(const char* message, const char* key, const char* mode) {
    char* translated = new char[MAX_LENGTH];  
    translated[0] = '\0';

    size_t keyIndex = 0;
    char uppercaseKey[MAX_LENGTH];
    size_t i = 0;
    while (key[i] != '\0') {
        uppercaseKey[i] = toupper(key[i]);
        i++;
    }
    uppercaseKey[i] = '\0'; 

    i = 0;
    while (message[i] != '\0') {
        char symbol = message[i];
        size_t num = LETTERS.find(toupper(symbol));
        if (num != string::npos) { 
            int keyNum = LETTERS.find(uppercaseKey[keyIndex]);
            
            if (std::strcmp(mode, "encrypt") == 0) {
                num += keyNum;
            } else if (std::strcmp(mode, "decrypt") == 0) {
                num -= keyNum;
                num = (num + LETTERS.length()) % LETTERS.length();
            }
            
            if (num < 0) {
        		num = static_cast<int>((num + LETTERS.length()) % LETTERS.length());
    		} else {
    			num %= LETTERS.length();
			}

            if (isupper(symbol)) {
                translated[i] = LETTERS[num];
            } else if (islower(symbol)) {
                translated[i] = tolower(LETTERS[num]);
            } else {
                translated[i] = symbol;
            }

            keyIndex++;
            if (keyIndex == strlen(key)) {
                keyIndex = 0;
            }
        } else {
            translated[i] = symbol; 
        }

        i++;
    }

    translated[i] = '\0';
    return translated;
}

#endif

