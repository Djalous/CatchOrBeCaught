//
// Created by aljalousd on 4/21/2024.
//

#ifndef HUNT24_ALJALOUSD_GORANOVICM_ENTITY_H
#define HUNT24_ALJALOUSD_GORANOVICM_ENTITY_H


#include "room.h"

class Kidnapper {
public:
  bool isPresent(room* otherRoom) { return currentRoom == otherRoom; }
  void printEncounter();
  void setPosition(room* room) { currentRoom = room; };
  std::tuple<int, int> getPosition() { return currentRoom->getCoordinates(); }
  room* getRoom() { return currentRoom; }
  std::string clue();

  static Kidnapper& getKidnapper() {
    if (!kidnapper) {
      kidnapper = new Kidnapper;
    }
    return *kidnapper;
  };
  Kidnapper(const Kidnapper&) = delete;
  Kidnapper& operator=(const Kidnapper&) = delete;

private:
  Kidnapper() { currentRoom = nullptr; }
  static Kidnapper* kidnapper;
  room* currentRoom;
};

class ChildrenGroup {
private:
  room* currentRoom;
  ChildrenGroup() {};
  static ChildrenGroup* children;

public:
  static ChildrenGroup& getChildren() {
    if (!children) {
      children = new ChildrenGroup;
    }
    return *children;
  }
  ChildrenGroup(const ChildrenGroup&) = delete;
  ChildrenGroup& operator=(const ChildrenGroup&) = delete;

  bool isPresent(room* otherRoom){ return currentRoom == otherRoom; };
  std::string clue();
  void printEncounter();
  void setPosition(room* newRoom) { currentRoom = newRoom; };
};


#endif //HUNT24_ALJALOUSD_GORANOVICM_ENTITY_H
