#ifndef HUNT24_ALJALOUSD_GORANOVICM_MAP_H
#define HUNT24_ALJALOUSD_GORANOVICM_MAP_H

#include <iostream>
#include <vector>
#include <deque>
#include <tuple>
#include "room.h"

class map {
private:
  int const MAX_ROOMS = 40;
  std::vector<std::deque<std::string>> layout;
  std::vector<std::vector<room*>> rooms;

  void generateLayout();
  void generateRooms();
  std::vector<int> randomizeAdjRooms();
  int generateRandInt(int, int);
  void initialize(int, int, std::vector<std::deque<std::string>>&);
  void populateRooms(std::vector<std::vector<room*>>&);
  void positionEntities(std::vector<std::vector<room*>>&);
  map() : layout{} {
    rooms = {};
    generateLayout();
    generateRooms();
  }
  static map* gameMap;

public:
  static map& getMap() {
    if (!gameMap) {
      gameMap = new map;
    }
    return *gameMap;
  }
  map(const map&) = delete;
  map& operator=(const map&) = delete;
  ~map() {
    delete gameMap;
  }

  std::vector<std::deque<std::string>>& getLayout() { return layout; };
  std::string toString();
  room* getRandomRoom();
};

class Player {
public:
  static Player& getPlayer() {
    if (!player) {
      player = new Player;
    }
    return *player;
  }
  Player(const Player&) = delete;
  Player& operator=(const Player&) = delete;
  ~Player() {
    delete currentRoom;
    delete player;
  }

  bool move(char);
  bool updateCurrent(room* otherRoom);
  void setStartingPosition(room* start);
  void printBatteryLevel();
  void printSanityLevel();
  room* getRoom() { return currentRoom; }
  void setBatteryLevel(int level) { batteryLevel = level; };
  void setSanityLevel(int level) { sanityLevel = level; };
  int getBatteryLevel() { return batteryLevel; };
  int getSanityLevel() { return sanityLevel; };
  void lowerLevels() {
    if (batteryLevel > 0) --batteryLevel;
    if (sanityLevel > 0) --sanityLevel;
  }

private:
  static Player* player;
  Player() : currentRoom{nullptr},
             sanityLevel{3},
             batteryLevel{3} {};

  room* currentRoom;
  int sanityLevel;
  int batteryLevel;
};

#endif
