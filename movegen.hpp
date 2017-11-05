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
  PositionArray & newPositions,
  int & numberNewPositions
)
{
  static Bitboard enPassant;
  static Bitboard occupancy;
  numberNewPositions = 0;
  enPassant = origPosition.enPassant;
  origPosition.enPassant = 0;
  origPosition.whoIsToMove *= -1;
  origPosition.numberHalfmovesPlayed += 1;
  occupancy = origPosition.colors[BLACK] | origPosition.colors[WHITE];

  generateMoves<KNIGHT>(
    us, enemy,
    origPosition,
    occupancy,
    newPositions,
    numberNewPositions
  );
  generateMoves<BISHOP>(
    us, enemy,
    origPosition,
    occupancy,
    newPositions,
    numberNewPositions
  );
  generateMoves<ROOK>(
    us, enemy,
    origPosition,
    occupancy,
    newPositions,
    numberNewPositions
  );
  generateMoves<QUEEN>(
    us, enemy,
    origPosition,
    occupancy,
    newPositions,
    numberNewPositions
  );
  generateMoves<KING>(
    us, enemy,
    origPosition,
    occupancy,
    newPositions,
    numberNewPositions
  );
  generatePawnMoves(
    us, enemy,
    origPosition,
    newPositions,
    numberNewPositions,
    enPassant
  );
  generateCastlingMoves(
      us, enemy,
      origPosition,
      newPositions,
      numberNewPositions,
      occupancy
  );

}
