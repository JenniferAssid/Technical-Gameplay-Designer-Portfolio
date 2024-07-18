// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
namespace CS170
{
  namespace WarBoatsOO
  {
    const int BOAT_LENGTH = 4; // Length of a boat
    const int HIT_OFFSET = 100; // Add this to the boat ID
    class OceanOO
    {
    public:
      // Constructor
      OceanOO(int num_boats, int x_quadrants, int y_quadrants);

      // Destructor
      ~OceanOO();

      BoatPlacement PlaceBoat(const Boat& boat);

      ShotResult TakeShot(const Point& coordinate);

      ShotStats GetShotStats() const;

      /*// Mutators (Settors)
      */

      // Accessors (Gettors)

      // Provided
      const int* GetGrid() const;
      Point GetDimensions() const;

    private:
      int* grid_;       // The 2D ocean
      int x_quadrants_; // Ocean size along x-axis
      int y_quadrants_; // Ocean size along y-axis

      Boat* boats_;      // Dynamic array of boats
      int num_boats_;    // Number of boats in the ocean
      ShotStats stats_;  // Status of the attack

    }; // class Ocean
  } // namespace WarBoats
} // namespace CS170