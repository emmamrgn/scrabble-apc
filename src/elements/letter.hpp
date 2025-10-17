//
// Created by Yaël-Alexandre on 04/02/2025.
//

#ifndef LETTER_H
#define LETTER_H

class letter {
    public:

    //Index of the letter in alphabetical order (eg. a = 1, z = 26)
    unsigned int letter_index;

    //The letter character (eg. a = 'a')
    char car;

    //Point gives by a letter (eg. a = 1, b = 3, d = 2)
    unsigned int points;

    letter();    
    letter(unsigned int nletter_index, char ncar, unsigned int npoints);

    ~letter();

    unsigned int get_points();

    bool operator<(const letter& other) const {
        return this->car < other.car;  // Compare les caractères de la lettre
    }

    operator char() const {
        return car;
    }
};
#endif //LETTER_H