/*!****************************************************************************
\file    Ocean.cpp
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Assignment #1
\date    26-01-2021

\brief
  This program holds functions necessary to establish, update, and track the
  playing of "Battleship"

  The functions include:

  - CreateOcean
      Dynamically creates the Ocean and all the necessary componenets as well
      as initializes all of the elements needed

  - DestroyOcean
      Frees all dynamic memory allocated in the program

  - PlaceBoat
      Places a boat on the ocean from the given information once determining
        if the boat can fit on the ocean without conflict

  - TakeShot
      Determines the outcome of the "shot" at the given location

  - GetShotStats
      Updates the current shot statistics when called upon

  - DumpOcean
      Prints the grid (ocean) to the screen.

  Hours spent on this assignment: ~6

  Specific portions that gave you the most trouble: 
    Allocating / Deallocating memory
    Understanding the levels of indirection between pointers and using
      dynamically allocated arrays via pointers within a stucture
******************************************************************************/

#include "WarBoats.h"
#include "Ocean.h"
#include <iostream> // cout, endl
#include <iomanip>  // setw

namespace CS170
{
  namespace WarBoats
  {
    const int BOAT_LENGTH = 4;  //!< Length of a boat
    const int HIT_OFFSET = 100; //!< Add this to the boat ID

    /*************************************************************************/
    /*!
      \brief
        Dynamically creates the Ocean and all the necessary componenets as well
        as initializes all of the elements needed

      \param num_boats
        The number of boats that will be added to the border

      \param x_quadrant
        How wide the new Ocean must be

      \param y_quadrants
        How tall the new Ocean must be

      \return
        Pointer to the new Ocean
    */
    /*************************************************************************/
    Ocean* CreateOcean(int num_boats, int x_quadrants, int y_quadrants)
    {
      // Dynamically creates an ocean dynamically
      Ocean* ocean = new Ocean;

      // Dynamically creates a 2D array for the grid
      ocean->grid = new int[x_quadrants * y_quadrants]();

      // Dynamically creates the array for the boats
      ocean->boats = new Boat[num_boats];

      // Initializing all of the fields in struct Boats
      for (int i = 0; i < num_boats; i++)
      {
        ocean->boats[i].hits = 0;
        ocean->boats[i].ID = i;
        ocean->boats[i].orientation = oHORIZONTAL;
        ocean->boats[i].position.x = 0;
        ocean->boats[i].position.y = 0;
      }
      
      // Initializing all fields in struct Ocean
      ocean->num_boats = num_boats;
      ocean->x_quadrants = x_quadrants;
      ocean->y_quadrants = y_quadrants;

      // Initializing all fields in struct ShotStats
      ocean->stats.hits = 0;
      ocean->stats.misses = 0;
      ocean->stats.duplicates = 0;
      ocean->stats.sunk = 0;

      // Return a pointer to the ocean
      return ocean;
    }

    /*************************************************************************/
    /*!
      \brief
        Frees all dynamic memory allocated in the program

      \param theOcean
        Pointer to the ocean that must be freed
    */
    /*************************************************************************/
    void DestroyOcean(Ocean* theOcean)
    {
      // Frees the grid
      delete[] theOcean->grid;

      // Frees the boats
      delete[] theOcean->boats;
      
      // Frees the Ocean overall
      delete theOcean;
    }

    /*************************************************************************/
    /*!
      \brief
        Places a boat on the ocean from the given information once determining
        if the boat can fit on the ocean without conflict

      \param ocean
        The Ocean that the boat is to be placed on

      \param boat
        The Boat that is the be placed on the Ocean

      \return
        Whether the boat placement was been acccepted (bpACCEPTED) or
        rejected (bpREJECTED)
    */
    /*************************************************************************/
    BoatPlacement PlaceBoat(Ocean& ocean, const Boat& boat)
    {
      int i;     // loop counter
      int index; // variable to hold the location of given index

      // Checks if the given position is outside of the board
      if (boat.position.x > (ocean.x_quadrants - 1) ||
        boat.position.y > (ocean.y_quadrants - 1))
      {
        // Returns REJECTED for illegal coordinates
        return bpREJECTED;
      }

      // If the orientation of the boat is horizontal...
      if (boat.orientation == oHORIZONTAL)
      {
        // Checks if the boat could go over the edge of the board
        if (boat.position.x + (BOAT_LENGTH - 1) >
          (ocean.x_quadrants - 1))
        {
          // Returns REJECTED for the boat not fitting on the board
          return bpREJECTED;
        }

        // Iterates across the grid to see if the spots are open
        for (i = 0; i < BOAT_LENGTH; i++)
        {
          index = boat.position.y * ocean.x_quadrants +
            (boat.position.x + i);

          // Checks if the current index is empty
          if (*(ocean.grid + index) != dtOK)
          {
            // Returns REJECTED is space is filled
            return bpREJECTED;
          }
        }

        // Iterates over the grid to place the boat ID
        for (i = 0; i < BOAT_LENGTH; i++)
        {
          index = boat.position.y * ocean.x_quadrants +
            (boat.position.x + i);

          // Assigns current index boat's ID
          ocean.grid[index] = boat.ID;
        }
      }
      // If the orientation of the boat is vertical...
      else
      {
        // Checks if the boat could go over the edge of the board
        if (boat.position.y + (BOAT_LENGTH - 1) >
          (ocean.y_quadrants - 1))
        {
          // Returns REJECTED for the boat not fitting on the board
          return bpREJECTED;
        }

        // Iterates across the grid to see if the spots are open
        for (i = 0; i < BOAT_LENGTH; i++)
        {
          index = (boat.position.y + i) * ocean.x_quadrants +
            boat.position.x;

          // Checks if the current index is empty
          if (*(ocean.grid + index) != dtOK)
          {
            // Returns REJECTED is space is filled
            return bpREJECTED;
          }
        }

        // Iterates over the grid to place the boat ID
        for (i = 0; i < BOAT_LENGTH; i++)
        {
          index = (boat.position.y + i) * ocean.x_quadrants +
            boat.position.x;

          // Assigns current index boat's ID
          *(ocean.grid + index) = boat.ID;
        }
      }

      // Returns ACCEPTED if function doesn't return beforehand
      return bpACCEPTED;
    }

    /*************************************************************************/
    /*!
      \brief
        Determines the outcome of the "shot" at the given location

      \param ocean
        The Ocean where the shot will land

      \param coordinate
        Location where the shot is to go

      \return
        Whether the shot was illegally placed (srILLEGAL), missed (srMISS),
        had already been attempted (srDUPLICATE), hit a boat (srHIT), or sunk 
        a boat (srSUNK)
    */
    /*************************************************************************/
    ShotResult TakeShot(Ocean& ocean, const Point& coordinate)
    {
      int index; // variable to hold the location of the given index
      int value; // variable to hold the value at the given index

      // Checks if the coordinates are off of the board
      if (coordinate.x > (ocean.x_quadrants - 1) ||
        coordinate.y > (ocean.y_quadrants - 1))
      {
        // Returns ILLEGAL due to illegal coordinates
        return srILLEGAL;
      }

      // Sets index variable to the location of the given index
      index = coordinate.y * ocean.x_quadrants + coordinate.x;

      // Sets value variable to the value at the given index
      value = ocean.grid[index];

      // If the given index is empty...
      if (value == dtOK)
      {
        // Increment misses in shot statistics
        ocean.stats.misses++;

        // Change the given index to reflect attempt
        ocean.grid[index] = dtBLOWNUP;

        // Return MISS
        return srMISS;
      }

      // If given index has already been targeted...
      if (value == dtBLOWNUP || value > HIT_OFFSET)
      {
        // Increment duplicates in the shot statistics
        ocean.stats.duplicates++;

        // Return DUPLICATE
        return srDUPLICATE;
      }

      // Increment hits on the boat at the given index
      ocean.boats[value - 1].hits++;

      // Increment hits in the shot statistics
      ocean.stats.hits++;

      // Change the given index to reflect hit
      ocean.grid[index] = value + HIT_OFFSET;

      // If all locations on the boat has been hit...
      if (ocean.boats[value - 1].hits == BOAT_LENGTH)
      {
        // Increment sunk in the shot statistics
        ocean.stats.sunk++;

        // Return SUNK
        return srSUNK;
      }

      // Return HIT as default
      return srHIT;
    }

    /*************************************************************************/
    /*!
      \brief
        Updates the current shot statistics when called upon

      \param ocean
        The Ocean where the current shot stastics are stored

      \return
        A ShotStats struct holding the current shot statstics
    */
    /*************************************************************************/
    ShotStats GetShotStats(const Ocean& ocean)
    {
      ShotStats current = ocean.stats;

      return current;
    }

    /*************************************************************************/
    /*!
      \brief
        Prints the grid (ocean) to the screen.

      \param ocean
        The Ocean to print

      \param field_width
        How much space each position takes when printed.

      \param extraline
        If true, an extra line is printed after each row. If false, no extra
        line is printed.

      \param showboats
        If true, the boats are shown in the output. (Debugging feature)
    */
    /*************************************************************************/
    void DumpOcean(const CS170::WarBoats::Ocean& ocean,
      int field_width,
      bool extraline,
      bool showboats)
    {
      // For each row
      for (int y = 0; y < ocean.y_quadrants; y++)
      {
        // For each column
        for (int x = 0; x < ocean.x_quadrants; x++)
        {
          // Get value at x/y position
          int value = ocean.grid[y * ocean.x_quadrants + x];

          // Is it a boat that we need to keep hidden?
          if ((value > 0) && (value < HIT_OFFSET) && (showboats == false))
            value = 0;

          std::cout << std::setw(field_width) << value;
        }
        std::cout << std::endl;
        if (extraline)
          std::cout << std::endl;
      }
    }

  } // namespace WarBoats
} // namespace CS170


