import vars


def translateMessage(message, key, mode):
    translated = []  # Stores the translated message

    keyIndex = 0
    key = key.upper()

    for symbol in message:
        num = vars.LETTERS.find(symbol.upper())
        if num != -1:  # Checks if the letter is in the list
            if mode == 'encrypt':
                num += vars.LETTERS.find(key[keyIndex])
            elif mode == 'decrypt':
                num -= vars.LETTERS.find(key[keyIndex])

            num %= len(vars.LETTERS)  # Wraparound

            # Adds the encrypted/decrypted letter
            if symbol.isupper():
                translated.append(vars.LETTERS[num])
            elif symbol.islower():
                translated.append(vars.LETTERS[num].lower())

            keyIndex += 1  # Move to the next letter
            if keyIndex == len(key):
                keyIndex = 0
        else:
            translated.append(symbol)  # If the letter is not in the list, append without changes

    return ''.join(translated)
