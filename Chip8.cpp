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

    isRunning = true;
    waitForKey = false;

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

bool Chip8::emulate(int circles) {
    for (int i = 0; i < circles; i++) {
        opcode = memory[pc] << 8 | memory[pc + 1];
        if (decodeOpcode()) {
            if (delay_timer > 0) {
                delay_timer--;
            }
            if (sound_timer > 0) {
                if (sound_timer == 1) {
                    playSound();
                }
                --sound_timer;
            }
            if (isRunning) { // might end up in endless loop
                pc += 2; // TODO: check if pc is > than 4095
                if (pc > 0xfff){
                    return false; // TODO: fix
                }
            }
        }
        else {
            return false;
        }
    }
    return true;

}

void Chip8::keyPress(char key) {
    if (waitForKey) { // if waitForKey is true it means that the opcode is FX0A
        waitForKey = false;
        isRunning = true;

        V[(opcode & 0x0f00) >> 8] = key;
    }
    keys[key] = true;
}

void Chip8::keyRelease(char key) {
    keys[key] = false;
}

void Chip8::playSound() {
    printf("%c", 7);
}

unsigned char Chip8::getPixel(int i) {
    return pixels[i];
}

bool Chip8::decodeOpcode() {
    switch (opcode & 0xf000){
        case 0x0000: {
            switch (opcode & 0x00ff) {
                case 0xe0: { // Clear screen
                    std::fill_n(pixels, 64 * 32, 0);
                    break;
                }
                case 0xee: { // Return from a subroutine
                    pc = stack[sp--];
                    break;
                }
                default: {
                    break;
                }
            }
            break;
        }
        case 0x1000: { // 1NNN: Jumps to address NNN
            pc = (opcode & 0x0fff) - 2;
            break;
        }
        case 0x2000: { // 2NNN: Calls subroutine at NNN
            stack[++sp] = pc; // TODO: check if sp is out of bounds
            pc = (opcode & 0x0fff) - 2;
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
            if (V[(opcode & 0x0f00) >> 8] == V[(opcode & 0x00f0) >> 4]){
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
            switch (opcode & 0x000f) {
                case 0x0000: { // 8XY0: Sets VX to the value of VY
                    V[(opcode & 0x0f00) >> 8] = V[(opcode & 0x00f0) >> 4];
                    break;
                }
                case 0x0001: { // 8XY1: Sets VX to VX or VY (Bitwise OR operation)
                    V[(opcode & 0x0f00) >> 8] |= V[(opcode & 0x00f0) >> 4];
                    break;
                }
                case 0x0002: { // 8XY2: Sets VX to VX and VY (Bitwise AND operation)
                    V[(opcode & 0x0f00) >> 8] &= V[(opcode & 0x00f0) >> 4];
                    break;
                }
                case 0x0003: { // 8XY3: Sets VX to VX xor VY
                    V[(opcode & 0x0f00) >> 8] ^= V[(opcode & 0x00f0) >> 4];
                    break;
                }
                case 0x0004: { // 8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
                    V[0xf] = (V[(opcode & 0x00f0) >> 4] > (0xff - V[(opcode & 0x0f00) >> 8])) ? 1 : 0;
                    V[(opcode & 0x0f00) >> 8] += V[(opcode & 0x00f0) >> 4];
                    break;
                }
                case 0x0005: { // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                    V[0xf] = (V[(opcode & 0x0f00) >> 8] > (V[(opcode & 0x00f0) >> 4])) ? 1 : 0;
                    V[(opcode & 0x0f00) >> 8] -= V[(opcode & 0x00f0) >> 4];
                    break;
                }
                case 0x0006: { // 8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1
                    V[0xf] = V[(opcode & 0x0f00) >> 8] & 1;
                    V[(opcode & 0x0f00) >> 8] >>= 1;
                    break;
                }
                case 0x0007: { // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                    V[0xf] = (V[(opcode & 0x00f0) >> 4] > (V[(opcode & 0x0f00) >> 8])) ? 1 : 0;
                    V[(opcode & 0x0f00) >> 8] = V[(opcode & 0x00f0) >> 4] - V[(opcode & 0x0f00) >> 8];
                    break;
                }
                case 0x000e: { // 8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1
                    V[0xf] = V[(opcode & 0x0f00) >> 8] >> 7;
                    V[(opcode & 0x0f00) >> 8] <<= 1;
                    break;
                }
                default: {

                    break;
                }
            }
            break;
        }
        case 0x9000: { // 9XY0: Skips the next instruction if VX doesn't equal VY
            if (V[(opcode & 0x0f00) >> 8] != V[(opcode & 0x00f0) >> 4]){
                pc += 2;
            }
            break;
        }
        case 0xa000: { // ANNN: Sets I to the address NNN
            I = opcode & 0x0fff;
            break;
        }
        case 0xb000: { // BNNN: Jumps to the address NNN plus V0
            pc = V[0] + (opcode & 0x0fff) - 2;
            break;
        }
        case 0xc000: { // CXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
            V[(opcode & 0x0f00) >> 8] = (rand() & 0x00ff) & (opcode & 0x00ff);
            break;
        }
        case 0xd000: { // DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a
            // height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory
            // location I; I value doesn’t change after the execution of this instruction. As described
            // above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is
            // drawn, and to 0 if that doesn’t happen
            unsigned char VX = V[(opcode & 0x0f00) >> 8];
            unsigned char VY = V[(opcode & 0x00f0) >> 4];
            unsigned short height = opcode & 0x000f;
            unsigned short pixel;

            V[0xf] = 0;
            for (int yline = 0; yline < height; yline++) {
                pixel = memory[I + yline];
                for (int xline = 0; xline < 8; xline++) {
                    if ((pixel & (0x80 >> xline)) != 0) {
                        if (pixels[(VX + xline + ((VY + yline) * 64))] == 1) {
                            V[0xF] = 1;
                        }
                        pixels[VX + xline + ((VY + yline) * 64)] ^= 1;
                    }
                }
            }
            break;
        }
        case 0xe000: {
            switch (opcode & 0x00ff) {
                case 0x009e: { // EX9E: Skips the next instruction if the key stored in VX is pressed
                    if (keys[V[(opcode & 0x0f00) >> 8]]){
                        pc += 2;
                    }

                    break;
                }
                case 0x00a1: { // EXA1: Skips the next instruction if the key stored in VX isn't pressed
                    if (!keys[V[(opcode & 0x0f00) >> 8]]){
                        pc += 2;
                    }

                    break;
                }
                default: {

                    break;
                }
            }
            break;
        }
        case 0xf000: {
            switch (opcode & 0x00ff) {
                case 0x0007: { // FX07: Sets VX to the value of the delay timer
                    V[(opcode & 0x0f00) >> 8] = delay_timer;
                    break;
                }
                case 0x000a: { // FX0A: A key press is awaited, and then stored in VX
                    waitForKey = true;
                    isRunning = false;
                    break;
                }
                case 0x0015: { // FX15: Sets the delay timer to VX
                    delay_timer = V[(opcode & 0x0f00) >> 8];
                    break;
                }
                case 0x0018: { // FX18: Sets the sound timer to VX
                    sound_timer = V[(opcode & 0x0f00) >> 8];
                    break;
                }
                case 0x001e: { // FX1E: Adds VX to I. VF is set to 1 when there is a range overflow
                    if (I > (0xfff - V[(opcode & 0x0f00) >> 8])){
                        V[0xf] = 1;
                    }
                    else {
                        V[0xf] = 0;
                    }
                    I += V[(opcode & 0x0f00) >> 8];
                    break;
                }
                case 0x0029: { // FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F
                    // (in hexadecimal) are represented by a 4x5 font
                    I = V[(opcode & 0x0f00) >> 8] * 5;
                    break;
                }
                case 0x0033: { // FX33: Take the decimal representation of VX, place the hundreds digit in
                    // memory at location in I, the tens digit at location I+1, and the ones digit at location I+2
                    unsigned char X = (opcode & 0x0f00) >> 8;
                    memory[I] = V[X] / 100;
                    memory[I+1] = (V[X] % 100) / 10;
                    memory[I+2] = V[X] % 10;
                    break;
                }
                case 0x0055: { // FX55: Stores V0 to VX (including VX) in memory starting at address I. The offset from
                    // I is increased by 1 for each value written, but I itself is left unmodified
                    auto X = (opcode & 0x0f00) >> 8;
                    for (auto i = 0; i <= X; i++){
                        memory[I + i] = V[X];
                    }
                    break;
                }
                case 0x0065: { // FX65: Fills V0 to VX (including VX) with values from memory starting at address I.
                    // The offset from I is increased by 1 for each value written, but I itself is left unmodified
                    auto X = (opcode & 0x0f00) >> 8;
                    for (auto i = 0; i <= X; i++){
                        V[i] = memory[I + i];
                    }
                    break;
                }
                default: {

                    break;
                }
            }
            break;
        }
        default: {

            break;
        }
    }
    return true;
}

