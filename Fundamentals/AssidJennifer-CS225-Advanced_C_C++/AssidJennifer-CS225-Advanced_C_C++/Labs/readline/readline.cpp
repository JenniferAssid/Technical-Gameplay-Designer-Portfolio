/*****************************************************************
 * \file   readline.c
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Lab #1
 * \date   04/09/2021
 * \brief
 *    This is a manual version of readline (present in C++ but not C)
 *********************************************************************/

#include "readline.h" // Associated header
#include <stdlib.h>   // calloc, realloc
#include <string.h>   // strlen


 /**
  * \brief
  *    Reads all characters before a new line (/n), places them in a buffer, and returns the result
  *
  * \param filepath
  *    Where the characters are being read from
  *
  * \return
  *    The pointer to the beginning of the buffer
  */
char* readline(FILE* filepath)
{
    // Check if file is valid
    if (filepath == NULL) return NULL;

    // Set base variables
    size_t capacity = 2; // Space for character read in and /0
    size_t size = 0;     // No data in the buffer yet

    // Preliminary allocation for buffer
    char* buffer = (char*)calloc(capacity, sizeof(size_t));

    // Iterate through the line
    while (fgets(buffer + size, (int)(capacity - size), filepath))
    {
        // Derive how many characters were read in
        size = strlen(buffer);

        // Check if the end token is /n
        if (*(buffer + size - 1) == '\n')
        {
            // Replace /n with /0
            *(buffer + size - 1) = '\0';

            // Exit out of iteration
            break;
        }

        // Increase the capacity
        capacity *= 2;

        // Reallocate the buffer
        buffer = (char*)realloc(buffer, capacity);
    }

    // Return the buffer
    return buffer;
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <stdio.h>  /* fopen/fclose, printf */
#include <stdlib.h> /* malloc/free */

/* read first line from file and dump it */
void simple_test(char const* filename)
{
    FILE* file_handle;
    fopen_s(&file_handle, filename, "r");
    if (!file_handle) {
        printf("Cannot open file for reading\n");
    }

    char* str = readline(file_handle);
    printf("\n----------------------\n");
    printf("-->%s<--", str);
    printf("\n----------------------\n");
    fclose(file_handle);
    free(str);
}

/*
   creates a file that contains character ch size times
   then if newline_in_middle is set, newline added followed by character ch+1 size times
   if newline_in_end is set, newline is added in the end of the file

   then readline is called and retuned value compared to expected:
   ch size times followed by null-character (ASCII 0)
 */
void read_long_line(int size, char ch, int newline_in_middle, int newline_in_end)
{
    /* create a file  */
    char const* filename = "temp_file";
    FILE* file_handle; /* write text */
    fopen_s(&file_handle, filename, "w");
    int i, total_size;
    char* buffer;

    if (!file_handle) {
        printf("Cannot open file for reading\n");
        return;
    }

    total_size = size + (newline_in_middle ? 1 + size : 0) + (newline_in_end ? 1 : 0);
    buffer = (char*)malloc(total_size);
    if (buffer) {
        for (i = 0; i < size; ++i) {
            buffer[i] = ch;
        }
        if (newline_in_middle) {
            buffer[i] = '\n'; /* position = size */
            ++i;
            for (; i < 2 * size + 1; ++i) {
                buffer[i] = (char)(ch + 1);
            }
        }
        if (newline_in_end) {
            buffer[i] = '\n';
        }
    }

    fwrite(buffer, total_size, 1, file_handle);
    fclose(file_handle);

    /* open the same file and pass to readline */ /* read text */
    fopen_s(&file_handle, filename, "r");
    if (!file_handle) {
        printf("Cannot open file for reading\n");
        return;
    }

    char* str = readline(file_handle);
    for (i = 0; i < size; ++i) {
        if (str[i] != ch) {
            printf("error: str[%i] = %c, when %c was expected\n", i, str[i], ch);
        }
    }
    if (str[i] != 0) {
        printf("error: str[%i] = %c, when null-char was expected\n", i, str[i]);
    }
    fclose(file_handle);
    remove(filename);
    free(str);
    free(buffer);
}

void readline_test0(void) { simple_test("Labs/replace/in0"); } /* 3 chars, newline in the end */
void readline_test1(void) { simple_test("Labs/replace/in1"); } /* 7 chars, newline in the end */
void readline_test2(void) { simple_test("Labs/replace/in2"); } /* 3 chars, newline, 3 chars, new line in the end */
void readline_test3(void) { simple_test("Labs/replace/in3"); } /* 7 chars, newline, 7 chars, new line in the end */
void readline_test4(void) { simple_test("Labs/replace/in4"); } /* 3 chars, NO new line in the end */
void readline_test5(void) { simple_test("Labs/replace/in5"); } /* empty file, but new line in the end. Filesize = 1 */
void readline_test6(void) { read_long_line(1 << 12, '.', 1, 1); }
void readline_test7(void) { read_long_line(1 << 20, '.', 0, 1); }
void readline_test8(void) { read_long_line(1 << 22, '.', 1, 1); }
void readline_test9(void) { read_long_line(1 << 26, '.', 0, 0); } /* about 0.5 sec */

void readline(void)
{
    readline_test0();
    readline_test1();
    readline_test2();
    readline_test3();
    readline_test4();
    readline_test5();
    readline_test6();
    readline_test7();
    readline_test8();
    readline_test9();
}