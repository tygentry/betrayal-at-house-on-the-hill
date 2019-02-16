#include "character.h"

//create formatted line based on input for character stats
void Character::createLine(std::ifstream &file, int* &arr)
{
  arr = new int[9];
  for (int i = 0; i < 9; i++)
    file >> arr[i];
}

//Constructor to create character object
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
  actions.push_back(new Action("Move", "Char", this));
  actions.push_back(new Action("Attack", "Char", this));
  actions.push_back(new Action("Drop", "Char", this));
  actions.push_back(new Action("Pickup", "Char", this));
  actions.push_back(new Action("Give", "Char", this));
}

std::string Character::printActions()
{
  std::string out = "", name;
  for (int i = 0; i < (int)actions.size(); i++)
  {
    out += "   " + std::to_string(i) + ") " + actions[i]->name;

    if (actions[i]->originType != "Char")
    {
      if (actions[i]->originType == "Room")
        ((Room *)actions[i]->origin)->name = name;
      else if (actions[i]->originType == "Card")
        ((Card *)actions[i]->origin)->name = name;
      out += "(" + name + ")";
    }

    out += "\n";
  }
  return out;
}

//helper function to print a character and their current stats
void Character::printChar()
{
  std::cout << name;
  std::cout << "\nSanity: " << getSanity() << std::endl;
  std::cout << "Knowledge: " << getKnowledge() << std::endl;
  std::cout << "Might: " << getMight() << std::endl;
  std::cout << "Speed: " << getSpeed() << std::endl;
  std::cout << "Age: " << age << std::endl;
}

//method to determine if a character is dead
bool Character::isDead()
{
  if (Sanity <= 0 || Knowledge <= 0 || Might <= 0 || Speed <= 0)
    return true;
  else
    return false;
}
