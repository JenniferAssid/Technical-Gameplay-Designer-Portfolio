/*****************************************************************
 * \file   cipher1.c
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Lab #3
 * \date   21/09/2021
 * \brief
 *    Takes encrypted text and decodes it as wellas vice versa
 *********************************************************************/

 /*
    Encoding and decoding for 26 characters of Latin alphabet.
    Instead of using stadard ASCII which uses 8 bits per character, we will
    use the following mapping:

    note each character is encoded by 2+(1 or 2 or 3 or 4) bits:

    0  a <-> 0     Group 0 (00) requires only 1 bit to encode 1 character
    1  b <-> 1     b is encoded as 001 (00 specifies group, 1 is the index inside the group
    ----------
    2  c <-> 00     Group 1 (01) requires only 2 bits to encode 1 character
    3  d <-> 01
    4  e <-> 10     e is encoded as 0110 (01 specifies group, 10 is the index inside the group
    5  f <-> 11
    ----------
    6  g <-> 000     Group 2 (10) requires only 3 bits to encode 1 character
    7  h <-> 001
    8  i <-> 010
    9  j <-> 011     j is encoded as 10011 (10 specifies group, 011 is the index inside the group
    10 k <-> 100
    11 l <-> 101
    12 m <-> 110
    13 n <-> 111
    ----------
    14 o <-> 0000     Group 3 (11) requires only 4 bits to encode 1 character
    15 p <-> 0001
    16 q <-> 0010
    17 r <-> 0011
    18 s <-> 0100
    19 t <-> 0101     t is encoded as 110101 (11 specifies group, 0101 is the index inside the group
    20 u <-> 0110
    21 v <-> 0111
    22 w <-> 1000
    23 x <-> 1001
    24 y <-> 1010
    25 z <-> 1011
  */

  /*
     When writing bits into encrypted text start at least significant bit (random choice, but
     that's how decoder will be reading)
     To write 01 into encrypted text
     7 6 5 4 3 2 1 0  <- bit positions
     - - - - - - 0 1
     to write a 'j' into encrypted text
     first write 10
     7 6 5 4 3 2 1 0  <- bit positions
     - - - - - - 1 0
     then 011
     7 6 5 4 3 2 1 0  <- bit positions
     - - - 1 1 0 1 0

     Note that you write bits without padding, so encrypted character
     may span 2 bytes (start in byte X and end in byte X+1)

     There will be unused bits in the end of the encrypted text, those
     should never be read, so may be garbage.

     Example: encode text "cccc"
     c - group 01, position in group 00, so
     7 6 5 4 3 2 1 0  <- bit positions
             0 0 0 1
     4 c's produce 16 bits (2 bytes)
     0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1
     if you write them into a file and "od -x" you will see
     1111 - 4 hexadecimal 1's, each 0x1 = 0001.
     */


     /* To decipher string, first read 2 bits that represent the group, i.e.
     how many bits to read during the second step
      b1b2 -> how many bits to read
      if
      00 -> read 1 bit  next, character with be    a or b
      01 -> read 2 bits next, character with be in range c-f
      10 -> read 3 bits next, character with be in range g-n
      11 -> read 4 bits next, character with be in range o-z

      example
      10110, 10 - read next 3 bits: 110 - 'm'
      0110 , 01 - read next 2 bits: 10  - 'e'
      000  , 00 - read next 1 bit : 0   - 'a'
      notice illegal sequences 1111..: 11 means read 4 bits, 11.. - there are no characters have representation 11..
      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cipher.h"

      /* may be usefule for debugging */
void print_bit(char v, int pos)
{
    char one = 1;
    if (v & one << pos) { printf("1"); }
    else { printf("0"); }
}

/* may be usefule for debugging */
void print_bits(char* buffer, int start_pos, int how_many)
{
    int i, j;
    printf("Bits: ");
    for (i = 0; i < how_many; ) { /* for each char */
        char* ch = buffer + (start_pos + i) / 8;
        for (j = 0; j < 8 && i < how_many; ++j, ++i) { /* from more significant to less */
            print_bit(*ch, j);
        }
    }
    printf("\n");
}

int read_bit(char const* buffer, int i)
{
    char const* ch = buffer + i / 8;
    int pos = i % 8;
    return (*ch & 1 << pos) ? 1 : 0;
}

/* decode num_chars characters from ciphertext */
/* function assumes output C-style string is allocated by someone else and big enough */
void decode(char const* ciphertext, int num_chars, char* plaintext)
{
    int i, j;
    int pos = 0;
    for (i = 0; i < num_chars; ++i) {
        /* read 2 bits for group (00,01,10,11) */
        int group_index = read_bit(ciphertext, pos) + 2 * read_bit(ciphertext, pos + 1);
        int index = 0; /* index inside group */
        pos += 2;
        for (j = 0; j < group_index + 1; ++j) {
            index += (read_bit(ciphertext, pos) << j);
            ++pos;
        }
        plaintext[i] = 'a' + ((1 << (group_index + 1)) - 2) + index;
    }
    plaintext[num_chars] = 0; /* null terminate final result */
}

int counter = 0;

void write_bit(char bit_value, char** encrypted_text)
{
    if (bit_value & (1 << 0))
    {
        /* Set bit */
        (**encrypted_text) |= (1 << counter);
    }
    else
    {
        /* Clear bit */
        (**encrypted_text) &= ~(1 << counter);
    }

    counter++;

    if (counter >= 8)
    {
        (*encrypted_text)++;
        counter = 0;
    }
}

/* assume all lower case input a-z (no spaces) */
void encode(char const* plaintext, char* encryptedtext, int* num_bits_used)
{
    counter = 0;
    (*num_bits_used) = 0;
    char* plain_walk = (char*)plaintext;
    char char_value = 0;
    int i = 0;

    while (*plain_walk)
    {
        char_value = *(plain_walk)-'a';

        if (char_value / 14 > 0)
        {
            char_value -= 14;

            write_bit(1, &encryptedtext);
            write_bit(1, &encryptedtext);

            for (i = 0; i < 4; i++)
            {
                write_bit(char_value, &encryptedtext);
                char_value >>= 1;
            }

            (*num_bits_used) += 6;
        }
        else if (char_value / 6 > 0)
        {
            char_value -= 6;

            write_bit(0, &encryptedtext);
            write_bit(1, &encryptedtext);

            for (i = 0; i < 3; i++)
            {
                write_bit(char_value, &encryptedtext);
                char_value >>= 1;
            }

            (*num_bits_used) += 5;
        }
        else if (char_value / 2 > 0)
        {
            char_value -= 2;

            write_bit(1, &encryptedtext);
            write_bit(0, &encryptedtext);

            for (i = 0; i < 2; i++)
            {
                write_bit(char_value, &encryptedtext);
                char_value >>= 1;
            }

            (*num_bits_used) += 4;
        }
        else
        {
            write_bit(0, &encryptedtext);
            write_bit(0, &encryptedtext);

            write_bit(char_value, &encryptedtext);

            (*num_bits_used) += 3;
        }

        plain_walk++;
    }
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encode_decode(char const* plaintext)
{
    int buff_size = strlen(plaintext);
    char* encryptedtext = (char*)malloc(buff_size); /* more then needed, so some space will be waisted */
    char* decodedtext = (char*)malloc(buff_size + 1);
    int num_bits_used; /*not used, but the actual number of bytes needed for encodedtext is num_bits_used/8 + (num_bits_used%8)?1:0*/

    encode(plaintext, encryptedtext, &num_bits_used);

    print_bits(encryptedtext, 0, num_bits_used);

    decode(encryptedtext, buff_size, decodedtext);
    printf("%s\n", decodedtext);

    free(decodedtext);
    free(encryptedtext);
}

void cipher_test0() { encode_decode("a"); }
void cipher_test1() { encode_decode("ab"); }
void cipher_test2() { encode_decode("cccc"); }
void cipher_test3() { encode_decode("xaxa"); }
void cipher_test4() { encode_decode("abcdefghijklmnopqrstuvwxyz"); }
void cipher_test5() { encode_decode("kdjasfhkdslfhksdjhfkldsfhlskdfjh"); }
void cipher_test6() { encode_decode("aardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvark"); }

#include <time.h>
#include <stdlib.h>
void cipher_test7() {
    int buff_size = 100000, i;
    char* plaintext = (char*)malloc(buff_size + 1);
    char* encryptedtext = (char*)malloc(buff_size + 1);
    char* decodedtext = (char*)malloc(buff_size + 1);
    int num_bits_used; /*not used, but the actual number of bytes needed for encodedtext is num_bits_used/8 + (num_bits_used%8)?1:0*/

    /* randomly generate plaintext */
    srand(time(NULL));
    for (i = 0; i < buff_size; ++i) {
        plaintext[i] = 'a' + rand() % 26;
    }
    plaintext[buff_size] = 0;

    encode(plaintext, encryptedtext, &num_bits_used);
    decode(encryptedtext, buff_size, decodedtext);

    if (strcmp(plaintext, decodedtext) == 0) {
        printf("All good\n");
    }

    free(decodedtext);
    free(encryptedtext);
    free(plaintext);
}

void cipher(void)
{
    cipher_test0();
    cipher_test1();
    cipher_test2();
    cipher_test3();
    cipher_test4();
    cipher_test5();
    cipher_test6();
    cipher_test7();
}
