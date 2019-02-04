#ifndef _character_h
#define _character_h

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class Character {
private:
  int* SanityLine;
  int* KnowledgeLine;
  int* MightLine;
  int* SpeedLine;

  void createLine(std::ifstream &file, int* &arr);

public:
  std::string name;
  std::string color;
  int age;
  int Sanity;
  int Knowledge;
  int Might;
  int Speed;

  //Constuctors and Destructor
  Character() {};
  Character(std::ifstream &file);
  ~Character() { delete SanityLine; delete KnowledgeLine; delete MightLine; delete SpeedLine; };

  //Alter stats
  int getSanity() { if (Sanity > 8) Sanity = 8; return SanityLine[Sanity]; }
  int getKnowledge() { if (Knowledge > 8) Knowledge = 8; return KnowledgeLine[Knowledge]; }
  int getMight() { if (Might > 8) Might = 8; return MightLine[Might]; }
  int getSpeed() { if (Speed > 8) Speed = 8; return SpeedLine[Speed]; }

  //Additional functions
  void printChar();
  bool isDead();
};

#endif
