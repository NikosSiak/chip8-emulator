//
// Created by nikos on 22-Jul-19.
//

#include <algorithm>
#include <fstream>
#include "Chip8.hpp"

Chip8::Chip8() {
    unsigned char chip8_fontset[80] =
        {
                0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                0x20, 0x60, 0x20, 0x20, 0x70, // 1
                0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = -1;
    delay_timer = 0;
    sound_timer = 0;

    std::fill_n(memory, 4096, 0);
    std::fill_n(V, 16, 0);
    std::fill_n(stack, 16, 0);
    std::fill_n(pixels, 64 * 32, 0);
    std::fill_n(keys, 16, false);

    for (int i = 0; i < 80; i++){
        memory[i] = chip8_fontset[i];
    }
}

void Chip8::loadGame(const char *romPath) {
    int fsize;

    std::ifstream rom(romPath, std::ios::binary | std::ios::ate);
    if (rom.fail()){
        throw std::runtime_error("Error: file");
    }

    fsize = rom.tellg();
    fsize += 0x200;
    rom.seekg(0, std::ios::beg);

    rom.read(reinterpret_cast<char *>(memory + 0x200), fsize);

    rom.close();
}

void Chip8::emulate() {
    opcode = memory[pc] << 8 | memory[pc + 1];
    if (decodeOpcode()){
        if(delay_timer > 0) {
            delay_timer--;
        }
        if(sound_timer > 0) {
            if(sound_timer == 1) {
                playSound();
            }
            --sound_timer;
        }
        pc += 2; // TODO: check if pc is > than 4095
    }

}

bool Chip8::decodeOpcode() {
    switch (opcode & 0xf000){
        case 0x0000: {
            switch (opcode & 0x00ff) {
                case 0xe0: // Clear screen
                    std::fill_n(pixels, 64 * 32, 0);
                    break;
                case 0xee: // Return from a subroutine
                    pc = stack[sp--];
                    break;
                default:
                    printf("Unknown opcode %04x at %04x\n", opcode, pc);
                    break;
            }
            break;
        }
        case 0x1000: { // 1NNN: Jumps to address NNN
            pc = opcode & 0x0fff;
            break;
        }
        case 0x2000: { // 2NNN: Calls subroutine at NNN
            stack[++sp] = pc; // TODO: check if sp is out of bounds
            pc = opcode & 0x0fff;
            break;
        }
        case 0x3000: { // 3XNN: Skips the next instruction if VX equals NN
            if (V[(opcode & 0x0f00) >> 8] == (opcode & 0x00ff)){
                pc += 2;
            }
            break;
        }
        case 0x4000: { // 4XNN: Skips the next instruction if VX doesn't equals NN
            if (V[(opcode & 0x0f00) >> 8] != (opcode & 0x00ff)){
                pc += 2;
            }
            break;
        }
        case 0x5000: { // 5XY0: Skips the next instruction if VX equals VY
            if (V[(opcode & 0x0f00) >> 8] == V[(opcode & 0x0f00) >> 4]){
                pc += 2;
            }
            break;
        }
        case 0x6000: { // 6XNN: Sets VX to NN
            V[(opcode & 0x0f00) >> 8] = opcode & 0x00ff;
            break;
        }
        case 0x7000: { // 7XNN: Adds NN to VX (Carry flag is not changed)
            V[(opcode & 0x0f00) >> 8] += (opcode & 0x00ff);
            break;
        }
        case 0x8000: {

            break;
        }
        case 0x9000: { // 9XY0: Skips the next instruction if VX doesn't equal VY
            if (V[(opcode & 0x0f00) >> 8] != V[(opcode & 0x0f00) >> 4]){
                pc += 2;
            }
            break;
        }
        case 0xa000: { // ANNN: Sets I to the address NNN
            I = opcode & 0x0fff;
            break;
        }
        case 0xb000: { // BNNN: Jumps to the address NNN plus V0
            pc = V[0] + (opcode & 0x0fff);
            break;
        }
        case 0xc000: { // CXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN

            break;
        }
        case 0xd000: { // DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a
                       // height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory
                       // location I; I value doesn’t change after the execution of this instruction. As described
                       // above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is
                       // drawn, and to 0 if that doesn’t happen

            break;
        }
        case 0xe000: {

            break;
        }
        case 0xf000: {

            break;
        }
    }
    return true;
}

void Chip8::playSound() {
    printf("%c", 7);
}
