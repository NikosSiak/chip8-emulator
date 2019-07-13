//
// Created by nikos on 13-Jul-19.
//

#include "Disassembler.hpp"
#include <iostream>
#include <fstream>

using namespace std;

void Disassembler::disassembleSource(char *romName) {
    char *buffer;
    int pc = 0x200;
    int fsize;
    uint8_t firstnib;
    uint8_t instruction[2];

    ifstream file(romName, ios::binary | ios::ate);

    fsize = file.tellg();
    fsize += 0x200;
    file.seekg(0, ios::beg);

    buffer = new char[fsize];
    file.read(buffer, fsize);

    file.close();

    while (pc < fsize){
        instruction[0] = buffer[pc];
        instruction[1] = buffer[pc+1];
        firstnib = instruction[0] >> 4u;

        printf("%04x %02x %02x ", pc, instruction[0], instruction[1]);

        switch (firstnib){
            case 0x0:
                switch (instruction[1]){
                    case 0xe0:  printf("%-10s", "CLS"); break;
                    case 0xee:  printf("%-10s", "RET"); break;
                    default:    printf("%-10s %01x%02x", "CALL_RCA", instruction[0] & 0xfu, instruction[1]); break;
                }
                break;
            case 0x1:   printf("%-10s $%01x%02x", "JUMP", instruction[0] & 0xfu, instruction[1]); break;
            case 0x2:   printf("%-10s %01x%02x", "CALL", instruction[0] & 0xfu, instruction[1]); break;
            case 0x3:   printf("%-10s V%01X, %02x", "SKIP.EQI", instruction[0] & 0xfu, instruction[1]); break;
            case 0x4:   printf("%-10s V%01X, %02x", "SKIP.NEI", instruction[0] & 0xfu, instruction[1]); break;
            case 0x5:   printf("%-10s V%01X, V%01X", "SKIP.EQ", instruction[0] & 0xfu, instruction[1] >> 4u); break;
            case 0x6:   printf("%-10s V%01X, %02x", "SETI", instruction[0] & 0xfu, instruction[1]); break;
            case 0x7:   printf("%-10s V%01X, %02x", "ADDI", instruction[0] & 0xfu, instruction[1]); break;
            case 0x8:
                switch (instruction[1] & 0xfu){
                    case 0x0:   printf("%-10s V%01X, V%01X", "SET", instruction[0] & 0xfu, instruction[1] >> 4u); break;
                    case 0x1:   printf("%-10s V%01X, V%01X", "OR", instruction[0] & 0xfu, instruction[1] >> 4u); break;
                    case 0x2:   printf("%-10s V%01X, V%01X", "AND", instruction[0] & 0xfu, instruction[1] >> 4u); break;
                    case 0x3:   printf("%-10s V%01X, V%01X", "XOR", instruction[0] & 0xfu, instruction[1] >> 4u); break;
                    case 0x4:   printf("%-10s V%01X, V%01X", "ADD", instruction[0] & 0xfu, instruction[1] >> 4u); break;
                    case 0x5:   printf("%-10s V%01X, V%01X - V%01X", "SUB", instruction[0] & 0xfu, instruction[0] & 0xfu, instruction[1] >> 4u); break;
                    case 0x6:   printf("%-10s V%01X, V%01X", "SR", instruction[0] & 0xfu, instruction[1] >> 4u); break;
                    case 0x7:   printf("%-10s V%01X, V%01X - V%01X", "SUB", instruction[0] & 0xfu, instruction[1] >> 4u, instruction[0] & 0xfu); break;
                    case 0xe:   printf("%-10s V%01X, V%01X", "SL", instruction[0] & 0xfu, instruction[1] >> 4u); break;
                    default:    printf("UNKNOWN 8"); break;
                }
                break;
            case 0x9:   printf("%-10s V%01X, V%01X", "SKIP.NE", instruction[0] & 0xfu, instruction[1] >> 4u); break;
            case 0xa:   printf("%-10s I, #$%01x%02x", "SETI", instruction[0] & 0xfu, instruction[1]); break;
            case 0xb:   printf("%-10s $%01x%02x(V0)", "JUMP", instruction[0] & 0xfu, instruction[1]); break;
            case 0xc:   printf("%-10s V%01X, #$%02X", "RNDMSK", instruction[0] & 0xfu, instruction[1]); break;
            case 0xd:   printf("%-10s V%01X, V%01X, #$%01x", "SPRITE", instruction[0] & 0xfu, instruction[1] >> 4u, instruction[1] & 0xfu); break;
            case 0xe:
                switch (instruction[1]){
                    case 0x9e:  printf("%-10s V%01X", "SKIPKEY.EQ", instruction[0] & 0xfu); break;
                    case 0xa1:  printf("%-10s V%01X", "SKIPKEY.NE", instruction[0] & 0xfu); break;
                    default:    printf("UNKNOWN E"); break;
                }
                break;
            case 0xf:
                switch (instruction[1]){
                    case 0x07:  printf("%-10s V%01X, DELAY", "SET", instruction[0] & 0xfu); break;
                    case 0x0a:  printf("%-10s V%01X", "KEY", instruction[0] & 0xfu); break;
                    case 0x15:  printf("%-10s DELAY, V%01X", "SET", instruction[0] & 0xfu); break;
                    case 0x18:  printf("%-10s SOUND, V%01X", "MOV", instruction[0] & 0xfu); break;
                    case 0x1e:  printf("%-10s I, V%01X", "ADD", instruction[0] & 0xfu); break;
                    case 0x29:  printf("%-10s I, V%01X", "SPRITECHAR", instruction[0] & 0xfu); break;
                    case 0x33:  printf("%-10s (I), V%01X", "SETBCD", instruction[0] & 0xfu); break;
                    case 0x55:  printf("%-10s (I), V0-V%01X", "SETM", instruction[0] & 0xfu); break;
                    case 0x65:  printf("%-10s V0-V%01X,(I)", "SETM", instruction[0] & 0xfu); break;
                    default:    printf("UNKNOWN F"); break;
                }
                break;
        }
        cout << endl;
        pc += 2;
    }

    delete[] buffer;
}
