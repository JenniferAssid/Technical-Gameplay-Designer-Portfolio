// I DO NOT OWN THIS CODE - IT WAS PROVIDED FOR THE CLASS TO TEST FUNCTIONALITY

#include <iostream> // cout, endl
#include <iomanip>  // setw

#include "WarBoatsOO.h"
#include "OceanOO.h"
#include "PRNG.h"

#pragma warning(disable : 4996)

void Dump(const CS170::WarBoatsOO::OceanOO &OceanOO, int field_width = 4, bool extraline = false, bool showboats = true)
{
    // Get the size of the OceanOO
    int width = OceanOO.GetDimensions().x;
    int height = OceanOO.GetDimensions().y;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int value = OceanOO.GetGrid()[y * width + x];
            if ( (value > 0) && (value < CS170::WarBoatsOO::HIT_OFFSET) && (showboats == false) )
                value = 0;
            std::cout << std::setw(field_width) << value;
        }
        std::cout << std::endl;
        if (extraline)
            std::cout << std::endl;
    }
}

void PrintShotResult(const CS170::WarBoatsOO::ShotResult &sr)
{
    const char *strings[] = {"HIT", "MISS", "DUPLICATE", "SUNK", "ILLEGAL"};
    std::cout << strings[static_cast<int>(sr)] << std::endl;
    //if (sr == CS170::WarBoatsOO::srSUNK)
    //  std::cout << "*************************************************\n";
}

void PrintStats(const CS170::WarBoatsOO::OceanOO &OceanOO)
{
    CS170::WarBoatsOO::ShotStats stats = OceanOO.GetShotStats();
    std::cout << "      Hits: " << std::setw(3) << stats.hits << std::endl;
    std::cout << "    Misses: " << std::setw(3) << stats.misses << std::endl;
    std::cout << "Duplicates: " << std::setw(3) << stats.duplicates << std::endl;
    std::cout << "Boats Sunk: " << std::setw(3) << stats.sunk << std::endl;
}

void TakeAShot(CS170::WarBoatsOO::OceanOO &OceanOO, const CS170::WarBoatsOO::Point& pt)
{
    CS170::WarBoatsOO::ShotResult sr;
    std::cout << "Shot: " << pt.x << ", " << pt.y << "  ";
    sr = OceanOO.TakeShot(pt);
    PrintShotResult(sr);
    PrintStats(OceanOO);
    Dump(OceanOO);
    std::cout << std::endl;
}

void warboatsOO_test0(void)
{
    // Initialize the pseudo-random number generator
    CS170::Utils::srand(0, 0);

    // Setup the OceanOO
    int num_boats = 3;
    int xsize = 8;
    int ysize = 8;
    CS170::WarBoatsOO::OceanOO OceanOO(num_boats, xsize, ysize);

    std::cout << "The empty board" << std::endl;
    Dump(OceanOO);
    std::cout << std::endl;

    CS170::WarBoatsOO::Boat boat;
    CS170::WarBoatsOO::Point location;
    CS170::WarBoatsOO::BoatPlacement bp;

    // Boat #1
    boat.ID = 1;
    location.x = 1;
    location.y = 3;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oHORIZONTAL;
    bp = OceanOO.PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Boat #2
    boat.ID = 2;
    location.x = 5;
    location.y = 1;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    bp = OceanOO.PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Boat #3
    boat.ID = 3;
    location.x = 0;
    location.y = 5;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oHORIZONTAL;
    bp = OceanOO.PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Illegal placement
    boat.ID = 4;
    location.x = 10;
    location.y = 5;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oHORIZONTAL;
    bp = OceanOO.PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Illegal placement
    boat.ID = 4;
    location.x = 2;
    location.y = 0;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    bp = OceanOO.PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Illegal placement
    boat.ID = 4;
    location.x = 5;
    location.y = 6;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oHORIZONTAL;
    bp = OceanOO.PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    std::cout << "The board with " << num_boats << " boats" << std::endl;
    Dump(OceanOO, 4, true);
    std::cout << std::endl;
}

void warboatsOO_test1(void)
{
    // Initialize the pseudo-random number generator
    CS170::Utils::srand(0, 0);

    // Setup the OceanOO
    int num_boats = 3;
    int xsize = 8;
    int ysize = 8;
    CS170::WarBoatsOO::OceanOO OceanOO(num_boats, xsize, ysize);

    std::cout << "The empty board" << std::endl;
    Dump(OceanOO);
    std::cout << std::endl;

    CS170::WarBoatsOO::Boat boat;
    CS170::WarBoatsOO::Point location;

    // Boat #1
    boat.ID = 1;
    location.x = 1;
    location.y = 3;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oHORIZONTAL;
    OceanOO.PlaceBoat(boat);

    // Boat #2
    boat.ID = 2;
    location.x = 5;
    location.y = 1;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    OceanOO.PlaceBoat(boat);

    // Boat #3
    boat.ID = 3;
    location.x = 0;
    location.y = 5;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oHORIZONTAL;
    OceanOO.PlaceBoat(boat);

    std::cout << "The board with " << num_boats << " boats" << std::endl;
    Dump(OceanOO);
    std::cout << std::endl;

    CS170::WarBoatsOO::Point coordinate;

    // Illegal coordinate
    coordinate.x = 10;
    coordinate.y = 5;
    TakeAShot(OceanOO, coordinate);

    // HIT
    coordinate.x = 1;
    coordinate.y = 3;
    TakeAShot(OceanOO, coordinate);

    // MISS
    coordinate.x = 5;
    coordinate.y = 7;
    TakeAShot(OceanOO, coordinate);

    // DUPLICATE
    coordinate.x = 1;
    coordinate.y = 3;
    TakeAShot(OceanOO, coordinate);

    // HIT
    coordinate.x = 2;
    coordinate.y = 3;
    TakeAShot(OceanOO, coordinate);

    // HIT
    coordinate.x = 3;
    coordinate.y = 3;
    TakeAShot(OceanOO, coordinate);

    // SUNK
    coordinate.x = 4;
    coordinate.y = 3;
    TakeAShot(OceanOO, coordinate);
}

void warboatsOO_test2(void)
{
    // Initialize the pseudo-random number generator
    CS170::Utils::srand(0, 0);

    // Setup the OceanOO (check 100, 30x30)
    int num_boats = 5;
    int xsize = 10;
    int ysize = 10;

    // Use a pointer to an OceanOO and dynamically create it
    CS170::WarBoatsOO::OceanOO *OceanOO;
    OceanOO = new CS170::WarBoatsOO::OceanOO(num_boats, xsize, ysize);

    std::cout << "The empty board" << std::endl;
    Dump(*OceanOO);
    std::cout << std::endl;

    CS170::WarBoatsOO::Boat boat;
    CS170::WarBoatsOO::Point location;

    // Boat #1
    boat.ID = 1;
    location.x = 1;
    location.y = 3;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oHORIZONTAL;
    OceanOO->PlaceBoat(boat);

    // Boat #2
    boat.ID = 2;
    location.x = 5;
    location.y = 1;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    OceanOO->PlaceBoat(boat);

    // Boat #3
    boat.ID = 3;
    location.x = 0;
    location.y = 5;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oHORIZONTAL;
    OceanOO->PlaceBoat(boat);

    // Boat #4
    boat.ID = 4;
    location.x = 3;
    location.y = 8;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oHORIZONTAL;
    OceanOO->PlaceBoat(boat);

    // Boat #5
    boat.ID = 5;
    location.x = 9;
    location.y = 6;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    OceanOO->PlaceBoat(boat);

    std::cout << "The board with " << num_boats << " boats" << std::endl;
    Dump(*OceanOO);
    std::cout << std::endl;

    // Try to sink the boats with random shots
    int shots = 0;
    while (num_boats > 0)
    {
        CS170::WarBoatsOO::ShotResult sr;
        do
        {
            CS170::WarBoatsOO::Point coordinate;
            coordinate.x = CS170::Utils::Random(0, xsize - 1);
            coordinate.y = CS170::Utils::Random(0, ysize - 1);
            std::cout << "Shot #"
                << ++shots
                << " at coordinate "
                << coordinate.x
                << ", "
                << coordinate.y
                << "  ";

            sr = OceanOO->TakeShot(coordinate);
            PrintShotResult(sr);
        } while (sr == CS170::WarBoatsOO::srDUPLICATE);

        // Sunk a boat
        if (sr == CS170::WarBoatsOO::srSUNK)
            num_boats--;

        //if (sr == CS170::WarBoatsOO::srSUNK)
        {
            //PrintShotResult(sr);
            PrintStats(*OceanOO);
            Dump(*OceanOO);
            std::cout << std::endl;
        }
    }

    // Must delete the OceanOO "manually"
    delete OceanOO;
}

void warboatsOO_test3(void)
{
    // Initialize the pseudo-random number generator
    CS170::Utils::srand(0, 0);

    // Setup the OceanOO
    int num_boats = 1;
    int xsize = 5;
    int ysize = 5;

    CS170::WarBoatsOO::OceanOO *OceanOO;
    OceanOO = new CS170::WarBoatsOO::OceanOO(num_boats, xsize, ysize);

    std::cout << "The empty board" << std::endl;
    Dump(*OceanOO);
    std::cout << std::endl;

    CS170::WarBoatsOO::Boat boat;
    CS170::WarBoatsOO::Point location;
    CS170::WarBoatsOO::BoatPlacement bp;

    // Illegal placement
    boat.ID = 1;
    location.x = -1;
    location.y = 0;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    bp = OceanOO->PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Illegal placement
    boat.ID = 1;
    location.x = -10;
    location.y = 0;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    bp = OceanOO->PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Illegal placement
    boat.ID = 1;
    location.x = 0;
    location.y = -1;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    bp = OceanOO->PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Illegal placement
    boat.ID = 1;
    location.x = 0;
    location.y = -10;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    bp = OceanOO->PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Illegal placement
    boat.ID = 1;
    location.x = 6;
    location.y = 0;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    bp = OceanOO->PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Illegal placement
    boat.ID = 1;
    location.x = 100;
    location.y = 0;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    bp = OceanOO->PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Illegal placement
    boat.ID = 1;
    location.x = 0;
    location.y = 6;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    bp = OceanOO->PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Illegal placement
    boat.ID = 1;
    location.x = 0;
    location.y = 100;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    bp = OceanOO->PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    // Legal placement
    boat.ID = 1;
    location.x = 0;
    location.y = 0;
    boat.position = location;
    boat.orientation = CS170::WarBoatsOO::oVERTICAL;
    bp = OceanOO->PlaceBoat(boat);
    if (bp == CS170::WarBoatsOO::bpREJECTED)
        std::cout << "Boat placement is rejected." << std::endl;
    else
        std::cout << "Boat placement is accepted." << std::endl;

    std::cout << "The board with " << num_boats << " boats" << std::endl;
    Dump(*OceanOO, 4, true);
    std::cout << std::endl;

    CS170::WarBoatsOO::Point coordinate;

    // Illegal coordinate
    coordinate.x = -1;
    coordinate.y = 0;
    TakeAShot(*OceanOO, coordinate);

    coordinate.x = -100;
    coordinate.y = 0;
    TakeAShot(*OceanOO, coordinate);

    coordinate.x = 0;
    coordinate.y = -1;
    TakeAShot(*OceanOO, coordinate);

    coordinate.x = 0;
    coordinate.y = -100;
    TakeAShot(*OceanOO, coordinate);

    coordinate.x = 6;
    coordinate.y = 0;
    TakeAShot(*OceanOO, coordinate);

    coordinate.x = 60;
    coordinate.y = 0;
    TakeAShot(*OceanOO, coordinate);

    coordinate.x = 0;
    coordinate.y = 6;
    TakeAShot(*OceanOO, coordinate);

    coordinate.x = 0;
    coordinate.y = 60;
    TakeAShot(*OceanOO, coordinate);

    delete OceanOO;
}

