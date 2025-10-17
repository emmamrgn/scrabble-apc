#include "elements/player.hpp"
#include "elements/Gaddag.hpp"

#include <iostream>
#include <sstream>

int main() {
  Board b ;
  bag sac = bag();
  sac.initialize_scrabble_bag();
  sac.shuffle_bag();
  player p = player(sac);

  std::cout << b << std::endl ;

  std::stringstream ss ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "....PROJET....." << std::endl ;
  ss << ".......O......." << std::endl ;
  ss << ".......U......." << std::endl ;
  ss << ".......E......." << std::endl ;
  ss << ".......U......." << std::endl ;
  ss << ".....SCRABBLE.." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;

  b.load(ss) ;
  std::cout << b << std::endl ;
  p.reload() ;
  p.showLetters() ;
  //décommenter la ligne suivante pour jouer un coup
  // int test = p.play(b) ;
  //commenter pour ne pas jouer le coup automatiquement
  int test = p.playAuto(b) ;
  std::cout << "Le mot a marqué : " << test << " points." << std::endl ;
  std::cout << b << std::endl ;
  return 0 ;
}