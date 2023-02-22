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

int getWordList(){
    printf("Opening file: %s\n", "/usr/share/dict/words");
    FILE *file = fopen("/usr/share/dict/words", "r");
    if (file == NULL) {
        perror("fopen");
    // error handling
    }
    FILE *outfile = fopen("wordslist", "w");
    if (file == NULL) {
        perror("fopen");
    // error handling
    }
    char line[500];
    char words[500][10]; 
    int idx=0;
    while (fgets(line, 500, file) != NULL) {
        //printf("%s", line);
        int size=strlen(line);
       int n=nospecial(line,size);
  
        //printf("\n%d\n", n);
        if(size==6 && nospecial(line,size)==0){
             //printf("%s", line);
            lowerstr(line);
             fputs(line, outfile);;
             idx++;
        }

        /* Process the line */

    }
    fclose(file);
    fclose(outfile);
    return idx;
}
char* getRandomWord(int idx){
    time_t t;
    srand((unsigned) time(&t));
    int random = rand() % idx;
    printf("%d\n", random);
    char line[50];
    char *target = malloc(7);
    FILE *outfile = fopen("wordslist", "r");
    while (fgets(line, 50, outfile) != NULL) {
        if (random==0){
            strcpy(target, line);
            printf("%s", target);
            break;
        }
        random--;
    }
    fclose(outfile);
    return target;
}
int main(void) {
    int i = getWordList();
    
// game start
    char name[100];
    printf("What your name: ");
    scanf("%s", name);
    printf("Welcome %s!\nYou have 6 chances to guess a 5 character word.\n", name);
    bool play = true;
    bool win = false;
    while (play){
        char *target = getRandomWord(i);
        printf("%s", target);
        for (int i = 0; i < 6; ++i) {
    
    
            char guess[100];
            printf("Enter your guess: ");
            scanf("%s", guess);
            
            
            if (strlen(guess) != 5) {
            printf("That is not 5 characters, Matthew. Please learn to play the game "
                    "correctly.\n");
            continue;
            }
            if (nospecial(guess, strlen(guess)+1)==1 || ifUpperStr(guess) == 1){
                printf("That is not valid guess, don't use upper case or special character!\n");
            continue;

            }
    
            printf("                  ");
            for (int j = 0; j < 5; ++j) {
                //printf("%c", target[j]);
                if (target[j] == guess[j]) {
                    printf("o");
                } else if (strchr(target, guess[j]) != NULL) {
                    printf("a");
                } else {
                    printf("x");
                }
            }
            puts("");

            if (strncmp(target, guess, 5) == 0) {
            printf("YOU WINNNNNNNNNNNNN!!!\n");
            win = true;
            break;
            }
        }
        if (!win){
            printf("You Lose\n");
        }
        
        char answer[5];
        printf("You what paly again?(y/n)\n");
        scanf("%s", answer);
        if (strcmp(answer,"y")!=0 && strcmp(answer,"Y")!=0 ){
            play = false;
        }
    }
    return 0;
}

