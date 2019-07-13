#include <iostream>
#include "Disassembler.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    Disassembler::disassembleSource(argv[1]);
    return 0;
}