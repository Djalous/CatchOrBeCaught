//
// Created by aljalousd on 4/21/2024.
//

#include "game_driver.h"
#include "entity.h"
#include "map.h"

using namespace std;

ChildrenGroup* ChildrenGroup::children = nullptr;
Kidnapper* Kidnapper::kidnapper = nullptr;
Player* Player::player = nullptr;
map* map::gameMap = nullptr;

gameDriver::gameDriver() { }

void gameDriver::prompt() {
  char input = '\0';
  bool gameFinished = false, inDebugMode = false;
  int moves;
  map::getMap();

  while(input != 'q' && !gameFinished) {
    cout << "***********************************\n";
    Player::getPlayer().printBatteryLevel();
    Player::getPlayer().printSanityLevel();
    cout << "***********************************\n";

    if (inDebugMode) {
      cout << map::getMap().toString();
      cout << "*********************************\n";
    }

    Player::getPlayer().getRoom()->checkAdjacent();
    cout << "Action: N)orth, S)outh, E)ast, W)est, M)ap, D)ebug [On/Off], H)elp, Q)uit: ";
    cin >> input;
    input = tolower(input);
    if(input == 'n' || input == 's' || input == 'e' || input == 'w') {
      cout << "You selected " << input << endl << endl;
      if (moves == 2) {
        Player::getPlayer().lowerLevels();
        moves = 0;
      }
      gameFinished = !Player::getPlayer().move(input);
      ++moves;
    } else if(input == 'h') {
      help();
    } else if (input == 'd') {
      cout << "You selected " << input << endl;
      inDebugMode = true;
    } else if (input == 'm') {
      cout << "You selected " << input << endl;
      cout << map::getMap().toString();
    } else if(input != 'q') {
      cout << "Invalid command." << endl;
    }

    input = '\0';
  }
}

void gameDriver::help() {
  cout << "The goal of the game is to find the kidnapped children without getting caught by the kidnapper. "
          "All the children are together in the same currentRoom and the kidnapper is in a separate currentRoom, away from "
          "the children. Specific clues will be provided to inform you when you are near the children or "
          "kidnapper. If the children are in an adjacent currentRoom, you will hear cries. If the kidnapper is in "
          "an adjacent currentRoom, you will hear murmurs from the kidnapper speaking to themselves. You will be "
          "informed if you ultimately enter the currentRoom with the children or kidnapper.\n"

          "You may encounter hazards along your way through the house, namely rooms infested with rats and "
          "rooms that trigger your paranoia. When you enter a currentRoom infested with rats, you will escape from "
          "the house out of fear. When you enter a currentRoom that triggers your paranoia, you will unconsciously "
          "run to a random currentRoom to hide. Specific clues will be provided to inform you when a hazard is in "
          "an adjacent currentRoom. If an adjacent currentRoom is infested with rats, you will begin to hear chewing and "
          "scurrying sounds. If an adjacent currentRoom is paranoia-triggering, you will begin to hear shuffles and "
          "footsteps. You will be informed if you ultimately enter a hazardous currentRoom.\n"

          "You may encounter items to help you overcome these hazards. Batteries will keep your flashlight "
          "on and thereby repel the rats; benzodiazepines will reduce your paranoia and thereby keep you "
          "composed. Specific clues will be provided to inform you when these items are found. Once an item "
          "is found, you will pick them up and either replenish the battery level in the flashlight or "
          "experience your paranoic thoughts die down." << endl;
}