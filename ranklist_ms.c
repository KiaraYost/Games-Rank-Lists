#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 12
#define NUMGAMES 7
#define TOTAL 6
#define BASECASESIZE 30

typedef struct Player {
char* name;
int scores[NUMGAMES];
} Player;

Player** readPlayers(int* numPlayers);
int compare(Player* ptrP1, Player* ptrP2, int key);
void swap(Player** list, int indexP1, int indexP2);
void insertionSort(Player** list, int low, int high, int key);
void merge(Player** list, int low, int mid, int high, int key);
void mergeSortRec(Player** list, int low, int high, int key);
void mergeSort(Player** list, int n, int key);
void printList(Player** list, int n, int key);
void freePlayers(Player** players, int numPlayers);


int main(void) {
    int numPlayers;
    Player** players;
    int key;

    players = readPlayers(&numPlayers);

    scanf("%d", &key);

    mergeSort(players, numPlayers, key);

    printList(players, numPlayers, key);

    freePlayers(players, numPlayers);

    return 0;
}

Player** readPlayers(int* numPlayers) {
    scanf("%d", numPlayers);

    Player** players = (Player**)malloc(*numPlayers * sizeof(Player*));
    char temp[MAXSIZE+1];

    for(int i = 0; i < *numPlayers; i++) {
        players[i] = (Player*)malloc(sizeof(Player));

        scanf("%s", temp);
        players[i]->name = (char*)malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(players[i]->name, temp);

        players[i]->scores[TOTAL] = 0;

        for(int j = 0; j < TOTAL; j++) {
            scanf(" %d ", &(players[i]->scores[j]));
            players[i]->scores[TOTAL] += players[i]->scores[j];
        }
    }

    return players;

    //dynamically allocates an array of player pointers and reads in each player's name and respective scores
}

int compare(Player* ptrP1, Player* ptrP2, int key) {

    if(ptrP1->scores[key] > ptrP2->scores[key]) {
        return -1;
    }
    if(ptrP2->scores[key] > ptrP1->scores[key]) {
        return 1;
    }

    return strcmp(ptrP1->name, ptrP2->name);
    
    /*Returns a negative integer if the player pointed to by ptrP1 "comes before"
    the player pointed to by ptrP2 on the ranklist sorted by the game indicated by the integer key,
    breaking ties by the player's name in alphabetical order.
    Returns 0 if the two players are identical and returns a positive integer if the player pointed
    to by ptrP2 comes before the player pointed to by ptrP1.*/
}

void swap(Player** list, int indexP1, int indexP2) {
    Player* temp = list[indexP1];
    list[indexP1] = list[indexP2];
    list[indexP2] = temp;
}

void insertionSort(Player** list, int low, int high, int key) {
    //Starts with the second element (low + 1) as the key.
    for(int i = low+1; i <= high; i++) {
        for(int j = i; j>low; j--) {
            //Compares the key with the elements before it
            //Moves the elements less than key one element to the right
            if(compare(list[j-1], list[j], key) > 0) {
                swap(list, j-1, j);
            }
        }
    }
}

void merge(Player** list, int low, int mid, int high, int key) {
    int len = high - low + 1;
    Player** temp = (Player**)malloc(sizeof(Player*) * len);

    int count1 = low;
    int count2 = mid;
    int mainCount = 0;

    //Copies values into the auxiliary array, so long as there are 
    //numbers from both lists to copy.
    while((count1 < mid) || (count2 <= high)) {

        //Makes sure we don't access index count2 if we aren't supposed to.
        //Copies value from list one if we cannot access index count2
        if(count2 > high) {
            temp[mainCount] = list[count1];
            count1++;
            mainCount++;
        }
        else {
            int result = compare(list[count1], list[count2], key);

            //Next value to copy comes from list one - makes sure list one isn't exhausted yet.
            if(count1 < mid && result <= 0) {
                temp[mainCount] = list[count1];
                count1++;
                mainCount++;
            }
            //Copies the next value from list two.
            else {
                temp[mainCount] = list[count2];
                count2++;
                mainCount++;
            }
        }
    }
    
    //copies sorted temp to list
    for(int i = low; i <= high; i++) {
        list[i] = temp[i - low];
    }
    
    free(temp);
}

void mergeSortRec(Player** list, int low, int high, int key) {
    int mid;

    //sorts list[low..high] by insertion sort when size of list[low..high] < BASECASESIZE
    if((high - low + 1) < BASECASESIZE) {
        insertionSort(list, low, high, key);
    }
    else {
        mid = (low+high)/2;

        //Sort the first half of the values
        mergeSortRec(list, low, mid, key);

        //Sort the second half of the values
        mergeSortRec(list, mid+1, high, key);

        //Put it all together
        merge(list, low, mid+1, high, key);
    }

    // Performs a Merge Sort on list[low..high] according to the game
    // indicated by the integer key.
}

void mergeSort(Player** list, int n, int key) {

    mergeSortRec(list, 0, n-1, key);
    
    // Wrapper function that Merge Sorts the array list of size n according to the game
    // indicated by the integer key.
}

void printList(Player** list, int n, int key) {
    const char GAMES[NUMGAMES][MAXSIZE+1] = {"Wordle", "Spelling Bee",
    "Crossword", "Connections", "Strands", "Letter Boxed", "Total"};

    printf("%s Ranklist\n", GAMES[key]);

    for(int i = 0; i < n; i++) {
        printf("%d. %-15s %d\n", i+1, list[i]->name, list[i]->scores[key]);
    }

    //prints list of players sorted from greatest to least according to the game indicated by the integer key
}

void freePlayers(Player** players, int numPlayers) {
    for(int i = 0; i < numPlayers; i++) {
        free(players[i]->name);
        free(players[i]);
    }
    free(players);

    //frees dynamically allocated array of player pointers
}