/*****************************************************************
 * \file   replace.c
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Lab #2
 * \date   04/09/2021
 * \brief
 *    Contains the following functions
 *
 *    -  replace
 *          Iterates through a given string to find a given subscript
 *          and replace that subscript with a given replacement. If the
 *          modified string is too large for the buffer, false is returned
 *********************************************************************/

#include "replace.h"
 // no more #includes

 /**
  * Function: replace
  *
  * \brief
  *          Iterates through a given string to find a given subscript
  *          and replace that subscript with a given replacement. If the
  *          modified string is too large for the buffer, false is returned
  *
  * \param find_substr
  *     Given subcript to find in overall string
  * \param find_size
  *     Size of find subscript
  * \param repl_substr
  *      Given replacement for found subscript
  * \param repl_size
  *      Size of replacement
  * \param str
  *      Overall string
  * \param str_size
  *      Size of overall string
  * \param buffer_size
  *      Size of the buffer the string is inside
  * \return
  *      A tuple that says whether the modified string could fit in the buffer and the new
  *      size of string
  */
std::tuple<bool, int> replace(char const* find_substr, int find_size,
    char const* repl_substr, int repl_size,
    char* str, int str_size,
    int buffer_size)
{
    // Determine the diff in length between repl and find
    int difference = repl_size - find_size;

    // Iterate over the overall string
    for (int i = 0; i < str_size; i++)
    {
        // Set the flag to default position
        bool matchFound = true;

        // Determine if there is enough space within the string for another find string
        if (i + find_size > str_size) return { true, str_size };

        // Iterate across the smaller string
        for (int j = 0; j < find_size; j++)
        {
            // Check if there is a match
            if (*(str + i + j) != *(find_substr + j))
            {
                // Exit is no match
                matchFound = false;
                break;
            }
        }

        // Continue to next iteration of no match has been found
        if (!matchFound) continue;

        /*
                                       ______________
                             _________|_______       |
                             |        |       |      |
        -----  -----  -----  -----  -----  -----  -----
        | a |  | b |  | c |  | d |  | e |  | f |  | g |
        -----  -----  -----  -----  -----  -----  -----
                        |______________|
                                |
                              -----
                              | 1 |
                              -----
        */

        // If replace is less than find, shift left
        if (difference < 0)
        {
            // Replace based on graphic above
            for (int j = i + repl_size; j < str_size - (difference * -1); j++)
                *(str + j) = *(str + j + (difference * -1));

            // Adjust string size
            str_size += difference;

            // Set the end of the string to 0
            *(str + str_size) = 0;
        }

        /*
                                                     ______________
                                               _____|________      |
                                     _________|_____|__      |     |
                                     |        |     | |      |     |
        -----  -----  -----  -----  -----  -----  -----
        | a |  | b |  | c |  | d |  | e |  | f |  | g |  -----  -----
        -----  -----  -----  -----  -----  -----  -----
                        |_______|
                            |
               -----  -----  -----  -----
               | 1 |  | 2 |  | 3 |  | 4 |
               -----  -----  -----  -----
        */

        // If replace is larger than find, shift right
        else if (difference > 0)
        {
            // Check if the buffer will be exceeded and return false if it will
            if (str_size + difference > buffer_size) return { false, 0 };

            // Replace based on the graphic given
            for (int j = str_size + difference - 1; j > i + find_size; j--)
                *(str + j) = *(str + j - difference);

            // Adjust string size
            str_size += difference;
        }

        // Replace find with replace
        for (int j = 0; j < repl_size; j++)
            *(str + i + j) = *(repl_substr + j);
    }

    // Return result
    return{ true, str_size };
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO replace_test FUNCTIONALITY
#include <cstring>
#include <iostream>
#include <tuple>

void print_result(std::tuple<bool, int> const& res, char* str)
{
    if (std::get<0>(res)) {
        std::cout << "FINAL size = " << std::get<1>(res) << ": -->";
        for (int i = 0; i < std::get<1>(res); ++i) {
            std::cout << str[i];
        }
        std::cout << "<--\n";
    }
    else {
        std::cout << "not enough space\n";
    }
}

///////////////////////////////////////////////////////////////////////////////////
// neither string itself or search and replace substring have to be NULL-terminated
///////////////////////////////////////////////////////////////////////////////////


void replace_test0() // no shifting
{
    int const buffer_size = 8;
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[] = { 'c', 'd' };
    char rplc[] = { '1', '2' };

    print_result(replace(find, 2, rplc, 2, str, 8, buffer_size),
        str
    );
}

void replace_test1() // no shifting
{
    int const buffer_size = 12;
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[] = { 'c', 'd' };
    char rplc[] = { '1', '2' };

    print_result(replace(find, 2, rplc, 2, str, 8, buffer_size),
        str
    );
}

void replace_test2() // no shifting, several occurences
{
    int const buffer_size = 20;
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c', 'd' };
    char find[] = { 'a', 'b', 'c' };
    char rplc[] = { '1', '2', '3' };

    print_result(replace(find, 3, rplc, 3, str, 9, buffer_size),
        str
    );
}

void replace_test3() // no shifting, several occurences, one in the very end
{
    int const buffer_size = 20;
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[] = { 'a', 'b', 'c' };
    char rplc[] = { '1', '2', '3' };

    print_result(replace(find, 3, rplc, 3, str, 8, buffer_size),
        str
    );
}

void replace_test4() // no shifting, several occurences, overlapping
{
    int const buffer_size = 20;
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'z', 'z', 'z', 'z' };
    char find[] = { 'z', 'z', 'z' };
    char rplc[] = { '1', '2', '3' };

    print_result(replace(find, 3, rplc, 3, str, 8, buffer_size),
        str
    );
}


////////////////////////////////////////////////////////////////////////////////
void replace_test5() // string shifted left 1 occurence
{
    int const buffer_size = 12;
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[] = { 'c', 'd' };
    char rplc[] = { '1' };

    print_result(replace(find, 2, rplc, 1, str, 8, buffer_size),
        str
    );
}

void replace_test6() // string shifted left 2 occurences
{
    int const buffer_size = 12;
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[] = { 'a', 'b' };
    char rplc[] = { '1' };

    print_result(replace(find, 2, rplc, 1, str, 8, buffer_size),
        str
    );
}

void replace_test7() // string shifted left 2 occurences, one in the very end
{
    int const buffer_size = 12;
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[] = { 'b', 'c' };
    char rplc[] = { '1' };

    print_result(replace(find, 2, rplc, 1, str, 8, buffer_size),
        str
    );
}

void replace_test8() // string shifted left 2 occurences, overlapping
{
    int const buffer_size = 12;
    char str[buffer_size] = { 'a', 'b', 'c', 'z', 'z', 'z', 'b', 'c' };
    char find[] = { 'z', 'z' };
    char rplc[] = { '1' };

    print_result(replace(find, 2, rplc, 1, str, 8, buffer_size),
        str
    );
}

////////////////////////////////////////////////////////////////////////////////
void replace_test9() // string shifted right, one occurences
{
    int const buffer_size = 12;
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[] = { 'c', 'd' };
    char rplc[] = { '1', '2', '3', '4' };

    print_result(replace(find, 2, rplc, 4, str, 8, buffer_size),
        str
    );
}

void replace_test10() // string shifted right, several occurences
{
    int const buffer_size = 20;
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c', 'd' };
    char find[] = { 'a', 'b', 'c' };
    char rplc[] = { '1', '2', '3', '4', '5' };

    print_result(replace(find, 3, rplc, 5, str, 9, buffer_size),
        str
    );
}

void replace_test11() // string shifted right, several occurences, one at the very end
{
    int const buffer_size = 20;
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[] = { 'a', 'b', 'c' };
    char rplc[] = { '1', '2', '3', '4', '5' };

    print_result(replace(find, 3, rplc, 5, str, 8, buffer_size),
        str
    );
}

void replace_test12() // string shifted right, one at the very end, overlapping
{
    int const buffer_size = 20;
    char str[buffer_size] = { 'a', 'b', 'c', 'z', 'z', 'z', 'z', 'z' };
    char find[] = { 'z', 'z', 'z' };
    char rplc[] = { '1', '2', '3', '4', '5' };

    print_result(replace(find, 3, rplc, 5, str, 8, buffer_size),
        str
    );
}


////////////////////////////////////////////////////////////////////////////////
void replace_test13() // string shifted right, not enough space
{
    int const buffer_size = 9; // <-- only 1 extra character possible
    char str[buffer_size] = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[] = { 'c', 'd' };
    char rplc[] = { '1', '2', '3', '4' };

    print_result(replace(find, 2, rplc, 4, str, 8, buffer_size),
        str
    );
}

#include <random>
#include <regex>
#include <string>
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////
void replace_test_stress_correctness(
    int str_size_min, int str_size_max,
    int find_size_min, int find_size_max,
    int repl_size_min, int repl_size_max,
    int number_distinct_chars,   // which chars to use in randomely generated strings ( 1 - to use only 'a')
    bool print_a_lot
)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis_size(str_size_min, str_size_max); // str size
    std::uniform_int_distribution<> dis_char('a', 'a' + number_distinct_chars - 1); // random char a...f

    // random str
    int str1_size = dis_size(gen);
    std::string str1(str1_size, 'a'); // many 'a's
    std::generate(str1.begin(), str1.end(), [&dis_char, &gen]() { return dis_char(gen); }); // random chars
    if (print_a_lot) {
        std::cout << "String -->" << str1 << "<--\n";
    }

    // random find: choose a random substring in str
    std::uniform_int_distribution<> dis_substring_start(0, str1_size / 2); // stating here
    std::uniform_int_distribution<> dis_substring_size(find_size_min, find_size_max); // of this length
    int find_size = dis_substring_size(gen);
    std::string find = str1.substr(dis_substring_start(gen), find_size);
    if (print_a_lot) {
        std::cout << "Find -->" << find << "<--\n";
    }

    // random replace
    std::uniform_int_distribution<> dis_repl_size(repl_size_min, repl_size_max); // replace substring size
    int repl_size = dis_repl_size(gen);
    std::string repl(repl_size, '1'); // many '1's - makes it easier to see
    //std::generate( repl.begin(), repl.end(), [&dis_char, &gen]() { return dis_char( gen );} ); // random chars
    if (print_a_lot) {
        std::cout << "Replace -->" << repl << "<--\n";
    }

    // prepare regular expression
    std::regex find_re(find);

    // replace using STL 
    std::string str2 = std::regex_replace(str1, find_re, repl);

    // prepare for our replace
    int buffer_size = (int)str2.size();
    if ((int)str1.size() > buffer_size) { buffer_size = (int)str1.size(); }
    char* str = new char[buffer_size]; // enough to hold all chars after replace
    std::copy(str1.begin(), str1.end(), str); // copy initial string, no new line

    // call 
    std::tuple<bool, int> res = replace(
        find.c_str(), find_size,
        repl.c_str(), repl_size,
        str, (int)str1.size(), buffer_size);

    if (print_a_lot) {
        std::cout << "Expected -->" << str2 << "<--\n";
        std::cout << "Result   -->";
        for (int i = 0; i < std::get<1>(res); ++i) {
            std::cout << str[i];
        }
        std::cout << "<--\n";
        std::cout << "Errors   -->";
    }
    // compare results
    if (std::get<0>(res)) {
        if (print_a_lot) {
            int m = std::min<int>(std::get<1>(res), (int)str2.size());
            for (int i = 0; i < m; ++i) {
                if (str2[i] != str[i]) { std::cout << "^"; }
                else { std::cout << " "; }
            }
            std::cout << "\n";
        }
        else { // brief
            if (std::get<1>(res) == (int)str2.size()) { // check size
                bool all_good = true;
                for (int i = 0; i < std::get<1>(res) and all_good; ++i) {
                    all_good = (str2[i] == str[i]);
                }
                if (all_good) {
                    std::cout << "Looks good\n";
                }
                else {

                    if (repl.size() == find.size()) std::cout << "No Shift: ";
                    else if (repl.size() > find.size()) std::cout << "Right Shift: ";
                    else std::cout << "Left Shift: ";

                    std::cout << "Size is correct, some errors\n";
                }
            }
            else {

                if (repl.size() == find.size()) std::cout << "No Shift: ";
                else if (repl.size() > find.size()) std::cout << "Right Shift: ";
                else std::cout << "Left Shift: ";
                std::cout << "Incorrect size after replace\n";
            }
        }
    }
    else { // if ( std::get<0>( res ) ) 
        if (repl.size() == find.size()) std::cout << "No Shift: ";
        else if (repl.size() > find.size()) std::cout << "Right Shift: ";
        else std::cout << "Left Shift: ";
        std::cout << "Not enough space\n";
    }

    delete[] str;
}

// random, small size - debugging
void replace_test14()
{
    replace_test_stress_correctness(10, 12, 2, 4, 1, 5, 6, true);
}


// random, medium size
void replace_test15()
{
    replace_test_stress_correctness(8000, 12000, 3, 5, 2, 6, 26, false);
}

// random, medium size, only few chars
// overlapping matches are possible:
// match "aaa" in "aaaa": correct is "AAAa"
void replace_test16()
{
    replace_test_stress_correctness(8000, 12000, 3, 5, 2, 6, 2, false);
}


void replace_test17() // random many times
{
    for (int i = 0; i < 500; ++i) {
        //replace_test15();
        replace_test16();
    }
}

////////////////////////////////////////////////////////////////////////////////
// this is NOT a random replace_test:
// large text with many substitutions
#include <cassert>
void stress_replace_test_runtime(int find_size, int repl_size, int num_replaces)
{ // abcabcabc - replace abc with 12345

    assert(find_size < 27 and repl_size < 27);

    int buffer_size = num_replaces * std::max(find_size, repl_size);

    // generate find substring
    char* find = new char[find_size];
    for (int j = 0; j < find_size; ++j) {
        find[j] = (char)('a' + j); // abcdef...
    }

    // generate text
    char* str = new char[buffer_size]; // enough to hold all chars after replace
    for (int i = 0; i < num_replaces; ++i) {
        std::memcpy(str + i * find_size, find, find_size);
    }

    // generate repl substring
    char* repl = new char[repl_size];
    for (int j = 0; j < repl_size; ++j) {
        repl[j] = (char)('z' - j); // zyxwv....
    }

    // call 
    std::tuple<bool, int> res = replace(
        find, find_size,
        repl, repl_size,
        str, find_size * num_replaces, buffer_size);

    // check results
    if (std::get<0>(res)) {
        bool OK = true;
        if (std::get<1>(res) == repl_size * num_replaces) { // check final size
            for (int i = 0; i < std::get<1>(res); ++i) {
                if (str[i] != repl[i % repl_size]) {
                    std::cout << "error an index " << i << std::endl;
                    OK = false;
                }
            }
            if (OK) {
                std::cout << "Looks good\n";
            }
        }
        else {
            std::cout << "Incorrect size after replace\n";
        }
    }
    else {
        std::cout << "Not enough space\n";
    }

    delete[] str;
    delete[] find;
    delete[] repl;
}

void replace_test18()
{
    stress_replace_test_runtime(2, 20, 1 << 15); // 2^15 - 0.85s with BF
}

void replace_test19()
{
    stress_replace_test_runtime(20, 2, 1 << 15); // 2^15 - 5.5s with BF
}

void replace(void)
{
    replace_test0();
    replace_test1();
    replace_test2();
    replace_test3();
    replace_test4();
    replace_test5();
    replace_test6();
    replace_test7();
    replace_test8();
    replace_test9();
    replace_test10();
    replace_test11();
    replace_test12();
    replace_test13();
    replace_test14();
    replace_test15();
    replace_test16();
    replace_test17();
    replace_test18();
    replace_test19();
}