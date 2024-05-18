#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <Windows.h>

#define WORDSNUM 4266
#define COMMONNUM 483


int generateFile(char* fileNameAllWords, char* new, int length)
{
    char s[100];
    FILE* fin = fopen(new, "w");
    FILE* fout = fopen(fileNameAllWords, "r");
    if (!fin || !fout)
    {
        printf("\nCouldn't Open the file :(\n");
    }

    int size = 0;
    while (fgets(s, 100, fout))
    {
        if (s[strlen(s) - 1] == '\n')
        {
            if (strlen(s) == length + 1)
            {
                fprintf(fin, "%s", s);
                size++;
            }
        }
        else if (strlen(s) == length)
        {
            fprintf(fin, "%s\n", s);
            size++;
        }

    }
    fclose(fin);
    fclose(fout);
    return size;
}

int randomNum(int min, int max)
{
    return rand() % (max - min) + min;
}

int randomRealNum(int min, int max, int loops)
{
    srand(time(NULL));
    int i, rand1;
    for (i = 0; i < loops; i++)
    {
        rand1 = rand() % (max - min) + min;
    }
    return rand1;
}

void createFiles(int* len, char** fileName1, char** fileName2, int* sizeAll, int* sizeCommon)
{
    *len = 0;
    while (*len < 3 || *len > 9)
    {
        printf("Enter the word length (3 - 9): ");
        scanf("%d", len);
        rewind(stdin);
    }

    *fileName1 = (char*)malloc(42 * sizeof(char));
    *fileName2 = (char*)malloc(48 * sizeof(char));
    strcpy(*fileName1, "EnglishWords_L.dat");
    strcpy(*fileName2, "EnglishWordsCommon_L.dat");
    (*fileName1)[35] = (*fileName2)[41] = (char)(*len) + '0';


    *sizeAll = generateFile("AllEnglishWords.dat", *fileName1, *len);
    *sizeCommon = generateFile("AllEnglishWordsCommon.dat", *fileName2, *len);
}

void deleteFiles(char* fileName1, char* fileName2)
{
    //----- Optional - Remove the "/" to keep the files created for this round ---------


    // printf("Do you wish to delete the files created for this round? (Y/N): ");
    // char answer;
    // scanf("%c", &answer);
    // rewind(stdin);
    // while (toupper(answer) != 'Y' && toupper(answer) != 'N')
    // {
    //     printf("Do you wish to delete the files created for this round? (Y/N): ");
    //     scanf("%c", &answer);
    //     rewind(stdin);
    // }

    // if (toupper(answer) == 'Y')
    // {
        remove(fileName1);
        remove(fileName2);
        //printf("The files were successfully deleted");
    //}
}

char* chooseWord(FILE* fp, int lenWord, int lenFile)
{
    int wordPlace = randomRealNum(1, lenFile, randomNum(1, 500));
    char* word = (char*)malloc((lenWord + 1) * sizeof(char));
    fseek(fp, (wordPlace - 1) * (lenWord + 2), SEEK_SET);
    fgets(word, lenWord + 1, fp);
    return word;
}

int checkWord(FILE* fp, char* word, int lenFile)
{
    int lenWord = strlen(word);
    int middle, low = 1, high = lenFile;
    char* buffer = (char*)malloc((lenWord + 1) * sizeof(char));
    int equivelent;
    while (low <= high)
    {
        middle = (low + high) / 2;
        fseek(fp, (middle - 1) * (lenWord + 2) * sizeof(char), SEEK_SET);
        fgets(buffer, lenWord + 1, fp);
        equivelent = strcmp(word, buffer);
        if (equivelent == 0)
        {
            return 1;
        }
        if (equivelent > 0)
        {
            low = middle + 1;
            continue;
        }

        high = middle - 1;
    }
    return 0;
}

int wordFeedback(FILE* fp, char* realWord, char* userGuess, int* realWordLetters, int lenFile)
{
    // FORMAT = "\033[{COLOR}m"
    // RESET = 0
    // RED = 031
    // GREEN = 032
    // YELLOW = 0ord33
    if (strlen(realWord) != strlen(userGuess))
    {
        printf("Enter a %d letter word!\n", strlen(realWord));
        return -1;
    }
    if (!checkWord(fp, userGuess, lenFile))
    {
        printf("This word doesn't exist!\n");
        return -1;
    }

    int i, equal = 0;
    int realWordLettersCopy[26] = { 0 };
    for (i = 0; i < 26; i++)
    {
        realWordLettersCopy[i] = realWordLetters[i];
    }

    for (i = 0; userGuess[i]; i++)
    {
        if (tolower(userGuess[i]) == realWord[i])
        {
            realWordLettersCopy[tolower(userGuess[i]) - 'a']--;
            equal++;
            printf("\033[032m");
            printf("%c ", toupper(userGuess[i]));
            printf("\033[0m");
            continue;
        }
        if (realWordLettersCopy[tolower(userGuess[i]) - 'a'] > 0)
        {
            realWordLettersCopy[tolower(userGuess[i]) - 'a']--;
            printf("\033[033m");
            printf("%c ", toupper(userGuess[i]));
            printf("\033[0m");
            continue;
        }
        printf("\033[031m");
        printf("%c ", toupper(userGuess[i]));
        printf("\033[0m");
    }

    return equal == strlen(realWord);
}

int main(int argc, char** argv)
{

    int len, sizeAll, sizeCommon;
    char* fileName1, * fileName2;
    char anotherGame = 'Y';
    while (toupper(anotherGame) != 'N')
    {
        createFiles(&len, &fileName1, &fileName2, &sizeAll, &sizeCommon);

        char* word = (char*)malloc((len + 1) * sizeof(char));

        FILE* fpAll = fopen(fileName1, "r");
        FILE* fpCommon = fopen(fileName2, "r");
        if (!fpAll || !fpCommon)
        {
            printf("\nCouldn't Open the file :(");
            break;
        }

        word = chooseWord(fpCommon, len, sizeCommon);
        int wordLetters[26] = { 0 }, i;
        for (i = 0; word[i]; i++)
        {
            wordLetters[word[i] - 'a']++;
        }


        // game run:
        int guesses = 0;
        char** userGuesses = (char**)malloc(guesses * sizeof(char*));

        printf("Enter the number of guesses: ");
        scanf("%d", &guesses);
        // GOOD LUCK! ASCII art
        printf("\033[032m  _____  ____   ____  _____    _     _    _  _____ _  ___ \n / ____|/ __ \\ / __ \\|  __ \\  | |   | |  | |/ ____| |/ / |\n| |  __| |  | | |  | | |  | | | |   | |  | | |    | ' /| |\n| | |_ | |  | | |  | | |  | | | |   | |  | | |    |  < | |\n| |__| | |__| | |__| | |__| | | |___| |__| | |____| . \\|_|\n \\_____|\\____/ \\____/|_____/  |______\\____/ \\_____|_|\\_(_)\033[0m");
        Sleep(5000);
        system("cls");
        int cur = 0, win = 0;
        while (cur != guesses && !win)
        {
            printf("Enter your guess: ");
            userGuesses[cur] = (char*)malloc(100 * sizeof(char));
            scanf("%s", userGuesses[cur]);
            rewind(stdin);
            system("cls");
            for (i = 0; i <= cur; i++)
            {

                if ((win = wordFeedback(fpAll, word, userGuesses[i], wordLetters, sizeAll)) == -1)
                {
                    win = 0;
                    cur--;
                    continue;
                }
                printf("\n");
            }
            cur++;
        }
        if (win)
        {
            printf("\n\033[032m __     ______  _    _  __          ______  _   _ _ \n \\ \\   / / __ \\| |  | | \\ \\        / / __ \\| \\ | | |\n  \\ \\_/ / |  | | |  | |  \\ \\  /\\  / / |  | |  \\| | |\n   \\   /| |  | | |  | |   \\ \\/  \\/ /| |  | | . ` | |\n    | | | |__| | |__| |    \\  /\\  / | |__| | |\\  |_|\n    |_|  \\____/ \\____/      \\/  \\/   \\____/|_| \\_(_)\n\033[0m");
        }
        else
        {
            printf("\033[031m\n __     ______  _    _   _      ____   _____ _______ _ \n \\ \\   / / __ \\| |  | | | |    / __ \\ / ____|__   __| |\n  \\ \\_/ / |  | | |  | | | |   | |  | | (___    | |  | |\n   \\   /| |  | | |  | | | |   | |  | |\\___ \\   | |  | |\n    | | | |__| | |__| | | |___| |__| |____) |  | |  |_|\n    |_|  \\____/ \\____/  |______\\____/|_____/   |_|  (_)\n\033[0m");
            printf("The word was: %s!\n", word);
        }

        Sleep(5000);
        fclose(fpAll);
        fclose(fpCommon);
        system("cls");
        deleteFiles(fileName1, fileName2);
        Sleep(2000);
        system("cls");

        printf("Do you want to play another game(Y/N)? ");
        scanf("%c", &anotherGame);
        rewind(stdin);
        while (toupper(anotherGame) != 'Y' && toupper(anotherGame) != 'N')
        {
            printf("Do you want to play another game(Y/N)? ");
            scanf("%c", &anotherGame);
            rewind(stdin);
        }
        system("cls");
    }
    return 0;
}
