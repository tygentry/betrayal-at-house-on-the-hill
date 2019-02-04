#include <iostream>

#include "character.h"

int static numOfChars = 12;

std::string printLine(std::ifstream &f)
{
  std::string out = "\033[0;91m";
  std::string tmp;
  for (int i = 0; i < 7; i++)
  {
    if (i == 0)
    {
      f >> tmp;
      out += tmp + "\033[0m ";
    }
    f >> tmp;
    if (tmp.substr(0,1) == "-")
    {
      out += "\033[0;92m" + tmp.substr(1) + "\033[0m ";
    }
    else
    {
      out += tmp + " ";
    }

  }
  return out;
}

void printCharOptions(Character** &players, std::ifstream &file, int i)
{
  bool skip;
  file.open("charactersBase.txt");
  std::string firstN, lastN, color, age, name;

  for (int j = 0; j < numOfChars; j++)
  {
    skip = false;
    name = "\n\033[0;";
    file >> firstN >> lastN >> color >> age;

    std::string charN = "\033[0;" + color + "m" + firstN + " " + lastN + "\033[0m";
    for (int k = 0; k < i; k++)
    {
      if (charN == players[k]->name)
        skip = true;
    }

    if (!skip)
    {
      name += color + "m" + firstN + " " + lastN + ", age " + age + "\033[0m";
      std::cout <<  name;
      std::cout << "\nSanity:    " << printLine(file);
      std::cout << "\nKnowledge: " << printLine(file);
      std::cout << "\nMight:     " << printLine(file);
      std::cout << "\nSpeed:     " << printLine(file) << std::endl;
    }
    else
    {
      printLine(file);
      printLine(file);
      printLine(file);
      printLine(file);
    }

  }
}

void deletePlayers(Character** &p, int n)
{
  for (int i = 0; i < n; i++)
  {
    delete p[i];
  }

  delete[] p;
}

int main()
{
  int playerNum;
  bool skip;
  Character** players;
  std::ifstream file, charData;
  std::string path, choice;

  std::cout << "Welcome to Betrayal at the House On the Hill, a game by Avalon Hill" << std::endl;
  std::cout << "Please enter the number of players: ";
  std::cin >> playerNum;

  players = new Character*[playerNum];
  std::string choices[playerNum];

  std::cout << "\nNow please choose characters around the table, as this will determine play order." << std::endl;
  std::cout << "Type out a character's initials to choose them, available characters will be printed on screen." << std::endl;

  for (int i = 0; i < playerNum; i++)
  {
    printCharOptions(players, file, i);

    std::cout << "\nEnter Choice's Initials: ";
    while(1)
    {
      skip = false;
      path = "characters/";
      std::cin >> choice;

      for (int j = 0; j < i; j++)
      {
        if (choice == choices[j])
          skip = true;
      }

      if (skip)
       std::cout << "That character has already been chosen, please enter a set of initials of the characters on screen.\n";
      else
      {
        path += choice + ".txt";
        charData.open(path);

        if (charData.is_open())
        {
          players[i] = new Character(charData);
          std::cout << "You chose: " << players[i]->name << std::endl;
          choices[i] = choice;
          charData.close();

          if (i < playerNum-1)
          {
            std::cout << "Type anything and press ENTER to choose for next player.";
            std::cin >> path;
          }

          break;
        }
        else
        {
          std::cout << "Initials " << choice.substr(0, 2) << " are not valid, please enter a valid set of initials.\n";
        }
      }

    }


    file.close();
  }

  deletePlayers(players, playerNum);

  return 0;
}
