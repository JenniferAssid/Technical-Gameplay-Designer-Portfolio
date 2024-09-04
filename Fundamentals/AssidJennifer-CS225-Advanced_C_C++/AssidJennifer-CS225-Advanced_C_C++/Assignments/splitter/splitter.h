#pragma once
/*****************************************************************
 * \file   splitter.h
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

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> /* size_t definition */

    /* Enum containing all of the error codes */
    enum
    {
        E_BAD_SOURCE = 1, E_BAD_DESTINATION, E_NO_MEMORY, E_NO_ACTION, E_SMALL_SIZE
    };

    void splitter_test0(void);
    void splitter_test1(void);
    void splitter_test2(void);
    void splitter_test3(void);

#ifdef __cplusplus
}
#endif
