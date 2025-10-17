#include "bag.hpp"

bag::bag(){
    std::vector<letter> nsac;
    sac = nsac;
}

bag::bag(std::vector<letter> nsac){
    sac = nsac;
}

bag::~bag(){
    sac.clear();
}

void bag::initialize_scrabble_bag() {
    sac.clear();
    std::vector<std::pair<char, unsigned int>> scrabble_letters = {
        {'A', 1}, {'B', 3}, {'C', 3}, {'D', 2}, {'E', 1}, {'F', 4}, {'G', 2}, {'H', 4}, {'I', 1}, {'J', 8}, {'K', 5}, {'L', 1}, {'M', 3}, {'N', 1}, {'O', 1}, {'P', 3}, {'Q', 10}, {'R', 1}, {'S', 1}, {'T', 1}, {'U', 1}, {'V', 4}, {'W', 4}, {'X', 8}, {'Y', 4}, {'Z', 10}
    };
    std::vector<unsigned int> letter_counts = {
        9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1
    };

    for (unsigned int i = 0; i < scrabble_letters.size(); ++i) {
        for (unsigned int j = 0; j < letter_counts[i]; ++j) {
            sac.push_back(letter(i, scrabble_letters[i].first, scrabble_letters[i].second));
        }
    }
}

void bag::shuffle_bag() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(sac.begin(), sac.end(), g);
}

bool bag::is_empty(){
    return sac.empty();
}

letter bag::draw_letter(){
    if(!is_empty()){
        letter l = sac[0];
        sac.erase(sac.begin());
        return l;
    }
    letter l = letter();
    return l;
}