#include "map.h"
#include "entity.h"
#include "room.h"

#include <random>
#include <algorithm>
#include <tuple>

using namespace std;

void map::generateLayout() {
  layout.push_back({});
  layout.at(0).push_back(".");

  initialize(0, 0, layout);
}

void map::initialize(int column,
                     int row,
                     vector<deque<string>> &rooms) {
  int numRooms{1};
  while (numRooms < MAX_ROOMS) {
    vector<int> adjRooms = randomizeAdjRooms();
    int limitProbability = generateRandInt(0, 100);

    // East currentRoom
    if (adjRooms.at(0) == 1) {
      int x = column + 1;
      auto &currentDeque = rooms.at(row);
      if (currentDeque.size() == x) {
        currentDeque.push_back(".");
        ++numRooms;
      } else if (currentDeque.at(x) == " ") {
        currentDeque.at(x) = ".";
        ++numRooms;

      }
    }

    // South currentRoom
    if (adjRooms.at(1) == 1 && limitProbability > 90) {

      int y = row + 1;
      if (rooms.size() == y) {
        rooms.push_back({});
        rooms.at(y).resize(column, " ");
      }

      auto &currentDeque = rooms.at(y);
      if (currentDeque.size() < column) {
        currentDeque.resize(column, " ");
      }

      if (currentDeque.size() == column) {
        currentDeque.push_back(".");
        ++numRooms;
      } else if (currentDeque.at(column) == " ") {
        currentDeque.at(column) = ".";
        ++numRooms;
      }
    } else {
      adjRooms.at(1) = 0;
    }

    // West currentRoom
    if (adjRooms.at(2) == 1) {
      int x = column - 1;
      if (x < 0) {
        for (auto &room: rooms) {
          room.push_front(" ");
        }
        x = 0;
      }

      auto &currentDeque = rooms.at(row);
      if (currentDeque.at(x) == " ") {
        currentDeque.at(x) = ".";
        ++numRooms;

      }
    }

    std::random_device rd;

    int order[]{0, 1, 2};
    shuffle(begin(order), end(order), std::mt19937(rd()));

    string direction = "";
    for (int i = 0; i < 3; ++i) {
      if (adjRooms.at(order[i]) == 1) {
        switch (order[i]) {
          case 0:
            ++column;
            break;
          case 1:
            ++row;
            break;
          case 2:
            --column;
            if (column < 0) {
              column = 0;
            }
            break;
        }
        break;
      }
    }
  }
  cout << endl;
}

vector<int> map::randomizeAdjRooms() {
  vector<int> adjRooms(3);

  int presentCount = 0;

  while (presentCount == 0) {
    for (int i = 0; i < 3; ++i) {
      int oneOrZero = generateRandInt(0, 1);
      adjRooms.at(i) = oneOrZero;

      if (oneOrZero == 1) ++presentCount;
    }
  }

  return adjRooms;
}

int map::generateRandInt(int a, int b) {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(a, b);

  return dis(gen);
}

string map::toString() {
  string mapString = "";

  for (auto& row: layout) {
    for (string column: row) {
      mapString += column;
    }
    mapString += "\n";
  }
  return mapString;
}

void map::generateRooms() {
  for (int i = 0; i < layout.size(); ++i) {
    rooms.push_back({});
    for (int j = 0; j < layout.at(i).size(); ++j) {
      string currentStr = layout.at(i).at(j);
      if (currentStr != " ") {

        room* current = new room();
        current->setCoordinates({i, j});
        rooms.at(i).push_back(current);

        if (j > 0) {
          room *prev = rooms.at(i).at(j - 1);
          if (prev != nullptr) {
            current->setWest(prev);
          }
        }

        if (i > 0) {
          room *above = (j < rooms.at(i - 1).size()) ? rooms.at(i - 1).at(j) : nullptr;
          if (above != nullptr) {
            current->setNorth(above);
          }
        }

      } else {
        rooms.at(i).push_back(nullptr);
      }
    }
  }

  populateRooms(rooms);
}

void map::populateRooms(std::vector<std::vector<room *>>& pRooms) {
  positionEntities(pRooms);

  for (int row = 0; row < pRooms.size(); ++row) {
    for (int col = 0; col < pRooms.at(row).size(); ++col) {
      room* room = pRooms.at(row).at(col);
      if (room &&
          !ChildrenGroup::getChildren().isPresent(room) &&
          !Kidnapper::getKidnapper().isPresent(room) &&
          room != Player::getPlayer().getRoom()) {

        int roomType = generateRandInt(0, 4);
        int limitProbability = generateRandInt(0, 100);

        if (limitProbability > 40) {
          string symbol;
          switch (roomType) {
            case 0:
              room->setHazard(new Rat);
              symbol = "@";
              layout.at(row).at(col) = symbol;
              break;
            case 1:
              room->setHazard(new Paranoia);
              symbol = "B";
              layout.at(row).at(col) = symbol;
              break;
            case 2:
              symbol = "-";
              layout.at(row).at(col) = symbol;
              room->setItem(new Pill);
              break;
            case 3:
              symbol = "-";
              layout.at(row).at(col) = symbol;
              room->setItem(new Battery);
              break;
            case 4:
              symbol = ".";
          }
          room->setSymbol(symbol);
        }
      }
    }
  }
}

void map::positionEntities(vector<std::vector<room *>>& pRooms) {
  string characters[] = {"Player", "Kidnapper", "ChildrenGroup"};

  Player& player = Player::getPlayer();
  Kidnapper& kidnapper = Kidnapper::getKidnapper();
  ChildrenGroup& children = ChildrenGroup::getChildren();

  for (string character: characters) {
    int row, col;
    room* assigned;
    do {
      row = generateRandInt(0, pRooms.size() - 1);
      col = generateRandInt(0, pRooms.at(row).size() - 1);
      assigned = pRooms.at(row).at(col);
    } while (!assigned ||
             kidnapper.isPresent(assigned) ||
             children.isPresent(assigned) ||
             assigned == player.getRoom());

    assigned->setSymbol(".");
    if (character == "Player") {
      player.setStartingPosition(assigned);
      layout.at(row).at(col) = "P";
    } else if (character == "Kidnapper") {
      kidnapper.setPosition(assigned);
      layout.at(row).at(col) = "!";
    } else if (character == "ChildrenGroup") {
      children.setPosition(assigned);
      layout.at(row).at(col) = "?";
    }
  }
}

room *map::getRandomRoom() {
  int row, col;
  room* assigned;
  do {
    row = generateRandInt(0, layout.size() - 1);
    col = generateRandInt(0, layout.at(row).size()-1);
    assigned = rooms.at(row).at(col);
  } while (!assigned);

  return assigned;
}

bool Player::move(char direction) {
  room* nextRoom;
  tuple<int, int> playerCoordinates = currentRoom->getCoordinates();
  map::getMap().getLayout().at(get<0>(playerCoordinates)).at(get<1>(playerCoordinates)) =
      currentRoom->getSymbol();

  switch (direction) {
    case 'n':
      nextRoom = currentRoom->getNorth();
      break;
    case 'e':
      nextRoom = currentRoom->getEast();
      break;
    case 's':
      nextRoom = currentRoom->getSouth();
      break;
    case 'w':
      nextRoom = currentRoom->getWest();
      break;
  }

  if (nextRoom) {
    return updateCurrent(nextRoom);
  }
  cout << "You seem to have hit a wall.\n";
  return true;
}

bool Player::updateCurrent(room* otherRoom) {
  currentRoom = otherRoom;
  bool isNotCaught = currentRoom->checkRoom();
  tuple<int, int> newCoordinates = currentRoom->getCoordinates();
  map::getMap().getLayout().at(get<0>(newCoordinates)).at(get<1>(newCoordinates)) = "P";
  return isNotCaught;
}

void Player::printBatteryLevel() {
  string battery = "";
  switch (batteryLevel) {
    case 0:
      battery = "DEAD";
      break;
    case 1:
      battery = "LOW";
      break;
    case 2:
      battery = "MEDIUM";
      break;
    case 3:
      battery = "HIGH";
      break;
  }
  cout << "Battery Level: " << battery << endl;
}

void Player::setStartingPosition(room *start) {
  currentRoom = start;
}

void Player::printSanityLevel() {
  string sanity = "";
  switch (sanityLevel) {
    case 0:
      sanity = "INSANE";
      break;
    case 1:
      sanity = "UNSTABLE";
      break;
    case 2:
      sanity = "STRESSED";
      break;
    case 3:
      sanity = "SANE";
  }
  cout << "Sanity Level: " << sanity << endl;
}
