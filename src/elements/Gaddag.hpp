#ifndef GADDAG_HPP
#define GADDAG_HPP

#include <string>
#include "NoeudGaddag.hpp"

class Gaddag {
        
    NoeudGaddag* racine;

    public:
    
    Gaddag();
    // ~Gaddag();

    void inserer(const std::string& mot);
    bool contient(const std::string& mot) const;
    void chargerDepuisFichier(const std::string& nomFichier);
};

#endif
