#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
#include <time.h>

#define NAMELEN 37
#define RESET   "\x1b[0m"
#define BOLD    "\x1b[1m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define ASCII_a 97
#define ASCII_z 122
#define EXTRAV  50 

void reverse(char *n) {
    char temp;
    int len = strlen(n);
    int j = len - 1;
    for (int i = 0; i < (len / 2); i++) {
        temp = n[i];
        n[i] = n[j];
        n[j] = temp;
        j--;
    }
}

int flines(FILE *f) {
    int i = 0;
    char str[NAMELEN];
    for (; fgets(str, NAMELEN, f) != NULL; i++);
    return i;
}

void fgenerate(FILE *f, char name[][NAMELEN], int n) {
    int len = flines(f);

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int ran = rand() % len;
        fseek(f, 0, SEEK_SET);
        for (int j = 0; j <= ran; j++) {
            fgets(name[i], NAMELEN, f);
        }
        for (int k = 0; k < NAMELEN || name[i][k - 1] == '\n'; k++) {
            if (name[i][k] == '\n') {
                name[i][k] = '\0';
            }
        }
    }
}

void rgenerate(char name[][NAMELEN], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int ranlen = (rand() % ((NAMELEN - 1) - (rand() % (NAMELEN - 3)))) + 1;
        for (int j = 0; j < ranlen; j++) {
            int ranchar = (rand() % ((ASCII_z + EXTRAV) - ASCII_a + 1)) + ASCII_a;
            if (ranchar > ASCII_z) {
                ranchar = rand() % 5;
                switch(ranchar) {
                    case 0:
                        ranchar = (int)'a';
                        break;
                    case 1:
                        ranchar = (int)'e';
                        break;
                    case 2:
                        ranchar = (int)'i';
                        break;
                    case 3:
                        ranchar = (int)'o';
                        break;
                    case 4:
                        ranchar = (int)'u';
                        break;
                }
            }
            name[i][j] = (char)ranchar;
        }
        name[i][0] -= 32;
        name[i][ranlen] = '\0';
    }
}

int main(int argc, char **argv) {
    // Help with options
    if (argc >= 2 && strcmp(argv[1], "help") == 0) {
        printf("%sOptions:%s [] is optional\n" 
                " %s%sr%s name\t\t reverse name\n"
                " %s%sf%s [num]\t randomly generate [num] fantastical names\n"
                " %s%se%s [num]\t randomly generate [num] English names\n"
                " %s%sa%s [num]\t randomly generate [num] assorted names\n"
                " %s%sg%s [num]\t randomly generate [num] possible names\n"
                " %s%sex%s \t\t to see an example command\n"
                "%sNote:%s %d character name max\n",
                BOLD, RESET, BOLD, RED, RESET, BOLD, GREEN, RESET,
                BOLD, BLUE, RESET, BOLD, MAGENTA, RESET, BOLD, YELLOW, RESET,
                BOLD, CYAN, RESET, BOLD, RESET, (NAMELEN - 1));
    } 

    // Example command
    if (argc >= 2 && strcmp(argv[1], "ex") == 0) {
        printf("For example, if you want 10 random english names, type:\n./name e 10\n");
    }

    // Reverse name
    else if (argc >= 3 && strcmp(argv[1], "r") == 0) {
        char name[NAMELEN];
        strcpy(name, argv[2]);
        reverse(name);
        printf("%sReversed Name:%s %s\n", BOLD, RESET, name);
    }

    // Randomly generated names
    else if (argc >= 2 && (strcmp(argv[1], "f") == 0 || strcmp(argv[1], "e") == 0 ||
                strcmp(argv[1], "a") == 0 || strcmp(argv[1], "g") == 0)) {
        int n = 1;
        if (argc >= 3) {
            n = abs((int)strtol(argv[2], (char **)NULL, 10));
        }

        char name[n][NAMELEN];
        // Actually randomly generated names (little better than keyboard smashing)
        if (strcmp(argv[1], "g") == 0) {
            rgenerate(name, n);
        } else {
            FILE *f;
            // Randomly generated fantasy-esque, English names or all language names
            if (strcmp(argv[1], "f") == 0) {
                f = fopen("fantasynames.txt", "r");
            } else if (strcmp(argv[1], "e") == 0) {
                f = fopen("englishnames.txt", "r");
            } else {
                f = fopen("allnames.txt", "r");
            }
            fgenerate(f, name, n);
            fclose(f);
        }

        printf("%sRandomly Generated Name", BOLD);
        if (n > 1) {
            printf("s:%s\n", RESET);
            for (int i = 0; i < n; i++) {
                printf(" %d. %s\n", (i + 1), name[i]);
            }
        } else {
            printf(":%s %s\n", RESET, name[0]);
        }
    }

    return 0;
}