# This project was created with the help of the yt channel - https://www.youtube.com/@LearningPythonProgramming
# As well as the book of the author (Al Sweigart) - https://nostarch.com/crackingcodes/

import pyperclip
import sys
import vigenere
import detectEnglish
import vigenereDictionaryHacker  # Dictionary brute-force
import vars
import re


def main():
    key = input('Key word please - ')  # Keyword input

    # Silet Mode
    if input('Do you want to execute the app in silent mode? (Y/N): ').upper().startswith('Y'):
        vars.SILENT_MODE = True
    else:
        vars.SILENT_MODE = False

    # Message reading and formating
    inputFile = open('input.txt')
    message = inputFile.read()
    inputFile.close()

    message = messageFormat(message)

    menu = input('Choose the action:\n Encryption - E\n Decryption - D\n Hacking - H\n\n > ')

    # Basic menu
    if menu.upper().startswith('E'):
        encryptMessage(message, key)
    elif menu.upper().startswith('D'):
        decryptMessage(message, key)
    else:
        crackVigenere(message)


# Nested method for encryption
def encryptMessage(message, key):
    translateVigenere(message, key, 'encrypt')


# Nested method for decription
def decryptMessage(message, key):
    translateVigenere(message, key, 'decrypt')


# Function that encrypts and decrypts a message using the Vigenere cipher
def translateVigenere(message, key, mode):
    if not validMode(mode):
        sys.exit('Specified mode is not valid')
    else:
        print('Processing your message...\n')
        translated = vigenere.translateMessage(message, key, mode)
        print('The %sed message: ' % mode)
        print(translated)
        with open('output.txt', 'w') as file:
            file.write(translated)
        print('\n***\nMessage copied to output file.\n***')

        if input('\nDo you wish to copy the message to the clipboard? (Y/N)').upper().startswith('Y'):
            pyperclip.copy(translated)
            print('\n***\nMessage copied to your clipboard.\n***')


# Decrypts messages encrypted using the Vigenere function, without the need of a key
def crackVigenere(message):
    return message


# Formats the input message to get rid of any non-letter cahracters
def messageFormat(message):
    formated = re.sub(f'[^{re.escape(vars.FULL_LETTER_PATTERN)}\n]', '', message)
    return formated


def validMode(mode):
    if mode.lower() != 'encrypt' and mode.lower() != 'decrypt':
        return False
    else:
        return True


# Call the main function if the script is executed directly, not as a module
if __name__ == '__main__':
    main()
