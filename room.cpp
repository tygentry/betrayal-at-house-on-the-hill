#include <iostream>

#include "room.h"

void Room::linkRooms(Room* rm, char dir)
{
  if (rm->isBasement())
    setB();
  else if (rm->isGround())
    setG();
  else if (rm->isUpper())
    setU();
  else if (rm->isRoof())
    setR();

  switch (dir)
  {
    case 'u':
    {
      up = rm;
      rm->down = this;
      rm->row = row - 1;
      rm->col = col;
      break;
    }
    case 'd':
    {
      down = rm;
      rm->up = this;
      rm->row = row + 1;
      rm->col = col;
      break;
    }
    case 'r':
    {
      right = rm;
      rm->left = this;
      rm->row = row;
      rm->col = col + 1;
      break;
    }
    case 'l':
    {
      left = rm;
      rm->right = this;
      rm->row = row;
      rm->col = col - 1;
      break;
    }
    default:
      std::cerr << "ERROR: Invalid Room Relationship" <<std::endl;
  }
}

void Room::linkSurrounding(Room* &wall, Room*** &floor)
{
  //uplink
  if (up != wall && floor[row-1][col] != NULL)
    if (floor[row-1][col]->down == NULL)
    {
      up = floor[row-1][col];
      floor[row-1][col]->down = up;
    }
  //downlink
  if (down != wall && floor[row+1][col] != NULL)
    if (floor[row+1][col]->up == NULL)
    {
      down = floor[row+1][col];
      floor[row+1][col]->up = down;
    }
  //leftlink
  if (left != wall && floor[row][col-1] != NULL)
    if (floor[row][col-1]->right == NULL)
    {
      left = floor[row][col-1];
      floor[row][col-1]->right = left;
    }
  //rightlink
  if (right != wall && floor[row][col+1] != NULL)
    if (floor[row][col+1]->left == NULL)
    {
      right = floor[row][col+1];
      floor[row][col+1]->left = right;
    }
}

std::string Room::printRoomOptions(Room* &wall)
{
  std::string out = "";
  int count = 1;

  if (up != wall)
  {
    out += "   " + std::to_string(count);
    count++;
    if (up == NULL)
      out += ") Discover a new room (UP)\n";
    else
      out += ") Enter the " + up->name + " (UP)\n";
  }
  if (down != wall)
  {
    out += "   " + std::to_string(count);
    count++;
    if (down == NULL)
      out += ") Discover a new room (DOWN)\n";
    else
      out += ") Enter the " + down->name + " (DOWN)\n";
  }
  if (left != wall)
  {
    out += "   " + std::to_string(count);
    count++;
    if (left == NULL)
      out += ") Discover a new room (LEFT)\n";
    else
      out += ") Enter the " + left->name + " (LEFT)\n";
  }
  if (right != wall)
  {
    out += "   " + std::to_string(count);
    count++;
    if (right == NULL)
      out += ") Discover a new room (RIGHT)\n";
    else
      out += ") Enter the " + right->name + " (RIGHT)\n";
  }

  numOptions = count;

  return out;
}
