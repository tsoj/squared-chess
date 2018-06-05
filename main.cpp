#include <iostream>
#include "chessData.hpp"
#include "chessTypes.hpp"
#include "position.hpp"

int main()
{
  std::cout << ChessTypes::getUnicodePiece(ChessTypes::BLACK, ChessTypes::PAWN) << std::endl;
}
