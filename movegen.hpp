#pragma once

#include <vector>
#include "position.hpp"
#include "types.hpp"

bool isKingInCheck(COLOR_TYPE us, COLOR_TYPE enemy, const Position & position);

void generateAllMoves(
  const COLOR_TYPE & us,
  const COLOR_TYPE & enemy,
  Position origPosition,
  std::vector<Position> & newPositions
);
void generateAllLegalMoves(
  const COLOR_TYPE & us,
  const COLOR_TYPE & enemy,
  Position origPosition,
  std::vector<Position> & newPositions
);
