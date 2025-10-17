#ifndef BOARD_GUI_HPP
#define BOARD_GUI_HPP

#include <vector>
#include <iostream>

struct Board_GUI {
    Board_GUI();
    void load(std::istream &is);
    friend std::ostream& operator<<(std::ostream &os, const Board_GUI &b);

    std::vector<std::vector<char>> board;
};

#endif // BOARD_GUI_HPP