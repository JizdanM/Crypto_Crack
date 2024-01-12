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

#include "vars.h"
#include "vigenere.h"
#include "freqAnalysis.h"

using namespace std;

int main(int argc, char *argv[]){
	if (argc == 1){
		cout << argv[0] << ": no option\n";
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
        		cout << "Unable to open the input file. \n" << endl;
    		}
    		
    		// Message output
    		ofstream writeFile("output.txt");
    		if (writeFile.is_open()) {
    			cout << "Writing the encrypted message to the output file... \n";
        		writeFile << translatedMessage;
        		cout << "Message coppied to the output file. \n";
				writeFile.close();
    		} else {
        		cout << "Unable to open the output file. \n" << endl;
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
        		cout << "Unable to open the input file. \n" << endl;
    		}
    		
    		// Message output
    		ofstream writeFile("output.txt");
    		if (writeFile.is_open()) {
    			cout << "Writing the decrypted message to the output file... \n";
        		writeFile << translatedMessage;
        		cout << "Message coppied to the output file. \n";
				writeFile.close();
    		} else {
        		cout << "Unable to open the output file. \n" << endl;
    		}
		}
		// Hacking
		else if (strcmp(argv[1], "-hack") == 0){
			cout << "hacking...\n";
		}
		else if (strcmp(argv[1], "-test") == 0){
			string testMsg = "This is text written in english language to be able to check the score of the frequency analysis module. The score should be right.";
			
			int score = englishFreqMatchScore(testMsg);
			cout << "The score of the message is - " << score << "\n";
		}
		else {
			cout << argv[0] << ": no such option\n";
			exit(1);
		}
	}
	if (argc > 2){
		cout << argv[0] << ": too many strings\n";
		exit(4);
	}
	
	cout << "\nPress Enter to close the terminal.\n";
	getchar();
	exit(0);
}

