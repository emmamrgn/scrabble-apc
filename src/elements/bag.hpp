//
// Created by Yaël-Alexandre on 04/02/2025.
//

#ifndef BAG_H
#define BAG_H

#include <algorithm>
#include <random>
#include <vector>
#include "letter.hpp"

class bag {
    //
    public :

    std::vector<letter> sac;


    bag();
    bag(std::vector<letter> nsac);

    ~bag();

    void shuffle_bag();
    void initialize_scrabble_bag();
    unsigned int get_points_lettre(letter l);
    bool is_empty();
    letter draw_letter();
};

#endif //BAG_H
