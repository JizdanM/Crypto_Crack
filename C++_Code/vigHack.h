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
            int otherFactor = num / i;
            if (otherFactor <= MAX_KEY_LENGTH && otherFactor != 1) {
                factors.push_back(otherFactor);
            }
        }
    }

    sort(factors.begin(), factors.end());
    factors.erase(unique(factors.begin(), factors.end()), factors.end());

    return factors;
}

bool comparePairs(const pair<int, int>& a, const pair<int, int>& b) {
    // Compare in reverse order based on the second element (value)
    return a.second > b.second;
}

vector<pair<int, int>> getMostCommonFactor(unordered_map<string, vector<int>>& seqFactors) {
    unordered_map<int, int> factorCounts;

    for (const auto& seqFactor : seqFactors) {
        const auto& factorList = seqFactor.second;
        for (int factor : factorList) {
            if (factorCounts.find(factor) == factorCounts.end()) {
                factorCounts[factor] = 0;
            }
            factorCounts[factor]++;
        }
    }

    vector<pair<int, int>> factorsByCount;
    for (const auto& factor : factorCounts) {
        if (factor.first <= MAX_KEY_LENGTH) {
            factorsByCount.emplace_back(factor.first, factor.second);
        }
    }

    sort(factorsByCount.begin(), factorsByCount.end(), comparePairs);

    return factorsByCount;
}

vector<int> kasiskiExamination(const string& ciphertext) {
    unordered_map<string, vector<int>> repeatedSeqSpacing = findRepeatingSequenceSpacing(ciphertext);
    
    unordered_map<string, vector<int>> seqFactors;

    for (const auto& seq : repeatedSeqSpacing) {
        seqFactors[seq.first] = vector<int>();
        for (int spacing : seq.second) {
        	const vector<int> usefulFactors = getUsefulFactors(spacing);
			seqFactors[seq.first].insert(seqFactors[seq.first].end(), usefulFactors.begin(), usefulFactors.end());
        }
    }

	vector<pair<int, int>> factorsByCount = getMostCommonFactor(seqFactors);

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
        string nthLetters = getNthSubkeysLetters(nth, mostLikelyKeyLength, ciphertextUp);

        vector<pair<char, int>> freqScores;
        for (char possibleKey : LETTERS) {
            string decryptedText = translateMessage(nthLetters, string(1, possibleKey), "decrypt");
            int freqMatchScore = englishFreqMatchScore(decryptedText);
            freqScores.emplace_back(possibleKey, freqMatchScore);
        }
        
        sort(freqScores.begin(), freqScores.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

        allFreqScores.push_back(vector<pair<char, int>>(freqScores.begin(), freqScores.begin() + NUM_MOST_FREQ_LETTERS));
    }

    if (!SILENT_MODE) {
        for (size_t i = 0; i < allFreqScores.size(); ++i) {
            cout << "Possible letters for letter " << i + 1 << " of the key: ";
            for (const auto& freqScore : allFreqScores[i]) {
                cout << freqScore.first << ' ';
            }
            cout << endl;
        }
    }

    vector<int> indexes(mostLikelyKeyLength, 0);
    int totalCombinations = pow(NUM_MOST_FREQ_LETTERS, mostLikelyKeyLength);

    for (int combination = 0; combination < totalCombinations; ++combination) {
        string possibleKey;
        for (int j = 0; j < mostLikelyKeyLength; ++j) {
            possibleKey += allFreqScores[j][indexes[j]].first;
        }

        if (!SILENT_MODE) {
            cout << "Attempting with key: " << possibleKey << endl;
        }

        string decryptedText = translateMessage(ciphertextUp, possibleKey, "decrypt");

        if (isEnglish(decryptedText)) {
            string origCase;
            for (size_t i = 0; i < ciphertext.size(); ++i) {
                if (isupper(ciphertext[i])) {
                    origCase += toupper(decryptedText[i]);
                } else {
                    origCase += tolower(decryptedText[i]);
                }
            }

            decryptedText = origCase;

            cout << "Possible encryption hack with key " << possibleKey << ":" << endl;
            cout << decryptedText.substr(0, 200) << endl;
            cout << endl << "Enter D if done, anything else to continue hacking:" << endl;

            string response;
            getline(cin, response);

            if (!response.empty() && toupper(response[0]) == 'D') {
                return decryptedText;
            }
        }

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
    auto allLikelyKeyLengths = kasiskiExamination(ciphertext);
    
    if (!SILENT_MODE) {
        cout << "Kasiski Examination results say the most likely key lengths are: ";
        for (int keyLength : allLikelyKeyLengths) {
            cout << keyLength << ' ';
        }
        cout << '\n';
    }

    string hackedMessage;
    
    for (const auto& keyLength : allLikelyKeyLengths) {
        if (!SILENT_MODE) {
            cout << "Attempting hack with key length " << keyLength << " (" << pow(NUM_MOST_FREQ_LETTERS, keyLength) << " possible keys)...\n";
        }
        hackedMessage = attemptHackWithKeyLength(ciphertext, keyLength);
        if (!hackedMessage.empty()) {
            break;
        }
    }

    return hackedMessage;
}

#endif

