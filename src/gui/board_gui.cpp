#include "board_gui.hpp"
#include <iostream>
#include <sstream>

Board_GUI::Board_GUI() {
    board.resize(15, std::vector<char>(15, ' '));
}

void Board_GUI::load(std::istream &is) {
    std::string line;
    int row = 0;
    while (std::getline(is, line) && row < 15) {
        for (int col = 0; col < line.size() && col < 15; ++col) {
            board[row][col] = line[col];
        }
        ++row;
    }
}

std::ostream& operator<<(std::ostream &os, const Board_GUI &b) {
    for (const auto &row : b.board) {
        for (const auto &cell : row) {
            os << cell;
        }
        os << std::endl;
    }
    return os;
}