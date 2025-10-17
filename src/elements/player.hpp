#ifndef JOUEUR_H
#define JOUEUR_H

#include "bag.hpp"
#include "board.hpp"
#include "Gaddag.hpp"


class player{

    public :

    player();
    player(bag nsac);

    bag sac;
    std::vector<letter> letters;


    ~player();

    void reload();

    void showLetters();

    bool isValid(Board b, std::string mot, int l, int c, char direc, Gaddag gaddag);

    std::string trouverCoup(int l, int c, char direc, Board b, Gaddag gaddag);

    std::string trouverCoupAuto(int& l, int& c, char& direc, Board b, Gaddag gaddag);

    int play(Board& b);

    int playAuto(Board& b);
};

#endif //JOUEUR_H