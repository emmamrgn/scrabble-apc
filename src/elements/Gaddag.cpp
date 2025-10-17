#include "Gaddag.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

Gaddag::Gaddag() {
    racine = new NoeudGaddag();
}

//fonction pour insérer un mot dans gaddag
void Gaddag::inserer(const std::string& mot) {
    NoeudGaddag* noeud = racine;
    //parcours du mot caractère par caractère
    for (char c : mot) {
        //si le caractère n'existe pas déjà parmi les enfants du noeud courant
        if (noeud->enfants.find(c) == noeud->enfants.end()) {
            //créer un nouveau noeud pour ce caractère
            noeud->enfants[c] = new NoeudGaddag();
        }
        //passer au noeud correspondant au caractère actuel
        noeud = noeud->enfants[c];
    }
    noeud->estTerminal = true;
}

bool Gaddag::contient(const std::string& mot) const {
    const NoeudGaddag* noeud = racine;
    for (char c : mot) {
        //si le caractère n'est pas trouvé parmi les enfants du noeud courant
        auto it = noeud->enfants.find(c);
        if (it == noeud->enfants.end())
            return false;
        //passer au noeud correspondant au caractère actuel
        noeud = it->second;
    }
    return noeud->estTerminal;
}

void Gaddag::chargerDepuisFichier(const std::string& nomFichier) {
    std::ifstream fichier(nomFichier);
    if (!fichier.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier \"" << nomFichier << "\"." << std::endl;
        return;
    }
    std::string mot;
    while (fichier >> mot) {
        inserer(mot);
    }
}
