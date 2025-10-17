#include "gui.hpp"
#include "board_gui.hpp" 
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

vector<vector<char>> convertBoardToGrid(const Board_GUI &b) {
    vector<vector<char>> grid(15, vector<char>(15, ' '));
    stringstream ss;
    ss << b;
    string line;
    int row = 0;
    while (getline(ss, line)) {
        for (int col = 0; col < line.size(); ++col) {
            grid[col][row] = line[col];
        }
        ++row;
    }
    return grid;
}

int main() {
    GUI gui;
    
    if (!gui.init()) {
        std::cerr << "Failed to initialize GUI. Exiting." << std::endl;
        return 1;
    }

    // Initial board setup for visualization
    Board_GUI b;
    stringstream ss;
    ss << "..............." << endl;
    ss << "..............." << endl;
    ss << "..............." << endl;
    ss << "..............." << endl;
    ss << "..............." << endl;
    ss << "..............." << endl;
    ss << "..............." << endl;
    ss << "....PROJET....." << endl;
    ss << ".......O......." << endl;
    ss << ".......U......." << endl;
    ss << ".......E......." << endl;
    ss << ".......U......." << endl;
    ss << ".....SCRABBLE.." << endl;
    ss << "..............." << endl;
    ss << "..............." << endl;
    b.load(ss);

    vector<vector<char>> grid = convertBoardToGrid(b);
    gui.updateGrid(grid);

    // The player's hand and game initialization will be handled through
    // the GUI interface with the Start Game button
    
    gui.run();
    return 0;
}