#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>

#include "character.h"
#include "cards.h"
#include "room.h"

//Static variables to update and allow expansion of game contents
int static numOfChars = 13;
int static numOfRooms = 61;
int static numOfEvents = 56;
int static numOfOmens = 21;
int static numOfItems = 34;
int static baseSideOfFloor = 25;

Room*** basementF;
Room*** groundF;
Room*** upperF;
Room*** roofF;

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

void fillFloor(Room*** &f)
{
  f = new Room**[baseSideOfFloor];
  for (int i = 0; i < baseSideOfFloor; i++)
  {
    f[i] = new Room*[baseSideOfFloor];
    for (int j = 0; j < baseSideOfFloor; j++)
      f[i][j] = NULL;
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

//helper function to move method to create instance of Room and setting appropriate variables
//also deals with any immediate actions necessary for a room discovery (i.e. item, event, and
//omen cards)
Room* newRoom(const std::string &name, Room* &wall, std::vector<std::list<std::string> > &cards)
{
    //initializing variables and opening room file
    std::ifstream file;
    std::string nm = "", tmp = "";
    int count = 0;
    std::string path = "rooms/" + name + ".txt";
    file.open(path);

    //obtaining name
    while (tmp != "$")
    {
      if (count >= 2)
        nm += " ";
      nm += tmp;
      file >> tmp;
      count++;
    }

    //initializing pointers
    Room* u;
    Room* d;
    Room* l;
    Room* r;
    u = NULL;
    d = NULL;
    l = NULL;
    r = NULL;

    //obtaining determining door orientation on room
    file >> tmp;
    int numO = 0;
    if (tmp[0] == '0') u = wall;
    else numO++;
    if (tmp[1] == '0') d = wall;
    else numO++;
    if (tmp[2] == '0') l = wall;
    else numO++;
    if (tmp[3] == '0') r = wall;
    else numO++;

    //creating room
    Room* next = new Room(nm, u, d, l, r, -1, -1, wall);
    next->numOptions += numO;

    //dealing with various cards on room discovery
    std::string card, cardName = "";
    std::list<std::string>::iterator itr;
    file >> card;
    int i = 0;
    while (card[i] != 'n')
    {
      switch (card[i])
      {
        case 'o': //omen
        {
          path = "cards/omens/";
          itr = cards[1].begin();
          cardName = *itr;
          cards[1].erase(itr);
          break;
        }
        case 'e': //event
        {
          path = "cards/events/";
          itr = cards[0].begin();
          cardName = *itr;
          cards[0].erase(itr);
          break;
        }
        case 'i': //item
        {
          path = "cards/items/";
          itr = cards[2].begin();
          cardName = *itr;
          cards[2].erase(itr);
          break;
        }
        case 'd': //dumbwaiter
        {
          next->setDumbwaiter();
          break;
        }
      }
      i++;
    }

    if (cardName != "")
    {
      path += cardName + ".txt";
      std::list<std::string> tmp; tmp.push_back(path);
      cards.push_back(tmp);
    }

    file.close();
    return next;
}

//helper function to move method that determines most optimal orientation of the newly discovered room
//optimal is defined as the most possible connected rooms
//rotates and shifts pointers to correctly orientate room after optimization has been made
void optimizeRoom(Room* &rmR, Room* &rmC, Room* &wall, Room*** &floor, char dir)
{
  //initializing variables
  int nullValid, rmValid, connect, numMax = 0, maxD = 0;
  Room* tmp;
  Room* u = rmR->up;
  Room* d = rmR->down;
  Room* l = rmR->left;
  Room* r = rmR->right;

  //loops through all 4 possible orientations to determine the number of connected possible rooms
  for (int i = 0; i < 4; i++)
  {
    nullValid = 0;
    rmValid = 0;
    //upcheck
    if (u != wall && rmR->row > 0)
    {
      if (floor[rmR->row-1][rmR->col] == NULL)
        nullValid++;
      else
        if (floor[rmR->row-1][rmR->col]->down == NULL || floor[rmR->row-1][rmR->col]->down == rmR)
          rmValid++;

      if (dir == 'd')
        rmValid++;
    }
    //downcheck
    if (d != wall && rmR->row < 24)
    {
      if (floor[rmR->row+1][rmR->col] == NULL)
        nullValid++;
      else
        if (floor[rmR->row+1][rmR->col]->up == NULL || floor[rmR->row+1][rmR->col]->up == rmR)
          rmValid++;

      if (dir == 'u')
        rmValid++;
    }
    //leftcheck
    if (l != wall && rmR->col > 0)
    {
      if (floor[rmR->row][rmR->col-1] == NULL)
        nullValid++;
      else
        if (floor[rmR->row][rmR->col-1]->right == NULL || floor[rmR->row][rmR->col-1]->right == rmR)
          rmValid++;

      if (dir == 'r')
        rmValid++;
    }
    //rightcheck
    if (r != wall && rmR->col < 24)
    {
      if (floor[rmR->row][rmR->col+1] == NULL)
        nullValid++;
      else
        if (floor[rmR->row][rmR->col+1]->left == NULL || floor[rmR->row][rmR->col+1]->left == rmR)
          rmValid++;

      if (dir == 'l')
        rmValid++;
    }

    connect = nullValid + (rmValid * 2);
    //altering max if new max
    if (connect > numMax)
    { numMax = connect;
      maxD = i; }

    //rotating pointers
    tmp = u;
    u = l;
    l = d;
    d = r;
    r = tmp;
  }

  //switch to determine the orientation based on the optimized orientation
  //shifts pointers to match
  switch (maxD)
  {
    case 0: // 0 degree rotated
    {
      rmR->linkSurrounding(wall, floor);
      break;
    }
    case 1: // 90 degrees rotated
    {
      tmp = rmR->up;
      rmR->up = rmR->left;
      rmR->left = rmR->down;
      rmR->down = rmR->right;
      rmR->right = tmp;
      rmR->linkSurrounding(wall, floor);
      break;
    }
    case 2: // 180 degrees rotated
    {
      tmp = rmR->up;
      rmR->up = rmR->down;
      rmR->down = tmp;
      tmp = rmR->right;
      rmR->right = rmR->left;
      rmR->left = tmp;
      rmR->linkSurrounding(wall, floor);
      break;
    }
    case 3: // 270 degrees rotated
    {
      tmp = rmR->up;
      rmR->up = rmR->right;
      rmR->right = rmR->down;
      rmR->down = rmR->left;
      rmR->left = tmp;
      rmR->linkSurrounding(wall, floor);
      break;
    }
  }
}

//main helper method to execute basic movement operation
//searches the passed rooms list for the next possible room that fits criteria
//creates that room using additional helper methods, and places it in the house
//based on optimal positioning
Room* move(std::list<std::pair<std::string, std::string> > &rooms, Room* rm, Room* &wall, char dir, std::vector<std::list<std::string> > &cards)
{
  //first case regarding moving into an already existing rooms
  switch (dir)
  {
    case 'u': //UP
    {
      if (rm->up != NULL && rm->up != wall)
      { return rm->up; }
      break;
    }
    case 'd': //DOWN
    {
      if (rm->down != NULL && rm->down != wall)
      { return rm->down; }
      break;
    }
    case 'r': //RIGHT
    {
      if (rm->right != NULL && rm->right != wall)
      { return rm->right; }
      break;
    }
    case 'l': //LEFT
    {
      if (rm->left != NULL && rm->left != wall)
      { return rm->left; }
      break;
    }
    case 'a': //ABOVE
    {
      if (rm->above != NULL && rm->above != wall)
      { return rm->above; }
      break;
    }
    case 'b': //BELOW
    {
      if (rm->below != NULL && rm->below != wall)
      { return rm->below; }
      break;
    }
  }
  //addition of end pair to prevent infinite looping
  rooms.push_back(std::make_pair("end", "end"));

  //declares iterators for comparison to prevent infinite looping
  std::list<std::pair<std::string, std::string> >::iterator end = rooms.begin();
  while (end->first != "end") end++;
  std::list<std::pair<std::string, std::string> >::iterator itr = rooms.begin();

  int floor;

  Room*** floorGrid;
  //second case regarding discovering a new room
  if (rm->isBasement()) { floor = 3; floorGrid = basementF; }
  else if (rm->isGround()) { floor = 2; floorGrid = groundF; }
  else if (rm->isUpper()) { floor = 1; floorGrid = upperF; }
  else if (rm->isRoof()) { floor = 0; floorGrid = roofF; }

  //searching list for a discoverable room
  //special case regarding roof, includes checking for both roof and upper
  if (floor == 0)
    while (itr != end && ((itr->second)[0] != '1' || (itr->second)[1] != '1'))
    {
      rooms.push_back(std::make_pair(itr->first, itr->second));
      rooms.pop_front();
      itr = rooms.begin();
    }
  else //other basic cases for not roof
    while (itr != end && (itr->second)[floor] != '1')
    {
      rooms.push_back(std::make_pair(itr->first, itr->second));
      rooms.pop_front();
      itr = rooms.begin();
    }

  //if no discoverable rooms left
  if (itr == end)
    return NULL;

  rooms.erase(end);

  //create the new room and deal with room data
  Room* discovered = newRoom(itr->first, wall, cards);

  //optimize positioning of the room, rotating if needed
  rm->linkRooms(discovered, dir, wall);
  optimizeRoom(discovered, rm, wall, floorGrid, dir);
  floorGrid[discovered->row][discovered->col] = discovered;

  rooms.erase(itr);
  return discovered;

}

void clearGrid(Room*** &f)
{
  for (int i = 0; i < baseSideOfFloor; i++)
    delete[] f[i];
  delete[] f;
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
  std::vector<Room*> roomVec;

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
  fillFloor(basementF);
  fillFloor(groundF);
  fillFloor(upperF);
  fillFloor(roofF);

  Room* wall = new Room("Wall", NULL, NULL, NULL, NULL, -1, -1, NULL); roomVec.push_back(wall);
  Room* roof = new Room("Roof Landing", NULL, NULL, NULL, NULL, 12, 12, wall); roof->setR(); roomVec.push_back(roof);
  Room* upper = new Room("Upper Landing", NULL, NULL, NULL, NULL, 12, 12, wall); upper->setU(); roomVec.push_back(upper);
  Room* ground = new Room("Front Entrance", NULL, wall, NULL, NULL, 14, 12, wall); ground->setG(); roomVec.push_back(ground);
  Room* basement = new Room("Basement Landing", NULL, NULL, NULL, NULL, 12, 12, wall); basement->setB(); roomVec.push_back(basement);
  roof->setBelow(upper);
  upper->setAbove(roof);
  ground->linkRooms(new Room("Foyer", NULL, NULL, NULL, NULL, 13, 12, wall), 'u', wall); roomVec.push_back(ground->up); ground->up->down = ground;
  ground->up->linkRooms(new Room("Grand Staircase", wall, NULL, wall, wall, 12, 12, wall), 'u', wall); roomVec.push_back(ground->up->up); ground->up->up->down = ground->up;
  ground->up->up->setAbove(upper);
  upper->setBelow(ground->up->up);
  roofF[12][12] = roof;
  upperF[12][12] = upper;
  groundF[14][12] = ground;
  groundF[13][12] = ground->up;
  groundF[12][12] = ground->up->up;
  basementF[12][12] = basement;

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
  std::vector<std::list<std::string> > cards; cards.push_back(events); cards.push_back(omens); cards.push_back(items);

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
        moves = players[i]->getSpeed();
        do {

          std::cout << "It's " << players[i]->name << " (Player " << i << ")'s turn." << std::endl;
          std::cout << "Your action options are:" << std::endl;
          std::cout << players[i]->printActions();
          std::cout << "Enter -1 to end your turn." << std::endl;

          std::cout << "Please enter your choice: ";
          std::cin >> choice;
          obtainValidNum(choice);
          if (choice >= players[i]->numActions())
            std::cout << "Please enter a valid choice." << std::endl;
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
                  Room* moved = wall;
                  std::string n = "\033[0;" + players[i]->color + "m" + players[i]->getLocation()->name + "\033[0m";

                  do {
                    //check to see if more movement is possible
                    if (moves == 0)
                    {
                      std::cout << "You are out of moves, you cannot continue moving." << std::endl;
                      break;
                    }

                    moved = wall;

                    std::cout << "You are in the " << n  << "." << std::endl;
                    std::cout << "Your movement options are:" << std::endl;
                    std::cout << players[i]->getLocation()->printRoomOptions(wall);
                    std::cout << "You have " << moves << " Speed remaining." << std::endl;
                    std::cout << "Enter the direction you wish to move, enter 0 to go back: ";
                    std::cin >> choice;
                    obtainValidNum(choice);

                    switch (choice)
                    {
                      case 1: //UP
                      {
                        if (players[i]->getLocation()->up == wall)
                          std::cout << "Please enter a valid choice." << std::endl;
                        else
                          moved = move(rooms, players[i]->getLocation(), wall, 'u', cards);
                        break;
                      }
                      case 2: //DOWN
                      {
                        if (players[i]->getLocation()->down == wall)
                          std::cout << "Please enter a valid choice." << std::endl;
                        else
                          moved = move(rooms, players[i]->getLocation(), wall, 'd', cards);
                        break;
                      }
                      case 3: //LEFT
                      {
                        if (players[i]->getLocation()->left == wall)
                          std::cout << "Please enter a valid choice." << std::endl;
                        else
                          moved = move(rooms, players[i]->getLocation(), wall, 'l', cards);
                        break;
                      }
                      case 4: //RIGHT
                      {
                        if (players[i]->getLocation()->right == wall)
                          std::cout << "Please enter a valid choice." << std::endl;
                        else
                          moved = move(rooms, players[i]->getLocation(), wall, 'r', cards);
                        break;
                      }
                      case 5: //ABOVE
                      {
                        if (players[i]->getLocation()->above == wall)
                          std::cout << "Please enter a valid choice." << std::endl;
                        else
                          moved = move(rooms, players[i]->getLocation(), wall, 'a', cards);
                        break;
                      }
                      case 6: //BELOW
                      {
                        if (players[i]->getLocation()->below == wall)
                          std::cout << "Please enter a valid choice." << std::endl;
                        else
                          moved = move(rooms, players[i]->getLocation(), wall, 'b', cards);
                        break;
                      }
                      case 0: //EXIT
                        break;
                      default:
                      {
                        std::cout << "Please enter a valid choice." << std::endl;
                        break;
                      }
                    }

                    if (moved == NULL)
                    {
                      std::cout << "There are no more remaining ";
                      if (players[i]->getLocation()->isBasement()) std::cout << "BASEMENT ";
                      else if (players[i]->getLocation()->isGround()) std::cout << "GROUND FLOOR ";
                      else if (players[i]->getLocation()->isUpper()) std::cout << "UPPER FLOOR ";
                      else if (players[i]->getLocation()->isRoof()) std::cout << "ROOF ";
                      std::cout << "tiles left in the deck,\nPlease choose another option.\n" << std::endl;
                    }
                    else if (moved != wall)
                    {
                      roomVec.push_back(moved);

                      players[i]->setRoom(moved);
                      n = "\033[0;" + players[i]->color + "m" + players[i]->getLocation()->name + "\033[0m";
                      std::cout << "You have moved into the " << n << "." << std::endl;
                      moves--;

                      if (cards.size() == 4)
                      {
                        std::list<std::string>::iterator tmp; tmp = cards[3].begin();
                        std::ifstream card;
                        card.open(*tmp);
                        cards.pop_back();

                        //INSERT CARD FUNCTIONALITY HERE

                        moves = 0;
                        card.close();
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
        } while(choice != -1);


      }
      else
      {
        std::cout << players[i]->name << "(Player " << i << ") is dead." << std::endl;
      }
    }
    break;
  }


  //GAME CLEANUP =======================================================================================================
  std::cout << roomVec.size() << std::endl;
  for (unsigned int i = 0; i < roomVec.size(); i++)
  {
    if (roomVec[i]) delete roomVec[i];
  }

  clearGrid(roofF);
  clearGrid(upperF);
  clearGrid(groundF);
  clearGrid(basementF);
  deletePlayers(players, playerNum);
  //=============================================================================================== END OF GAME CLEANUP

  return 0;
}
