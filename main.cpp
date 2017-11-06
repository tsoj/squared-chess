#include <iostream>
#include <vector>
#include <sys/resource.h>
#include "bitOperations.hpp"
#include "position.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "bitMasks.hpp"
#include "text.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "search.hpp"
#include "moveOrdering.hpp"
#include <chrono>

using namespace BitMasks;
int main()
{
  //initStack();
  Position p;
  p = setPositionFromFen("r3k2r/pp1nnp1p/2pqb1p1/3p4/3PP3/2NB1P2/PPQ1N1PP/R3K2R w KQkq - 0 0");
  printPosition(p);
  if(false)
  {
    PositionVector newPositions;
    newPositions.size = 0;
    generateAllMoves(BLACK, WHITE, p, newPositions);
    for(int i = 0; i<newPositions.size; i++)
    {
      printPosition(newPositions.array[i]);
      newPositions.array[i].lastMove.print();
    }
  }
  if(true)
  {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    p = startSearch(p, 8);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
  }
  if(false)
  {
    Move m1;
    Move m2;
    m2.from=13;
    m1.from=13;
    m2.to=40;
    m1.to=40;
    if(m1==m2)
    {
      std::cout <<"hi"<<std::endl;
    }
  }
}
