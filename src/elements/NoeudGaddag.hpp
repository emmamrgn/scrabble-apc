#ifndef NOEUD_GADDAG_HPP
#define NOEUD_GADDAG_HPP

#include <unordered_map>

class NoeudGaddag {
    
    public:
    
    bool estTerminal;
    std::unordered_map<char, NoeudGaddag*> enfants;

    NoeudGaddag();
    // ~NoeudGaddag();
};

#endif
