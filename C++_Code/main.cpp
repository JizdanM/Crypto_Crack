#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "vars.h"
#include "vigenere.h"

using namespace std;

int main(int argc, char *argv[]){
	if (argc == 1){
		cout << argv[0] << ": no option\n";
		exit(3);
	}
	if (argc == 2){
		if (strcmp(argv[1], "-encrypt") == 0){
			char key[100];
			cout << "Key - ";
			cin >> key;
			
			char* translatedMessage = translateMessage("Sample Message", key, "encrypt");
			cout << "Translated message - " << translatedMessage << "\n";
		}
		else if (strcmp(argv[1], "-decrypt") == 0){
			char key[100];
			cout << "Key - ";
			cin >> key;
			
			char* translatedMessage = translateMessage("Cekzpc Wiqceeo", key, "decrypt");
			cout << "Translated message - " << translatedMessage << "\n";
		}
		else if (strcmp(argv[1], "-hack") == 0){
			cout << "hacking...\n";
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
	exit(0);
}
