//
// Created by aljalousd on 4/21/2024.
//

#include <iostream>
#include "entity.h"

using namespace std;

string ChildrenGroup::clue() {
  return "You hear hushed cries nearby.";
}

void ChildrenGroup::printEncounter() {
  cout << "*********************************************************************\n"
       << "You found the kids and quickly call the police! They're on their way!";
}

void Kidnapper::printEncounter() {
  cout << "************************************************************************************\n"
       << "You seem to get dizzy and your vision blurs. A man drags you away and you black out.";
}

string Kidnapper::clue() {
  return "You hear quiet breathing nearby.";
}