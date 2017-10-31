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
  printPosition(p);
  std::vector<Position> newPositions;
  Bitboard occupancy = p.colors[WHITE] | p.colors[BLACK];
  generateMoves<ROOK, BLACK, WHITE>(p, occupancy, newPositions);
  for(int i = 0; i<newPositions.size(); i++)
  {
    printPosition(newPositions[i]);
  }
  return 0;
}
