#include "letter.hpp"

letter::letter(){
    letter_index = -1;
    car = '?';
    points = -1;
};

letter::letter(unsigned int nletter_index, char ncar, unsigned int npoints){
    letter_index = nletter_index;
    car = ncar;
    points = npoints;
}

letter::~letter(){
}

unsigned int letter::get_points(){
    return points;
}