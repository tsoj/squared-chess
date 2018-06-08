#include <iostream>
#include "chessData.hpp"
#include "chessTypes.hpp"
#include "position.hpp"
#include "move.hpp"
#include "debug.hpp"

int main()
{
  Position p = Position::emptyPosition();
  if(not p.setFromFen("3r1rk1/pQ3pbp/5qp1/4n3/3N2b1/P3P1P1/1P3PBP/R1B2RK1 w - - 1 16"))
  {
    std::cout << "BUH!" << std::endl;
  }
  std::cout << p.getString() << std::endl;
  MoveList ml = MoveList();
  ml.generateMoves<PAWN>(p, 0, false);
}
