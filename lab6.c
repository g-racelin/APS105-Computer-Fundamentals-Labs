#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void readWordPuzzle(const int Size, char puzzle[][Size]);
char* readWord(int* wordSize);
void printWordPuzzle(const int Size, char puzzle[][Size]);
void printWord(char* word, const int wordSize);

void search(const int Size,
        char puzzle[][Size],
        const int wordSize,
        char* word);

bool inBounds(int row, int col, const int Size);

int main(void) {
    const int Size = 20;
    char puzzle[Size][Size];
    readWordPuzzle(Size, puzzle);
    int wordSize = 0;
    char* word = readWord(&wordSize);
    printWordPuzzle(Size, puzzle);
    printf("The word you are looking for is\n");
    printWord(word, wordSize);
    printf("\n");
    printf("Do you want to search? (Y or N)\n");
    char isSearch;
    scanf(" %c", &isSearch);
    if (isSearch == 'Y' || isSearch == 'y') {
        search(Size, puzzle, wordSize, word);
    }
    free(word);
    return 0;
}

void readWordPuzzle(const int Size, char puzzle[][Size]) {
    printf("Please enter the word puzzle:\n");
    for (int row = 0; row < Size; row ++){
        for (int col = 0; col < Size; col ++){
            scanf(" %c", &puzzle[row][col]);
        }
    }
}

char* readWord(int* wordSize) {
    printf("How many characters are there in the word?\n");
    scanf("%d", wordSize);
    char* word = (char*) malloc (*wordSize * sizeof(char));
    printf("What is the word that you are looking for?\n");
    for(int i = 0; i < *wordSize; i++){
        scanf(" %c", word + i);
    }
    return word;
}

void printWordPuzzle(const int Size, char puzzle[][Size]) {
    printf("The word puzzle entered is\n");
    for (int row = 0; row < Size; row ++){
        for (int col = 0; col < Size; col ++){
            printf("%c ", puzzle [row][col]);
        }
        printf("\n");
    }
}

void printWord(char* word, const int wordSize) {
    for(int i = 0; i < wordSize; i++){
        printf("%c", *(word + i));
    }
}

void search(const int Size,char puzzle[][Size], const int wordSize, char* word) {
    bool found = false, foundComplete = false;
    int row, col; 
    int direction = 1;
    for (int i = 0; i < Size && foundComplete != true; i ++){
        for (int j = 0; j < Size && foundComplete != true; j ++){
            if (puzzle[i][j] == *word){
                found = true;
                row = i;
                col = j;
                bool search = inBounds (row, col, Size);
                while (found == true && foundComplete != true){
                    for (int k = 0, x = 0; k < wordSize && x < wordSize && found; k ++, x ++){
                        search = inBounds (k + row, x + col, Size);
                        if ((puzzle[k + row][x + col] != *(word + k) && found) || (search == false)){ //check southeast
                            found = false;
                        }
                        if (k == wordSize - 1 && found){
                            foundComplete = true;
                        }
                    }
                }
                found = true;
                while (found == true && foundComplete != true){
                    direction = 2;
                    for (int k = 0; k < wordSize && found; k ++){
                        search = inBounds (k + row, col, Size);
                        if ((puzzle[k + row][col] != *(word + k) && found) || (search == false)){ //check south
                            found = false;
                        }
                        if (k == wordSize - 1 && found){
                            foundComplete = true;
                        }
                    }
                }
                found = true;
                while (found == true && foundComplete != true){
                    direction = 3;
                    for (int k = 0, x = 0; k < wordSize && x > -1*wordSize && found; k ++, x --){
                        search = inBounds (k + row, x + col, Size);
                        if ((puzzle[k + row][x + col] != *(word + k) && found) || (search == false)){ //check southwest
                            found = false;
                        }
                        if (k == wordSize - 1 && found){
                            foundComplete = true;
                        }
                    }
                }
                found = true;
                while (found == true && foundComplete != true){   
                    direction = 4;             
                    for (int x = 0; x > -1*wordSize && found; x --){
                        search = inBounds (row, x + col, Size);
                        if ((puzzle[row][x + col] != *(word + -1*x) && found) || (search == false)){ //check west
                            found = false;
                        }
                        if (x == -1*wordSize + 1 && found){
                            foundComplete = true;
                        }
                    }
                }
                found = true;
                while (found == true && foundComplete != true){
                    direction = 5;
                    for (int k = 0, x = 0; k > -1*wordSize && x > -1*wordSize && found; k --, x --){
                        search = inBounds (k + row, x + col, Size);
                        if ((puzzle[k + row][x + col] != *(word + -1*k) && found) || (search == false)){ //check northwest
                            found = false;
                        }
                        if (k == -1*wordSize + 1 && found){
                            foundComplete = true;
                        }
                    }
                }
                found = true;
                while (found == true && foundComplete != true){
                    direction = 6;
                    for (int k = 0; k > -1*wordSize && found; k --){
                        search = inBounds (k + row, col, Size);
                        if ((puzzle[k + row][col] != *(word + -1*k) && found) || (search == false)){ //check north
                            found = false;
                        }
                        if (k == -1*wordSize + 1 && found){
                            foundComplete = true;
                        }
                    }
                }
                found = true;
                while (found == true && foundComplete != true){
                    direction = 7;
                    for (int k = 0, x = 0; k > -1*wordSize && x < wordSize && found; k --, x ++){
                        search = inBounds (k + row, x + col, Size);
                        if ((puzzle[k + row][x + col] != *(word + x) && found) || (search == false)){ //check northeast
                            found = false;
                        }
                        if (x == wordSize - 1 && found){
                            foundComplete = true;
                        }
                    }
                }
                found = true;
                while (found == true && foundComplete != true){
                    direction = 8;
                    for (int x = 0; x < wordSize && found; x ++){
                        search = inBounds (row, x + col, Size);
                        if ((puzzle[row][x + col] != *(word + x) && found) || (search == false)){ //check east
                            found = false;
                        }
                        if (x == wordSize - 1 && found){
                            foundComplete = true;
                        }
                    }
                }
            }
            if (foundComplete == true){
                printWord(word, wordSize);
                if (direction == 1){
                    printf(" can be found at row , col = (%d, %d) in the south-east direction.\n", row + 1, col + 1);
                } else if (direction == 2){
                    printf(" can be found at row , col = (%d, %d) in the south direction.\n", row + 1, col + 1);
                } else if (direction == 3){
                    printf(" can be found at row , col = (%d, %d) in the south-west direction.\n", row + 1, col + 1);
                } else if (direction == 4){
                    printf(" can be found at row , col = (%d, %d) in the west direction.\n", row + 1, col + 1);
                } else if (direction == 5){
                    printf(" can be found at row , col = (%d, %d) in the north-west direction.\n", row + 1, col + 1);
                } else if (direction == 6){
                    printf(" can be found at row , col = (%d, %d) in the north direction.\n", row + 1, col + 1);
                } else if (direction == 7){
                    printf(" can be found at row , col = (%d, %d) in the north-east direction.\n", row + 1, col + 1);
                } else {
                    printf(" can be found at row , col = (%d, %d) in the east direction.\n", row + 1, col + 1);  
                } 
            } else if (i == Size - 1 && j == Size - 1) {
                printWord (word, wordSize);
                printf (" cannot be found!");
            }
        }
    }

}

bool inBounds(int row, int col, const int Size) {
    bool in = false; 
    if (row < Size && col < Size){
        in = true;
    }
    return in;
}
