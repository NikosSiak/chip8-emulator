#include <iostream>
#include "Chip8.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    Chip8 chip;
    chip.loadGame(argv[1]);
    return 0;
}