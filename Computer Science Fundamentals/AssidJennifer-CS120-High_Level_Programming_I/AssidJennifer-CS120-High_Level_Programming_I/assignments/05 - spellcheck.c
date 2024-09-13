/***************************************
filename    spellcheck.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    11
due date    6 December 2020

Brief Description: analyze strings to a given file

***************************************/

#include <stdio.h>      /* fopen, fgets, fclose, feof       */
#include <string.h>     /* strlen                           */

#include "assignments.h"

/* check, info, words_starting_with, word_lengths, mystrupr */
/* FILE_OK, FILE_ERR_OPEN, WORD_OK, WORD_BAD, LONGEST_WORD  */
#define FILE_OK       -1 /* If the file was opened successfully         */
#define FILE_ERR_OPEN -2 /* If the file was unable to be opened         */
#define WORD_OK        1 /* If the word was found in the dictionary     */
#define WORD_BAD       2 /* If the word was not found in the dictionary */

#define LONGEST_WORD 50  /* This is the longest word you need to handle */

struct DICTIONARY_INFO
{
    int shortest;
    int longest;
    int count;
};

/******************************************************************************
   Function: *mystrupr

Description: converts all lowercase letters to uppercase in a given string

     Inputs: *string - string being analyzed

    Outputs: string off all uppercase letters and returns the string pointer
******************************************************************************/
char* mystrupr(char* string)
{
    int i;   /* loop counter         */
    int len; /* length of the string */

    /* determines the length of the string */
    len = strlen(string);

    /* iterates through the string */
    for (i = 0; i < len; i++)
    {
        /* checks if the current character is lowercase */
        if (*(string + i) >= 'a' && *(string + i) <= 'z')
            /* changes the lowercase to uppercase */
            *(string + i) -= 32;
    }

    /* returns pointer to beginning of the string */
    return string;
}

/******************************************************************************
   Function: words_starting_with

Description: counting how many words start with a specific letter

     Inputs: *dictionary - strings to analyze
                  letter - letter to check for

    Outputs: a total of words beginning with letter specified
******************************************************************************/
int words_starting_with(const char* dictionary, char letter)
{
    char buffer[LONGEST_WORD]; /* storage for string to be analyzed */
    FILE* file;                /* pointer to file                   */

    int total = 0;             /* running total                     */

    char* letterMod = &letter; /* pointer to the letter             */

    /* attempts to open file */
    file = fopen(dictionary, "r");
    /* checks if file is opened*/
    if (file)
    {
        /* while the code is not at the end of the document*/
        while (!feof(file))
        {
            /* gets line in file */
            if (fgets(buffer, LONGEST_WORD, file))
            {
                /* capitalizes the string */
                mystrupr(buffer);
                /* capitalizes the letter */
                mystrupr(letterMod);

                /* if the first letter of the word if equal to specified letter */
                if (buffer[0] == letter)
                    /* increments the total */
                    total++;
            }
        }

        /* closes file */
        fclose(file);

        /* returns the total */
        return total;

    }
    /* checks if file wasn't opened */
    else
    {
        /* send error message */
        return FILE_ERR_OPEN;
    }
}

/******************************************************************************
   Function: word_lengths

Description: determines word lengths and stores them in proper index in array

     Inputs: *dictionary - file to be analyzed
               lengths[] - array to store calculated values
                   count - max length to check

    Outputs: array with proper totals for each word length
******************************************************************************/
int word_lengths(const char* dictionary, int lengths[], int count)
{
    char buffer[LONGEST_WORD]; /* storage for string to be analyzed */
    FILE* file;                /* pointer to file                   */

    int len;                   /* length of string                  */

    /* attempts to open file */
    file = fopen(dictionary, "r");
    /* checks if file is opened*/
    if (file)
    {
        /* while the code is not at the end of the document*/
        while (!feof(file))
        {
            /* gets line in file */
            if (fgets(buffer, LONGEST_WORD, file))
            {
                /* removes the /n */
                buffer[strcspn(buffer, "\n")] = 0;

                /* length of the buffer */
                len = strlen(buffer);

                /* checks if the length of the string is within the array bounds */
                if (len <= count)
                    /* increments the proper index according the string length */
                    lengths[len]++;
            }
        }

        /* closes file */
        fclose(file);

        /* returns that file opened */
        return FILE_OK;

    }
    else
    {
        /* returns that file failed to open */
        return FILE_ERR_OPEN;
    }
}

/******************************************************************************
   Function: info

Description: inputs information of a file into a struct

     Inputs: *dictionary - string to analyze
                  *dinfo - struct to modify

    Outputs: modified struct holding correct information for given file
******************************************************************************/
int info(const char* dictionary, struct DICTIONARY_INFO* dinfo)
{
    char buffer[LONGEST_WORD]; /* storage for string to be analyzed */
    FILE* file;                /* pointer to file                   */

    int len;                   /* length of string                  */

    /* initializing all elements in struct */
    dinfo->count = 0;
    dinfo->longest = 0;
    dinfo->shortest = LONGEST_WORD;

    /* attempts to open file */
    file = fopen(dictionary, "r");
    /* checks if file is opened*/
    if (file)
    {
        /* while the code is not at the end of the document*/
        while (!feof(file))
        {
            /* gets line in file */
            if (fgets(buffer, LONGEST_WORD, file))
            {
                /* removes the /n */
                buffer[strcspn(buffer, "\n")] = 0;

                /* increments the word count */
                dinfo->count++;

                /* length of the buffer */
                len = strlen(buffer);

                /* checks if current length is shorter than recorded shortest */
                if (len < dinfo->shortest)
                    /* sets the shortest length element to current string */
                    dinfo->shortest = len;

                /* check if current length is longer than recorded longest */
                if (len > dinfo->longest)
                    /* sets the longest length element to current string */
                    dinfo->longest = len;
            }
        }

        /* returns that file opened */
        return FILE_OK;
    }
    else
        /* returns that file failed to open */
        return FILE_ERR_OPEN;
}

/******************************************************************************
   Function: spell_check

Description: determines if a word given is also included in given file

     Inputs: *dictionary - string to analyze
                   *word - word to compare with

    Outputs: returns whether the word is included in file or not
******************************************************************************/
int spell_check(const char* dictionary, const char* word)
{
    char buffer[LONGEST_WORD];     /* storage for string to be analyzed      */
    FILE* file;                    /* pointer to file                        */

    int i;                         /* loop counter                           */
    int len;                       /* length of string                       */
    int wordMatch;                 /* a counter to track is the word matches */

    char wordMod;                  /* variable to hold the value of *word    */

    /* attempts to open file */
    file = fopen(dictionary, "r");
    /* checks if file is opened*/
    if (file)
    {
        /* while the code is not at the end of the document*/
        while (!feof(file))
        {
            /* gets line in file */
            if (fgets(buffer, LONGEST_WORD, file))
            {
                /* initializes wordMatch to 0 for each string */
                wordMatch = 0;

                /* removes the /n */
                buffer[strcspn(buffer, "\n")] = 0;

                /* capitalizes the string in the buffer */
                mystrupr(buffer);

                /* length of the buffer */
                len = strlen(buffer);

                /* checks if the length of the buffer and given word are the same */
                if (len == (int)strlen(word))
                {
                    /* iterates through the string */
                    for (i = 0; i < len; i++)
                    {
                        /* sets the value of *word into a variable that can be changed*/
                        wordMod = *(word + i);

                        /* capitalizes the current letter*/
                        mystrupr(&wordMod);

                        /* checks if the elements in the arrays match */
                        if (buffer[i] == wordMod)
                            /* increments the checker variable */
                            wordMatch++;

                        /* checks if all the indexes in the buffer match the word*/
                        if (wordMatch == len)
                            /* returns success */
                            return WORD_OK;
                    }
                }
            }
        }
        /* returns failure */
        return WORD_BAD;
    }
    else
        /* returns that file failed to open */
        return FILE_ERR_OPEN;
}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/30/24 ******/
// Derived from main file given with lab from class - I DO NOT OWN THE FOLLOWING CODE
void spellcheck_test1(void)
{
    int i;
    const char* words[] = { "Four", "score", "and", "seven", "years", "ago", "our",
                           "fathers", "brought", "forth", "on", "this",
                           "continent", "a", "new", "NATION", "fast123",
                           "123  abc", "Hello!!", "", "*&^%$#8UPPERlower"
    };

    int num_words = sizeof(words) / sizeof(*words);

    printf("\nspellcheck_test1--------------------------------------------------------\n");
    for (i = 0; i < num_words; i++)
    {
        char buffer[LONGEST_WORD + 1];
        strcpy(buffer, words[i]);
        mystrupr(buffer);
        printf("Word: %s (%s)\n", words[i], buffer);
    }
}

void spellcheck_test2(const char* dictionary)
{
    struct DICTIONARY_INFO dinfo; /* To hold the info from the dictionary */
    int file_result;              /* For file errors                      */

    printf("\nspellcheck_test2--------------------------------------------------------\n");
    file_result = info(dictionary, &dinfo);
    if (file_result == FILE_ERR_OPEN)
    {
        printf("Can't open %s\n", dictionary);
        return;
    }

    printf("Dictionary: %s\n", dictionary);
    printf("Number of words: %i\n", dinfo.count);
    printf("Shortest word: %i letters\n", dinfo.shortest);
    printf("Longest word: %i letters\n", dinfo.longest);
}

void spellcheck_test3(const char* dictionary, char letter)
{
    int file_result;

    printf("\nspellcheck_test3--------------------------------------------------------\n");
    file_result = words_starting_with(dictionary, letter);

    if (file_result == FILE_ERR_OPEN)
    {
        printf("Can't open %s\n", dictionary);
        return;
    }

    printf("Dictionary: %s\n", dictionary);
    printf("Number of words starting with %c: %i\n", letter, file_result);
}

void spellcheck_test4(const char* dictionary, const char* word)
{
    int file_result;

    printf("\nspellcheck_test4--------------------------------------------------------\n");
    file_result = spell_check(dictionary, word);
    if (file_result == FILE_ERR_OPEN)
    {
        printf("Can't open %s\n", dictionary);
        return;
    }

    if (file_result == WORD_OK)
        printf("The word %s is spelled correctly.\n", word);
    else
        printf("The word %s is misspelled.\n", word);
}

void spellcheck_test5(void)
{
    int i;
    int file_result;
    const char* words[] = { "Four", "score", "and", "seven", "years", "ago", "our",
                     "fathers", "brought", "forth", "on", "this", "continent",
                     "a", "new", "nation"
    };

    int num_misspelled = 0;
    int num_words = sizeof(words) / sizeof(*words);

    /* Name of the dictionary file */
    const char* dictionary = "allwords.txt";

    printf("\nspellcheck_test5--------------------------------------------------------\n");
    printf("Misspelled words: ");
    for (i = 0; i < num_words; i++)
    {
        file_result = spell_check(dictionary, words[i]);
        if (file_result == FILE_ERR_OPEN)
        {
            printf("Can't open %s\n", dictionary);
            return;
        }

        if (file_result == WORD_BAD)
        {
            printf("%s ", words[i]);
            num_misspelled++;
        }
    }
    if (!num_misspelled)
        printf("*** None ***");
    printf("\n");
}

void spellcheck_test6(void)
{
    int i;
    int file_result;
    const char* words[] = { "Four", "SCORE", "and", "sevn", "years", "ago", "ar",
                     "fawthers", "brought", "foarth", "on", "this", "contnent",
                     "a", "gnu", "nashun"
    };

    int num_misspelled = 0;
    int num_words = sizeof(words) / sizeof(*words);

    /* Name of the dictionary file */
    const char* dictionary = "allwords.txt";

    printf("\nspellcheck_test6--------------------------------------------------------\n");
    printf("Misspelled words: ");
    for (i = 0; i < num_words; i++)
    {
        file_result = spell_check(dictionary, words[i]);
        if (file_result == FILE_ERR_OPEN)
        {
            printf("Can't open %s\n", dictionary);
            return;
        }

        if (file_result == WORD_BAD)
        {
            printf("%s ", words[i]);
            num_misspelled++;
        }
    }
    if (!num_misspelled)
        printf("*** None ***");
    printf("\n");
}

void spellcheck_test7(int max_length)
{
    int file_result; /* For file errors                       */
    int total = 0;
    int length;
    int lengths[21] = { 0 };

    /* Name of the dictionary file */
    const char* dictionary = "allwords.txt";

    printf("\nspellcheck_test7--------------------------------------------------------\n");
    printf("Dictionary: %s\n", dictionary);
    file_result = word_lengths(dictionary, lengths, max_length);
    if (file_result == FILE_ERR_OPEN)
    {
        printf("Can't open %s\n", dictionary);
        return;
    }
    for (length = 1; length <= max_length; length++)
    {
        printf("Number of words of length %2i is %6i\n", length, lengths[length]);
        total += lengths[length];
    }
    printf("Total number of words counted: %i\n", total);
}


void spellcheck(void)
{
    spellcheck_test1();

    spellcheck_test2("lexicon.txt");
    spellcheck_test2("small.txt");
    spellcheck_test2("allwords.txt");

    spellcheck_test3("lexicon.txt", 'a');
    spellcheck_test3("lexicon.txt", 'A');
    spellcheck_test3("lexicon.txt", 'l');
    spellcheck_test3("lexicon.txt", '$');
    spellcheck_test3("lexicon.txt", 'e');

    spellcheck_test3("allwords.txt", 'M');
    spellcheck_test3("allwords.txt", 'q');
    spellcheck_test3("allwords.txt", '5');

    spellcheck_test4("lexicon.txt", "APPLE");
    spellcheck_test4("lexicon.txt", "Apple");
    spellcheck_test4("lexicon.txt", "apple");
    spellcheck_test4("lexicon.txt", "appl");
    spellcheck_test4("small.txt", "abalone");
    spellcheck_test4("allwords.txt", "interdigitation");
    spellcheck_test4("allwords.txt", "interdigalizing");

    spellcheck_test5();
    spellcheck_test6();
    spellcheck_test7(5);
    spellcheck_test7(20);
}