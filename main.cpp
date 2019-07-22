#include <iostream>
#include "Chip8.hpp"
#include "Disassembler.hpp"

using namespace std;

int main(int argc, char *argv[]) {
//    Disassembler::disassembleSource(argv[1]);
    Chip8 chip;
    chip.loadGame(argv[1]);
    return 0;
}