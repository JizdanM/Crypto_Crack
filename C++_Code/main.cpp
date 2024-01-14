#include <iostream>

#include <fstream>
#include <cctype>
#include <cstring>
#include <cstdio>

#include <stdio.h>
#include <stdlib.h>

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
		string key;
		
		// Encryption
		if (strcmp(argv[1], "-encrypt") == 0){
			string translatedMessage;
			
			cout << "Processing the message... \n";
			
			// Message input
			ifstream readFile("input.txt");
			if (readFile.is_open()) {
        		string message((istreambuf_iterator<char>(readFile)), istreambuf_iterator<char>());
        		cout << "Message loaded. \n";
				readFile.close();
				
				cout << "Key - ";
				cin >> key;
				cin.ignore();
			
				translatedMessage = translateMessage(message, key, "encrypt");
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
        		string message((istreambuf_iterator<char>(readFile)), istreambuf_iterator<char>());
        		cout << "Message loaded. \n";
				readFile.close();
				
				cout << "Key - ";
				cin >> key;
				cin.ignore();
			
				translatedMessage = translateMessage(message, key, "decrypt");
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
		// Hacking
		else if (strcmp(argv[1], "-hack") == 0){
			// Message input
			string hackedMessage;
			
			ifstream readFile("input.txt");
			if (readFile.is_open()) {
        		string message((istreambuf_iterator<char>(readFile)), istreambuf_iterator<char>());
        		cout << "Message loaded. \n";
				readFile.close();
			
				hackedMessage = hackVigenere(message);
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
		else {
			cerr << argv[0] << ": no such option\n";
			exit(1);
		}
	}
	if (argc > 2){
		cerr << argv[0] << ": too many strings\n";
		exit(4);
	}
	
	cout << "\nPress Enter to close the terminal.\n";
	getchar();
	exit(0);
}

