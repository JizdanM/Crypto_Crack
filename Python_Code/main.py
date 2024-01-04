import pyperclip
import sys
import vigenere


def main():
    translateVigenere('Nlkl mn s wgtvxn qglwtai', 'SECRET', 'decrypt')


# Function that encrypts and decrypts a message using the Vigenere cipher
def translateVigenere(message, key, mode):
    if not validMode(mode):
        sys.exit('Specified mode is not valid')
    else:
        print('Processing your message...\n')
        translated = vigenere.translateMessage(message, key, mode)
        print('The %sed message: ' % mode)
        print(translated)
        pyperclip.copy(translated)
        print('\n***\nMessage copied to the clipboard.\n***')


# Function that decrypts messages encrypted using the Vigenere function, without the need of a key
def crackVigenere(message):
    return 0


def validMode(mode):
    if mode.lower() != 'encrypt' and mode.lower() != 'decrypt':
        return False
    else:
        return True


# Call the main function if the script is executed directly, not as a module
if __name__ == '__main__':
    main()
