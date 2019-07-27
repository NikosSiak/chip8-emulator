#include <iostream>
#include <ctime>
#include "Chip8.hpp"
#include <SFML/Graphics.hpp>


#define RES_MULT 20


int main(int argc, char *argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(64 * RES_MULT, 32 * RES_MULT), "Chip8 Emulator",
            sf::Style::Titlebar | sf::Style::Close);

    sf::Color bgColor(sf::Color::Black);
    sf::Color fgColor(sf::Color::White);

    Chip8 chip;
    try{
        chip.loadGame(argv[1]);
    }
    catch (const std::runtime_error& error){
        // TODO: popup window
        window.close();
        std::cout << "error reading rom" << std::endl;
        exit(2);
    }

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed: {
                    switch (event.key.code) {
                        case sf::Keyboard::Num1: chip.keyPress(0x1); break;
                        case sf::Keyboard::Num2: chip.keyPress(0x2); break;
                        case sf::Keyboard::Num3: chip.keyPress(0x3); break;
                        case sf::Keyboard::Num4: chip.keyPress(0xc); break;
                        case sf::Keyboard::Q: chip.keyPress(0x4); break;
                        case sf::Keyboard::W: chip.keyPress(0x5); break;
                        case sf::Keyboard::E: chip.keyPress(0x6); break;
                        case sf::Keyboard::R: chip.keyPress(0xd); break;
                        case sf::Keyboard::A: chip.keyPress(0x7); break;
                        case sf::Keyboard::S: chip.keyPress(0x8); break;
                        case sf::Keyboard::D: chip.keyPress(0x9); break;
                        case sf::Keyboard::F: chip.keyPress(0xe); break;
                        case sf::Keyboard::Z: chip.keyPress(0xa); break;
                        case sf::Keyboard::X: chip.keyPress(0x0); break;
                        case sf::Keyboard::C: chip.keyPress(0xb); break;
                        case sf::Keyboard::V: chip.keyPress(0xf); break;
                    }
                    break;
                }
                case sf::Event::KeyReleased: {
                    switch (event.key.code) {
                        case sf::Keyboard::Num1: chip.keyRelease(0x1); break;
                        case sf::Keyboard::Num2: chip.keyRelease(0x2); break;
                        case sf::Keyboard::Num3: chip.keyRelease(0x3); break;
                        case sf::Keyboard::Num4: chip.keyRelease(0xc); break;
                        case sf::Keyboard::Q: chip.keyRelease(0x4); break;
                        case sf::Keyboard::W: chip.keyRelease(0x5); break;
                        case sf::Keyboard::E: chip.keyRelease(0x6); break;
                        case sf::Keyboard::R: chip.keyRelease(0xd); break;
                        case sf::Keyboard::A: chip.keyRelease(0x7); break;
                        case sf::Keyboard::S: chip.keyRelease(0x8); break;
                        case sf::Keyboard::D: chip.keyRelease(0x9); break;
                        case sf::Keyboard::F: chip.keyRelease(0xe); break;
                        case sf::Keyboard::Z: chip.keyRelease(0xa); break;
                        case sf::Keyboard::X: chip.keyRelease(0x0); break;
                        case sf::Keyboard::C: chip.keyRelease(0xb); break;
                        case sf::Keyboard::V: chip.keyRelease(0xf); break;
                    }
                    break;
                }
            }
        }

        if (!chip.emulate(1)) {
            // TODO: popup window
            std::cout << "Emulation failed" << std::endl;
            window.close();
            exit(3);
        }

        sf::RectangleShape pixel;
        window.clear();
        for (int i = 0; i < 64 * 32; i++) {
            pixel.setFillColor(chip.getPixel(i) ? fgColor : bgColor);
            pixel.setPosition(i % 64 * RES_MULT, i / 64 * RES_MULT);
            pixel.setSize(sf::Vector2f(RES_MULT, RES_MULT));
            window.draw(pixel);
        }

        window.display();
    }

    return 0;
}
