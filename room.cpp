#include <iostream>

#include "room.h"

void Room::linkRooms(Room* rm, char dir)
{
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
