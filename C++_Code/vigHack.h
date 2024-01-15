#ifndef VIGHACK_H
#define VIGHACK_H

using namespace std;

const int NUM_MOST_FREQ_LETTERS = 4;
const int MAX_KEY_LENGTH = 16;

unordered_map<string, vector<int>> findRepeatingSequenceSpacing(const string& message) {
    unordered_map<string, vector<int>> seqSpacing;
    string formattedMessage = message;
    
    transform(formattedMessage.begin(), formattedMessage.end(), formattedMessage.begin(), ::toupper);
    formattedMessage.erase(remove_if(formattedMessage.begin(), formattedMessage.end(), [](char c) { return !isalpha(c); }), formattedMessage.end());

    for (int seqLen = 3; seqLen <= 5; ++seqLen) {
        for (size_t seqStart = 0; seqStart < formattedMessage.length() - seqLen + 1; ++seqStart) {
            string seq = formattedMessage.substr(seqStart, seqLen);
            
            for (size_t i = seqStart + seqLen; i < formattedMessage.length() - seqLen + 1; ++i) {
                if (formattedMessage.substr(i, seqLen) == seq) {
                	if (seqSpacing.find(seq) == seqSpacing.end()) {
            			seqSpacing[seq] = vector<int>();
        			}
                    seqSpacing[seq].push_back(i - seqStart);
                }
            }
        }
    }

    return seqSpacing;
}

vector<int> getUsefulFactors(int num) {
    vector<int> factors;
    
    if (num < 2){
    	return factors;
	}

    for (int i = 2; i <= MAX_KEY_LENGTH; ++i) {
        if (num % i == 0) {
            factors.push_back(i);
            int* otherFactor = new int(num / i);
            if (*otherFactor <= MAX_KEY_LENGTH && *otherFactor != 1) {
                factors.push_back(*otherFactor);
            }
            delete otherFactor;
        }
    }

    sort(factors.begin(), factors.end());
    factors.erase(unique(factors.begin(), factors.end()), factors.end());

    return factors;
}

vector<pair<int, int>> getMostCommonFactor(unordered_map<string, vector<int>>& seqFactors) {
    unordered_map<int, int> factorCounts;

    for (const pair<string, vector<int>>& seqFactor : seqFactors) {
        const vector<int>& factorList = seqFactor.second;
        for (int factor : factorList) {
            if (factorCounts.find(factor) == factorCounts.end()) {
                factorCounts[factor] = 0;
            }
            factorCounts[factor]++;
        }
    }

    vector<pair<int, int>> factorsByCount;
    for (const pair<int, int>& factor : factorCounts) {
        if (factor.first <= MAX_KEY_LENGTH) {
            factorsByCount.emplace_back(factor.first, factor.second);
        }
    }
    
    factorCounts.clear();

    sort(factorsByCount.begin(), factorsByCount.end(), [](pair<int, int>& a, pair<int, int>& b){
    	return a.second > b.second;
	});

    return factorsByCount;
}

vector<int> kasiskiExamination(const string& ciphertext) {
    unordered_map<string, vector<int>> repeatedSeqSpacing = findRepeatingSequenceSpacing(ciphertext);
    
    unordered_map<string, vector<int>> seqFactors;

    for (const pair<string, vector<int>>& seq : repeatedSeqSpacing) {
        seqFactors[seq.first] = vector<int>();
        for (int spacing : seq.second) {
        	const vector<int> usefulFactors = getUsefulFactors(spacing);
			seqFactors[seq.first].insert(seqFactors[seq.first].end(), usefulFactors.begin(), usefulFactors.end());
        }
    }
    
    repeatedSeqSpacing.clear();

	vector<pair<int, int>> factorsByCount = getMostCommonFactor(seqFactors);
	
	seqFactors.clear();

    vector<int> allLikelyKeyLengths;
    for (const pair<int, int>& factorPair : factorsByCount) {
        allLikelyKeyLengths.push_back(factorPair.first); 
    }

    return allLikelyKeyLengths;
}

string getNthSubkeysLetters(int nth, int keyLength, const string& message) {
    string formattedMessage;
    for (char c : message) {
        if (isalpha(c)) {
            formattedMessage += toupper(c);
        }
    }
    
    transform(formattedMessage.begin(), formattedMessage.end(), formattedMessage.begin(), ::toupper);
    
    int i = nth - 1;
    string letters;
    
    while (i < formattedMessage.length()){
    	letters += formattedMessage[i];
    	i += keyLength;
	}
    
    return letters;
}

string attemptHackWithKeyLength(const string& ciphertext, int mostLikelyKeyLength) {
    string ciphertextUp = ciphertext;
    transform(ciphertextUp.begin(), ciphertextUp.end(), ciphertextUp.begin(), ::toupper);

    vector<vector<pair<char, int>>> allFreqScores;
    for (int nth = 1; nth <= mostLikelyKeyLength; ++nth) {
        string* nthLetters = new string(getNthSubkeysLetters(nth, mostLikelyKeyLength, ciphertextUp));

        vector<pair<char, int>> freqScores;
        for (char possibleKey : LETTERS) {
            string* decryptedText = new string(translateMessage(*nthLetters, string(1, possibleKey), "decrypt"));
            int freqMatchScore = englishFreqMatchScore(*decryptedText);
            delete decryptedText;
            freqScores.emplace_back(possibleKey, freqMatchScore);
        }
        
        delete nthLetters;
        
        sort(freqScores.begin(), freqScores.end(), [](const pair<char, int>& a, const pair<char, int>& b) {
            return a.second > b.second;
        });

        allFreqScores.push_back(vector<pair<char, int>>(freqScores.begin(), freqScores.begin() + NUM_MOST_FREQ_LETTERS));
    }

    if (!SILENT_MODE) {
        for (size_t i = 0; i < allFreqScores.size(); ++i) {
            cout << "Possible letters for letter " << i + 1 << " of the key: ";
            for (const pair<char, int>& freqScore : allFreqScores[i]) {
                cout << freqScore.first << ' ';
            }
            cout << endl;
        }
    }

    vector<int> indexes(mostLikelyKeyLength, 0);
    int totalCombinations = pow(NUM_MOST_FREQ_LETTERS, mostLikelyKeyLength);

    for (int combination = 0; combination < totalCombinations; ++combination) {
        string* possibleKey = new string;
        for (int j = 0; j < mostLikelyKeyLength; ++j) {
            *possibleKey += allFreqScores[j][indexes[j]].first;
        }

        if (!SILENT_MODE) {
            cout << "Attempting with key: " << *possibleKey << endl;
        }

        string* decryptedText = new string(translateMessage(ciphertextUp, *possibleKey, "decrypt"));

        if (isEnglish(*decryptedText)) {
            string* origCase = new string;
            for (size_t i = 0; i < ciphertext.size(); ++i) {
                if (isupper(ciphertext[i])) {
                    *origCase += toupper((*decryptedText)[i]);
                } else {
                    *origCase += tolower((*decryptedText)[i]);
                }
            }

            *decryptedText = *origCase;
            delete origCase;

            cout << "Possible encryption hack with key " << *possibleKey << ":" << endl;
            cout << (*decryptedText).substr(0, 200) << endl;
            cout << endl << "Enter D if done, anything else to continue hacking:" << endl;

            string response;
            getline(cin, response);

            if (!response.empty() && toupper(response[0]) == 'D') {
                return *decryptedText;
            }
        }
        
        delete possibleKey;
        delete decryptedText;

        for (int j = 0; j < mostLikelyKeyLength; ++j) {
            indexes[j] = (indexes[j] + 1) % NUM_MOST_FREQ_LETTERS;
            if (indexes[j] != 0) {
                break; 
            }
        }
    }

    return "";
}

string hackVigenere(const string& ciphertext) {
    vector<int> allLikelyKeyLengths = kasiskiExamination(ciphertext);
    
    if (!SILENT_MODE) {
        cout << "Kasiski Examination results say the most likely key lengths are: ";
        for (int keyLength : allLikelyKeyLengths) {
            cout << keyLength << ' ';
        }
        cout << '\n';
    }

    string hackedMessage;
    
    for (const int& keyLength : allLikelyKeyLengths) {
        if (!SILENT_MODE) {
            cout << "Attempting hack with key length " << keyLength << " (" << pow(NUM_MOST_FREQ_LETTERS, keyLength) << " possible keys)...\n";
        }
        hackedMessage = attemptHackWithKeyLength(ciphertext, keyLength);
        if (!hackedMessage.empty()) {
            return hackedMessage;
        }
    }
    
    if (!SILENT_MODE) {
        cout << "Unable to hack message with likely key length(s). Brute forcing key length...\n";
    }

    for (int keyLength = 1; keyLength <= MAX_KEY_LENGTH; ++keyLength) {
        if (find(allLikelyKeyLengths.begin(), allLikelyKeyLengths.end(), keyLength) != allLikelyKeyLengths.end()) {
            continue;
        }

        if (!SILENT_MODE) {
            cout << "Attempting hack with key length " << keyLength << " (" << pow(NUM_MOST_FREQ_LETTERS, keyLength) << " possible keys)...\n";
        }

        hackedMessage = attemptHackWithKeyLength(ciphertext, keyLength);
        if (!hackedMessage.empty()) {
            return hackedMessage;
        }
    }

    return hackedMessage;
}

#endif

