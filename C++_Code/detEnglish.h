#ifndef DETENGLISH_H
#define DETENGLISH_H

using namespace std;

vector<string> loadDictionary() {
    ifstream dictionaryFile("dictionary.txt");
    vector<string> englishWords;

    if (dictionaryFile.is_open()) {
        string word;
        while (getline(dictionaryFile, word)) {
            englishWords.push_back(word);
        }
        dictionaryFile.close();
    } else {
        cerr << "Unable to open dictionary file." << endl;
    }

    return englishWords;
}

const vector<string> ENGLISH_WORDS = loadDictionary();

string removeNonLetters(const string& message) {
    string lettersOnly;
    for (char symbol : message) {
        if (FULL_LETTER_PATTERN.find(symbol) != string::npos) {
            lettersOnly += symbol;
        }
    }
    return lettersOnly;
}

double getEnglishCount(const string& message) {
    char* upperMessage = new char[message.length() + 1];
    for (size_t i = 0; i < message.length(); ++i) {
        upperMessage[i] = std::toupper(message[i]);
    }
    upperMessage[message.length()] = '\0';

    string cleanedMessage = removeNonLetters(upperMessage);
    delete[] upperMessage;
    
    vector<string> possibleWords;

    size_t* pos = new size_t(0);
    while ((*pos = cleanedMessage.find(' ')) != string::npos) {
        possibleWords.push_back(cleanedMessage.substr(0, *pos));
        cleanedMessage.erase(0, *pos + 1);
    }
    delete pos;
    possibleWords.push_back(cleanedMessage);

    if (possibleWords.empty()) {
        return 0.0;
    }

    size_t matches = 0;
    for (const string& word : possibleWords) {
        if (find(ENGLISH_WORDS.begin(), ENGLISH_WORDS.end(), word) != ENGLISH_WORDS.end()) {
            matches++;
        }
    }

    return static_cast<double>(matches) / possibleWords.size();
}

bool isEnglish(const string& message) {
	const double wordPercentage = 20.0;
	const double letterPercentage = 85.0;
    double wordsMatch = getEnglishCount(message) * 100.0 >= wordPercentage;

    size_t numLetters = removeNonLetters(message).length();
    double messageLettersPercentage = static_cast<double>(numLetters) / message.length() * 100.0;
    bool lettersMatch = messageLettersPercentage >= letterPercentage;

    return wordsMatch && lettersMatch;
}

#endif

