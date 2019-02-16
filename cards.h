#ifndef _cards_h
#define _cards_h

class Character;

class Card {
public:
  std::string name;

  Card()
  {

  }

  std::string read()
  {
    return "";
  }
};

class ItemCard : public Card {
public:
  Character* owner;

  ItemCard(Character* &own) : Card()
  {
    owner = own;
  }
};

class EventCard : public Card {
public:
  bool isPermanent;

  EventCard() : Card()
  {

  }
};

class OmenCard : public Card {
public:
  Character* owner;

  OmenCard(Character* &own) : Card ()
  {
    owner = own;
  }
};

#endif
