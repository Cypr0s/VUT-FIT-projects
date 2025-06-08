/*  IZP first project
    Author: xluptak00
    Date: 22-10-2024 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 100

typedef struct{
    char ContactName[MAX_LINE_LENGTH + 1];
    char ContactNumber[MAX_LINE_LENGTH + 1];
} Contact;

bool CheckInput(char* InputString);

bool FindContacts(char* SubString, bool InterruptedSequence);

Contact GetOneContact();

void ConvertName(char *ConvertedName, char* ContactName);

char CharToDigit(char c);

bool CheckContact(char* ContactName, char* PhoneNumber,char* InputString , bool InterruptedSequence);

bool CheckNumber(char* Number,char* SubString);

bool CheckInterruptedNumber(char* Number, char*SubString);


int main(int argc, char* argv[]){
    bool InterruptedSequence = false;
    if(argc > 2){
        if(!(CheckInput(argv[2]))){
            fprintf(stderr,"Second input needs to be a number\n");
            return 1;
        }
        if(strcmp(argv[1], "-s") == 0){
            InterruptedSequence = true;
            if ((!FindContacts(argv[2], InterruptedSequence))){
                return 1;
            }
            return 0;
        }
            
        fprintf(stderr, "Input has to have 1 argument or 2 while the first being -s \n");
        return 1;
    }

    if(argc == 1){
        if(!(FindContacts("", InterruptedSequence))){
            return 1;
        }
        return 0;
    }

    if(!(CheckInput(argv[1]))){
        fprintf(stderr,"Input needs to be a number\n");
        return 1;
    }
    if(!(FindContacts(argv[1], InterruptedSequence))){
        return 1;
    }
    return 0;
}


// Checks if the input is in the sequence of numbers 0 - 9 
bool CheckInput(char* input){
    int InputLength = strlen(input);
    for(int i = 0; i < InputLength; i++){
        if(input[i] < '0' || input[i] > '9'){
            return false;
        }
    }
    return true;
}

/*  Finds all the contacts that match the input number and prints them
    Returns False if eirther Line is too long or Contact is missing name or number */
bool FindContacts(char* InputString, bool InterruptedSequence){
    bool FoundOneContact = false;
    int chr;
    while ((chr = getchar()) != EOF){
        ungetc(chr, stdin);
        Contact NextContact = GetOneContact();
        if ((strlen(NextContact.ContactName) == 0 || strlen(NextContact.ContactNumber) == 0)){
            return false;
        }

        char ConvertedName[strlen(NextContact.ContactName) + 1];
        ConvertName(ConvertedName,NextContact.ContactName); // converts name to numbers
        if(CheckContact(ConvertedName,NextContact.ContactNumber, InputString, InterruptedSequence)){
            fprintf(stdout, "%s, %s\n",NextContact.ContactName, NextContact.ContactNumber);
            FoundOneContact = true;
        }
    }
    if(!(FoundOneContact)){
        fprintf(stdout,"Not Found\n");
    }
    return true;
}


/*  Reads 2 lines at time from stdin (Concact Name, Number)
    If either of the lines is longer than 100 characters returns an empty contact*/
Contact GetOneContact(){
    Contact NewContact;
    char Buffer[MAX_LINE_LENGTH + 2];

    fgets(Buffer, MAX_LINE_LENGTH + 2, stdin);
    if (strlen(Buffer) > MAX_LINE_LENGTH  && Buffer[MAX_LINE_LENGTH] != '\n'){
        fprintf(stderr, "Line exceeds 100 characters\n");
        return NewContact;

    }

    Buffer[strcspn(Buffer, "\n")] = '\0';
    strncpy(NewContact.ContactName, Buffer, MAX_LINE_LENGTH + 1);

    fgets(Buffer, MAX_LINE_LENGTH + 2, stdin);
    if (strlen(Buffer) > MAX_LINE_LENGTH  && Buffer[MAX_LINE_LENGTH] != '\n'){
        fprintf(stderr, "Line exceeds 100 characters\n");
        return NewContact;
    }

    Buffer[strcspn(Buffer, "\n")] = '\0';
    strncpy(NewContact.ContactNumber, Buffer, MAX_LINE_LENGTH + 1);
    return NewContact;
}


// Converts Whole input char array into T9 equivalent digits
void ConvertName(char *ConvertedName, char* ContactName){
    int ContactNameLength = strlen(ContactName);
    for(int i = 0; i < ContactNameLength; i++){
        ConvertedName[i] = CharToDigit(ContactName[i]);
    }
    ConvertedName[ContactNameLength] = '\0';
}


// converts letter a - z to number 0 - 9 by T9 standard for example a||b||c = 2
char CharToDigit(char c) {
    c = tolower(c);
    switch (c) {
        case 'a': case 'b': case 'c': return '2';
        case 'd': case 'e': case 'f': return '3';
        case 'g': case 'h': case 'i': return '4';
        case 'j': case 'k': case 'l': return '5';
        case 'm': case 'n': case 'o': return '6';
        case 'p': case 'q': case 'r': case 's': return '7';
        case 't': case 'u': case 'v': return '8';
        case 'w': case 'x': case 'y': case 'z': return '9';
        default: return '0';
    }
}


bool CheckContact(char* ContactName, char* ContactNumber,char* InputString ,bool InterruptedSequence){
    if(InterruptedSequence){
        return (CheckInterruptedNumber(ContactNumber, InputString)
                 || CheckInterruptedNumber(ContactName, InputString));
    }
    return (CheckNumber(ContactNumber, InputString) || CheckNumber(ContactName, InputString));
}


/*  Sliding window algorithm to check if the input phone number contains a Substring
    Doesnt allow interruptions between characters */
bool CheckNumber(char* Number,char* SubString){
    int NumberLength = strlen(Number);
    int SubStringLength = strlen(SubString);
    for(int i = 0; i < NumberLength - SubStringLength+ 1; i++){
        int j;
        for(j = 0; j < SubStringLength; j++){
            if(Number[i] == '+' && SubString[j] == '0'){
                continue;
            }
            if(Number[i + j] != SubString[j]){
                break;
            }
        }
        if(j == SubStringLength){
            return true;
        }
    }
    return false;
}

/*  Checks if the input Number contains a Substring
    Allows interruptions between characters */ 
bool CheckInterruptedNumber(char* Number, char* SubString){
    int i = 0, j = 0;
    int NumberLength = strlen(Number);
    int SubStringLength = strlen(SubString);

    while (i < NumberLength && j < SubStringLength) {
        if (Number[i]== SubString[j]) {
            j++;
        }
        i++;
    }
    return (j == SubStringLength);
}