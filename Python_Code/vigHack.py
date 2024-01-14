import itertools
import re
import vigenere
import freqAnalysis
import detectEnglish
import vars


NUM_MOST_FREQ_LETTERS = 4  # Num of letters per subkey
MAX_KEY_LENGTH = 16  # Doesn't try keys longer
NONLETTERS_PATTERN = re.compile('[^A-Z]')


def findRepeatingSequenceSpacing(message):
    # Formats the message to contain only letters
    message = NONLETTERS_PATTERN.sub('', message.upper())

    # Compile a list of seqLen-letter sequences found in message
    seqSpacing = {}  # Key - sequences, Values - list of spacings
    for seqLen in range(3, 6):
        for seqStart in range(len(message) - seqLen):
            # Determines the sequence and stores in seq
            seq = message[seqStart:seqStart + seqLen]

            # Looks for similar sequences
            for i in range(seqStart + seqLen, len(message) - seqLen):
                if message[i:i + seqLen] == seq:
                    # When a repeated seq is found, initiates a blank list
                    if seq not in seqSpacing:
                        seqSpacing[seq] = []

                    seqSpacing[seq].append(i - seqStart)  # Append the space between repetitions
    return seqSpacing


def getUsefulFactors(num):
    # Factors less that the max key length + 1 and not 1

    factors = []  # List of factors

    if num < 2:
        return []  # Numbers <2 have no factors

    # Check integers up to the key max length
    for i in range(2, MAX_KEY_LENGTH + 1):
        if num % i == 0:
            factors.append(i)
            otherFactor = int(num / i)
            if otherFactor < MAX_KEY_LENGTH + 1 and otherFactor != 1:
                factors.append(otherFactor)
    return list(set(factors))  # Remove duplicates


def getItemAtIndexOne(items):
    return items[1]


def getMostCommonFactor(seqFactors):
    factorCounts = {}  # Key - factor. Value - how many times it occurs

    for seq in seqFactors:
        factorList = seqFactors[seq]
        for factor in factorList:
            if factor not in factorCounts:
                factorCounts[factor] = 0
            factorCounts[factor] += 1

    factorsByCount = []
    for factor in factorCounts:
        if factor <= MAX_KEY_LENGTH:
            factorsByCount.append((factor, factorCounts[factor]))

    factorsByCount.sort(key=getItemAtIndexOne, reverse=True)

    return factorsByCount


def kasiskiExamination(chiphertext):

    repeteadSeqSpacing = findRepeatingSequenceSpacing(chiphertext)

    seqFactors = {}
    for seq in repeteadSeqSpacing:
        seqFactors[seq] = []
        for spacing in repeteadSeqSpacing[seq]:
            seqFactors[seq].extend(getUsefulFactors(spacing))

    factorsByCount = getMostCommonFactor(seqFactors)

    allLikelyKeyLengths = []
    for twoIntTuple in factorsByCount:
        allLikelyKeyLengths.append(twoIntTuple[0])

    return allLikelyKeyLengths


def getNthSubkeysLetters(nth, keyLength, message):
    message = NONLETTERS_PATTERN.sub('', message)

    i = nth - 1
    letters = []

    while i < len(message):
        letters.append(message[i])
        i += keyLength

    return ''.join(letters)


def attemptHackWithKeyLength(ciphertext, mostLikelyKeyLength):
    ciphertextUp = ciphertext.upper()

    allFreqScores = []
    for nth in range(1, mostLikelyKeyLength + 1):
        nthLetters = getNthSubkeysLetters(nth, mostLikelyKeyLength, ciphertextUp)

        # Frequency scores of letters in the message
        freqScores = []
        for possibleKey in vars.LETTERS:
            decryptedText = vigenere.translateMessage(nthLetters, possibleKey, 'decrypt')
            keyAndFreqMatchTuple = (possibleKey, freqAnalysis.englishFreqMatchScore(decryptedText))
            freqScores.append(keyAndFreqMatchTuple)
        # Sort by english matching score
        freqScores.sort(key=getItemAtIndexOne, reverse=True)

        allFreqScores.append(freqScores[:NUM_MOST_FREQ_LETTERS])

    if not vars.SILENT_MODE:
        for i in range(len(allFreqScores)):
            print('Possible letters for letter %s of the key: ' % (i + 1), end='')
            for freqScore in allFreqScores[i]:
                print('%s ' % freqScore[0], end='')
            print()

    # Tries every combination of the most likely letters
    for indexes in itertools.product(range(NUM_MOST_FREQ_LETTERS), repeat=mostLikelyKeyLength):
        # Creates every possible key using itertools cartesian product and tests it
        possibleKey = ''
        for i in range(mostLikelyKeyLength):
            possibleKey += allFreqScores[i][indexes[i]][0]

        if not vars.SILENT_MODE:
            print('Attempting with key: %s' % possibleKey)

        decryptedText = vigenere.translateMessage(ciphertextUp, possibleKey, 'decrypt')

        if detectEnglish.isEnglish(decryptedText):
            # Restores the original casing of the message
            origCase = []
            for i in range(len(ciphertext)):
                if ciphertext[i].isupper():
                    origCase.append(decryptedText[i].upper())
                else:
                    origCase.append(decryptedText[i].lower())
            decryptedText = ''.join(origCase)

            # Let user see if the text is decrypted correctly
            print('Possible encryption hack with key %s:' % possibleKey)
            print(decryptedText[:200])  # First 200 characters.
            print()
            print('Enter D if done, anything else to continue hacking:')
            response = input('> ')

            if response.strip().upper().startswith('D'):
                return decryptedText

    # No English-looking decryption found, so return None:
    return None


def hackVigenere(ciphertext):
    # Kasiski Examination to figure out the key length
    allLikelyKeyLengths = kasiskiExamination(ciphertext)
    if not vars.SILENT_MODE:
        keyLengthStr = ''
        for keyLength in allLikelyKeyLengths:
            keyLengthStr += '%s ' % keyLength
        print('Kasiski Examination results say the most likely key lengths are: ' + keyLengthStr + '\n')

    hackedMessage = None

    # Check all possible key lengths
    for keyLength in allLikelyKeyLengths:
        if not vars.SILENT_MODE:
            print('Attempting hack with key length %s (%s possible keys)...' %
                  (keyLength, NUM_MOST_FREQ_LETTERS ** keyLength))
        hackedMessage = attemptHackWithKeyLength(ciphertext, keyLength)
        if hackedMessage is not None:
            break

    # If Kasiski Examination didn't work, brute forces the ley lengths
    if hackedMessage is None:
        if not vars.SILENT_MODE:
            print('Unable to hack message with likely key length(s). Brute forcing key length...')
        for keyLength in range(1, MAX_KEY_LENGTH + 1):
            # Don't check the Kasiski results
            if keyLength not in allLikelyKeyLengths:
                if not vars.SILENT_MODE:
                    print('Attempting hack with key length %s (%s possible keys)...' %
                          (keyLength, NUM_MOST_FREQ_LETTERS ** keyLength))
                hackedMessage = attemptHackWithKeyLength(ciphertext, keyLength)
                if hackedMessage is not None:
                    break
    return hackedMessage
