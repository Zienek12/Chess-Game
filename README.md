# Chess-Game

The goal of this project is to create a simple chess game with a graphical user interface and a basic chess engine.

## Features

- Graphical interface based on the SFML library
- Simple AI opponent

## Requirements

- CMake (recommended version 3.20 or newer)
- Ninja
- C++20 compatible compiler
- SFML 3.0.0 (Graphics, System, Window)

## Building the Project

In the main project directory, run the following commands:

cmake -S . -B build -G Ninja cmake --build build


## Running

After building, run the executable from the `build` directory:

./build/Chess


The game is controlled using the mouse.

## Dependencies

The project only requires the SFML library.

## Operating Systems

The project has been tested on Windows.

## Known Issues

There are known bugs and limitations – work is ongoing to resolve them.

## License

This project is available for open use.
