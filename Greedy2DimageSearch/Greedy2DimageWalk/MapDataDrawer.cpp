#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

using namespace std;

#include <Bridges.h> //code will not compile without this.  It is only provided by the university
#include "Color.h"
#include "ColorGrid.h"

using namespace bridges;

class MapDataDrawer
{
private:
    int min, max, row, col, size, current_row, current_col, *a, *gray;
    ColorGrid *cg;
public:
    //Initialize our map drawer, populates the 2D array with .dat file values
    MapDataDrawer(fstream& file_name, int choice)
    {
        Bridges::initialize((choice+1), "", "");	//uses email, and a 13 digit integer for posting to bridges online
        if(choice == 1)
            Bridges::setTitle("Module3: Milestone2, Colorado_480x480.dat");
        else
            Bridges::setTitle("Module3: Milestone2, colorado844x480.dat");
        //determine dataset size
        file_name>>col>>row;
        cg = new ColorGrid(row, col, Color("black"));
        //set up our 1D array based on size
        size = row*col;
        a = new int[size];
        for(int i = 0; i < size; i++)
            file_name>>a[i];
    }
    // destructor
    ~MapDataDrawer()
    {
        delete a, gray;
    }
    //returns the minimum value out of the dataset
    int get_min()
    {
        min = a[0];
        for(int i = 0; i < size; i++)
        {
            if(a[i] < min)
                min = a[i];
        }
        return min;
    }
    //returns the maximum value out of the dataset
    int get_max()
    {
        max = a[0];
        for(int i = 0; i < size; i++)
        {
            if(a[i] > max)
                max = a[i];
        }
        return max;
    }
    //self explainatory
    int get_vertical_resolution()
    {
        return col;
    }
    void draw_map()
    {
        min = get_min();
        max = get_max();

        //set up our 2D array based on size
        gray = new int[size];
        //populate array with 0 to 255 colorspace values based on min and max scaled range
        for(int i = 0; i < size; i++)
            gray[i] = (int) ((255.0*(a[i] - min)) / (max - min));

        //print grayscaled image to Bridges

        current_row = 0;
        current_col = 0;
        for(int i = 0; i < size; i++)
        {
            cg->set(current_row,current_col,Color(gray[i], gray[i], gray[i]));

            /* if i divided by col has remainder of zero, then it is a multiple of col
               so increase our row by 1.  An example is if i = 480 and col = 480,
               480%480 is 0, so move to next row */
            if(i%col==0 && i != 0)
                current_row++;

            /*increment our current column every loop, then check to see if it is equal to the value stored in col,
              if it is then set our current column to 0. This is done when we've reached the end of a column, which
              also implies that we will also be starting on a new row. */
            current_col++;
            if(current_col == col)
                current_col = 0;
        }

        Bridges::setDataStructure(cg);
        Bridges::visualize();
    }
    /*draw a path of least position change. takes in an initial starting position and draws a line across the map
      from west to east in red.
    */
    void draw_path(int walk_pos)
    {
        //initializers
        int current, left, right, forward, smallest, track_switch, random = 0;
        srand (time(NULL)); // random generator
        int location = walk_pos; //current location within the vertical space

        //loop to walk across the map from west to east
        for(int i = 0; i < col; i++)
        {
            //set up current, as well as next three perpindicular positions, with foward centered on the current value
            current = a[location];
            left = a[location+row-1];
            forward = a[location+row];
            right = a[location+row+1];
            //color current occupied space red
            cg->set(walk_pos,i,Color("red"));
            //boundary check, if we haven't reached the end of the map, check to see where to move next
            if(i < (col-1))
            {
                //if we are at the top of the map, use this
                if(location%row==0)
                {
                    //if both values are the same then do a coin flip
                    if(forward == right)
                    {
                        random = rand() % 10+1;
                        if(random > 5)
                            location+=row;
                        else
                        {
                            location+=row+1;
                            walk_pos++;
                        }
                    }
                    //otherwise determine which is the smallest and go forward or right
                    else
                    {
                        smallest = abs(current - forward);
                        if(abs(current - right) > smallest)
                            location+=row;//walk_pos unchanged
                        else
                        {
                            location+=row+1;
                            walk_pos++;
                        }
                    }
                }
                //if we are at the bottom of the map, use this
                else if((location+1)%row==0)
                {
                    //if both values are the same, do a coin flip
                    if(forward == left)
                    {
                        random = rand() % 10+1;
                        if(random > 5)
                            location+=row;
                        else
                        {
                            location+=row-1;
                            walk_pos--;
                        }
                    }
                    //otherwise, determine which is smallest and go forward or left
                    else
                    {
                        smallest = abs(current - forward);
                        if(abs(current - left) > smallest)
                            location+=row;//walk_pos unchanged
                        else
                        {
                            location+=col-1;
                            walk_pos--;
                        }
                    }
                }
                //otherwise we are somewhere between the top and bottom
                else
                {
                    /*keep track of the smallest value.
                      track_switch is incremented if we find a smaller value, this keeps track of
                      which direction we want to go
                    */
                    smallest = abs(current - left);
                    track_switch = 0;
                    if(abs(current - forward) <= smallest)
                    {
                        smallest = abs(current - forward);
                        track_switch = 1;
                    }

                    if(abs(current - right) < smallest)
                    {
                        smallest = abs(current - right);
                        track_switch = 2;
                    }
                    //check the smallest values, and check which track we are thinking of taking
                    //in every case, if forward is the smallest, we go forward.
                    if(track_switch == 1)
                        location+=row;//walk_pos unchanged
                    //if the first value encountered (left) is the smallest, check if it is equal in size to (right)
                    else if (track_switch == 0)
                    {
                        //if the left and right vals are equal, do a coin flip
                        if(abs(current - left) == abs(current - right))
                        {
                            random = rand() % 10+1;
                            if(random > 5)
                            {
                                location+=row+1;
                                walk_pos++;
                            }
                            else
                            {
                                location+=row-1;
                                walk_pos--;
                            }

                        }
                        //otherwise go left
                        else
                        {
                            location+=row-1;
                            walk_pos--;
                        }
                    }
                    //if the right value is the smallest, check if it is equal in size to left
                    else
                    {
                        //if it is do a coin flip
                        if(abs(current - left) == abs(current - right))
                        {
                            random = rand() % 10+1;
                            if(random > 5)
                            {
                                location+=row+1;
                                walk_pos++;
                            }
                            else
                            {
                                location+=row-1;
                                walk_pos--;
                            }
                        }
                        //otherwise go right
                        else
                        {
                            location+=row+1;
                            walk_pos++;
                        }
                    }
                }
            }
        }
        Bridges::setDataStructure(cg);
        Bridges::visualize();
    }
};
