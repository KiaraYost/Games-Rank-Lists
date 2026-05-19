#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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
void quickSort(Player** list, int n, int key);
int partition(Player** list, int low, int high, int key);
void quickSortRec(Player** list, int low, int high, int key);
void printList(Player** list, int n, int key);
void freePlayers(Player** players, int numPlayers);


int main(void) {
    int numPlayers;
    Player** players;
    int key;

    players = readPlayers(&numPlayers);
    
    scanf("%d", &key);
    
    quickSort(players, numPlayers, key);
    
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

void quickSort(Player** list, int n, int key) {
    quickSortRec(list, 0, n-1, key);

    // Quick Sorts the array list of size n according to the game 
    // indicated by the integer key. 
}

int partition(Player** list, int low, int high, int key) {
    int i, lowpos;

    //Picks a random partition element and swaps it into index low
    i = rand()%(high-low+1) + low;
    swap(list, low, i);

    //stores the index of the partition element
    lowpos = low;

    //updates low pointer
    low++;

    //Runs the partition so long as the low and high counters don't cross
    while(low <= high) {
        //Moves the low pointer until we find a player score too small for this side
        while((low <= high) && (compare(list[low], list[lowpos], key) <= 0)) low++;

        //Moves the high pointer until we find a player score too large for this side
        while((high >= low) && (compare(list[high], list[lowpos], key) > 0)) high--;

        //Now that we've identified two values on the wrong side, swap them
        if(low < high) {
            swap(list, low, high);
        }
    }

    //Swaps the partition element into it's correct location
    swap(list, lowpos, high);

    return high; //Returns the index of the partition element
}

void quickSortRec(Player** list, int low, int high, int key) {
    //sorts list[low..high] by insertion sort when size of list[low..high] < BASECASESIZE
    if((high - low + 1) < BASECASESIZE) {
        insertionSort(list, low, high, key);
    }
    else {
        int split = partition(list, low, high, key);
        quickSortRec(list, low, split-1, key);
        quickSortRec(list, split+1, high, key);
    }

    // Performs a Quick Sort on list[low..high] according to the game
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