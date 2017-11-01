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
  p = setPositionFromFen("r3k1nr/p3pppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq -");
  /*p.colors[BLACK] |= bitAtIndex[34];
  p.pieces[PAWN] |= bitAtIndex[34];
  Bitboard eP = bitAtIndex[42];
  p.colors[WHITE] |= bitAtIndex[35];
  p.pieces[PAWN] |= bitAtIndex[35];*/
  printPosition(p);
  //printPositionBitboards(p);
  std::vector<Position> newPositions;
  Bitboard occupancy = p.colors[WHITE] | p.colors[BLACK];
  generateCastlingMoves(BLACK, WHITE, p, newPositions, occupancy);
  for(int i = 0; i<newPositions.size(); i++)
  {
    printPosition(newPositions[i]);
    //printPositionBitboards(newPositions[i]);
  }
  /*p = setPositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e3 0 0");
  printPosition(p);
  printPositionBitboards(p);
  return 0;*/
}
