import vars


def getLetterCount(message):
    # Returns a dictionary with letters and their score representing the frequency of the letter use
    letterCount = {'A': 0, 'B': 0, 'C': 0, 'D': 0, 'E': 0, 'F': 0, 'G': 0, 'H': 0, 'I': 0, 'J': 0, 'K': 0, 'L': 0,
                   'M': 0, 'N': 0, 'O': 0, 'P': 0, 'Q': 0, 'R': 0, 'S': 0, 'T': 0, 'U': 0, 'V': 0, 'W': 0, 'X': 0,
                   'Y': 0, 'Z': 0, }

    for letter in message.upper():
        if letter in vars.LETTERS:
            letterCount[letter] += 1

    return letterCount


def getItemAtIndexZero(items):
    return items[0]


def getFrequencyOrder(message):
    # Returns the string of the most frequent used letters in the message

    # Gets the letter frequency in the message
    letterToFreq = getLetterCount(message)

    # Make a dictionary of each frequency to each corresponding letter
    freqToLetter = {}
    for letter in vars.LETTERS:
        if letterToFreq[letter] not in freqToLetter:
            freqToLetter[letterToFreq[letter]] = [letter]
        else:
            freqToLetter[letterToFreq[letter]].append(letter)

    # Puts each list of letters in reverse "ETAOIN" order and convert to a string
    for freq in freqToLetter:
        freqToLetter[freq].sort(key=vars.ETAOIN.find, reverse=True)
        freqToLetter[freq] = ''.join(freqToLetter[freq])

    # Converts freqToLetter dictionary to a list of tuple pairs and sorts them
    freqPairs = list(freqToLetter.items())
    freqPairs.sort(key=getItemAtIndexZero, reverse=True)

    # Extracts all letters to a final string
    freqOrder = []
    for freqPair in freqPairs:
        freqOrder.append(freqPair[1])

    return ''.join(freqOrder)


def englishFreqMatchScore(message):
    # Returns the numbers of matches of the english language letters with the message letters
    # The match is considered first and last 6 letters from the lists

    freOrder = getFrequencyOrder(message)

    matchScore = 0

    # Finds the number of matches for first 6 letters
    for commonLetter in vars.ETAOIN[:6]:
        if commonLetter in freOrder[:6]:
            matchScore += 1

    # Finds the number of matches for last 6 letters
    for uncommonLetter in vars.ETAOIN[-6:]:
        if uncommonLetter in freOrder[-6:]:
            matchScore += 1

    return matchScore
