# A Chip-8 emulator in C++ using SFML.

## Compile in linux
```
sudo apt-get install libsfml-dev
g++ -c /path/to/main.cpp /path/to/Chip8.cpp
g++ main.o Chip8.o -o chip8 -lsfml-graphics -lsfml-window -lsfml-system
```

## Usage
`./chip8 path/to/rom`

## Screenshots
![Annotation 2019-07-27 152633](https://user-images.githubusercontent.com/34605386/61994525-ff2f3900-b083-11e9-9feb-3530b99a48b2.jpg)

![Annotation 2019-07-27 152652](https://user-images.githubusercontent.com/34605386/61994526-01919300-b084-11e9-84cd-c2963c3c0c28.jpg)
