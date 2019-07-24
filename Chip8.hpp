//
// Created by nikos on 22-Jul-19.
//

#ifndef CHIP8_CHIP8_HPP
#define CHIP8_CHIP8_HPP


class Chip8 {
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned char pixels[64 * 32];
    bool keys[16];

    unsigned short pc;
    unsigned short I;

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[16];
    char sp;

    unsigned short opcode; //current opcode

    bool decodeOpcode();
    void display();
    void playSound();

public:
    Chip8();
    void loadGame(const char *romPath);
    void emulate();
};


#endif //CHIP8_CHIP8_HPP
