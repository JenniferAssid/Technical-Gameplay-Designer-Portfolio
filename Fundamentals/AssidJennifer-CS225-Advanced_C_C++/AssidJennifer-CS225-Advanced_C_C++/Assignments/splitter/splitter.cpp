/*****************************************************************
 * \file   splitter.c
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Assignment 01
 * \date   13/09/2021
 * \brief
 *    Contains the following functions
 *
 *    -  SplitFile
 *          Splits given file into as many files as needed that contain
 *          "chunks" within the given size.
 *
 *    -  JoinFiles
 *          Joins the contents of given files into one combined file with
 *          same size as the sum of all the given files.
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "splitter.h"

 /* Max capacity for read / write buffer */
#define FOUR_K 4096

/**
 * Splits given file into as many files as needed that contain "chunks" within the given size.
 *
 * \param filename File that is being split
 * \param output The suffix of the output file to put determined chunks into
 * \param size Size of the desired chunks
 * \return Error codes (if any)
 */
int SplitFile(char const* filename, char const* output, size_t size)
{
    /* Initiating all variables necessary for function */
    int nextAmountReadIn = 0;       /* Next amount of characters being read in from stream */
    int bufferSize = 0;             /* Size of the buffer to read / write values           */
    int bufferSizeCopy = 0;         /* Variable to store the determined buffer size        */
    char* writeInBuff = 0;          /* The buffer to retrieve data and then output         */
    int chunkCount = 1;             /* Number of "chunks" (files) created                  */
    char outFileName[256] = { 0 };  /* Buffer to calculate the name of output file         */
    FILE* output_fp = 0;            /* Pointer to the current output file                  */
    int spaceInChunk = (int)size;   /* Space remaining in the current chunk                */
    int amountReadIn = 0;           /* Current amount of characters read in from stream    */

    /* Attempt to open the input file*/
    FILE* input_fp = fopen(filename, "rb");
    if (!input_fp) return E_BAD_SOURCE;

    /* Determine the buffer size*/
    bufferSize = (int)size;
    if (size > FOUR_K) bufferSize = FOUR_K;
    bufferSizeCopy = bufferSize;

    /* Create write-in buffer based on previous calculations*/
    writeInBuff = (char*)calloc(bufferSize, sizeof(char));
    if (!writeInBuff) return E_NO_MEMORY;

    /* Determine the output file suffix*/
    sprintf(outFileName, "%s%04i", output, chunkCount);

    /* Attempt to open the output file*/
    output_fp = fopen(outFileName, "ab");
    if (!output_fp) { fclose(input_fp); return E_BAD_DESTINATION; }

    /* First amount read in from the input file */
    amountReadIn = fread(writeInBuff, sizeof(char), bufferSize, input_fp);

    /* Iterate over the file*/
    while (amountReadIn)
    {
        /* If less than count has been achieved*/
        if (amountReadIn < bufferSize)
        {
            /* Reallocate the buffer to the smaller size*/
            writeInBuff = (char*)realloc(writeInBuff, amountReadIn * sizeof(char));
            if (!writeInBuff) { fclose(input_fp); fclose(output_fp); return E_NO_MEMORY; }

            /* Write the contents to the output file*/
            fwrite(writeInBuff, sizeof(char), amountReadIn, output_fp);

            /* Re-read file to get 0*/
            amountReadIn = fread(writeInBuff, sizeof(char), amountReadIn, input_fp);

            /* Start into the next loop */
            continue;
        }

        /* Place contents of read-in buffer to output file*/
        fwrite(writeInBuff, sizeof(char), bufferSize, output_fp);

        /* Adjust the space in chunk integer*/
        spaceInChunk -= bufferSize;

        /* If the chunk has been filled*/
        if (spaceInChunk == 0)
        {
            /* Reset the buffer size*/
            bufferSize = bufferSizeCopy;

            /* Reallocate the buffer to original size*/
            writeInBuff = (char*)realloc(writeInBuff, bufferSize * sizeof(char));

            /* Get next amount of character read in*/
            nextAmountReadIn = fread(writeInBuff, sizeof(char), bufferSize, input_fp);

            /* If at the end of file - free and leave*/
            if (nextAmountReadIn == 0)
            {
                fclose(output_fp); fclose(input_fp); free(writeInBuff);
                return 0;
            }
            /* Else prepare for next iteration*/
            else
                amountReadIn = nextAmountReadIn;

            /* Restore space in chunk*/
            spaceInChunk = size;

            /* Close current output file*/
            fclose(output_fp);

            /* Increase the chunk count and determine new output filename*/
            sprintf(outFileName, "%s%04i", output, ++chunkCount);

            /* Attempt to open new output file*/
            output_fp = fopen(outFileName, "ab");
            if (!output_fp) { fclose(input_fp); free(writeInBuff); return E_BAD_DESTINATION; }

            /* Continue to next loop */
            continue;
        }

        /* If the space remaining is less than the buffer size */
        if (spaceInChunk < bufferSize)
        {
            /* Set the buffer size to be the rest of the chunk */
            bufferSize = spaceInChunk;
            /* Reallocate the buffer */
            writeInBuff = (char*)realloc(writeInBuff, bufferSize * sizeof(char));
        }

        /* Read in next segement*/
        amountReadIn = fread(writeInBuff, sizeof(char), bufferSize, input_fp);
    }


    /* Close / free / return */
    fclose(output_fp); fclose(input_fp); free(writeInBuff);
    return 0;
}

/**
 * Joins the contents of given files into one combined file with
 *  same size as the sum of all the given files.
 *
 * \param filenames Files being combined together
 * \param num_files Number of files being combined together
 * \param output Combined file
 * \return Errors (if any)
 */
int JoinFiles(char** filenames, int num_files, char const* output)
{
    /* Initiating all variables necessary for function */
    FILE* input_fp = 0;       /* Pointer to the input file                */
    int amountReadIn = 0;     /* Amount of characters read in from stream */
    int bufferSize = FOUR_K;  /* Buffer size set to the max               */
    char* retrieveBuffer = 0; /* Buffer to read in characters to          */
    int i = 0;                /* Iterator                                 */

    /* Open the output file*/
    FILE* output_fp = fopen(output, "ab");
    if (!output_fp) return E_BAD_DESTINATION;

    /* Allocate the buffer at max capacity (FOUR_K)*/
    retrieveBuffer = (char*)calloc(FOUR_K, sizeof(char));
    if (!retrieveBuffer) { fclose(output_fp); return E_NO_MEMORY; }

    /* Iterate through input files*/
    for (i = 0; i < num_files; i++)
    {
        /* Attempt to open the input file */
        input_fp = fopen(*(filenames + i), "rb");
        if (!input_fp) { fclose(output_fp); free(retrieveBuffer); return E_BAD_SOURCE; }

        /* Reallocate the buffer if necessary */
        if (bufferSize < FOUR_K) retrieveBuffer = (char*)realloc(retrieveBuffer, FOUR_K * sizeof(char));

        /* Get the intial reading */
        amountReadIn = (int)fread(retrieveBuffer, sizeof(char), FOUR_K, input_fp);

        /* If the amount read in is less than the max buffer */
        if (amountReadIn < FOUR_K)
        {
            /* Change the buffer size to reflect to new size */
            bufferSize = amountReadIn;

            /* Resize the buffer to the new size */
            retrieveBuffer = (char*)realloc(retrieveBuffer, bufferSize * sizeof(char));
        }

        /* Go through the input file */
        while (amountReadIn)
        {
            /* If the read in amount is less than the buffer size */
            if (amountReadIn < bufferSize)
                /* Resize the buffer to the specific size */
                retrieveBuffer = (char*)realloc(retrieveBuffer, amountReadIn * sizeof(char));

            /* Write in the read in amount */
            fwrite(retrieveBuffer, sizeof(char), amountReadIn, output_fp);

            /* Re-cycle through the file */
            amountReadIn = fread(retrieveBuffer, sizeof(char), amountReadIn, input_fp);
        }

        /* Close the input loop */
        fclose(input_fp);
    }

    /* Close / free / return */
    fclose(output_fp); free(retrieveBuffer);
    return 0;
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
//
// this is a more automated driver C++ version
// it will perform splits and joins 
// and then check the resulting chunks and combined files.
//
// Noted that the other driver (driver.cpp and driver.c) 
// will just split or join, and then you have to run diff manually
// to check correctness of chunks and combined files. 
//
//
#include <cstdio>  /* printf */
#include <cstring> /* strstr,strlen */
#include <cstdlib> /* atoi for C++ compiler */
#include "splitter.h"

int     test_chunks(char* in, char* out, int size);
int     test_combined(char* combined, char** chunks, int num_files);
int     filesize(char* filename);
char** find_files(char* prefix, int* num_files);
void    clean_filenames(char** pp, int num_files);

void test0(void);
void test1(void);
void test2(void);
void test3(void);
void test4(void);
void test5(void);

int filesize(char* filename) {
    FILE* F = fopen(filename, "rb");
    int size;
    fseek(F, 0L, SEEK_END);
    size = ftell(F);
    fclose(F);
    return size;
}

void clean_filenames(char** pp, int num_files) {
    char** head = pp;
    int i = 0;
    for (; i < num_files; ++i) {
        /*printf( "clean --> %s\n", pp[i] );*/
        free(pp[i]);
    }
    free(head);
}

char** find_files(char* prefix, int* num_files) {
    char** pp_filenames;
    int count = 0, i = 0, stop = 0;
    char filename[256];

    while (!stop) {
        sprintf(filename, "%s%04u", prefix, (unsigned)++count);
        printf("%s\n", filename);
        FILE* OUT = fopen(filename, "rb");


        /* does file exist? */
        if (!OUT) {
            /*printf("NO %s\n",filename);*/
            --count;
            stop = 1;
        }
        else {
            /*printf("found %s\n",filename);*/
            fclose(OUT);
        }
    }

    pp_filenames = (char**)malloc((count + 1) * sizeof(char*));
    while (i < count) {
        sprintf(filename, "%s%04u", prefix, (unsigned)(i + 1));
        pp_filenames[i] = (char*)malloc(strlen(filename) + 1);
        strcpy(pp_filenames[i], filename);
        ++i;
    }

    *num_files = count;
    /*printf("found %i files\n",*num_files);*/
    pp_filenames[count] = NULL;
    return pp_filenames;
}

int test_chunks(char* in, char* out, int size) {
    size_t file_count = 0;
    FILE* IN; FILE* OUT;
    int size_in;
    int pos_in = 0;
    char outfile_name[256];
    char* buffer = (char*)malloc(2 * size);
    IN = fopen(in, "rb");


    fseek(IN, 0L, SEEK_END);
    size_in = ftell(IN);
    fseek(IN, 0L, SEEK_SET);

    printf("+++Input file \"%s\" size is %d\n", in, size_in);

    /* try to open chunks */
    while (pos_in < size_in) {
        int i = 0;
        if (size_in - pos_in < size) { /* for the last chunk */
            size = size_in - pos_in;
        }
        sprintf(outfile_name, "%s%04u", out, (unsigned)++file_count);
        printf("+++Trying to open file \"%s\" ", outfile_name);
        OUT = fopen(outfile_name, "rb");
        /* does file exist? */
        if (!OUT) {
            printf("FAILED\n");
            free(buffer); fclose(IN); return 1;
        }
        else {
            printf("OK\n");
        }
        /* is it the right size? */
        fseek(OUT, 0L, SEEK_END);
        printf("+++    checking chunk size, should be %d ", size);
        if (size != ftell(OUT)) {
            printf("FAILED\n");
            free(buffer); fclose(OUT); fclose(IN); return 2;
        }
        else {
            printf("OK\n");
        }
        /* is the content correct? */
        printf("+++    checking the content of the chunk ");
        fseek(OUT, 0L, SEEK_SET);
        pos_in += fread(buffer, 1, size, IN);
        fread(buffer + size, 1, size, OUT);
        fclose(OUT);
        for (; i < size; ++i) {
            if (buffer[i] != buffer[i + size]) {
                printf("FAILED at byte %d\n+++ should be %c, read %c", i, buffer[i], buffer[i + size]);
                free(buffer);
                fclose(IN);
                return 3;
            }
        }
        printf("OK\n");


    }
    /* check no extra chunks exist */
    sprintf(outfile_name, "%s%04u", out, (unsigned)++file_count);
    printf("+++Trying to open file \"%s\" ", outfile_name);
    OUT = fopen(outfile_name, "rb");
    /* does file exist? */
    if (OUT) {
        printf("extra file exists - FAILED\n");
        free(buffer);
        fclose(IN);
        fclose(OUT);
        return 4;
    }
    else {
        printf("no extra file - OK\n");
    }

    free(buffer);
    fclose(IN);
    return 0;
}

int test_combined(char* combined, char** pp, int num_files) {
    FILE* COMBINED; FILE* CHUNK;
    int   size_chunk; int j;
    char* buffer;
    int size_combined = 0; int pos_combined = 0; int i = 0;
    COMBINED = fopen(combined, "rb");


    if (!COMBINED) {
        printf("+++FAILED to open combined file \"%s\"\n", combined);
        return 1;
    }

    /* size of combined */
    fseek(COMBINED, 0L, SEEK_END);
    size_combined = ftell(COMBINED);
    fseek(COMBINED, 0L, SEEK_SET);

    for (i = i; i < num_files; ++i) {
        /* read the whole chunk, read same number of bytes from combined */

        CHUNK = fopen(pp[i], "rb");

        printf("+++Open chunk \"%s\" ", pp[i]);
        if (!CHUNK) {
            printf("FAILED\n");
            fclose(COMBINED);
            return 2;
        }
        else {
            printf("OK\n");
        }

        /* size of chunk */
        fseek(CHUNK, 0L, SEEK_END);
        size_chunk = ftell(CHUNK);
        fseek(CHUNK, 0L, SEEK_SET);
        buffer = (char*)malloc(2 * size_chunk);

        fread(buffer, 1, size_chunk, CHUNK);
        fread(buffer + size_chunk, 1, size_chunk, COMBINED);
        fclose(CHUNK);
        pos_combined += size_chunk;

        for (j = 0; j < size_chunk; ++j) {
            if (buffer[j] != buffer[j + size_chunk]) {
                printf("+++FAILED at byte %d\n+++ should be %c, read %c", j, buffer[j], buffer[j + size_chunk]);
                free(buffer);
                fclose(COMBINED);
                return 3;
            }
        }

        free(buffer);
        printf("+++  chunk and joined file match\n");
    }

    fclose(COMBINED);

    if (pos_combined != size_combined) {
        printf("+++Combined file size is not the sum of the chunk sizes\n");
        return 5;
    }

    return 0;
}

// note change from C to C++, C-style strings have to be "char []" instead of a pointer
void splitter_test0(void) {
    char in[] = "Assignments/splitter/input0"; /* this file is 120 bytes, so expecting two chunks: 100 and 20 bytes */
    char out[] = "Assignments/splitter/student_test0_";
    int size = 100;
    SplitFile(in, out, size);
    test_chunks(in, out, size);
}

void splitter_test1(void) {
    char in[] = "Assignments/splitter/input1"; /* this file is 120 bytes, so expecting two chunks: 100 and 100 bytes */
    char out[] = "Assignments/splitter/student_test1_";
    int size = 100;
    SplitFile(in, out, size);
    test_chunks(in, out, size);
}

void splitter_test2(void) {
    char in[] = "Assignments/splitter/input2"; /* this file is 19999 bytes, so expecting two chunks: 10000 and 9999 bytes */
    char out[] = "Assignments/splitter/student_test2_";
    int size = 10000; /* chunk size is larger then 4096, so you will need to read input 3 times 4096,4096,1808 to assemble the first chunk */
    SplitFile(in, out, size);
    test_chunks(in, out, size);
}

/* join tests */
void splitter_test3(void) {
    char combined[] = "Assignments/splitter/student_combined5";
    char chunks[] = "Assignments/splitter/input5_";
    int    num_files;
    char** pp = find_files(chunks, &num_files); /* search directory for input5_* */
    JoinFiles(pp, num_files, combined);
    test_combined(combined, pp, num_files);
    clean_filenames(pp, num_files);
}
