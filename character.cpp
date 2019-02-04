#include "character.h"

void Character::createLine(std::ifstream &file, int* &arr)
{
  arr = new int[9];
  for (int i = 0; i < 9; i++)
    file >> arr[i];
}

Character::Character(std::ifstream &file)
{
  std::string first, last;

  file >> first >> last >> color;
  name = "\033[0;" + color + "m" + first + " " + last + "\033[0m";
  createLine(file, SanityLine);
  file >> Sanity;
  createLine(file, KnowledgeLine);
  file >> Knowledge;
  createLine(file, MightLine);
  file >> Might;
  createLine(file, SpeedLine);
  file >> Speed >> age;

}

void Character::printChar()
{
  std::cout << name;
  std::cout << "\nSanity: " << getSanity() << std::endl;
  std::cout << "Knowledge: " << getKnowledge() << std::endl;
  std::cout << "Might: " << getMight() << std::endl;
  std::cout << "Speed: " << getSpeed() << std::endl;
  std::cout << "Age: " << age << std::endl;
}

bool Character::isDead()
{
  if (Sanity <= 0 || Knowledge <= 0 || Might <= 0 || Speed <= 0)
    return true;
  else
    return false;
}
