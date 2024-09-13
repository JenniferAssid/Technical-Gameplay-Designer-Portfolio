/*!****************************************************************************
\file    OceanOO.cpp
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Assignment #2
\date    

\brief
  

  The functions include:

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:
******************************************************************************/

#include "WarBoatsOO.h"
#include "OceanOO.h"
namespace CS170
{
  namespace WarBoatsOO
  {
    OceanOO::OceanOO(int num_boats, int x_quadrants, int y_quadrants)
    {
      // Dynamically creates a 2D array for the grid
      OceanOO::grid_ = new int[x_quadrants * y_quadrants]();

      // Dynamically creates the array for the boats
      OceanOO::boats_ = new Boat[num_boats]();

      // Initializing all of the fields in struct Boats
      for (int i = 0; i < num_boats; i++)
      {
        OceanOO::boats_[i].ID = i;
      }

      // Initializing all fields in struct OceanOO
      OceanOO::num_boats_ = num_boats;
      OceanOO::x_quadrants_ = x_quadrants;
      OceanOO::y_quadrants_ = y_quadrants;

      // Initializing all fields in struct ShotStats
      OceanOO::stats_.hits = 0;
      OceanOO::stats_.misses = 0;
      OceanOO::stats_.duplicates = 0;
      OceanOO::stats_.sunk = 0;
    }
    
    OceanOO::~OceanOO(void)
    {
      // Frees the grid
      delete[] OceanOO::grid_;

      // Frees the boats
      delete[] OceanOO::boats_;
    }

    /*************************************************************************/
    /*!
      \brief
        Places a boat on the OceanOO from the given information once determining
        if the boat can fit on the OceanOO without conflict

      \param boat
        The Boat that is the be placed on the OceanOO

      \return
        Whether the boat placement was been acccepted (bpACCEPTED) or
        rejected (bpREJECTED)
    */
    /*************************************************************************/
    BoatPlacement OceanOO::PlaceBoat(const Boat& boat)
    {
      int i;     // loop counter
      int index; // variable to hold the location of given index

      // Checks if the given position is outside of the board
      if (boat.position.x > (OceanOO::x_quadrants_ - 1) ||
        boat.position.y > (OceanOO::y_quadrants_ - 1) ||
        boat.position.x < 0 || boat.position.y < 0)
        return bpREJECTED;

      // If the orientation of the boat is horizontal...
      if (boat.orientation == oHORIZONTAL)
      {
        // Checks if the boat could go over the edge of the board
        if (boat.position.x + (BOAT_LENGTH - 1) >
          (OceanOO::x_quadrants_ - 1))
          return bpREJECTED;

        // Iterates across the grid to see if the spots are open
        for (i = 0; i < BOAT_LENGTH; i++)
        {
          index = boat.position.y * OceanOO::x_quadrants_ +
            (boat.position.x + i);

          // Checks if the current index is empty
          if (*(OceanOO::grid_ + index) != dtOK)
            // Returns REJECTED because space is filled
            return bpREJECTED;
        }

        // Iterates across the grid to place the boat ID
        for (i = 0; i < BOAT_LENGTH; i++)
        {
          index = boat.position.y * OceanOO::x_quadrants_ +
            (boat.position.x + i);

          // Assigns current index boat's ID
          OceanOO::grid_[index] = boat.ID;
        }
      }
      // If the orientation of the boat is vertical...
      else
      {
        // Checks if the boat could go over the edge of the board
        if (boat.position.y + (BOAT_LENGTH - 1) >
          (OceanOO::y_quadrants_ - 1))
          // Returns REJECTED for the boat not fitting on the board
          return bpREJECTED;

        // Iterates across the grid to see if the spots are open
        for (i = 0; i < BOAT_LENGTH; i++)
        {
          index = (boat.position.y + i) * OceanOO::x_quadrants_ +
            boat.position.x;

          // Checks if the current index is empty
          if (*(OceanOO::grid_ + index) != dtOK)
          {
            // Returns REJECTED is space is filled
            return bpREJECTED;
          }
        }

        // Iterates over the grid to place the boat ID
        for (i = 0; i < BOAT_LENGTH; i++)
        {
          index = (boat.position.y + i) * OceanOO::x_quadrants_ +
            boat.position.x;

          // Assigns current index boat's ID
          *(OceanOO::grid_ + index) = boat.ID;
        }
      }

      // Returns ACCEPTED if function doesn't return beforehand
      return bpACCEPTED;
    }

    /*************************************************************************/
    /*!
      \brief
        Determines the outcome of the "shot" at the given location
      \param coordinate
        Location where the shot is to go

      \return
        Whether the shot was illegally placed (srILLEGAL), missed (srMISS),
        had already been attempted (srDUPLICATE), hit a boat (srHIT), or sunk
        a boat (srSUNK)
    */
    /*************************************************************************/
    ShotResult OceanOO::TakeShot(const Point& coordinate)
    {
      int index; // variable to hold the location of the given index
      int value; // variable to hold the value at the given index

      // Checks if the coordinates are off of the board
      if (coordinate.x > (OceanOO::x_quadrants_ - 1) ||
        coordinate.y > (OceanOO::y_quadrants_ - 1) ||
        coordinate.x < 0 || coordinate.y < 0)
        // Returns ILLEGAL due to illegal coordinates
        return srILLEGAL;

      // Sets index variable to the location of the given index
      index = coordinate.y * OceanOO::x_quadrants_ + coordinate.x;

      // Sets value variable to the value at the given index
      value = OceanOO::grid_[index];

      // If the given index is empty...
      if (value == dtOK)
      {
        // Increment misses in shot statistics
        OceanOO::stats_.misses++;

        // Change the given index to reflect attempt
        OceanOO::grid_[index] = dtBLOWNUP;

        // Return MISS
        return srMISS;
      }

      // If given index has already been targeted...
      if (value == dtBLOWNUP || value > HIT_OFFSET)
      {
        // Increment duplicates in the shot statistics
        OceanOO::stats_.duplicates++;

        // Return DUPLICATE
        return srDUPLICATE;
      }

      // Increment hits on the boat at the given index
      OceanOO::boats_[value - 1].hits++;

      // Increment hits in the shot statistics
      OceanOO::stats_.hits++;

      // Change the given index to reflect hit
      OceanOO::grid_[index] = value + HIT_OFFSET;

      // If all locations on the boat has been hit...
      if (OceanOO::boats_[value - 1].hits == BOAT_LENGTH)
      {
        // Increment sunk in the shot statistics
        OceanOO::stats_.sunk++;

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

     \return
       A ShotStats struct holding the current shot statstics
   */
   /*************************************************************************/
    ShotStats OceanOO::GetShotStats() const
    {
      return this->stats_;
    }

    const int* OceanOO::GetGrid() const
    {
      return grid_;
    }

    Point OceanOO::GetDimensions() const
    {
      Point pt = { x_quadrants_, y_quadrants_ };
      return pt;
    }
  } // namespace WarBoats
} // namespace CS170