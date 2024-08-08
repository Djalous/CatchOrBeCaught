#include "map.h"
#include "game_driver.h"
#include <iostream>

using namespace std;

int main() {
    gameDriver *driver = new gameDriver();
    driver->prompt();
    return 0;
}