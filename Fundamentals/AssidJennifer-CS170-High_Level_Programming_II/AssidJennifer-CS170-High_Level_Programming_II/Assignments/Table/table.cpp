/*!****************************************************************************
\file    table.cpp
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

#include "table.h"
#include <iomanip>

namespace CS170 {

  /*!**************************************************************************
  \brief
    Default constructor for the Table class
    (sets all relevant variables to 0)
  ****************************************************************************/
  Table::Table()
  {
    // Sets the rows and columns to 0
    numRows_ = numCols_ = 0;

    // Sets the pointer to the 2D array to NULL
    ppData = NULL;
  }

  /*!**************************************************************************
  \brief
    Non-default constructor for the Table class
    (sets all relevant variables to given values)

  \param numRows
    The number of rows within the new Table

  \param numCols
    The number of columns within the new Table
  ****************************************************************************/
  Table::Table(int numRows, int numCols)
  {
    // Sets the rows and cols to given values
    numRows_ = numRows;
    numCols_ = numCols;

    // Assigns an array of row string pointers to the 2D array pointer
    ppData = new std::string * [numRows_];
    
    // Assigns string col array of string pointers to each of the row pointers
    for (unsigned int r = 0; r < numRows_; ++r)
    {
      ppData[r] = new std::string[numCols_];
    }

    // Initalizes all of the indexes within the 2D array to a set amaount
    for (unsigned int r = 0; r < numRows_; ++r)
    {
      for (unsigned int c = 0; c < numCols_; ++c)
        ppData[r][c] = (char)(r + c);
    }
  }

  /*!**************************************************************************
  \brief
    Default destructor for the Table class
    (deletes all dynamically allocated associated with the class)
  ****************************************************************************/
  Table::~Table()
  {
    // Iterates through the 2D array and frees associated allocated memory
    for(unsigned int r = 0; r < numRows_; ++r)
      delete[] ppData[r];

    if (ppData)
      delete[] ppData;
  }

  /*!**************************************************************************
  \brief
    Copy constructor for the Table class
  
  \param table
    The Table class that is being (deep) copied
  ****************************************************************************/
  Table::Table(const Table& table)
  {
    // Sets the class Table's objects to the given table's values
    numCols_ = table.numCols_;
    numRows_ = table.numRows_;

    // Creates a new 2D array to serve as the deep copy
    ppData = new std::string * [numRows_];
    for (unsigned int r = 0; r < numRows_; ++r)
    {
      ppData[r] = new std::string[numCols_];
    }

    // Sets the values in the new 2D array to the values in the given table
    for (unsigned int r = 0; r < numRows_; ++r)
    {
      for (unsigned int c = 0; c < numCols_; ++c)
        ppData[r][c] = table.ppData[r][c];
    }
  }

  /*!**************************************************************************
  \brief
    Assignment operator for the Table class
  
  \param rhs
    The Table that the new table is being set equal too

  \return
    A new Table object holding the values from the given Table object
  ****************************************************************************/
  Table& Table::operator=(const Table& rhs)
  {
    // Checks if the given table is the class table
    if (this == &rhs) return *this;

    // Calls the destructor to prevent memory leaks
    this->~Table();

    // Performs a deep copy of the given table
    new (this) Table(rhs);

    // Returns the new Table object
    return *this;
  }

  /*!**************************************************************************
  \brief
    Displays the class Table object without any special formatting

  \param os
    The output stream where the class Table object is being printed to

  \return
    An output stream containing the printed out Table object
  ****************************************************************************/
  void Table::Display( std::ostream & os ) const
  {
    // Iterates over the Table and prints out the values
      for( unsigned int r=0; r<numRows_; ++r ) {
          for( unsigned int c=0; c<numCols_; ++c ) {
              os << ppData[r][c] << " ";
          }
          os << std::endl;
      }
  }
   
    /*!**************************************************************************
  \brief
    Displays the class Table object within a formatted table

  \param os
    The output stream where the class Table object is being printed to

  \return
    An output stream containing the printed out Table object
  ****************************************************************************/                  
  void Table::Display2( std::ostream & os ) const
  {
    // An array to hold the max size for each column
    int* sizes = new int [numCols_] ();

    // Iterates over the class Table object
    for (unsigned int c = 0; c < numCols_; ++c)
    {
      for (unsigned int r = 0; r < numRows_; ++r)
      {
        // Determines if the current index's length is longer than recorded
        std::string temp = ppData[r][c];

        // Sets the size array as necessary (this is to determine formatting)
        if ((int)temp.length() > sizes[c])
          sizes[c] = (int)temp.length();
      }
    }
    
    // Iterates over the 2D array to print out the top line for the table
    for (unsigned int r = 0; r < numRows_; ++r)
    {
      for (unsigned int i = 0; i < numCols_; ++i)
      {
        os << "+-";
        os << std::setfill('-');
        os << std::setw(sizes[i]); // Determines the size based off prev calc
        os << "-";
        os << "-";
      }

      // Ends the top border and starts next line
      os << "+" << std::endl;

      // Interates over the 2D array and prints out the values stored
      for (unsigned int i = 0; i < numCols_; ++i)
      {
        os << "| ";
        os << std::setw(sizes[i]);
        os << std::setfill(' ');
        os << ppData[r][i] << " ";
      }

      // Ends the line and moves onto next line
      os << "|" << std::endl;
    }

    // Formats the bottom border for the chart
    for (unsigned int i = 0; i < numCols_; ++i)
    {
      os << "+-";
      os << std::setfill('-');
      os << std::setw(sizes[i]);
      os << "-";
      os << "-";
    }

    // Ends the formatting for the chart
    os << "+" << std::endl;

    // Deletes the allocated memorys associated with the sizes array
    // (Prevents memory leaks)
    delete[] sizes;
  }

  /*!**************************************************************************
  \brief
    Settor function that pulls the address for the specified index from the
    class Table object

  \param r
    The row of the specified index

  \param c
    The column of the specified index

  \return
    The address to the specified of the index in the class Table object
  ****************************************************************************/                     
  std::string& Table::operator () (unsigned int r, unsigned int c)
  {
    // Gets the address of the specified index and returns it
    std::string* result = &ppData[r][c];
    return *result;
  }

  /*!**************************************************************************
  \brief
    Gettor function that gets the value at the specified index from the
    class Table object
 
  \param r 
    The row of the specified index

  \param c
    The column of the specified index

  \return
    The value to the specified of the index in the class Table object
  ****************************************************************************/
  std::string const& Table::operator () (unsigned int r, unsigned int c) const
  {
    // Returns the value at the specified index
    return ppData[r][c];
  }

  /*!**************************************************************************
  \brief
    Reverses the string at the given index

  \param r
    The row of the specified index

  \param c
    The column of the specified index    
  ****************************************************************************/
  void Table::Reverse(unsigned int r, unsigned int c)
  {
    // Determines the length of the string at the given index
    std::string temp = ppData[r][c];
    int len = (int)temp.length();

    // Iterates over half of the length to prevent overwriting values
    for (unsigned int i = 0; i < (unsigned int)(len / 2); i++)
      // Swaps the value on the left for the value on the right
      std::swap(temp[i], temp[len - i - 1]);

    // Sets the index to the newly reverse string
    ppData[r][c] = temp;
  }

  /*!**************************************************************************
  \brief
    Reverses the indexes within the row at the given index

  \param r
    The given index for the row that is going to be reverse
  ****************************************************************************/  
  void Table::Reverse(unsigned int r)
  {
    // Determines the starting and ending index of the row
    int start = 0;
    int end = numCols_ - 1;

    // While the index counters converge on the middle
    while (start < end)
    {
      // Swaps the values at the specified indexes
      swap(ppData[r][start], ppData[r][end]);

      // Modifies the values of the index counters
      start++;
      end--;
    }
  }

  /*!**************************************************************************
  \brief
    Reverses the columns and rows of the class Table object (transpose)
  ****************************************************************************/
  void Table::Reverse()
  {
    // Creates a new  local 2D array
    std::string **ppLocal = new std::string * [numCols_];
    for (unsigned int r = 0; r < numCols_; ++r)
    {
      ppLocal[r] = new std::string[numRows_];
    }

    // Sets the local array values to be the transpose of the class Table
    for (unsigned int r = 0; r < numRows_; ++r)
    {
      for (unsigned int c = 0; c < numCols_; ++c)
        ppLocal[c][r] = ppData[r][c];
    }

    // Deletes the existing 2D array for the class Table
    for (unsigned int r = 0; r < numRows_; ++r)
      delete[] ppData[r];

    if (ppData)
      delete[] ppData;

    // Assigns the class Table object variables are necessary
    ppData = ppLocal;
    int temp = numRows_;
    numRows_ = numCols_;
    numCols_ = temp;
  }

  /*!**************************************************************************
  \brief
    Overlaps the given table with the class Table object. The places where the
    two tables overlap, the values are "multiplied" (appended). Where there is 
    no overlap but still relevant values, the relevant values are placed in the
    needed positions. Where there are no values, a space is places

  \param right
    The table that is being laid on top of the class Table object

  \return
    The new joint table between to class Table and the given table
  ****************************************************************************/
  Table Table::operator+(const Table& right) const
  {
    // Variables to hold calculations
    unsigned int numCols, numRows;   // The dimensions of the new table
    unsigned int rowLower, colLower; // Index where the tables don't overlap

    // Determine the dimensions of the larger table
    // Determines how the two tables overlap over each other
    if (numCols_ >= right.numCols_)
    {
      numCols = numCols_;
      colLower = right.numCols_;
    }
    else
    {
      numCols = right.numCols_;
      colLower = numCols_;
    }
        
    if (numRows_ >= right.numRows_)
    {
      numRows = numRows_;
      rowLower = right.numRows_;
    }
    else
    {
      numRows = right.numRows_;
      rowLower = numRows_;
    }

    // Create the new table based off of the specified dimensions
    Table result(numRows, numCols);
    Table current = *this;


    // For the sections of the graphcs that overlap
    for (unsigned int r = 0; r < numRows_ && r < right.numRows_; ++r)
    {
      for (unsigned int c = 0; c < numCols_ && c < right.numCols_; ++c)
      {
        // Append the two strings together into the result
        current(r, c).append(right(r, c));
        result.ppData[r][c] = current(r, c);
      }
    }

    if (result.numCols_ != colLower)
    {
      // For the upper right-hand hang over
      for (unsigned int r = 0; r < rowLower; ++r)
      {
        for (unsigned int c = colLower; c < numCols; ++c)
        {
          // Determines which table's values are going to be placed
          if (numCols_ <= colLower)
            result.ppData[r][c] = right.ppData[r][c];
          else
            result.ppData[r][c] = ppData[r][c];
        }
      }
    }

    if (result.numRows_ != rowLower)
    {
      // For the lower left-hand hang ouver
      for (unsigned int r = rowLower; r < numRows; ++r)
      {
        for (unsigned int c = 0; c < colLower; ++c)
        {
          // Determines which table's values are going to be placed
          if (numRows_ > rowLower)
            result.ppData[r][c] = ppData[r][c];
          else
            result.ppData[r][c] = right.ppData[r][c];
        }
      }
    }

    if (result.numCols_ != colLower && result.numRows_ != rowLower)
    {
      // For the lower right-hand "empty" space
      for (unsigned int r = rowLower; r < numRows; ++r)
      {
        for (unsigned int c = colLower; c < numCols; ++c)
          // Places empty space to notate no overlap / value at the position
          result.ppData[r][c] = "";
      }
    }

    // Returns the new, larger table
    return result;
  }

  /*!**************************************************************************
  \brief
    "Multiples" (appends) the contents of the given table to each index within
    the class Table object
  
  \param right
    The given table whose indexes will be "multipled" (appended) to the class
    Table object
  ****************************************************************************/
  Table Table::operator*(const Table& right) const
  {
    // Calculates the new dimensions of the table
    unsigned int numRows = numRows_ * right.numRows_;
    unsigned int numCols = numCols_ * right.numCols_;

    // Creates the new table and gets variable for class Table
    Table result(numRows, numCols);
    Table current = *this;

    // Establish index counters
    unsigned int i, j;
    i = j = 0;


    // Iterates through the result Table
    for (unsigned int r = 0; r < result.numRows_; r++)
    {
      if (r % right.numRows_ == 0 && r != 0 && j + 1 < numRows_)
        j++;

      i = 0;
      for (unsigned int c = 0; c < result.numCols_; c++)
      {
        // If the secondary table has been iterated over, increment i
        if (c % right.numCols_ == 0 && c != 0 && i + 1 < numCols_)
          i++;

        // Get the string at the given index within the larger table
        std::string resStr = result.ppData[r][c];
        
        // Set the current index to equal to corresponding index value
        //  within the class Table object
        resStr = current(j, i);

        // Appnds the corresponding index value in the secondary table
        resStr.append(right(r - j * right.numRows_, c - i * right.numCols_));

        // Assigns the specified index to equal the calculated string
        result.ppData[r][c] = resStr;
      }
    }

    // Return the new, larger table
    return result;
  }

  /*!**************************************************************************
  \brief
    Calls the "fancy" display function when the user tries to print out the
    given the << operator

  \param os
    The output stream where the given Table will be printed out to

  \ t
    The table that is being printed out to the output stream

  \return
    The output stream where the given table has been printed out to
  ****************************************************************************/
  std::ostream & operator<<(std::ostream & os, Table const & t)
  {
    // Call the "fancy" display to get the formatted table of the give table
      t.Display2( os );   // fancy (see fancy_out* )
      //t.Display( os );   // simple (see out* )

      return os;
  }
}
