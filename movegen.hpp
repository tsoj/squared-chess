#pragma once

#include <vector>
#include "position.hpp"
#include "types.hpp"
#include "movegen.hpp"
#include "movegenElements.hpp"

 inline bool isKingInCheck(COLOR_TYPE us, COLOR_TYPE enemy, const Position & position);

inline void generateAllMoves(
  const COLOR_TYPE & us,
  const COLOR_TYPE & enemy,
  Position origPosition,
  std::vector<Position> & newPositions
)
{
  static Bitboard enPassant;
  static Bitboard occupancy;
  enPassant = origPosition.enPassant;
  origPosition.enPassant = 0;
  origPosition.whoIsToMove *= -1;
  origPosition.numberHalfmovesPlayed += 1;
  occupancy = origPosition.colors[BLACK] | origPosition.colors[WHITE];

  generateMoves<KNIGHT>(
    us, enemy,
    origPosition,
    occupancy,
    newPositions
  );
  generateMoves<BISHOP>(
    us, enemy,
    origPosition,
    occupancy,
    newPositions
  );
  generateMoves<ROOK>(
    us, enemy,
    origPosition,
    occupancy,
    newPositions
  );
  generateMoves<QUEEN>(
    us, enemy,
    origPosition,
    occupancy,
    newPositions
  );
  generateMoves<KING>(
    us, enemy,
    origPosition,
    occupancy,
    newPositions
  );
  generatePawnMoves(
    us, enemy,
    origPosition,
    newPositions,
    enPassant
  );
  generateCastlingMoves(
      us, enemy,
      origPosition,
      newPositions,
      occupancy
  );

}
