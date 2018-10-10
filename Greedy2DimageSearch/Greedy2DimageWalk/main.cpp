#include <iostream>
#include <fstream>
#include "MapDataDrawer.cpp"

using namespace std;

int main()
{
    //choose which .dat file to use
    int input;
    cout << "-- Map Select --\n"
         << "1: Colorado_480x480.dat\n"
         << "2: colorado844x480.dat\n"
         << "input:";
    cin >> input;
    //init file
    fstream file;
    //access our file
    if(input == 1)
        file.open("Colorado_480x480.dat");
    else
        file.open("colorado844x480.dat");
    //Pass chosen .dat to MapDataDrawer to populate necessary array
    MapDataDrawer* colorado = new MapDataDrawer(file, input);
    //draw the map to Bridges
    colorado->draw_map();

    cout << "--Starting position--\n"
         << "Between 0 and 479\nInput:";

    cin >> input;
    //if garbage input is taken, set to center of map
    if(input < 0 || input > colorado->get_vertical_resolution())
        input = (colorado->get_vertical_resolution() / 2);
    colorado->draw_path(input);

    file.close();
    delete colorado;
    return 0;
}
