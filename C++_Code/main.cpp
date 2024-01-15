#include <iostream>

#include <fstream>
#include <cctype>
#include <cstring>

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <cmath>

#include "vars.h"
#include "vigenere.h"
#include "freqAnalysis.h"
#include "detEnglish.h"
#include "vigHack.h"

using namespace std;

int main(int argc, char *argv[]){
	if (argc == 1){
		cerr << argv[0] << ": no option\n";
		exit(3);
	}
	if (argc == 2){
		// Hacking
		if (strcmp(argv[1], "-hack") == 0){
			// Ask user if he want to use SILENT_MODE
			cout << "Do you wish to enable Silent Mode? (Y/N) ";

            string* response = new string;
            getline(cin, *response);

            if (!response->empty() && toupper((*response)[0]) == 'Y') {
                SILENT_MODE = true;
            }
            
            delete response;
			
			// Message input
			string hackedMessage;
			
			ifstream readFile("input.txt");
			if (readFile.is_open()) {
				if (readFile.peek() == ::eof()) {
        			cerr << "Error: The file is empty, copy the message to input.txt" << endl;
    			} else {
        			string message((istreambuf_iterator<char>(readFile)), istreambuf_iterator<char>());
        			cout << "Message loaded. Starting to crack the message...\n";
					readFile.close();
				
					hackedMessage = hackVigenere(message);
				}
    		} else {
        		cerr << "Unable to open the input file. \n" << endl;
    		}
    		
    		//Message output
    		if(!hackedMessage.empty()){
    			ofstream writeFile("output.txt");
    			if (writeFile.is_open()) {
    				cout << "Writing the hacked message to the output file... \n";
        			writeFile << hackedMessage;
        			cout << "Message coppied to the output file. \n";
					writeFile.close();
    			} else {
        			cerr << "Unable to open the output file. \n" << endl;
    			}
			} else {
				cerr << "Was not able to hack the message";
			}
		}
		else if(strcmp(argv[1], "-encrypt") == 0 || strcmp(argv[1], "-decrypt") == 0){
			cerr << "Specify the key!" << endl;
		}
		else {
			cerr << argv[0] << ": no such option\n";
			exit(1);
		}
	}
	if (argc == 3){
		if(strlen(argv[2]) == 1){
			cerr << "Please specify a stronger key!" << endl;
		}
		// Encryption
		else if (strcmp(argv[1], "-encrypt") == 0){
			string translatedMessage;
			
			cout << "Processing the message... \n";
			
			// Message input
			ifstream readFile("input.txt");
			if (readFile.is_open()) {
				if (readFile.peek() == ::eof()) {
        			cerr << "Error: The file is empty, copy the message to input.txt" << endl;
    			} else {
        			string message((istreambuf_iterator<char>(readFile)), istreambuf_iterator<char>());
        			cout << "Message loaded. \n";
					readFile.close();
				
					translatedMessage = translateMessage(message, argv[2], "encrypt");
				}
    		} else {
        		cerr << "Unable to open the input file. \n" << endl;
    		}
    		
    		// Message output
    		ofstream writeFile("output.txt");
    		if (writeFile.is_open()) {
    			cout << "Writing the encrypted message to the output file... \n";
        		writeFile << translatedMessage;
        		cout << "Message coppied to the output file. \n";
				writeFile.close();
    		} else {
        		cerr << "Unable to open the output file. \n" << endl;
    		}
		}
		// Decryption
		else if (strcmp(argv[1], "-decrypt") == 0){
			string translatedMessage;
			
			cout << "Processing the message... \n";
			
			// Message input
			ifstream readFile("input.txt");
			if (readFile.is_open()) {
				if (readFile.peek() == ::eof()) {
        			cerr << "Error: The file is empty, copy the message to input.txt" << endl;
    			} else {
        			string message((istreambuf_iterator<char>(readFile)), istreambuf_iterator<char>());
        			cout << "Message loaded. \n";
					readFile.close();
			
					translatedMessage = translateMessage(message, argv[2], "decrypt");
				}
    		} else {
        		cerr << "Unable to open the input file. \n" << endl;
    		}
    		
    		// Message output
    		ofstream writeFile("output.txt");
    		if (writeFile.is_open()) {
    			cout << "Writing the decrypted message to the output file... \n";
        		writeFile << translatedMessage;
        		cout << "Message coppied to the output file. \n";
				writeFile.close();
    		} else {
        		cerr << "Unable to open the output file. \n" << endl;
    		}
		}
	}
	if (argc > 3){
		cerr << argv[0] << ": too many strings\n";
		exit(4);
	}
	
	cout << "\nPress Enter to close the terminal.\n";
	getchar();
	exit(0);
}

