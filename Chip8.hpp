//
// Created by nikos on 22-Jul-19.
//

#ifndef CHIP8_CHIP8_HPP
#define CHIP8_CHIP8_HPP


class Chip8 {
    unsigned char mem[4096];
    unsigned char V[16];
    unsigned char screen[64 * 32];
    bool keys[16];

    unsigned short pc;
    unsigned short I;

    unsigned char delay_timer;
    unsigned char soundTimer;

    unsigned short stack[16];
    unsigned short sp;

    unsigned short opcode; //current opcode

public:
    Chip8();
    bool loadGame(const char *romPath);
};


#endif //CHIP8_CHIP8_HPP
