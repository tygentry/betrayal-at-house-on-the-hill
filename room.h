#ifndef room_h_
#define room_h_

#include <string>

class Room {
public:
  Room* up;
  Room* down;
  Room* left;
  Room* right;
  Room* below;
  Room* above;
  std::string name;

  Room (const std::string &n, Room* u, Room* d, Room* l, Room* r)
  {
    name = n;
    up = u;
    down = d;
    left = l;
    right = r;
    above = NULL;
    below = NULL;
  }

  void setBelow(Room* &b) { below = b; }
  void setAbove(Room* &a) { above = a; }

  void linkRooms(Room* rm, char dir);

  bool isBasement() { return basement; }
  bool isGround() { return ground; }
  bool isUpper() { return upper; }
  bool isRoof() { return roof; }

private:
  bool basement;
  bool ground;
  bool upper;
  bool roof;
  bool dumbwaiter;

};

#endif
