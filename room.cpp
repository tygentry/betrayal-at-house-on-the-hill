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
      break;
    }
    case 'd':
    {
      down = rm;
      rm->up = this;
      break;
    }
    case 'r':
    {
      right = rm;
      rm->left = this;
      break;
    }
    case 'l':
    {
      left = rm;
      rm->right = this;
      break;
    }
    default:
      std::cerr << "ERROR: Invalid Room Relationship" <<std::endl;
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
