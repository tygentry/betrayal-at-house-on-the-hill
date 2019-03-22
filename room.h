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
  int numOptions;
  int row, col;

  Room () {};

  //constructor
  Room (const std::string &n, Room* u, Room* d, Room* l, Room* r, int rw, int c)
  {
    name = n;
    up = u;
    down = d;
    left = l;
    right = r;
    above = NULL;
    below = NULL;
    basement = false;
    ground = false;
    upper = false;
    roof = false;
    row = rw; col = c;
  }

  //Setters
  void setBelow(Room* &b) { below = b; }
  void setAbove(Room* &a) { above = a; }
  void setB() { basement = true; }
  void setG() { ground = true; }
  void setU() { upper = true; }
  void setR() { roof = true; }

  //basic functions
  void linkRooms(Room* rm, char dir, Room* wall);
  void linkSurrounding(Room* &wall, Room*** &floor);
  std::string printRoomOptions(Room* &wall);

  //checkers
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
