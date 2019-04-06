#include <iostream>

#include "room.h"

// //method to search for next possible movement option for door attachment
// void Room::attach(Room* rm, char dir, Room* wall)
// {
//   switch (dir)
//   {
//     case 'u':
//
//   }
// }

//function to link a passed room to the current room object in a given direction
void Room::linkRooms(Room* rm, char dir, Room* wall)
{
  //matching the same floor
  if (isBasement())
    rm->setB();
  else if (isGround())
    rm->setG();
  else if (isUpper())
    rm->setU();
  else if (isRoof())
    rm->setR();

  //depending on direction, links the correct pointers
  switch (dir)
  {
    case 'u':
    {
      up = rm;
      //if (rm->down != wall) rm->down = this;
      rm->row = row - 1;
      rm->col = col;
      break;
    }
    case 'd':
    {
      down = rm;
      //if (rm->up != wall) rm->up = this;
      rm->row = row + 1;
      rm->col = col;
      break;
    }
    case 'r':
    {
      right = rm;
      //if (rm->left != wall) rm->left = this;
      rm->row = row;
      rm->col = col + 1;
      break;
    }
    case 'l':
    {
      left = rm;
      //if (rm->right != wall) rm->right = this;
      rm->row = row;
      rm->col = col - 1;
      break;
    }
    default:
      std::cerr << "ERROR: Invalid Room Relationship" <<std::endl;
  }
}

//method to link a room to all surrounding rooms based on floor pointer
void Room::linkSurrounding(Room* &wall, Room*** &floor)
{
  //uplink
  if (row > 0 && floor[row-1][col] != NULL)
  {
    if (up != wall)
    {
      if (floor[row-1][col]->down == NULL || floor[row-1][col]->down == this)
      {
        up = floor[row-1][col];
        floor[row-1][col]->down = this;
      }
      else
      {
        floor[row-1][col]->down = wall;
        up = wall;
      }
    }
    else
    {
      floor[row-1][col]->down = wall;
      up = wall;
    }
  }

  //downlink
  if (row < 24 && floor[row+1][col] != NULL)
  {
    if (down != wall)
    {
      if (floor[row+1][col]->up == NULL || floor[row+1][col]->up == this)
      {
        down = floor[row+1][col];
        floor[row+1][col]->up = this;
      }
      else
      {
        floor[row+1][col]->up = wall;
        down = wall;
      }
    }
    else
    {
      floor[row+1][col]->up = wall;
      down = wall;
    }
  }
  //leftlink
  if (col > 0 && floor[row][col-1] != NULL)
  {
    if (left != wall)
    {
      if (floor[row][col-1]->right == NULL || floor[row][col-1]->right == this)
      {
        left = floor[row][col-1];
        floor[row][col-1]->right = this;
      }
      else
      {
        floor[row][col-1]->right = wall;
        left = wall;
      }
    }
    else
    {
      floor[row][col-1]->right = wall;
      left = wall;
    }
  }
  //rightlink
  if (col < 24 && floor[row][col+1] != NULL)
  {
    if (right != wall)
    {
      if (floor[row][col+1]->left == NULL || floor[row][col+1]->left == this)
      {
        right = floor[row][col+1];
        floor[row][col+1]->left = this;
      }
      else
      {
        floor[row][col+1]->left = wall;
        right = wall;
      }
    }
    else
    {
      floor[row][col+1]->left = wall;
      right = wall;
    }
  }
}

//helper function to print out possible movement options for a given room
//only prints if the pointer is not a wall
std::string Room::printRoomOptions(Room* &wall)
{
  //declaring initial variables
  std::string out = "";
  int count = 1;

  //up options
  if (up != wall)
  {
    out += "   1";
    count++;
    if (up == NULL)
      out += ") Discover a new room (UP)\n";
    else
      out += ") Enter the " + up->name + " (UP)\n";
  }
  //down options
  if (down != wall)
  {
    out += "   2";
    count++;
    if (down == NULL)
      out += ") Discover a new room (DOWN)\n";
    else
      out += ") Enter the " + down->name + " (DOWN)\n";
  }
  //left options
  if (left != wall)
  {
    out += "   3";
    count++;
    if (left == NULL)
      out += ") Discover a new room (LEFT)\n";
    else
      out += ") Enter the " + left->name + " (LEFT)\n";
  }
  //right options
  if (right != wall)
  {
    out += "   4";
    count++;
    if (right == NULL)
      out += ") Discover a new room (RIGHT)\n";
    else
      out += ") Enter the " + right->name + " (RIGHT)\n";
  }
  //above options
  if (above != wall)
  {
    out += "   5";
    count++;
    if (above == NULL)
      out += ") Discover a new room (ABOVE)\n";
    else
      out += ") Enter the " + above->name + " (ABOVE)\n";
  }
  //below options
  if (below != wall)
  {
    out += "   6";
    count++;
    if (below == NULL)
      out += ") Discover a new room (BELOW)\n";
    else
      out += ") Enter the " + below->name + " (BELOW)\n";
  }

  numOptions = count;

  return out;
}
