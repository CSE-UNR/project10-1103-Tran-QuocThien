//project10
//Name: Quoc Thien Tran
//Date: 5/1/2026

#include <stdio.h>
#include <stdbool.h>

#define WORD_LENGTH 5
#define MAX_GUESSES 6
#define DISPLAY_WIDTH 32

bool loadWord(char mystery[]) {
    FILE *fp = fopen("word.txt", "r");
    if (fp == NULL) {
        printf("Error: Could not open word.txt\n");
        return false;
    }
    fscanf(fp, "%s", mystery);
    fclose(fp);
    return true;
}

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int strLen(char str[]) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void toLower(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
    }
}

bool validateGuess(char guess[]) {
    int len = strLen(guess);
    bool allLetters = true;

    for (int i = 0; i < len; i++) {
        if (!isLetter(guess[i])) {
            allLetters = false;
            break;
        }
    }

    if (len != WORD_LENGTH && !allLetters) {
        printf("Your guess must be 5 letters long.");
        printf("Your guess must contain only letters.\n");
        return false;
    } else if (len != WORD_LENGTH) {
        printf("Your guess must be 5 letters long.\n");
        return false;
    } else if (!allLetters) {
        printf("Your guess must contain only letters.\n");
        return false;
    }
    return true;
}

void buildDisplay(char guess[], char mystery[], char display[]) {
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == mystery[i]) {
            display[i] = guess[i] - 32;
        } else {
            display[i] = guess[i];
        }
    }
    display[WORD_LENGTH] = '\0';
}

void buildPointers(char guess[], char mystery[], char display[], char pointers[]) {
    int idx = 0;

    for (int i = 0; i < WORD_LENGTH; i++) {
        bool inWord = false;
        if (display[i] >= 'a' && display[i] <= 'z') {
            for (int j = 0; j < WORD_LENGTH; j++) {
                if (guess[i] == mystery[j]) {
                    if (display[j] < 'a') {
                        continue;
                    }
                    inWord = true;
                    break;
                }
            }
        }
        if (inWord) {
            pointers[idx++] = ' ';
        }
    }

    idx = 0;
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (display[i] >= 'a' && display[i] <= 'z') {
            bool found = false;
            for (int j = 0; j < WORD_LENGTH; j++) {
                if (guess[i] == mystery[j] && i != j) {
                    found = true;
                    break;
                }
            }
            if (found) {
                pointers[idx++] = '^';
            } else {
                pointers[idx++] = ' ';
            }
        } else {
            pointers[idx++] = ' ';
        }
    }

    pointers[WORD_LENGTH] = '\0';
    int end = WORD_LENGTH - 1;
    while (end >= 0 && pointers[end] == ' ') {
        pointers[end] = '\0';
        end--;
    }
}

void printCentered(char str[], int width) {
    int len = strLen(str);
    int padding = (width - len) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", str);
}

void printHistory(char displays[][WORD_LENGTH + 1], char pointerRows[][WORD_LENGTH + 1], int count) {
    printf("================================\n");
    for (int i = 0; i < count; i++) {
        printCentered(displays[i], DISPLAY_WIDTH);
        if (pointerRows[i][0] != '\0') {
            printCentered(pointerRows[i], DISPLAY_WIDTH);
        } else if (i < count - 1) {
            printf("\n");
        }
    }
}

bool isWin(char guess[], char mystery[]) {
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] != mystery[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    char mystery[WORD_LENGTH + 1];

    if (!loadWord(mystery)) {
        return 1;
    }

    char displays[MAX_GUESSES][WORD_LENGTH + 1];
    char pointerRows[MAX_GUESSES][WORD_LENGTH + 1];
    int guessCount = 0;
    bool won = false;

    while (guessCount < MAX_GUESSES && !won) {
        char guess[100];

        if (guessCount == MAX_GUESSES - 1) {
            printf("FINAL GUESS : ");
        } else {
            printf("GUESS %d! Enter your guess: ", guessCount + 1);
        }
        scanf("%s", guess);

        if (!validateGuess(guess)) {
            printf("Please try again: ");
            while (true) {
                scanf("%s", guess);
                if (validateGuess(guess)) {
                    break;
                }
                printf("Please try again: ");
            }
        }

        toLower(guess);

        if (isWin(guess, mystery)) {
            buildDisplay(guess, mystery, displays[guessCount]);
            buildPointers(guess, mystery, displays[guessCount], pointerRows[guessCount]);
            guessCount++;
            printHistory(displays, pointerRows, guessCount);
            won = true;
        } else {
            buildDisplay(guess, mystery, displays[guessCount]);
            buildPointers(guess, mystery, displays[guessCount], pointerRows[guessCount]);
            guessCount++;
            printHistory(displays, pointerRows, guessCount);
        }
    }

    if (won) {
        if (guessCount == 1) {
            printf("      You won in 1 guess!\n");
            printf("           GOATED!\n");
        } else {
            printf("   You won in %d guesses!\n", guessCount);
        }
    } else {
        printf("You lost, better luck next time!\n");
    }

    return 0;
}


