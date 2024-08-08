//
// Created by aljalousd on 4/21/2024.
//

#ifndef HUNT24_ALJALOUSD_GORANOVICM_ROOM_H
#define HUNT24_ALJALOUSD_GORANOVICM_ROOM_H


#include <string>
#include <tuple>

class room;

class Hazard {
public:
  virtual bool calculateProbability() = 0;
  virtual std::string clue() = 0;
  virtual room* performHazard() = 0;
  virtual ~Hazard() { delete this; };
};

class Rat : public Hazard {
public:
  bool calculateProbability() override;
  std::string clue() override;
  room* performHazard() override;
};

class Paranoia: public Hazard {
public:
  bool calculateProbability() override;
  std::string clue() override;
  room* performHazard() override;
};

class Item {
public:
  virtual void pickup() = 0;
  virtual std::string clue() = 0;
  virtual ~Item() { delete this; };
};

class Battery : public Item {
public:
  void pickup();
  std::string clue();
};

class Pill : public Item {
public:
  void pickup();
  std::string clue();
};

class room {
private:
  room* northRoom;
  room* eastRoom;
  room* southRoom;
  room* westRoom;
  Hazard* hazard;
  Item* item;
  std::tuple<int, int> coordinates;
  std::string symbol;

public:
  room() : westRoom{nullptr},
           eastRoom{nullptr},
           southRoom{nullptr},
           northRoom{nullptr},
           hazard{nullptr},
           item{nullptr},
           symbol{""} {}
  ~room() {
    delete westRoom;
    delete northRoom;
    delete southRoom;
    delete eastRoom;
    delete hazard;
    delete item;
    delete this;
  }
  bool checkRoom();
  void checkAdjacent();
  room* getSouth() { return southRoom; }
  room* getWest() { return westRoom; }
  room* getNorth() { return northRoom; }
  room* getEast() { return eastRoom; }
  void setNorth(room* room) {
    room->southRoom = this;
    northRoom = room;
  }
  void setWest(room* room) {
    room->eastRoom = this;
    westRoom = room;
  }
  void setHazard(Hazard* h) { this->hazard = h; }
  void setItem(Item* i) { this->item = i; }
  void setCoordinates(std::tuple<int, int> newCoordinates) {coordinates = newCoordinates; }
  std::tuple<int, int> getCoordinates() { return coordinates; }
  void setSymbol(std::string newSymbol) {
    symbol = newSymbol;
  };
  std::string getSymbol() { return symbol; };
};


#endif //HUNT24_ALJALOUSD_GORANOVICM_ROOM_H
