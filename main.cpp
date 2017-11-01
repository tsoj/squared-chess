#include <iostream>
#include <vector>
#include "bitOperations.hpp"
#include "position.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "bitMasks.hpp"
#include "text.hpp"
#include "movegen.hpp"

using namespace BitMasks;
int main()
{
  //std::cout << LOGO << std::endl;
  Position p;
  p.colors[BLACK] |= bitAtIndex[34];
  p.pieces[PAWN] |= bitAtIndex[34];
  Bitboard eP = bitAtIndex[42];
  p.colors[WHITE] |= bitAtIndex[35];
  p.pieces[PAWN] |= bitAtIndex[35];
  printPosition(p);
  std::vector<Position> newPositions;
  //Bitboard occupancy = p.colors[WHITE] | p.colors[BLACK];
  generatePawnMoves(WHITE, BLACK, p,  newPositions, eP);
  for(int i = 0; i<newPositions.size(); i++)
  {
    printPosition(newPositions[i]);
  }
  return 0;
}
