/*!****************************************************************************
\file    table.h
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Assignment #4
\date    12-03-2021

\brief
  This program contains functions pertaining to the creation, maintenance, and
  manipulation of 2D string arrays

  - Table
      Default constructor for the Table class

  - Table
      Non-default constructor for the Table class

  - ~Table
      Default destructor for the Table class

  - Table
      Copy constructor for the Table class

  - operator=
      Assignment operator for the Table class

  - Display
      Displays the class Table object without any special formatting

  - Display2
      Displays the class Table object within a formatted table

  - operator()
       Settor function that pulls the address for the specified index from the
       class Table object

  - operator()
       Gettor function that gets the value at the specified index from the
       class Table object

  - Reverse
       Reverses the string at the given index

  - Revese
      Reverses the indexes within the row at the given index

  - Reverse
      Reverses the columns and rows of the class Table object (transpose)

  - operator+
      Overlaps the given table with the class Table object. The places where
      the two tables overlap, the values are "multiplied" (appended). Where
      there is no overlap but still relevant values, the relevant values are
      placed in the needed positions. Where there are no values, a space is
      places

  - operator*
      "Multiples" (appends) the contents of the given table to each index
      within the class Table object

  - operator<<
      Calls the "fancy" display function when the user tries to print out the
      given the << operator
******************************************************************************/

#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <fstream>
#include <iostream>

namespace CS170 {
    class Table {
    public:
      /*!**********************************************************************
      \brief
        Default constructor for the Table class
        (sets all relevant variables to 0)
      ************************************************************************/
      Table();

      /*!**********************************************************************
      \brief
      Non-default constructor for the Table class
      (sets all relevant variables to given values)

      \param numRows
      The number of rows within the new Table

      \param numCols
      The number of columns within the new Table
      ************************************************************************/
      Table(int numRows, int numCols);

      /*!**********************************************************************
      \brief
      Default destructor for the Table class
      (deletes all dynamically allocated associated with the class)
      ************************************************************************/
      ~Table();

      /*!**********************************************************************
      \brief
      Copy constructor for the Table class

      \param table
      The Table class that is being (deep) copied
      ************************************************************************/
      Table(const Table& table);

      /*!**********************************************************************
      \brief
      Assignment operator for the Table class

      \param rhs
      The Table that the new table is being set equal too

      \return
      A new Table object holding the values from the given Table object
      ************************************************************************/
      Table& operator=(const Table& rhs);

      /*!**********************************************************************
      \brief
      Displays the class Table object without any special formatting

      \param os
      The output stream where the class Table object is being printed to

      \return
      An output stream containing the printed out Table object
      ************************************************************************/
      void Display( std::ostream & os ) const;    // provided

      /*!**********************************************************************
      \brief
      Displays the class Table object within a formatted table

      \param os
      The output stream where the class Table object is being printed to

      \return
      An output stream containing the printed out Table object
      ************************************************************************/
      void Display2( std::ostream & os ) const;   // fancy  (10% of your grade)

      /*!**********************************************************************
      \brief
      Settor function that pulls the address for the specified index from the
      class Table object

      \param r
      The row of the specified index

      \param c
      The column of the specified index

      \return
      The address to the specified of the index in the class Table object
      ************************************************************************/
      std::string& operator () (unsigned int r, unsigned int c);

      /*!**********************************************************************
      \brief
      Gettor function that gets the value at the specified index from the
      class Table object

      \param r
      The row of the specified index

      \param c
      The column of the specified index

      \return
      The value to the specified of the index in the class Table object
      ************************************************************************/
      std::string const& operator () (unsigned int r, unsigned int c) const;

      /*!**********************************************************************
      \brief
      Reverses the string at the given index

      \param r
      The row of the specified index

      \param c
      The column of the specified index
      ************************************************************************/
      void Reverse(unsigned int r, unsigned int c);
      
      /*!**********************************************************************
      \brief
      Reverses the indexes within the row at the given index

      \param r
      The given index for the row that is going to be reverse
      ************************************************************************/
      void Reverse(unsigned int r);

      /*!**********************************************************************
      \brief
      Reverses the columns and rows of the class Table object (transpose)
      ************************************************************************/
      void Reverse();

      /*!**********************************************************************
      \brief
      Overlaps the given table with the class Table object. The places where the
      two tables overlap, the values are "multiplied" (appended). Where there is
      no overlap but still relevant values, the relevant values are placed in the
      needed positions. Where there are no values, a space is places

      \param right
      The table that is being laid on top of the class Table object

      \return
      The new joint table between to class Table and the given table
      ************************************************************************/
      Table operator +(const Table& right) const;


      /*!**********************************************************************
      \brief
      "Multiples" (appends) the contents of the given table to each index within
      the class Table object

      \param right
      The given table whose indexes will be "multipled" (appended) to the class
      Table object
      ************************************************************************/
      Table operator *(const Table& right) const;

    private:
      /*!**********************************************************************
      \brief
        How many rows the class Table object has
      ************************************************************************/
      unsigned int numRows_;

      /*!**********************************************************************
      \brief
        How many columns the class Table object has
      ************************************************************************/
      unsigned int numCols_;

      /*!**********************************************************************
      \brief
        Pointer to 2D array of strings
      ************************************************************************/
      std::string** ppData;
    };

    /*!************************************************************************
    \brief
     Calls the "fancy" display function when the user tries to print out the
     given the << operator

    \param os
     The output stream where the given Table will be printed out to

    \ t
     The table that is being printed out to the output stream

    \return
     The output stream where the given table has been printed out to
    **************************************************************************/
    std::ostream & operator<<( std::ostream & os, Table const& t );
}
#endif
