#ifndef FREQUENCY_H
#define FREQUENCY_H

using namespace std;

unordered_map<char, int> getLetterCount(const string& message) {
    unordered_map<char, int> letterCount = {
        {'A', 0}, {'B', 0}, {'C', 0}, {'D', 0}, {'E', 0},
        {'F', 0}, {'G', 0}, {'H', 0}, {'I', 0}, {'J', 0},
        {'K', 0}, {'L', 0}, {'M', 0}, {'N', 0}, {'O', 0},
        {'P', 0}, {'Q', 0}, {'R', 0}, {'S', 0}, {'T', 0},
        {'U', 0}, {'V', 0}, {'W', 0}, {'X', 0}, {'Y', 0},
        {'Z', 0}
    };

    for (char letter : message) {
        char uppercaseLetter = toupper(letter);
        if (LETTERS.find(uppercaseLetter) != string::npos) {
            letterCount[uppercaseLetter]++;
        }
    }

    return letterCount;
}

char getItemAtIndexZero(const vector<char>& items) {
    return items[0];
}

string getFrequencyOrder(const string& message) {
    unordered_map<char, int> letterToFreq = getLetterCount(message);

    unordered_map<int, string> freqToLetter;
    for (char letter : LETTERS) {
        int freq = letterToFreq[letter];
        freqToLetter[freq] += letter;
    }

    for (auto& pair : freqToLetter) {
        sort(pair.second.begin(), pair.second.end(), [](char a, char b) {
            return ETAOIN.find(a) > ETAOIN.find(b);
        });
    }

    vector<pair<int, string>> freqPairs(freqToLetter.begin(), freqToLetter.end());
    sort(freqPairs.begin(), freqPairs.end(), [](auto& a, auto& b) {
        return a.first > b.first;
    });

	// Change striung to a list of strings
    string freqOrder;
    for (auto& pair : freqPairs) {
        freqOrder += pair.second;
    }

    return freqOrder;
}

int englishFreqMatchScore(const string& message) {
    string freqOrder = getFrequencyOrder(message);

    int matchScore = 0;

    for (char commonLetter : ETAOIN.substr(0, 6)) {
        if (freqOrder.substr(0, 6).find(commonLetter) != string::npos) {
            matchScore++;
        }
    }

    for (char uncommonLetter : ETAOIN.substr(ETAOIN.length() - 6)) {
        if (freqOrder.substr(freqOrder.length() - 6).find(uncommonLetter) != string::npos) {
            matchScore++;
        }
    }

    return matchScore;
}

#endif

