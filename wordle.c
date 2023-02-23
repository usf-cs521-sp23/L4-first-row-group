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
            //printf("s is %s",s);
            //printf("c is %c", s[j]);
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
    //printf("%d\n", random);
    char line[50];
    char *target = malloc(7);
    FILE *outfile = fopen("wordslist", "r");
    while (fgets(line, 50, outfile) != NULL) {
        if (random==0){
            strcpy(target, line);
            // printf("%s", target);
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
        printf("********************** Hi %s, welcome to wordle! **********************\n", name);
        // Choose difficulty level
        printf("Please chooose the difficulty level (1: Easy, 2: Medium 3: Hard) (Default: Easy)\n");
        int difficulty;
        int length;
        scanf("%d", &difficulty);
        printf("\n");

        if (difficulty == 2){
            printf("Game start! Mediam Level, 6 letter words, 7 guesses\n");
            length = 6;
            
        } else if (difficulty == 3){
            printf("Game start! Hard Level, 7 letter words, 8 guesses\n");
            length = 7;
        } else {
            difficulty = 1;
            printf("Game start! Easy Level, 5 letter words, 6 guesses\n");
            length = 5;
        }
        printf("You will have %d chances\nCorrect letters print in blue, out-of-place letters print in yellow, wrong letter print in red.\n\n", length+1);
        
        int i = getWordList(length);    // Generate word list
        char *target = getRandomWord(i);    // Choose the target

        // Store wrong letters
        char wrong_letters[26];
        int index = 0;
        bool win = false;
        
        for (int i = 0; i < length+1; ++i) {
            char guess[100];
            printf("Enter your guess (%d/%d):", i+1, length+1);
            scanf("%s", guess);
            
            
            if (strlen(guess) != length) {
                printf("That is not %d characters. Please enter %d-letter word!\n", length, length);
                continue;
            }
            if (nospecial(guess, strlen(guess)+1)==1 || ifUpperStr(guess) == 1){
                printf("That is not valid guess, don't use upper case or special character!\n");
                continue;
            }
    
            printf("                       "); 
            for (int j = 0; j < length; ++j) {
                if (target[j] == guess[j]) {
                    printf("\033[1;34m"); // Print in Blue
                    printf("%c", guess[j]);
                    printf("\033[0;37m");
                } else if (strchr(target, guess[j]) != NULL) {
                    printf("\033[1;33m");  // Print in Yellow
                    printf("%c", guess[j]);
                    printf("\033[0;37m");
                } else {
                    printf("\033[1;31m"); // Print in Red
                    printf("%c", guess[j]);
                    printf("\033[0;37m");
                    if (strchr(wrong_letters, guess[j]) == NULL){
                    wrong_letters[index] = guess[j];
                    index++;
                    }
                }
            }

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
        
        char answer[5];
        printf("You what play again?(y/n)\n");
        scanf("%s", answer);
        lowerstr(answer);
        if (strcmp(answer,"y")!=0 && strcmp(answer,"yes")!=0){
            play = false;
        } else {
            printf("\n");
        }
    }
    return 0;
}

