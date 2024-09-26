//Project 1: Ciphers

//Author(s): starter code given by Professor Ethan Ordentlich and Professor Drishika Dey

/* 
 Project Description: The purpose of this project is to accomplish 3 main tasks: encrypting plain text with ciphers
 by a constant amount, encrypting text with a key, and decrypting an encrypted text into plain text. The basis of this
 project is that it is using a user inputted rotation amount and a given alphabet of letters in order to produce the 
 given response back to the user. This project handles I/O + file streams, working with string function + string parsing.
 
 */
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //

// Function prototypes at the top of the file for function decomposition

//--------------------------------------------------------------------------------------------------------------
//Print instructions for using the program.

void printMenu();

//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
//   Returns the 0-based index in the English alphabet where `c` appears,
//   or -1 if `c` is not an uppercase letter in the alphabet.
//  
//   For example:
//   - `findIndexInAlphabet('A')` returns 0
//   - `findIndexInAlphabet('D')` returns 3
//   - `findIndexInAlphabet('+')` returns -1
//   - `findIndexInAlphabet('a')` returns -1

int findIndexInAlphabet(char c);
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
//   Returns `c` rotated by `amount` many characters forward. If we run out
//   of letters in the alphabet, wrap around back to 'A'. This method
//   assumes that `c` is an uppercase letter.
//   For example:
//    - `rot('A', 0)` returns 'A'
//    - `rot('A', 1)` returns 'B'
//    - `rot('Z', 1)` returns 'A'
//    - `rot('A', 10)` returns 'K'
//    - `rot('J', 25)` returns 'I'

char rot(char c, int amount);

//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
//   Returns a new string in which every character has been rotated by `amount`
//   letters. Lowercase letters are converted to uppercase and rotated.
//   Non-alphabetic characters are left alone.
//   For example:
//     `rot("A", 0)` returns "A"
//     `rot("AA", 3)` returns "DD"
//     `rot("HELLO", 0)` returns "HELLO"
//     `rot("HELLO", 1)` returns "IFMMP"
//     `rot("oh HEL-LO!", 1)` returns "PI IFM-MP!"
// 
string rot(string line, int amount);

//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------

/* CaesarEncrypt: This function is a cipher encoder in which it changes a user-given plain text by a 
user-given rotation amount that each letter in the text will change to.*/ 
string CaesarEncrypt(){
    string encryptTxt;
    string strNums;
    int userNum;

    cout << "Enter the text to encrypt:";
    getline(cin, encryptTxt); //user will type in a word or a phrase of their choice to encrypt
    cout << endl;
    cout << "Enter the number of characters to rotate by:";
    getline(cin, strNums); //user will enter an integer of only positive numbers that each letter will rotate by
    userNum = stoi(strNums); //change the string from user input into a number
    cout << endl;

    //user given text will go through string rot, char rot, and then findIndexinAlphabet to produce the encrypted version
    string newText = rot(encryptTxt, userNum);  
    
    return newText;
}

//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------

/* convertToVigenere: This function will ask the user for a word or phrase, and then a key. The function parses
through the key, only looking for letters, and each letter in the phrase will change depending on the alphabet index 
of each character in the key (e.g "abc --> a = 0; b = 1; c = 2") */

string convertToVigenere(){
    string vinTxt;
    string userKey;
    int alphaInd;

    cout << "Enter text to encrypt:";
    getline(cin, vinTxt); //user will type in a word or a phrase of their choice to encrypt
    cout << endl;
    cout << "Enter the Vigenère key:"; //user will enter a small key that they want to see the encryption of that phrase
    getline(cin, userKey);

    vector<int> keytoIntVec;

    //goes through each character in the key and matches the index in the ALPHABET string
    for (int k = 0; k < userKey.size(); k++){ 
        userKey.at(k) = toupper(userKey.at(k));  

        if (isalpha(userKey.at(k))){ //ignores non-letter characters
            char keyChar = userKey.at(k);
            alphaInd = findIndexInAlphabet(keyChar);
            keytoIntVec.push_back(alphaInd); //will find the index and push back the indices found inside of a vector
        }
    }
    
    int subIndex = 0; //counter that goes through the keytoIntVec vector to find the corresponding index to character
    for (int t = 0; t < vinTxt.size(); t++){ //parse through the phrase/ 1st getline given in user input
        if (isalpha(vinTxt.at(t))){
            char vinChar = toupper(vinTxt.at(t));

            /*rotates the character based on placement of index + updates the subIndex to the next character in the key 
            to rotate to */
            char updChar = rot(vinChar, keytoIntVec.at(subIndex)); 
            vinTxt.at(t) = updChar;
            
            subIndex += 1;
            subIndex = subIndex % keytoIntVec.size();
            
        }
    }
    return vinTxt;
}

/* CaesarDecoder: This function will ask the user for a word or phrase that they would like to decrypt, and from there,
based on the input, it will go through a dictionary.txt file that has been opened and closed in the main function to 
find the rotated word in the dictionary  */

void CaesarDecoder(vector<string> &dictStr, vector<string> &wordsVec){
    vector<string> updatedWords;
    bool finalStatement = false;
    int uncleanWrdCount = 0;
    int foundWrdCount = 0;
    string currWord = "";
    string decryptTxt;
    cout << "Enter the text to Caesar-cipher decrypt:";
    getline(cin, decryptTxt); //asks the user for a word or phrase of their choice
    
    string userTxt = "";
    for (int r = 0; r < decryptTxt.size(); r++){
        if (isalpha(decryptTxt.at(r))){
            decryptTxt.at(r) = toupper(decryptTxt.at(r));
            userTxt += decryptTxt.at(r); //accumalate all the words inside of the user inputted string being read 
        }

        /*checks if the character in the user input is a space or has reached the end of the string to push it back
        into a vector called wordsVec, taking in only words and ignoring spaces */
        if (decryptTxt.at(r) == ' ' || r == decryptTxt.size() - 1){ 
            if (userTxt != ""){
                wordsVec.push_back(userTxt);
                userTxt = "";
            } 
        }

    }

    for (int j = 0; j < 26; j++){ //since the rotation amount is unknown, it loop through each rotation of the alphabet
        foundWrdCount=0; //accounts for the words found in the dictionary 
        uncleanWrdCount=0; //all the unclean tokens in the wordsVec
        for (int i = 0; i < wordsVec.size(); i++){
            currWord = wordsVec.at(i);
            uncleanWrdCount++; //increment for each count, will be a 1 each iteration
            
            string updWord = rot(currWord, j); //rotate the word with the current index

            for (int k = 0; k < dictStr.size(); k++){ //checking if word is in the dictionary
                if (dictStr.at(k) == updWord){
                    foundWrdCount++; //if the word is found in a dictionary, then it will change into a 1 + breaks from loop of dictStr
                    break;
                }
            }
                
        }

        //checking for half of the words
        //if foundWrdCount + uncleanWrdCount = 1:1, then it indicates that 1 > 0, which means it is good enoug to decrypt
        if (foundWrdCount > uncleanWrdCount/2){ 
            cout << rot(decryptTxt, j) << endl;
            finalStatement = true;
        }

    }

    if (finalStatement == false){ //if no lines were produced, then no good encryption was found
        cout << "No good decryptions found" << endl;
    }

}
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
int main() {
    string command;

    cout << "Welcome to Ciphers!" << endl;
    cout << "-------------------" << endl;
    cout << endl;

    do {
        printMenu();
        cout << endl
             << "Enter a command (case does not matter): ";

        // Use getline for all user input to avoid needing to handle
        // input buffer issues relating to using both >> and getline
        getline(cin, command);
        cout << endl;

        if (command == "c" || command == "C") { //will go throug the Caesar Encoder function to encrypt the plain text
            string outEncrypt = CaesarEncrypt();
            cout << outEncrypt << endl;
        }
        if (command == "d" || command == "D") { //will go to decrypt the word given by user
            vector<string> wordsVec;
            vector<string> dictStr;
            string readLine;
            ifstream inFile;
            inFile.open("dictionary.txt"); //opens the dictionary file to read into the input stream

            if (!inFile.is_open()){ //condition to check if the file opened exists or not
            cout << "**file not found" << endl;
            return -1;
            }
            while(!inFile.eof()){ //load the words from the dictionary inside of a vector to easily compare with the user input
                getline(inFile, readLine);
                dictStr.push_back(readLine);
            }
            inFile.close();

            CaesarDecoder(dictStr, wordsVec); //takes in the words Vector containing user input words and the dicitonary words into the decryption
        }

        if (command == "v" || command == "V") { // will take user into the Vignere function with a key/input output
            string outVigenere = convertToVigenere();
            cout << outVigenere << endl;
        }
        cout << endl;

    } while (!(command == "x" || command == "X")); //x to exit the program if user is done

    return 0;
}
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
void printMenu() {
    cout << "Ciphers Menu" << endl;
    cout << "------------" << endl;
    // Caesar Ciphers
    cout << "C - Encrypt with Caesar Cipher" << endl;
    cout << "D - Decrypt Caesar Cipher" << endl;
    // Vigenere
    cout << "V - Encrypt with Vigenère" << endl;
    // Utility
    cout << "X - Exit Program" << endl;
}

int findIndexInAlphabet(char c) { //this will loop through to check if the character in user string exists in the alphabet
    int aNum = ALPHABET.find(c);
    if (aNum != string::npos){ //checks if no letter was found or if it is a non-letter character
        return aNum;
    }
    return -1;
}

char rot(char c, int amount) { //check each character int eh string and rotate it by a certain amount
    int charNum = findIndexInAlphabet(c);
    charNum = charNum + amount; //adds the amount to the index found from the alphabet
    if (charNum > 25){ //since it cannot exceed 25, it will subtract to find the corresponding letter, starting from beingnning
        charNum = charNum - 26;
    }
    char newC = ALPHABET.at(charNum); //converts the integer amount into the final ecrypted/decrypted character
    return newC;
}

string rot(string line, int amount) { //will take in the string given from user input
    for (int i = 0; i < line.size(); i++){
        char inputChar;
        line.at(i) = toupper(line.at(i)); //makes everything into uppercase

        if(isalpha(line.at(i))){
            inputChar = rot(line.at(i), amount); //will update the character after it has been rotated
            line.at(i) = inputChar; //replace orginal character in the string with the new encrypted character
        }
    }
    return line;
}
//------------------------------------------------------------------------------------------------------------------