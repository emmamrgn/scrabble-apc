#include "player.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

player::player(){
    letters = std::vector<letter>();
}

player::player(bag nsac){
    sac = nsac;
}

player::~player(){
    letters.clear();
}

//remplis la main du joueur si possible
void player::reload(){
    while(letters.size() < 7 && !sac.is_empty()){
        letters.push_back(sac.draw_letter());
    }
}

void player::showLetters(){
    std::cout << "[";
    if(letters.size() == 0){
        std::cout << "]" << std::endl;
        return;
    }
    for(long unsigned int i = 0; i < letters.size() - 1; i++){
        std::cout << letters[i].car << ";";
    }
    std::cout << letters[letters.size() - 1].car;
    std::cout << "]" << std::endl;
}

//vérifie si le coup est valide
//il manque des conditions tel que la vérification des mots formés par le coup
//la condition de la présence d'une lettre sur le plateau ne marche pas
bool player::isValid(Board b, std::string mot, int l, int c, char direc, Gaddag gaddag){
    //vérification des coordonnées pour voir si l'on est dans le plateau
    if(l > 14 || c > 14 || l < 0 || c < 0){
        return false;
    }
    //vérification si une des lettres appartient déja au plateau (ne fonctionne pas)
    bool in_board = false;
    for(long unsigned int i = 0; i < mot.size(); i++){
        //vérification si la lettre est sur le plateau OU dans la main du joueur
        bool found = false;
        //si il y'a déja une lettre sur l'emplacement on vérifie si c'est la même
        //c'est ce if qui pose problème
        if (b(l, c).letter != 0) {
            if (mot[i] == b(l, c).letter) {
                in_board = true;
                found = true;
            }
            else{
                return false;
            }
        }
        //si on pas trouvé la lettre sur le plateau on vérifie si elle est dans la main du joueur
        if(!found){
            bool letter_found = false;
            for(long unsigned int j = 0; j < letters.size(); j++){
                if(mot[i] == letters[j].car){
                    letter_found = true;
                    break;
                }
            }
            //si on a toujours pas trouvé la lettre alors le coup n'est pas légal
            if (!letter_found) {
                return false;
            }
        }
        // Calcul de la nouvelle position en fonction de la direction
        switch (direc){
            case 'b': l++; break;
            case 'd': c++; break;
            default: return false;
        }
    }

    if(!in_board){
        return false;
    }

    //si le mot n'est pas dans le dictionnaire le coup n'est pas légal
    if(!gaddag.contient(mot)){
        return false;
    }
    
    return true;
}

///trouve le meilleur coup possible en fonction de la position de la première lettre et de la direction
std::string player::trouverCoup(int l, int c, char direc, Board b, Gaddag gaddag){
    std::string meilleur_mot = "";
    int meilleur_score = -1;
    for(long unsigned int i = 1; i <= this->letters.size(); i++){
        //on créé une copie des lettres
        std::vector<letter> letters_copy = this->letters;
        std::sort(letters_copy.begin(), letters_copy.end(), [](const letter& a, const letter& b){ return a.car < b.car; });

        //on génère toutes les permutations de la main du joueur
        while(std::next_permutation(letters_copy.begin(), letters_copy.end())){
            std::string mot(letters_copy.begin(), letters_copy.begin() + i);
            //on vérifie si le mot est dans le dictionnaire puis on calcule le score
            if(gaddag.contient(mot)){
                int score = b.calculateScore(l, c, mot, direc);
                if(score > meilleur_score){
                    meilleur_score = score;
                    meilleur_mot = mot;
                }
            }
        }

    }
    return meilleur_mot;
}

///trouve le meilleur coup possible dans toute la grille en utilisant la fonction précédent sur chaque case avec les 2 directions possibles
std::string player::trouverCoupAuto(int& l, int& c, char& direc, Board b, Gaddag gaddag){
    int meilleur_score = -1;
    std::string meilleur_mot = "";

    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 15; j++){
            //on trouve le meilleur coup pour chaque direction
            std::string mot_b = trouverCoup(i, j, 'b', b, gaddag);
            std::string mot_d = trouverCoup(i, j, 'h', b, gaddag);

            //on regarde quelle est la meilleur direction pour mettre le mot
            if (!mot_b.empty() && b.calculateScore(i, j, mot_b, 'b') > meilleur_score) {
                meilleur_score = b.calculateScore(i, j, mot_b, 'b');
                meilleur_mot = mot_b;
                l = i;
                c = j;
                direc = 'b';
            }

            if (!mot_d.empty() && b.calculateScore(i, j, mot_d, 'd') > meilleur_score) {
                meilleur_score = b.calculateScore(i, j, mot_d, 'd');
                meilleur_mot = mot_d;
                l = i;
                c = j;
                direc = 'd';
            }
        }
    }
    return meilleur_mot;
}

//fonction pour jouer un coup soi-même
int player::play(Board& b){
    Gaddag gaddag;
    gaddag.chargerDepuisFichier("../data/dico.txt");

    std::cout << "Entrez des coordonnées" << std::endl;
    int l, c;
    std::cin >> l >> c;

    std::string mot;
    std::cout << "Entrez un mot" << std::endl;
    std::cin >> mot;

    std::cout << "Entrez une direction" << std::endl;
    char direc;
    std::cin >> direc;

    //vérification si le mot est dans le dictionnaire
    if(isValid(b, mot, l, c, direc, gaddag) == false){
        return -1;
    }

    //place le mot sur le plateau et renvoie le score
    int score = b.placeWord(l, c, mot, direc);
    std :: cout << b << std::endl;
    return score;
}

//fonction pour jouer un coup automatiquement de façon optimal
int player::playAuto(Board& b){
    Gaddag gaddag;
    gaddag.chargerDepuisFichier("../data/dico.txt");
    int l, c;
    char direc;
    std::string mot = trouverCoupAuto(l, c, direc, b, gaddag);
    return b.placeWord(l, c, mot, direc);
}