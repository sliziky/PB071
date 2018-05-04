/*Link to the task: https://www.fi.muni.cz/pb071/hw/homework-01/ */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw01.h"

/****  UTILITIES  ************************************************************/


bool isOneOf(char c, char str[])
{
    return strchr(str, c) != NULL;
}

bool isSentenceStopper(char c){
    if (isOneOf(c,".?!")){
        return true;
    }
    return false;
}

bool isOpeningBracket(char c){
    if (isOneOf(c,"([{")){
        return true;
    }
    return false;
}

bool isClosingBracket(char c){
    if (isOneOf(c,")]}")){
        return true;
    }
    return false;
}

bool isWhiteSpaceCharacter(char c){
    if (isOneOf(c,"(){}[] "))
        return true;
    return false;
}

bool isSeparator (char c){
    if (isOneOf(c,".,!?"))
        return true;
    return false;
}

bool isBracket(char c){
    if (isOneOf(c,"()[]{}"))
        return true;
    return false;
}
char oppositeBracket (char c){
    switch (c){
    case ')':
        return '(';
    case ']':
        return '[';
    case '}':
        return '{';
    case '(':
        return ')';
    case '[':
        return ']';
    case '{':
        return '}';
    }
    return ' ';
}
/****  HERE BEGINS YOUR HOMEWORK  ********************************************/

/*
 * There shall be no two consecutive spaces in the string.
 * You are to FIND and FIX an error in this code.
 *
 * Examples:
 *  Wrong: "Hello  world."
 *  Wrong: "Hello.  World."
 *  Right: "Hello world."
 *  Right: "Hello. Wonderful. World."
 */


bool noTwoConsecutiveSpaces(char str[], size_t len)
{
    for (size_t i = 1; i < len; i++) {
        if (isspace(str[i]) && isspace(str[i - 1])) { // two consecutive spaces
            return false;
        }
    }
    return true;
}


/**
 * The string represents a list of sentences. A sentence ends with with a
 * '.', '?', or a '!' sign, and starts with the first nonspace character
 * in the string or after the end of the previous sentence.
 *
 * Hint: see 'isupper' function
 */



bool sentenceStartsWithUppercase(char str[], size_t len)
{
    bool inSentence = false;

    for (size_t j = 0; j < len; j++) {
        if (!isWhiteSpaceCharacter(str[j]) && !inSentence) { // sentence found
            inSentence = true;
            if (!isupper(str[j])) { // first letter is NOT uppercase
                return false;
            }
        }
        if (isSentenceStopper(str[j])) { // end of sentence
            inSentence = false;
        }

    }
    return true;
}





bool allSentencesAreClosed(char str[], size_t len)
{

    bool sentenceFound = false;
    bool separatorFound = false;

    for (int i = len - 1; i >= 0; i--) {

        if (isSentenceStopper(str[i])) { // sentence separator
            separatorFound = true;
        }
        if (!isWhiteSpaceCharacter(str[i])) { // non whitespace character
            sentenceFound = true;
        }
        if (sentenceFound && !separatorFound) { // if sentence was found
            // earlier than separator
            return false;
        }
    }

    return true;
}

/**
 * There is no separator with a space before it.
 *
 *  Wrong: "Hello, World !"
 *  Wrong: "Hello , World!"
 *  Right: "Hello, World!"
 */


bool noSpaceBeforeSeparators(char str[], size_t len)
{
    if (!len) {
        return true;
    }
    for (size_t i = 1; i < len; i++) {
        if (isSeparator(str[i]) && isspace(str[i - 1])) { // space before
            return false;                                         // separator
        }
    }
    return true;
}

/**
 * Every seprator (i.e. '.', ',', '!' or '?') is followed by a space,
 * with the exception of the separator is the last character in the string.
 *
 *  Wrong: "Hello,world."
 *  Right: "Hello, world."
 */



bool spaceAfterSeparators(char str[], size_t len)
{
    if (!len) {
        return true;
    }
    for (size_t i = 0; i < len - 1; i++) {
        if (isSeparator(str[i]) && !isspace(str[i + 1])) { // space after
            return false;                                            // separator
        }
    }
    return true;
}

/**
 * Every opening bracket '(' is properly closed with ')' and there are no
 * nested parentheses.
 *
 *  Wrong: ":("
 *  Right: "Hello world (again)!"
 *  Wrong: "Hello, (tiny (puny)) world!"
 */


bool properParenthesesPairing(char str[], size_t len)
{
    if (!len)
        return true;
    char storedOpeningBracket = ' ';
    char storedClosingBracket = ' ';
    bool openingFound = false;
    for (size_t i = 0; i < len; i++) {
        if (isBracket(str[i]) && openingFound){
            storedClosingBracket = str[i];
            if (storedOpeningBracket != oppositeBracket(storedClosingBracket)){
                return false;
            }
            if (isClosingBracket(str[i]) && storedOpeningBracket == ' '){
                return false;
            }
            openingFound = false;
            storedOpeningBracket = ' ';
            continue;
        }
        if (isOpeningBracket(str[i])){
            openingFound = true;
            storedOpeningBracket = str[i];
        }
        if (isClosingBracket(str[i]) && !openingFound){
            return false;
        }
    }

    if (!openingFound)
        return true;
    return false;
}

/****  BONUS  ****************************************************************/

/*
 * You are not required to implement the following functions, but you
 * can get up to 3 bonus points if you do it correctly.
 */

/**
 * Parentheses should not cross sencentes. That is, if a bracket is opened
 * outside of a sentence, it must be closed outside as well.
 * On the other hand, a bracket opened inside a sentence has to be
 * closed before the sentence ends.
 *
 *  Right: "(First sentence. Second sentence.) Third sentence."
 *  Right: "First (inside) sentence."
 *  Wrong: "First (sentence. Second) sentence."
 *  Wrong: "(First sentence. Second) sentence."
 */
bool noParenthesesSentenceCrossing(char str[], size_t len)
{
    bool openedInside = false;
    bool closedInside = false;
    bool insideSent = false;
    bool outsideSent = true;

    for (size_t i = 0; i < len; i++) {

        if (!isWhiteSpaceCharacter(str[i])) {
            insideSent = true;
            outsideSent = false;
        }
        if (isSentenceStopper(str[i])) {
            insideSent = false;
            outsideSent = true;
        }

        if (isOpeningBracket(str[i]) && insideSent) {
            openedInside = true;
        }

        if (isOpeningBracket(str[i]) && outsideSent) {
            openedInside = false;
        }

        if (isClosingBracket(str[i]) && insideSent) {
            closedInside = true;
            if (!openedInside && closedInside) {
                return false;
            }
            return true;
        }

        if (isClosingBracket(str[i]) && outsideSent) {
            closedInside = false;
            if (openedInside && !closedInside) {
                return false;
            }
            return true;
        }
        if (openedInside && outsideSent) {
            return false;
        }
    }

    return true;
}

/**
 * There can be double quotes inside the text '"', but always in pair.
 * Opening quote must not be followed by a space and similarly
 * there can be no space before the closing quote.
 */
bool noSpaceBeforeClosingOrAfterOpeningApostrophe(char str[], size_t len)
{
    bool opening = true;
    bool closing = false;
    unsigned int counter = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '"') {
            if (opening && isspace(str[i + 1])) { // space after quote
                return false;
            }
            if (closing && isspace(str[i - 1])) { // space before quote
                return false;
            }
            counter++;
        }
        if (counter % 2 == 0) {
            opening = true;
            closing = false;
        } else {
            opening = false;
            closing = true;
        }
    }
    if (counter % 2 != 0) {
        return false;
    }
    return true;
}

