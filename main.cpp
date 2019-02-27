#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>

#include "character.h"
#include "cards.h"
#include "room.h"

//Static variables to update and allow expansion of game contents
int static numOfChars = 12;
int static numOfRooms = 61;
int static numOfEvents = 56;
int static numOfOmens = 21;
int static numOfItems = 34;

//Error checking to ensure user enters a valid integer input
void obtainValidNum(int &in)
{
  while (std::cin.fail())
  {
    std::cin.clear();
    std::cin.ignore(256, '\n');
    std::cout << "Please enter a number: ";
    std::cin >> in;
  }
}

//Helper method to print out stat lines with formatted output from the input file
std::string printLine(std::ifstream &f)
{
  //beginning string of stats
  std::string out = "\033[0;91m";
  std::string tmp;
  for (int i = 0; i < 7; i++)
  {
    //first (critical) stat is colored red
    if (i == 0)
    {
      f >> tmp;
      out += tmp + "\033[0m ";
    }

    //all remaining stats are added
    f >> tmp;

    //but green coloring is used to denote starting stat
    if (tmp.substr(0,1) == "-")
    {
      out += "\033[0;92m" + tmp.substr(1) + "\033[0m ";
    }
    else
    {
      out += tmp + " ";
    }
  }
  //completed stat string is returned
  return out;
}

//Helper method to print out all available characters during character selection
void printCharOptions(Character** &players, std::ifstream &file, int i)
{
  bool skip;
  file.open("charactersBase.txt");
  std::string firstN, lastN, color, age, name;

  //printing out all character options
  for (int j = 0; j < numOfChars; j++)
  {
    //initializing variables and obtaining initial input
    skip = false;
    name = "\n\033[0;";
    file >> firstN >> lastN >> color >> age;
    std::string charN = "\033[0;" + color + "m" + firstN + " " + lastN + "\033[0m";

    //comparison to check to ensure no previously chosen characters are displayed
    for (int k = 0; k < i; k++)
    {
      if (charN == players[k]->name)
        skip = true;
    }

    //if the current character has not been chosen, print out that character and their stats
    if (!skip)
    {
      name += color + "m" + firstN + " " + lastN + ", age " + age + "\033[0m";
      std::cout <<  name;
      std::cout << "\nSanity:    " << printLine(file);
      std::cout << "\nKnowledge: " << printLine(file);
      std::cout << "\nMight:     " << printLine(file);
      std::cout << "\nSpeed:     " << printLine(file) << std::endl;
    }
    else //otherwise parse input but do not print any out
    {
      printLine(file);
      printLine(file);
      printLine(file);
      printLine(file);
    }
  }
  file.close();
}

//helper method to allow player to choose their character, containing all necessary checks for invalid or incorrect input, updating
//the players Character* array so the game can progress
void pickCharacter(Character** &players, std::string* &choices, int i, const int &playerNum)
{
  bool skip;
  std::string path, choice;
  std::ifstream charData;

  std::cout << "\nEnter Choice's Initials: ";
  while(1)
  {
    skip = false;
    path = "characters/";
    std::cin >> choice;

    //check to ensure no repeated choices
    for (int j = 0; j < i; j++)
    {
      if (choice == choices[j])
        skip = true;
    }

    //correct error message is repeated choice
    if (skip)
     std::cout << "That character has already been chosen, please enter a set of initials of the characters on screen.\n";
    else
    {
      //otherwise attempt to search and open a character file
      path += choice + ".txt";
      charData.open(path);

      //if that character exists, choice is displayed and character is created
      if (charData.is_open())
      {
        players[i] = new Character(charData);
        std::cout << "You chose: " << players[i]->name << std::endl;
        choices[i] = choice;
        charData.close();

        //Pause to allow another player to prepare to choose
        if (i < playerNum-1)
        {
          std::cout << "Type anything and press ENTER to choose for next player.";
          std::cin >> path;
        }
        break;
      }
      else //otherwise, character does not exist
      {
        std::cout << "Initials " << choice.substr(0, 2) << " are not valid, please enter a valid set of initials.\n";
      }
    }
  }
}

//helper method to fill a passed string array with file input (Helper to shuffle)
void fillStack(std::ifstream &in, std::string* &l, int size)
{
  std::string tmp;
  for (int i = 0; i < size; i++)
  {
    in >> tmp;
    l[i] = tmp;
  }
}

//helper method to fill a list with the shuffled values from a passed string array (Helper to shuffle)
void fillList(std::list<std::string> &l, std::string* &s, int size)
{
  for (int i = 0; i < size; i++)
  {
    l.push_back(s[i]);
  }
}

//Method to randomly shuffle a string array full of card
void shuffle(std::string* &list, int size)
{
  int random;
  std::string tmp;

  for (int i = 0; i < size; i++)
  {
    random = std::rand() % size;
    tmp = list[i];
    list[i] = list[random];
    list[random] = tmp;
  }
}

//helper method to fill a passed pair array with file input (Helper to shuffle)
void fillStackR(std::ifstream &in, std::pair<std::string, std::string>* &l, int size)
{
  std::string tmp;
  for (int i = 0; i < size; i++)
  {
    in >> tmp;
    l[i].first = tmp;
    in >> tmp;
    l[i].second = tmp;
  }
}

//helper method to fill a list with the shuffled values from a passed pair array (Helper to shuffle)
void fillListR(std::list<std::pair<std::string, std::string> > &l, std::pair<std::string, std::string>* &s, int size)
{
  for (int i = 0; i < size; i++)
  {
    l.push_back(s[i]);
  }
}

//Method to randomly shuffle a pair array full of room names and possible levels
void shuffleR(std::pair<std::string, std::string>* &list, int size)
{
  int random;
  std::pair<std::string, std::string> tmp;

  for (int i = 0; i < size; i++)
  {
    random = std::rand() % size;
    tmp = list[i];
    list[i] = list[random];
    list[random] = tmp;
  }
}

Room* newRoom(const std::string &name, Room* &wall)
{
    std::ifstream file;
    std::string nm = "", tmp = "";
    int count = 0;
    std::string path = "rooms/" + name + ".txt";
    file.open(path);

    while (tmp != "$")
    {
      if (count > 1)
        nm += " ";
      tmp += nm;
      file >> tmp;
      count++;
    }

    Room* u;
    Room* d;
    Room* l;
    Room* r;
    u = NULL;
    d = NULL;
    l = NULL;
    r = NULL;

    file >> tmp;

    if (tmp[0] == 1) u = wall;
    if (tmp[1] == 1) d = wall;
    if (tmp[2] == 1) l = wall;
    if (tmp[3] == 1) r = wall;

    Room* next = new Room(nm, u, d, l, r);

    file.close();
    return next;
}

int move(std::list<std::pair<std::string, std::string> > &rooms, Room* rm, Room* &wall, char dir)
{
  rooms.push_back(std::make_pair("end", "end"));

  std::list<std::pair<std::string, std::string> >::iterator end = rooms.begin();
  while (end->first != "end") end++;
  std::list<std::pair<std::string, std::string> >::iterator itr = rooms.begin();

  int floor;

  //first case regarding moving into an already existing rooms
  switch (dir)
  {
    case 'u':
    {
      if (rm->up != NULL && rm->up != wall)
      { rm = rm->up; return 0; }
    }
    case 'd':
    {
      if (rm->down != NULL && rm->down != wall)
      { rm = rm->down; return 0; }
    }
    case 'r':
    {
      if (rm->right != NULL && rm->right != wall)
      { rm = rm->right; return 0; }
    }
    case 'l':
    {
      if (rm->left != NULL && rm->left != wall)
      { rm = rm->left; return 0; }
    }
  }

  //second case regarding discovering a new room
  if (rm->isBasement()) floor = 3;
  else if (rm->isGround()) floor = 2;
  else if (rm->isUpper()) floor = 1;
  else if (rm->isRoof()) floor = 0;

  //searching list for a discoverable room
  if (floor == 0)
    while (itr != end || (itr->second)[0] != 1 || (itr->second)[1] != 1)
    {
      rooms.push_back(std::make_pair(itr->first, itr->second));
      rooms.pop_front();
      itr = rooms.begin();
    }
  else
    while (itr != end || (itr->second)[floor] != 1)
    {
      rooms.push_back(std::make_pair(itr->first, itr->second));
      rooms.pop_front();
      itr = rooms.begin();
    }

  //if no discoverable rooms left
  if (itr == end)
    return 1;

  rooms.erase(end);

  Room* discovered = newRoom(itr->first, wall);

  rm->linkRooms(discovered, dir);

  rooms.erase(itr);
  return 0;

}

//helper method to clean up allocated floors
void clearFloor(Room* &rm, Room* &prev)
{
  if (rm->up && prev != rm->up) clearFloor(rm->up, rm);
  if (rm->down && prev != rm->down) clearFloor(rm->down, rm);
  if (rm->left && prev != rm->left) clearFloor(rm->left, rm);
  if(rm->right && prev != rm->right) clearFloor(rm->right, rm);

  delete rm;
  rm = NULL;
}

//helper method to clean up allocated memory
void deletePlayers(Character** &p, int n)
{
  for (int i = 0; i < n; i++)
    delete p[i];
  delete[] p;
}

int main()
{
  //Declaring variables
  int playerNum, choice;
  bool haunt = false;
  Character** players;
  std::ifstream file, charData;

  //intro text and determining number of players
  std::cout << "Welcome to Betrayal at the House On the Hill, a game by Avalon Hill" << std::endl;
  std::cout << "Please enter the number of players: ";
  std::cin >> playerNum;

  obtainValidNum(playerNum);

  //dynamic allocation based on number of players
  players = new Character*[playerNum];
  std::string* choices;
  choices = new std::string[playerNum];

  //CHARACTER SELECTION =================================================================================================
  //Explanation of character selection
  std::cout << "\nNow please choose characters around the table, as this will determine play order." << std::endl;
  std::cout << "Type out a character's initials to choose them, available characters will be printed on screen." << std::endl;

  //for every player
  for (int i = 0; i < playerNum; i++)
  {
    //display char options
    printCharOptions(players, file, i);
    //obtaining input until valid choice is made
    pickCharacter(players, choices, i, playerNum);
  }
  delete[] choices;
  //========================================================================================= END OF CHARACTER SELECTION
  //GAME SETUP =========================================================================================================
  //Initialization of base rooms
  Room* wall = new Room("Wall", NULL, NULL, NULL, NULL);
  Room* roof = new Room("Roof Landing", NULL, NULL, NULL, NULL); roof->setR();
  Room* upper = new Room("Upper Landing", NULL, NULL, NULL, NULL); upper->setU();
  Room* ground = new Room("Front Entrance", NULL, wall, NULL, NULL); ground->setG();
  Room* basement = new Room("Basement Landing", NULL, NULL, NULL, NULL); basement->setB();
  roof->setBelow(upper);
  upper->setAbove(roof);
  ground->linkRooms(new Room("Foyer", NULL, NULL, NULL, NULL), 'u');
  ground->up->linkRooms(new Room("Grand Staircase", wall, NULL, NULL, NULL), 'u');
  ground->up->up->setAbove(upper);
  upper->setBelow(ground->up->up);

  //Obtaining lists of cards and rooms, shuffling to begin game
  std::srand(std::time(0));
  std::string* unshuffled;
  std::pair<std::string, std::string>* unshuffledR;
  file.open("eventsBase.txt"); unshuffled = new std::string[numOfEvents]; fillStack(file, unshuffled, numOfEvents);  file.close();
  shuffle(unshuffled, numOfEvents); std::list<std::string> events; fillList(events, unshuffled, numOfEvents); delete[] unshuffled;
  file.open("omensBase.txt");  unshuffled = new std::string[numOfOmens]; fillStack(file, unshuffled, numOfOmens);  file.close();
  shuffle(unshuffled, numOfOmens); std::list<std::string> omens; fillList(omens, unshuffled, numOfOmens); delete[] unshuffled;
  file.open("itemsBase.txt");  unshuffled = new std::string[numOfItems]; fillStack(file, unshuffled, numOfItems);  file.close();
  shuffle(unshuffled, numOfItems); std::list<std::string> items; fillList(items, unshuffled, numOfItems); delete[] unshuffled;
  file.open("roomsBase.txt");  unshuffledR = new std::pair<std::string, std::string>[numOfRooms]; fillStackR(file, unshuffledR, numOfRooms);  file.close();
  shuffleR(unshuffledR, numOfRooms); std::list<std::pair<std::string, std::string> > rooms; fillListR(rooms, unshuffledR, numOfRooms); delete[] unshuffledR;

  //starting all players in front Entrance
  for (int i = 0; i < playerNum; i++)
    players[i]->setRoom(ground);
  //================================================================================================== END OF GAME SETUP
  //MAIN GAMEPLAY LOOP =================================================================================================

  int moves;
  //main loop for pre-haunt gameplay
  while (!haunt)
  {
    //player turn loop
    for (int i = 0; i < playerNum; i++)
    {
      if (!players[i]->isDead())
      {
        //printing out possible player actions
        moves = players[i]->Speed;
        do {

          std::cout << "It's " << players[i]->name << "'s (Player " << i << ") turn." << std::endl;
          std::cout << "Your action options are:" << std::endl;
          std::cout << players[i]->printActions();
          std::cout << "Enter -1 to end your turn." << std::endl;

          std::cout << "Please enter your choice: ";
          std::cin >> choice;
          if (choice >= players[i]->numActions())
            std::cout << "Please enter a valid choice.";
          else
          {
            if (choice == -1)
              break;
            else
            {
              switch (choice)
              {
                case 0:
                {
                  do {
                    std::cout << "Your movement options are:\n";
                    std::cout << players[i]->getLocation()->printRoomOptions(wall);
                    std::cout << "Enter the direction you wish to move, enter 0 to go back: ";
                    std::cin >> choice;
                    int error;
                    if (choice > players[i]->getLocation()->numOptions)
                      std::cout << "Please enter a valid choice.";
                    else
                    {
                      switch (choice)
                      {
                        case 1: //UP
                        {
                          error = move(rooms, players[i]->getLocation(), wall, 'u');
                          break;
                        }
                        case 2: //DOWN
                        {
                          error = move(rooms, players[i]->getLocation(), wall, 'd');
                          break;
                        }
                        case 3:
                        {
                          error = move(rooms, players[i]->getLocation(), wall, 'l');
                          break; //LEFT
                        }
                        case 4:
                        {
                          error = move(rooms, players[i]->getLocation(), wall, 'r');
                          break; //RIGHT
                        }
                      }

                      if (error == 1)
                      {
                        std::cout << "There are no more remaining ";
                        if (players[i]->getLocation()->isBasement()) std::cout << "BASEMENT ";
                        else if (players[i]->getLocation()->isGround()) std::cout << "GROUND FLOOR ";
                        else if (players[i]->getLocation()->isUpper()) std::cout << "UPPER FLOOR ";
                        else if (players[i]->getLocation()->isRoof()) std::cout << "ROOF ";
                        std::cout << "tiles left in the deck,\nPlease choose another option.\n" << std::endl;
                      }
                    }
                  } while (choice != 0) ;



                  break;
                }
                case 1:
                {
                  break;
                }
                case 2:
                {
                  break;
                }
                case 3:
                {
                  break;
                }
                case 4:
                {
                  break;
                }
                default:
                {
                  break;
                }
              }
            }
          }
        } while(choice != -1 && moves > 0);


      }
      else
      {
        std::cout << players[i]->name << "(Player " << i << ") is dead." << std::endl;
      }
    }
    break;
  }


  //GAME CLEANUP =======================================================================================================
  clearFloor(roof, wall);
  clearFloor(upper, wall);
  clearFloor(ground, wall);
  clearFloor(basement, wall);
  deletePlayers(players, playerNum);
  //=============================================================================================== END OF GAME CLEANUP

  return 0;
}
