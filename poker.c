/* Link to the task: https://www.fi.muni.cz/pb071/hw/homework-02/*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


const int CARDS = 7;
const int CARDS_TABLE = 16;
const int CARD_LENGTH = 3;
const int PLAYERS = 2;

void play_poker();
void read_cards();
void best_card();
void swap(int *xp, int *yp);
void analyze_cards(int val1[], int val2[], char col1[], char col2[]);
void parseCards(char player[], char deck[], char arr[CARDS][CARD_LENGTH]);
void sort(int arr[], char arr2[], int n);
void swapChar(char *xp, char *yp);
int In(int value, int val2[]);
int countChar(char str[], char c);
int occurence (int x,int arr[]);

int* kickerOnePair(int arr[]);
int* kickerTwoPair(int arr[]);
int* kickerThreePair(int arr[]);
int* kickerThrees(int arr[]);
int kickerFour(int arr[]);
int* removeDuplicates(int arr[]);

int straightFlush(int arr1[], int arr2[], char col1[], char col2[]);
int count_four(int arr[]);
int* fullHouse(int arr[]);
int* straight (int arr[]);
int* flush(int arr[],char col[]);
int count_threes(int arr[]);
int count_pairs(int arr[]);
int high_card(int val1[], int val2[]);

int compareKickerFour(int arr1[], int arr2[]);
int compare_fullhouse(int *arr1, int *arr2);
int compareFullHouse(int arr1[], int arr2[]);
int compareStraight(int arr1[], int arr2[]);
int compareFlush(int arr1[], int arr2[], char col1[], char col2[]);
int compareKickerThrees(int arr1[], int arr2[]);
int compareKickerPairs(int arr1[], int arr2[]);
int compareArray(int *arr1, int *arr2, int n);

int main(int argc, char *argv[])
{

    for (;;) {
        if (argc < 2)
            play_poker();
        else if (strcmp(argv[1],"-r"))
            best_card();
    }
    return 0;
}

void play_poker()
{
    read_cards();
}

void best_card()
{

}

void read_cards()
{
    char parsedCards[PLAYERS][CARDS + 1][CARD_LENGTH];
    char playerCards[PLAYERS][CARDS];
    char deck [CARDS_TABLE];
    char colors[PLAYERS][CARDS];
    int values[PLAYERS][CARDS];

    if (fgets(playerCards[0], CARDS, stdin) == NULL) { //read cards
        exit(0);
    }
    fgets(playerCards[1], CARDS, stdin);
    fgets(deck, CARDS_TABLE, stdin);

    parseCards(playerCards[0], deck, parsedCards[0]); //split by space
    parseCards(playerCards[1], deck, parsedCards[1]);

    for (int i = 0; i < CARDS; i++) { // parse values and colors to array
        if (parsedCards[0][i][0] >= '2' && parsedCards[0][i][0] <= '9') {
            values[0][i] = parsedCards[0][i][0] - '0';
        } else if (parsedCards[0][i][0] == 'T') {
            values[0][i] = 10;
        } else if (parsedCards[0][i][0] == 'J') {
            values[0][i] = 11;
        } else if (parsedCards[0][i][0] == 'Q') {
            values[0][i] = 12;
        } else if (parsedCards[0][i][0] == 'K') {
            values[0][i] = 13;
        } else {
            values[0][i] = 14;
        }
        colors[0][i] = parsedCards[0][i][1];
    }
    for (int i = 0; i < CARDS; i++) {
        if (parsedCards[1][i][0] >= '2' && parsedCards[1][i][0] <= '9') {
            values[1][i] = parsedCards[1][i][0] - '0';
        } else if (parsedCards[1][i][0] == 'T') {
            values[1][i] = 10;
        } else if (parsedCards[1][i][0] == 'J') {
            values[1][i] = 11;
        } else if (parsedCards[1][i][0] == 'Q') {
            values[1][i] = 12;
        } else if (parsedCards[1][i][0] == 'K') {
            values[1][i] = 13;
        } else {
            values[1][i] = 14;
        }
        colors[1][i] = parsedCards[1][i][1];
    }

    colors[1][7] = '\0';

    sort(values[0], colors[0], CARDS); //sort cards by their value
    sort(values[1], colors[1], CARDS);

    analyze_cards(values[0],values[1],colors[0],colors[1]); // check for combs
}

void analyze_cards(int val1[], int val2[], char col1[], char col2[])
{// function analyzes cards of both players and evaluates winner
    switch(straightFlush(val1,val2,col1,col2)){
        case 1:
            printf("W\n");
            return;
        case -1:
            printf("L\n");
            return;
    }
    if (count_four(val1) > count_four(val2)) {printf("W\n");return;}
    if (count_four(val1) < count_four(val2)) {printf("L\n");return;}
    if (count_four(val1) == count_four(val2) && count_four(val1) != 0){
        switch (compareKickerFour(val1,val2)){
            case 1:
                printf("W\n");
                return;
            case -1:
                printf("L\n");
                return;
            case 0:
                printf("D\n");
                return;
        }
    }
    switch (compareFullHouse(val1,val2)){
        case 1:
            printf("W\n");
            return;
        case -1:
            printf("L\n");
            return;
        case 0:
            printf("D\n");
            return;
    }
    switch (compareFlush(val1,val2,col1,col2)){
        case 1:
            printf("W\n");
            return;
        case -1:
            printf("L\n");
            return;
        case 0:
            printf("D\n");
            return;
    }
    switch(compareStraight(val1,val2)){
        case 1:
            printf("W\n");
            return;
        case -1:
            printf("L\n");
            return;
        case 0:
            printf("D\n");
            return;
    }
    if (count_threes(val1) > count_threes(val2)) {printf("W\n");return;}
    if (count_threes(val1) < count_threes(val2)) {printf("L\n");return;}
    if (count_threes(val1) == count_threes(val2) && count_threes(val1) != 0){
        switch(compareKickerThrees(val1,val2)){
            case 1:
                printf("W\n");
                return;
            case -1:
                printf("L\n");
                return;
            case 0:
                printf("D\n");
                return;
        }
    }
    if (count_pairs(val1) > count_pairs(val2) && count_pairs(val1) != 3) { printf("W\n");return;}
    if (count_pairs(val1) < count_pairs(val2) && count_pairs(val2)!=3) {printf("L\n");return;}
    if (count_pairs(val1) - count_pairs(val2) == 2) { printf("W\n");return;}
    if (count_pairs(val1) - count_pairs(val2) == -2) {printf("L\n");return;}
    if ((count_pairs(val1) == count_pairs(val2) || count_pairs(val1) == 3 || count_pairs(val2) ==3 ) && count_pairs(val1) !=0){
        switch (compareKickerPairs(val1,val2)){
        case 1:
            printf("W\n");
            return;
        case -1:
            printf("L\n");
            return;
        case 0:
            printf("D\n");
            return;
        }
    }
    switch (high_card(val1,val2)) {
        case 1:
            printf("W\n");
            return;
        case -1:
            printf("L\n");
            return;
        case 0:
            printf("D\n");
            return;
    }
}

int straightFlush(int arr1[], int arr2[], char col1[], char col2[])
{ // returns winner based on comparision between cards of both players
    int *flushOne = flush(arr1,col1);
    int *flushTwo = flush(arr2,col2);
    int *straightOne = straight(arr1);
    int *straightTwo = straight(arr2);
    int max1 = 0;
    int max2 = 0;

    int counter1 = 0;
    int counter2 = 0;
    if (In(1,straightOne) && In(14,flushOne))
        counter1++;
    if (In(1,straightTwo) && In(14,flushTwo))
        counter1++;
    for (int i = 4; i >= 0; i--){
        if (In(straightOne[i],flushOne) && straightOne[i]!=0){
            if (straightOne[i] > max1)
                max1 = straightOne[i];
            counter1++;
        }
        if (In(straightTwo[i],flushTwo) && straightTwo[i] != 0){
            if (straightTwo[i] > max2)
                max2 = straightTwo[i];
            counter2++;
        }
    }

    if (counter1 >= 5 && counter2 < 5){
        free(flushOne);
        free(flushTwo);
        return 1;
    }
    if (counter1 < 5 && counter2 >= 5){
        free(flushOne);
        free(flushTwo);
        return -1;
    }
    if (max1 > max2){
        free(flushOne);
        free(flushTwo);
        return 1;
    }
    if (max1 < max2)
    {   free(flushOne);
        free(flushTwo);
        return -1;
    }
    free(flushOne);
    free(flushTwo);
    return -42;
}

int* fullHouse(int arr[])
{ // function returns array with one Three Of A Kind and One Pair
    int *res = (int*) malloc(sizeof(int) * 2);
    int countThrees = count_threes(arr);
    res[0] = 0;
    res[1] = 0;
    if (countThrees == 2) // two threes, take higher and lower
    {
        for (int i = CARDS - 1; i >= 0; i-- ){
            if (occurence(arr[i],arr) == 3){
                res[0] = arr[i];
                break;
            }
        }
        for (int i = 0; i < CARDS; i++){
            if (occurence(arr[i],arr) == 3){
                res[1] = arr[i];
                break;
            }
        }
        return res;
    }
    if (countThrees == 1){ // only one three , take Three and Pair
        for (int i = 0; i < CARDS; i++){
            if (occurence(arr[i],arr) == 3)
                res[0] = arr[i];
        }
        for (int i = CARDS - 1; i >= 0; i--){
            if (occurence(arr[i],arr) == 2)
                res[1] = arr[i];
        }
        return res;
    }
    return res;

}
int* straight (int arr[])
{ // function returns array with the cards of the straight
    int* noDupl = removeDuplicates(arr);
    int *res = (int*)malloc(sizeof(int) * 7);
    for (int i = 0; i < CARDS;i++)
        res[i] = 0;

    for (int i = CARDS; i >= 4; i--){
        if (noDupl[i] == noDupl[i-1] + 1 && noDupl[i-1] == noDupl[i-2] + 1 && noDupl[i-2] == noDupl[i-3] + 1 && noDupl[i-3] == noDupl[i-4] + 1){
            res[0] = noDupl[i];
            res[1] = noDupl[i-1];
            res[2] = noDupl[i-2];
            res[3] = noDupl[i-3];
            res[4] = noDupl[i-4];
            break;
        }
    }
    free(noDupl);
    return res;
}

int compareStraight(int arr1[],int arr2[])
{ // function compares cards of a straight
    int *strRes1 = straight(arr1);
    int *strRes2 = straight(arr2);

    if (strRes1[4] == strRes2[4] && strRes1[4] == 0)
        return -42;
    if (strRes1[4] > strRes2[4]){
        free(strRes1);
        free(strRes2);
        return 1;
    }
    if (strRes1[4] < strRes2[4]){
        free(strRes1);
        free(strRes2);
        return -1;
    }
    if (strRes1[4] == strRes2[4]){
        free(strRes1);
        free(strRes2);
        return 0;
    }
    free(strRes1);
    free(strRes2);
    return -42;
}

int compareFullHouse (int arr1[],int arr2[])
{ // function compares cards of Full House
    int *res1 = fullHouse(arr1);
    int *res2 = fullHouse(arr2);
    int nonNullValues1 = 0;
    int nonNullValues2 = 0;
    for (int i = 0; i < 2; i++){
        if (res1[i] != 0)
            nonNullValues1++;
        if (res2[i] != 0){
            nonNullValues2++;
        }
    }
    if (nonNullValues1==2 && nonNullValues2 != 2){
        free(res1);
        free(res2);
        return 1;
    }
    if (nonNullValues1!=2 && nonNullValues2 == 2){
        free(res1);
        free(res2);
        return -1;
    }
    if (nonNullValues1!=2 && nonNullValues2!= 2)
    {   free(res1);
        free(res2);
        return -42;
    }

    for (int i = 0; i < 2; i++){
        if (res1[i] > res2[i]){
            free(res1);
            free(res2);
            return 1;
        }
        if (res1[i] < res2[i]){
            free(res1);
            free(res2);
            return -1;
        }
    }
    free(res1);
    free(res2);
    return 0;
}


char colorOfFlush(char col[])
{ // returns color of a flush
    if (countChar(col,'h') >= 5)
        return 'h';
    if (countChar(col,'d') >= 5)
        return 'd';
    if (countChar(col,'s') >= 5)
         return 's';
    if (countChar(col,'c') >= 5)
        return 'c';
    return '!';
}

int compareFlush (int arr1[],int arr2[], char col1[],char col2[])
{ // function compares cards of a flush
    int* pRes1 = flush(arr1,col1);
    int* pRes2 = flush(arr2,col2);
    int count1 = 0;
    int count2 = 0;
    for (int i = 0; i < CARDS - 2; i++){
        if (pRes1[i] > 0)
            count1++;
        if (pRes2[i] > 0)
            count2++;
    }
    for (int i = 0; i < CARDS - 2; i++){
        if (pRes1[i] > pRes2[i]) {
            free(pRes1);
            free(pRes2);
            return 1;
        }
        if (pRes1[i] < pRes2[i]){
            free(pRes1);
            free(pRes2);
            return -1;
        }
    }
    free(pRes1);
    free(pRes2);
    if (count1 == 5 && count2 == 5)
        return 0;
    return -42;
}

int* flush(int arr[],char col[])
{ //function returns array with cards of a flush
    int* res = (int*)malloc(sizeof(int) * 7);
    int j = 0;
    for (int i = 0 ; i < 7; i++)
        res[i] = 0;
    if (colorOfFlush(col) == '!')
        return res;
    for (int i = CARDS - 1; i >= 0 || j < 5; i--){
        if (col[i] == colorOfFlush(col))
            res[j++] = arr[i];
    }
    return res;
}

int compareKickerFour(int arr1[],int arr2[])
{ // function compares kicker card
    int card1 = kickerFour(arr1);
    int card2 = kickerFour(arr2);
    if (card1 > card2)
        return 1;
    if (card1 < card2)
        return -1;
    return 0;
}

int kickerFour(int arr[])
{ // function returns kicker with Four Of A Kind
    for (int i = CARDS - 1; i >= 0; i--){
        if (occurence(arr[i],arr) != 4) {
            return arr[i];
        }
    }
    return 0;
}


int compareKickerThrees(int arr1[],int arr2[])
{ // function compares cards if both players have same Three Of A Kind
    int* res1 = kickerThrees(arr1);
    int* res2 = kickerThrees(arr2);
    return compareArray(res1,res2,5);
}

int* kickerThrees(int arr[])
{ // function returns array with Three of A Kind with two highest cards
    int* res = (int*)malloc(sizeof(int) * 7);
    for (int i = 0; i < 7; i++)
        res[i] = 0;
    int j = 0;
    for (int i = CARDS - 1;i >= 0 || j<3; i--)
    {
        if(occurence(arr[i],arr) == 3)
            res[j++] = arr[i];
    }
    for (int i = CARDS - 1; i >= 0 || j<5; i--){
        if (!In(arr[i],res))
            res[j++] = arr[i];
    }
    return res;
}

int compare_fullhouse(int *arr1,int *arr2)
{ // function compare cards of both players
    for (int i = 0; i < 2 ; i++){
        if (arr1[0] == 0 && arr2[0] == 0)
            return -42;
        if (arr1[i] > arr2[i])
            return 1;
        if (arr1[i] < arr2[i])
            return -1;
    }
    return 0;
}

int count_threes (int arr[])
{ // function returns number of Three Of A Kinds
    int threes = 0;
    int actual = -1;
    for (int i = 0 ; i < CARDS - 1; i++){
        if (actual == arr[i+1] && i != 0){
            actual = arr[i+1];
            continue;
        }
        if (occurence(actual,arr) == 3) {
            threes++;
        }
        actual = arr[i+1];
    }
    if (arr[CARDS - 1] == arr[CARDS - 2] && arr[CARDS-2] == arr[CARDS - 3 ] &&arr[CARDS - 1]!= arr[CARDS - 4])
        threes++;
    return threes;
}


int count_four (int arr[])
{ // function returns T if there is Poker
    int four = 0;
    int actual = -1;
    for (int i = 0 ; i < CARDS - 1; i++){
        if (actual == arr[i+1] && i != 0){
            actual = arr[i+1];
            continue;
        }
        if (occurence(actual,arr) == 4) {
            four++;
        }
        actual = arr[i+1];
    }
    if (arr[CARDS - 1] == arr[CARDS - 2] && arr[CARDS-2] == arr[CARDS - 3 ] && arr[CARDS - 3] == arr[CARDS - 4] &&arr[CARDS - 1]!= arr[CARDS - 5])
        four++;
    return four;

}

int compareArray(int* arr1,int* arr2,int n)
{ // function compares first N values of arrays
    for (int i = 0; i < n; i++){
        if (arr1[i] > arr2[i]) {
            free(arr1);
            free(arr2);
            return 1;
        }
        if (arr1[i]< arr2[i]) {
            free(arr1);
            free(arr2);
            return -1;
        }
    }
    free(arr1);
    free(arr2);
    return 0;
}

int compareKickerPairs(int arr1[],int arr2[]){

    int* res1 = NULL;
    int* res2 = NULL;

    if (count_pairs(arr1) == 1) { //1 pair both
        res1 = kickerOnePair(arr1);
        res2 = kickerOnePair(arr2);
        return compareArray(res1,res2,5);
    }
    if (count_pairs(arr1) == 2) // 2 pairs both
    {
        res1 = kickerTwoPair(arr1);
        res2 = kickerTwoPair(arr2);
        return compareArray(res1,res2,5);
    }
    if (count_pairs(arr1) == 3) //3 pairs both
    {
        res1 = kickerThreePair(arr1);
        res2 = kickerThreePair(arr2);
        return compareArray(res1,res2,5);
    }
    return -42;
}

int* kickerOnePair(int arr[])
{ // functions returns array with one pair and three highest cards
    int *res = (int *)malloc(sizeof(int) * CARDS);
    int j = 2;
    for (int i = CARDS - 1; i >= 0 || j<=5 ; i--){
        if (occurence(arr[i],arr) == 2){
            res[0] = arr[i];
            res[1] = arr[i];
        }
        else {
            res[j++] = arr[i];
        }
    }
    return res;
}

int* kickerTwoPair(int arr[])
{ // function returns array with two highest pairs and kicker
    int *res = (int *)malloc(sizeof(int) * 5);
    int found = 0;
    for (int i = CARDS - 1; i >= 0  ; i--){
        if (occurence(arr[i],arr) == 2 && found < 2){
            res[0] = arr[i];
            res[1] = arr[i];
            found++;
        }
        if (occurence (arr[i],arr) == 2 && found >= 2){
            res[2] = arr[i];
            res[3] = arr[i];
            found++;
            if (found == 4)
                break;
        }
    }
    for (int i = CARDS - 1; i >= 0; i--){
        if (!In(arr[i],res)) {
            res[4] = arr[i];
            break;
        }
    }
    return res;
}

int* kickerThreePair(int arr[])
{ // function returns array with two pairs and one kicker
    int *res = (int *)malloc(sizeof(int) * 5);
    int x = 0;
    for (int i = CARDS - 1; i >= 0  ; i--){
        if (occurence(arr[i],arr) == 2){
            res[x++] = arr[i];
            if (x==4)
                break;
        }
    }
    res[4] = 0;
    for (int i = CARDS - 1; i>=0; i--){
        if (!In(arr[i],res)) {
            res[4] = arr[i];
            break;
        }
    }
    return res;

}

int count_pairs(int arr[])
{ // function returns number of pairs
    int pairs = 0;
    int actual = -1;
    for (int i = 0 ; i < CARDS - 1; i++){
        if (actual == arr[i+1] && i != 0){
            actual = arr[i+1];
            continue;
        }
        if (occurence(actual,arr) == 2)
            pairs++;
        actual = arr[i+1];
    }
    if (arr[CARDS - 1] == arr[CARDS - 2] && arr[CARDS - 1]!= arr[CARDS - 3])
        pairs++;
    return pairs;
}

int high_card (int val1[], int val2[])
{ // function returns winner based on comparision of highest card
    for (int i = CARDS - 1; i >= 2; i--) {
        if (val1[i] > val2[i])
            return 1;
        if (val1[i] < val2[i])
            return -1;
    }
    return 0;
}

int In(int value,int val2[])
{ // functions returns T if value exists in array
    for (int i = 0; i < CARDS; i++ ) {
        if (val2[i] == value)
            return 1;
    }
    return 0;
}

void parseCards(char player[], char deck[], char arr[CARDS][CARD_LENGTH])
{ // functions splits input by space
    char addedStrings[25] = "";
    char *pch = NULL;
    const char sep[2] = " ";
    size_t j = 0;
    for (int i = 0 ; i < CARD_LENGTH; i++){
        arr[0][i] = ' ';
        arr[1][i] = ' ';
    }
    strncat(addedStrings, player, CARDS);
    strncat(addedStrings, " ", 2);
    strncat(addedStrings, deck, 16);
    pch = strtok(addedStrings, sep);
    while (pch != NULL) {
        strncpy(arr[j], pch, 2);
        j++;
        pch = strtok (NULL, sep);
    }
}

int countChar (char str[], char c)
{ // functions counts occurence of character c in string
    int counter = 0;
    for (int i = 0; i < CARDS; i++){
        if (str[i] == c)
            counter++;
    }
    return counter;
}

void sort(int arr[],char arr2[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++){
        for (j = 0; j < n - i - 1; j++){
            if (arr[j] > arr[j + 1]){
                swap(&arr[j], &arr[j + 1]); //swap values and color
                swapChar(&arr2[j], &arr2[j + 1]);
            }
        }
    }
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swapChar(char *xp, char *yp)
{
    char temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int occurence (int x,int arr[])
{ // returns occurence of element x in array
    int counter = 0;
    for (int i = 0; i < CARDS; i++){
        {
            if (x == arr[i])
                counter++;
        }
    }
    return counter;
}


int* removeDuplicates(int arr[])
{ //function removes duplicates and returns new array
    int j = 0;
    int* temp = (int*) malloc (sizeof (int) * (CARDS + 1));
    for (int i = 0;i < 8; i++)
        temp[i] = 0;
    if (arr[CARDS - 1] == 14){
        temp[0] = 1;
        j = 1;
    }
    for (int i=0; i < CARDS-1; i++)
        if (arr[i] != arr[i+1])
            temp[j++] = arr[i];

    temp[j++] = arr[CARDS-1];
    return temp;
}

