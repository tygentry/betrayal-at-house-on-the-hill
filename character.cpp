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
  file >> Sanity; SanityStart = Sanity;
  createLine(file, KnowledgeLine);
  file >> Knowledge; KnowledgeStart = Knowledge;
  createLine(file, MightLine);
  file >> Might; MightStart = Might;
  createLine(file, SpeedLine);
  file >> Speed >> age; SpeedStart = Speed;
  //Adding default actions to characters
  actions.push_back(new Action("Move", "Char", this));
  actions.push_back(new Action("Attack", "Char", this));
  actions.push_back(new Action("Drop", "Char", this));
  actions.push_back(new Action("Pickup", "Char", this));
  actions.push_back(new Action("Give", "Char", this));
}

Character::~Character()
{ delete[] SanityLine; delete[] KnowledgeLine; delete[] MightLine; delete[] SpeedLine;
  for (unsigned int i = 0; i < actions.size(); i++)
    delete actions[i];
}

//processing taking any physical damage, allowing user to distribute damage however they wish
void Character::takePhys(int dmg)
{
  std::string choice;
  int used = 0;

  for (; dmg > 0; dmg--)
  {
    std::cout << "Your Might: " << getMight() << std::endl;
    std::cout << "Your Speed: " << getSpeed() << std::endl;
    std::cout << "You have " << dmg << " physical damage left to take.\n" << std::endl;
    if (Might - 1 > 0 && Speed - 1 > 0 && Might + Speed - dmg > 0)
    {
      std::cout << "You can choose to take damage in Might or Speed.\nEnter M or S to choose: ";
      std::cin >> choice;
      do {
        if (choice == "M")
        { Might--; used = 0; }
        else if (choice == "S")
        { Speed--; used = 0; }
        else { used = 1; std::cout << "\nPlease enter a valid choice (M / S): "; }
      } while (used);
    }
    else if (Might - dmg > 0)
    {
      std::cout << "You took the damage in Might because any more damage in Speed would kill you." << std::endl;
      Might -= dmg;
    }
    else if (Speed - dmg > 0)
    {
      std::cout << "You took the damage in Speed because any more damage in Might would kill you." << std::endl;
      Speed -= dmg;
    }
    else
    {
      std::cout << "You have died." << std::endl;
      return;
    }
  }
}

//processing taking any mental damage, allowing user to distribute damage however they wish
void Character::takeMent(int dmg)
{
  std::string choice;
  int used = 0;

  for (; dmg > 0; dmg--)
  {
    std::cout << "Your Knowledge: " << getKnowledge() << std::endl;
    std::cout << "Your Sanity: " << getSanity() << std::endl;
    std::cout << "You have " << dmg << " mental damage left to take.\n" << std::endl;
    if (Knowledge - 1 > 0 && Sanity - 1 > 0 && Knowledge + Sanity - dmg > 0)
    {
      std::cout << "You can choose to take damage in Knowledge or Sanity.\nEnter K or S to choose: ";
      std::cin >> choice;
      do {
        if (choice == "K")
        { Knowledge--; used = 0; }
        else if (choice == "S")
        { Sanity--; used = 0; }
        else { used = 1; std::cout << "\nPlease enter a valid choice (K / S): "; }
      } while (used);
    }
    else if (Knowledge - dmg > 0)
    {
      std::cout << "You took the damage in Knowledge because any more damage in Sanity would kill you." << std::endl;
      Knowledge -= dmg;
      return;
    }
    else if (Sanity - dmg > 0)
    {
      std::cout << "You took the damage in Sanity because any more damage in Knowledge would kill you." << std::endl;
      Sanity -= dmg;
      return;
    }
    else
    {
      std::cout << "You have died." << std::endl;
      return;
    }
  }
}

//Helper method to print out all characters actions
std::string Character::printActions()
{
  //creating string containing all possible actions
  std::string out = "", name;
  for (int i = 0; i < (int)actions.size(); i++)
  {
    //adding action name
    out += "   " + std::to_string(i) + ") " + actions[i]->name;

    //if the action's origin is not from the player (gained from an item or room)
    if (actions[i]->originType != "Char")
    {
      //cast and add the appropriate origin name
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

//helper function to add any additional actions to the action vector
void Character::addAction(std::string name, std::string type, void* source)
{
  Action* tmp = new Action(name, type, source);
  actions.push_back(tmp);
}

void Character::removeAction(std::string name)
{
  std::vector<Action*>::iterator itr = actions.begin();
  while (itr != actions.end())
  {
    if ((*itr)->name == name)
    {
      actions.erase(itr);
      itr = actions.begin();
    }
  }
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
