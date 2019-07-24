#include <iostream>
#include <time.h>
#include "Chip8.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    srand(time(nullptr));
    Chip8 chip;
    chip.loadGame(argv[1]);
    return 0;
}