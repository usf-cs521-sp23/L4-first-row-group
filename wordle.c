#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>


int lowerstr(char *str){
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
    return 0;
}
int ifUpperStr(char *str){
    for (int i = 0; str[i] != '\0'; i++) {
        if (isupper(str[i])){
            return 1;
        }
    }
    return 0;
}
int nospecial(char *s, int size){
    for (int j = 0; j < size-1; j++) {
        if ((s[j] <'a' || s[j] >'z')&&(s[j] <'A' || s[j] >'Z')) {
            return 1;
        } 
    }
    return 0;
}

int getWordList(int length){
    FILE *file = fopen("/usr/share/dict/words", "r");
    if (file == NULL) {
        perror("fopen");    // error handling
    }
    FILE *outfile = fopen("wordslist", "w");
    if (file == NULL) {
        perror("fopen");    // error handling
    }
    char line[500];
    char words[500][10]; 
    int idx=0;
   
    while (fgets(line, 500, file) != NULL) {
        int size=strlen(line);
        int n=nospecial(line,size);
  
        if(size==length+1 && nospecial(line,size)==0){
            lowerstr(line);
            fputs(line, outfile);;
            idx++;
        }
    }
    fclose(file);
    fclose(outfile);
    return idx;
}

char* getRandomWord(int idx){
    time_t t;
    srand((unsigned) time(&t));
    int random = rand() % idx;
    char line[50];
    char *target = malloc(7);
    FILE *outfile = fopen("wordslist", "r");
    while (fgets(line, 50, outfile) != NULL) {
        if (random==0){
            strcpy(target, line);
            break;
        }
        random--;
    }
    fclose(outfile);
    return target;
}

int main(void) {
    // game start, acquire player name  
    char name[100];
    printf("What's your name: ");
    scanf("%s", name);
    printf("\n");
    
    bool play = true;

    while (play){
        printf("********************** Hi %s, welcome to Wordle! **********************\n", name);
        // Choose difficulty level
        printf("Please choose the difficulty level (1: Easy, 2: Medium, 3: Hard) (Default: Easy)\n");
        char *input;
        int difficulty;
        int length;
        scanf("%s", input);
        printf("\n");
        sscanf(input, "%d", &difficulty);

        printf("Game start! ");
        if (difficulty == 2){
            printf("Median Level, ");
            length = 6;
            
        } else if (difficulty == 3){
            printf("Hard Level, ");
            length = 7;
        } else {
            difficulty = 1;
            printf("Easy Level, ");
            length = 5;
        }
        printf("target word has %d letters (lowercase).\nYou will have %d chances to guess.\n"
        "Correct letters print in blue, out-of-place letters print in yellow, wrong letter print in red.\n\n", length, length+1);
        
        int i = getWordList(length);            // Generate word-list
        char *target = getRandomWord(i);    // Choose the target

        // Store wrong letters
        char wrong_letters[26] = {'0'};
        int index = 0;
        bool win = false;
        
        for (int i = 0; i < length+1; ++i) {
            char guess[100];
            printf("Enter your guess (%d/%d): ", i+1, length+1);
            scanf("%s", guess);
            
            
            if (strlen(guess) != length) {
                printf("That is not %d letters. Please enter a %d-letter word!\n", length, length);
                continue;
            }
            if (nospecial(guess, strlen(guess)+1)==1 || ifUpperStr(guess) == 1){
                printf("That is not a valid guess, don't use upper case or special character!\n");
                continue;
            }
    
            printf("                        "); 
            for (int j = 0; j < length; ++j) {
                if (target[j] == guess[j]) {
                    printf("\033[1;34m"); // Print in Blue
                    printf("%c", guess[j]);
                } else if (strchr(target, guess[j]) != NULL) {
                    printf("\033[1;33m");  // Print in Yellow
                    printf("%c", guess[j]);
                } else {
                    printf("\033[1;31m"); // Print in Red
                    printf("%c", guess[j]);
                    if (strchr(wrong_letters, guess[j]) == NULL){
                    wrong_letters[index] = guess[j];
                    index++;
                    }
                }
            }
            printf("\033[0;37m");

            // Reminder of wrong letters for the players
            printf("\nGuessed wrong letters: ");
            for(int i = 0; i < index; i++){
                printf("%c ", wrong_letters[i]);
            }
            printf("\n");

            puts("");

            if (strncmp(target, guess, length) == 0) {
                printf("YOU WINNNNNNNNNNNNN!!!\n");
                win = true;
                break;
            }
        }

        if (!win){
            printf("You Lose, the answer is: %s\n", target);
        }

        free(target);
        
        char again[5];
        printf("Play again? (y/n)\n");
        scanf("%s", again);
        lowerstr(again);
        if (strcmp(again,"y")!=0 && strcmp(again,"yes")!=0){
            play = false;
        } else {
            printf("\n");
        }
    }
    return 0;
}

