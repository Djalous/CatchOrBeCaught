//
// Created by aljalousd on 4/21/2024.
//

#include <random>
#include <tuple>
#include "room.h"
#include "map.h"
#include "entity.h"

using namespace std;

bool room::checkRoom() {
  room* currentRoom = this;
  if (item) {
    item->pickup();
  } else if (hazard){
    bool willOccur = hazard->calculateProbability();

    if (willOccur) {
      currentRoom = hazard->performHazard();
    }

    if (currentRoom == nullptr) {
      return false;
    }
  } else if (Kidnapper::getKidnapper().isPresent(currentRoom)) {
    Kidnapper::getKidnapper().printEncounter();
    return false;
  } else if (ChildrenGroup::getChildren().isPresent(currentRoom)) {
    ChildrenGroup::getChildren().printEncounter();
    return false;
  }
  return true;
}

void room::checkAdjacent() {
  room* adjRooms[] = {getWest(), getSouth(), getEast(), getNorth()};
  string clues = "";
  for (room* room: adjRooms) {
    if (room) {
      if (room->hazard) {
        clues += room->hazard->clue() + "\n";
      } else if (room->item) {
        clues += room->item->clue() + "\n";
      } else if (Kidnapper::getKidnapper().isPresent(room)) {
        clues += Kidnapper::getKidnapper().clue() + "\n";
      } else if (ChildrenGroup::getChildren().isPresent(room)) {
        clues += ChildrenGroup::getChildren().clue() + "\n";
      }
    }
  }
  if (clues != "") {
    cout << clues << "******************************\n";
  }
}

bool Rat::calculateProbability() {
  random_device rd;
  mt19937 gen(rd());

  int willOccur, threshold;
  switch (Player::getPlayer().getBatteryLevel()) {
    case 0:
      return true;
    case 1:
      threshold = 75;
      break;
    case 2:
      threshold = 25;
      break;
    case 3:
      return false;
  }

  uniform_int_distribution<> dis(0, 100);
  willOccur = dis(gen);

  if (willOccur > threshold) {
    return true;
  }
  cout << "Your flashlight appears to have scared the mysterious creature away.\n";
  return false;
}

std::string Rat::clue() {
  return "You hear scurrying in the distance.";
}

room* Rat::performHazard() {
  cout << "\n===================================================\n"
          "You were attacked by a rat and ran out of the house.\n"
          "====================================================\n" << endl;
  return nullptr;
}

bool Paranoia::calculateProbability() {
  int willOccur, threshold;
  switch (Player::getPlayer().getSanityLevel()) {
    case 0:
      return true;
    case 1:
      threshold = 75;
      break;
    case 2:
      threshold = 25;
      break;
    case 3:
      cout << "Your nerves die down and your head clears.";
      return false;
  }

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, 100);
  willOccur = dis(gen);

  return willOccur > threshold;
}

string Paranoia::clue() {
  return "You feel a wave of paranoia hit you.";
}

room* Paranoia::performHazard() {
  room* playerRoom = Player::getPlayer().getRoom();
  tuple<int, int> playerCoordinates = playerRoom->getCoordinates();
  map::getMap().getLayout().at(get<0>(playerCoordinates)).at(get<1>(playerCoordinates)) =
      playerRoom->getSymbol();

  room* kidnapperRoom = Kidnapper::getKidnapper().getRoom();
  tuple<int, int> kidnapperCoordinates = kidnapperRoom->getCoordinates();
  map::getMap().getLayout().at(get<0>(kidnapperCoordinates)).at(get<1>(kidnapperCoordinates)) =
      kidnapperRoom->getSymbol();

  // Debug
  auto layout = map::getMap().getLayout();

  room* moveTo = map::getMap().getRandomRoom();
  bool isCaught = !Player::getPlayer().updateCurrent(moveTo);
  if (isCaught) {
    return nullptr;
  }

  uniform_int_distribution<> kidnapperMove(0, 3);
  random_device rd;
  mt19937 gen(rd());


  room* newRoom = nullptr;
  while (!newRoom) {
    switch (kidnapperMove(gen)) {
      case 0:
        newRoom = moveTo->getNorth();
        break;
      case 1:
        newRoom = moveTo->getEast();
        break;
      case 2:
        newRoom = moveTo->getSouth();
        break;
      case 3:
        newRoom = moveTo->getWest();
        break;
    }
  }

  Kidnapper::getKidnapper().setPosition(newRoom);
  tuple<int, int> newCoordinates = newRoom->getCoordinates();
  map::getMap().getLayout().at(get<0>(newCoordinates)).at(get<1>(newCoordinates)) = "!";

  return moveTo;
}

void Battery::pickup() {
  cout << "****************************************************\n" <<
       "You seem to have a picked up a battery lying around." << endl;
  Player::getPlayer().setBatteryLevel(3);
}

string Battery::clue() {
  return "You see something rolling on the ground nearby.";
}

void Pill::pickup() {
  cout << "You pickup what looks to be Benzodiazepines and your desperation pushes you to swallow it." << endl;
  Player::getPlayer().setSanityLevel(3);
}

string Pill::clue() {
  return "There seems to be a bottle of sorts on what appears to be a table nearby.";
}