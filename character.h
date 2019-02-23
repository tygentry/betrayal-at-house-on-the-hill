#ifndef _character_h
#define _character_h

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include "cards.h"
#include "room.h"

//Action subclass to help keep track of possible actions based on various origins
class Action {
public:
  std::string name;
  std::string originType;
  void* origin;

  Action() : origin(NULL) {}
  Action(std::string n, std::string t, void* o)
  {
    name = n;
    originType = t;
    origin = o;
  }

};



class Character {
private:
  //stat decay lines
  int* SanityLine;
  int* KnowledgeLine;
  int* MightLine;
  int* SpeedLine;
  Room* location;
  std::vector<Action*> actions;

  //private helper methods
  void createLine(std::ifstream &file, int* &arr);

public:
  //accessible stats
  std::string name;
  std::string color;

  //made public to allow for increment and decrement along stat lines
  int age;
  int Sanity;
  int Knowledge;
  int Might;
  int Speed;

  //Constuctors and Destructor
  Character() {};
  Character(std::ifstream &file);
  ~Character();

  //Alter stats
  int getSanity() { if (Sanity > 8) Sanity = 8; return SanityLine[Sanity]; }
  int getKnowledge() { if (Knowledge > 8) Knowledge = 8; return KnowledgeLine[Knowledge]; }
  int getMight() { if (Might > 8) Might = 8; return MightLine[Might]; }
  int getSpeed() { if (Speed > 8) Speed = 8; return SpeedLine[Speed]; }

  void setRoom(Room* l) { location = l; }

  //Additional functions
  std::string printActions();
  int numActions() { return actions.size(); }
  void printChar();
  bool isDead();
};

#endif
