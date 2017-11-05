#pragma once

#include "bitMasks.hpp"
#include "bitOperations.hpp"
#include "utils.hpp"
#include <vector>

using namespace BitOperations;
using namespace BitMasks;

inline bool isKingInCheck(COLOR_TYPE us, COLOR_TYPE enemy, const Position & position, int kingsIndex)
{
  static int moveDirection;
  //PAWN
  if(us == WHITE)
  {
    moveDirection = 8;
  }
  else
  {
    moveDirection = -8;
  }
  if(
    (bitAtIndex[kingsIndex + moveDirection + EAST] | bitAtIndex[kingsIndex + moveDirection + WEST]) &
    position.pieces[PAWN] & position.colors[enemy]
  )
  {
    return true;
  }

  Bitboard occupancy = position.colors[WHITE] | position.colors[BLACK];
  //KNIGHT
  if(
    getAttackBitboard<KNIGHT>(occupancy, kingsIndex) &
    position.pieces[KNIGHT] & position.colors[enemy]
  )
  {
    return true;
  }
  //BISHOP
  if(
    getAttackBitboard<BISHOP>(occupancy, kingsIndex) &
    position.pieces[BISHOP] & position.colors[enemy]
  )
  {
    return true;
  }
  //ROOK
  if(
    getAttackBitboard<ROOK>(occupancy, kingsIndex) &
    position.pieces[ROOK] & position.colors[enemy]
  )
  {
    return true;
  }
  //QUEEN
  if(
    getAttackBitboard<QUEEN>(occupancy, kingsIndex) &
    position.pieces[QUEEN] & position.colors[enemy]
  )
  {
    return true;
  }
  //KING
  if(
    getAttackBitboard<KING>(occupancy, kingsIndex) &
    position.pieces[KING] & position.colors[enemy]
  )
  {
    return true;
  }
  return false;
}
inline bool isKingInCheck(COLOR_TYPE us, COLOR_TYPE enemy, const Position & position)
{
    unsigned long kingsIndex;
    bitScanForward(kingsIndex, position.pieces[KING] & position.colors[us]);
    return isKingInCheck(us, enemy, position, kingsIndex);
}

template<PIECE_TYPE Pt>
inline void applyQuietMove(
  const COLOR_TYPE & us, const COLOR_TYPE & enemy,
  Position & newPosition,
  const int & toFieldIndex, const int & fromFieldIndex
)

{
  newPosition.lastCapturedPieceType = NO_PIECE;
  newPosition.colors[us] = (newPosition.colors[us] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.pieces[Pt] = (newPosition.pieces[Pt] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.lastMovedPieceType = Pt;
  newPosition.lastPieceMovedToIndex = toFieldIndex;
  newPosition.lastPieceMovedFromIndex = fromFieldIndex;
  newPosition.castling[us] &= ~bitAtIndex[fromFieldIndex];
}
template<PIECE_TYPE Pt>
inline void applyCaptureMove(
  const COLOR_TYPE & us, const COLOR_TYPE & enemy,
  Position & newPosition,
  const int & toFieldIndex, const int & fromFieldIndex
)
{
  for(int i = 0; i<6; i+=1)
  {
    if(newPosition.pieces[i] & bitAtIndex[toFieldIndex])
    {
      newPosition.lastCapturedPieceType = PIECE_TYPE(i);
      newPosition.colors[enemy] = newPosition.colors[enemy] & ~bitAtIndex[toFieldIndex];
      newPosition.pieces[PIECE_TYPE(i)] = newPosition.pieces[PIECE_TYPE(i)] & ~bitAtIndex[toFieldIndex];
    }
  }
  newPosition.colors[us] = (newPosition.colors[us] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.pieces[Pt] = (newPosition.pieces[Pt] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.lastMovedPieceType = Pt;
  newPosition.lastPieceMovedToIndex = toFieldIndex;
  newPosition.lastPieceMovedFromIndex = fromFieldIndex;
  newPosition.castling[us] &= ~bitAtIndex[fromFieldIndex];
  newPosition.castling[enemy] &= ~bitAtIndex[toFieldIndex];
}

inline void applyCaptureEnPassantMove(
  const COLOR_TYPE & us, const COLOR_TYPE & enemy,
  Position & newPosition,
  const int & toFieldIndex, const int & fromFieldIndex,
  const Bitboard & enPassant,
  const int & moveDirection
)
{
  newPosition.lastCapturedPieceType = PAWN;
  newPosition.colors[enemy] = newPosition.colors[enemy] & ~bitAtIndex[toFieldIndex - moveDirection];
  newPosition.pieces[PAWN] = newPosition.pieces[PAWN] & ~bitAtIndex[toFieldIndex - moveDirection];
  newPosition.colors[us] = (newPosition.colors[us] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.pieces[PAWN] = (newPosition.pieces[PAWN] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.lastMovedPieceType = PAWN;
  newPosition.lastPieceMovedToIndex = toFieldIndex;
  newPosition.lastPieceMovedFromIndex = fromFieldIndex;
}

template<PIECE_TYPE Pt>
inline void generateMoves(
  const COLOR_TYPE & us, const COLOR_TYPE & enemy,
  const Position & origPosition,
  const Bitboard & occupancy,
  PositionArray & newPositions,
  int & numberNewPositions
)
{
  static Bitboard pieceOccupancy;
  static Bitboard quietAttackMask;
  static Bitboard captureAttackMask;
  static unsigned long fromFieldIndex;
  static unsigned long toFieldIndex;
  static Position newPosition;

  pieceOccupancy = origPosition.pieces[Pt] & origPosition.colors[us];
  if(pieceOccupancy)
  {
    do
    {
      bitScanForward(fromFieldIndex, pieceOccupancy);
      quietAttackMask = getAttackBitboard<Pt>(occupancy, fromFieldIndex) & ~origPosition.colors[us];
      captureAttackMask = quietAttackMask & origPosition.colors[enemy];
      quietAttackMask &= ~captureAttackMask;
      if(quietAttackMask)
      {
        do
        {
          bitScanForward(toFieldIndex, quietAttackMask);
          newPosition = origPosition;
          applyQuietMove<Pt>(us, enemy, newPosition, toFieldIndex, fromFieldIndex);
          newPositions[numberNewPositions] = newPosition;
          numberNewPositions++;
          quietAttackMask &= ~bitAtIndex[toFieldIndex];
        } while(quietAttackMask);
      }
      if(captureAttackMask)
      {
        do
        {
          bitScanForward(toFieldIndex, captureAttackMask);
          newPosition = origPosition;
          applyCaptureMove<Pt>(us, enemy, newPosition, toFieldIndex, fromFieldIndex);
          newPositions[numberNewPositions] = newPosition;
          numberNewPositions++;
          captureAttackMask &= ~bitAtIndex[toFieldIndex];
        } while(captureAttackMask);
      }
      pieceOccupancy &= ~bitAtIndex[fromFieldIndex];
    } while(pieceOccupancy);
  }
}

//TODO: promotion
inline void generatePawnMoves(
  const COLOR_TYPE & us, const COLOR_TYPE & enemy,
  const Position & origPosition,
  PositionArray & newPositions,
  int & numberNewPositions,
  const Bitboard & enPassant
)
{
  static Bitboard pawnOccupancy;
  static Bitboard pawnDoubleMove;
  static Bitboard promotionMove;
  static unsigned long fromFieldIndex;
  static unsigned long toFieldIndex;
  static Position newPosition;
  static int moveDirection;

  if(us == WHITE)
  {
    moveDirection = 8;
    pawnDoubleMove = ranks[1];
    promotionMove = ranks[7];
  }
  else
  {
    moveDirection = -8;
    pawnDoubleMove = ranks[6];
    promotionMove = ranks[0];
  }

  pawnOccupancy = origPosition.pieces[PAWN] & origPosition.colors[us];

  do
  {
    bitScanForward(fromFieldIndex, pawnOccupancy);
    toFieldIndex = fromFieldIndex + moveDirection;
    if(!((origPosition.colors[BLACK] | origPosition.colors[WHITE]) & bitAtIndex[toFieldIndex]))
    {
      newPosition = origPosition;
      applyQuietMove<PAWN>(us, enemy, newPosition, toFieldIndex, fromFieldIndex);
      if(bitAtIndex[toFieldIndex] & promotionMove)
      {
        newPosition.pieces[PAWN] = newPosition.pieces[PAWN] & ~bitAtIndex[toFieldIndex];

        newPosition.pieces[KNIGHT] = newPosition.pieces[KNIGHT] | bitAtIndex[toFieldIndex];
        newPosition.lastMovedPieceType = KNIGHT;
        newPositions[numberNewPositions] = newPosition;
        numberNewPositions++;
        newPosition.pieces[KNIGHT] = newPosition.pieces[KNIGHT] &  ~bitAtIndex[toFieldIndex];

        newPosition.pieces[BISHOP] = newPosition.pieces[BISHOP] | bitAtIndex[toFieldIndex];
        newPosition.lastMovedPieceType = BISHOP;
        newPositions[numberNewPositions] = newPosition;
        numberNewPositions++;
        newPosition.pieces[BISHOP] = newPosition.pieces[BISHOP] &  ~bitAtIndex[toFieldIndex];

        newPosition.pieces[ROOK] = newPosition.pieces[ROOK] | bitAtIndex[toFieldIndex];
        newPosition.lastMovedPieceType = ROOK;
        newPositions[numberNewPositions] = newPosition;
        numberNewPositions++;
        newPosition.pieces[ROOK] = newPosition.pieces[ROOK] &  ~bitAtIndex[toFieldIndex];

        newPosition.pieces[QUEEN] = newPosition.pieces[QUEEN] | bitAtIndex[toFieldIndex];
        newPosition.lastMovedPieceType = QUEEN;
      }
      newPositions[numberNewPositions] = newPosition;
      numberNewPositions++;

      toFieldIndex += moveDirection;
      if(
        !((origPosition.colors[BLACK] | origPosition.colors[WHITE]) & bitAtIndex[toFieldIndex]) &&
        (bitAtIndex[fromFieldIndex] & pawnDoubleMove)
      )
      {
        newPosition = origPosition;
        newPosition.enPassant = bitAtIndex[fromFieldIndex - moveDirection];
        applyQuietMove<PAWN>(us, enemy, newPosition, toFieldIndex, fromFieldIndex);
        newPositions[numberNewPositions] = newPosition;
        numberNewPositions++;
      }
    }
    toFieldIndex = fromFieldIndex + moveDirection + EAST;
    if(!(bitAtIndex[fromFieldIndex] & files[7]) && (origPosition.colors[enemy] | enPassant) & bitAtIndex[toFieldIndex])
    {
      if(enPassant & bitAtIndex[toFieldIndex])
      {
        newPosition = origPosition;
        applyCaptureEnPassantMove(
          us, enemy,
          newPosition,
          toFieldIndex,
          fromFieldIndex,
          enPassant,
          moveDirection
        );
        newPositions[numberNewPositions] = newPosition;
        numberNewPositions++;
      }
      else
      {
        newPosition = origPosition;
        applyCaptureMove<PAWN>(us, enemy, newPosition, toFieldIndex, fromFieldIndex);
        if(bitAtIndex[toFieldIndex] & promotionMove)
        {
          newPosition.pieces[PAWN] = newPosition.pieces[PAWN] & ~bitAtIndex[toFieldIndex];

          newPosition.pieces[KNIGHT] = newPosition.pieces[KNIGHT] | bitAtIndex[toFieldIndex];
          newPosition.lastMovedPieceType = KNIGHT;
          newPositions[numberNewPositions] = newPosition;
          numberNewPositions++;
          newPosition.pieces[KNIGHT] = newPosition.pieces[KNIGHT] &  ~bitAtIndex[toFieldIndex];

          newPosition.pieces[BISHOP] = newPosition.pieces[BISHOP] | bitAtIndex[toFieldIndex];
          newPosition.lastMovedPieceType = BISHOP;
          newPositions[numberNewPositions] = newPosition;
          numberNewPositions++;
          newPosition.pieces[BISHOP] = newPosition.pieces[BISHOP] &  ~bitAtIndex[toFieldIndex];

          newPosition.pieces[ROOK] = newPosition.pieces[ROOK] | bitAtIndex[toFieldIndex];
          newPosition.lastMovedPieceType = ROOK;
          newPositions[numberNewPositions] = newPosition;
          numberNewPositions++;
          newPosition.pieces[ROOK] = newPosition.pieces[ROOK] &  ~bitAtIndex[toFieldIndex];

          newPosition.pieces[QUEEN] = newPosition.pieces[QUEEN] | bitAtIndex[toFieldIndex];
          newPosition.lastMovedPieceType = QUEEN;
        }
        newPositions[numberNewPositions] = newPosition;
        numberNewPositions++;
      }
    }
    toFieldIndex = fromFieldIndex + moveDirection + WEST;
    if(!(bitAtIndex[fromFieldIndex] & files[0]) && (origPosition.colors[enemy] | enPassant) & bitAtIndex[toFieldIndex])
    {
      if(enPassant & bitAtIndex[toFieldIndex])
      {
        newPosition = origPosition;
        applyCaptureEnPassantMove(
          us, enemy,
          newPosition,
          toFieldIndex,
          fromFieldIndex,
          enPassant,
          moveDirection
        );
        newPositions[numberNewPositions] = newPosition;
        numberNewPositions++;
      }
      else
      {
        newPosition = origPosition;
        applyCaptureMove<PAWN>(us, enemy, newPosition, toFieldIndex, fromFieldIndex);
        if(bitAtIndex[toFieldIndex] & promotionMove)
        {
          newPosition.pieces[PAWN] = newPosition.pieces[PAWN] & ~bitAtIndex[toFieldIndex];

          newPosition.pieces[KNIGHT] = newPosition.pieces[KNIGHT] | bitAtIndex[toFieldIndex];
          newPosition.lastMovedPieceType = KNIGHT;
          newPositions[numberNewPositions] = newPosition;
          numberNewPositions++;
          newPosition.pieces[KNIGHT] = newPosition.pieces[KNIGHT] &  ~bitAtIndex[toFieldIndex];

          newPosition.pieces[BISHOP] = newPosition.pieces[BISHOP] | bitAtIndex[toFieldIndex];
          newPosition.lastMovedPieceType = BISHOP;
          newPositions[numberNewPositions] = newPosition;
          numberNewPositions++;
          newPosition.pieces[BISHOP] = newPosition.pieces[BISHOP] &  ~bitAtIndex[toFieldIndex];

          newPosition.pieces[ROOK] = newPosition.pieces[ROOK] | bitAtIndex[toFieldIndex];
          newPosition.lastMovedPieceType = ROOK;
          newPositions[numberNewPositions] = newPosition;
          numberNewPositions++;
          newPosition.pieces[ROOK] = newPosition.pieces[ROOK] &  ~bitAtIndex[toFieldIndex];

          newPosition.pieces[QUEEN] = newPosition.pieces[QUEEN] | bitAtIndex[toFieldIndex];
          newPosition.lastMovedPieceType = QUEEN;
        }
        newPositions[numberNewPositions] = newPosition;
        numberNewPositions++;
      }
    }
    pawnOccupancy &= ~bitAtIndex[fromFieldIndex];
  } while(pawnOccupancy);
}


inline void generateCastlingMoves(
    const COLOR_TYPE & us, const COLOR_TYPE & enemy,
    const Position & origPosition,
    PositionArray & newPositions,
    int & numberNewPositions,
    const Bitboard & occupancy
)
{
  static Position newPosition;
  //kingsside
  if(
    (!isKingInCheck(us, enemy, origPosition, castlingKingFrom[us]) &&
    !isKingInCheck(us, enemy, origPosition, castlingKingKingsideTo[us]) &&
    !isKingInCheck(us, enemy, origPosition, castlingKingsideCheckRelevant[us])) &&
    !(occupancy & castlingKingsideOccupancyRelevant[us]) &&
    (origPosition.castling[us] & castlingRookKingsideFrom[us])
  )
  {
    newPosition = origPosition;
    newPosition.lastCapturedPieceType = NO_PIECE;
    newPosition.colors[us] =
      (newPosition.colors[us] & ~bitAtIndex[castlingKingFrom[us]]) | bitAtIndex[castlingKingKingsideTo[us]];
    newPosition.pieces[KING] =
      (newPosition.pieces[KING] & ~bitAtIndex[castlingKingFrom[us]]) | bitAtIndex[castlingKingKingsideTo[us]];
    newPosition.colors[us] =
      (newPosition.colors[us] & ~bitAtIndex[castlingRookKingsideFrom[us]]) | bitAtIndex[castlingRookKingsideTo[us]];
    newPosition.pieces[ROOK] =
      (newPosition.pieces[ROOK] & ~bitAtIndex[castlingRookKingsideFrom[us]]) | bitAtIndex[castlingRookKingsideTo[us]];
    newPosition.lastMovedPieceType = KING;
    newPosition.lastPieceMovedToIndex = castlingKingKingsideTo[us];
    newPosition.lastPieceMovedFromIndex = castlingKingFrom[us];
    newPosition.castling[us] = 0;
    newPositions[numberNewPositions] = newPosition;
    numberNewPositions++;
  }
  //queenside
  if(
    (!isKingInCheck(us, enemy, origPosition, castlingKingFrom[us]) &&
    !isKingInCheck(us, enemy, origPosition, castlingKingQueensideTo[us]) &&
    !isKingInCheck(us, enemy, origPosition, castlingQueensideCheckRelevant[us])) &&
    !(occupancy & castlingQueensideOccupancyRelevant[us]) &&
    (origPosition.castling[us] & castlingRookQueensideFrom[us])
  )
  {
    newPosition = origPosition;
    newPosition.lastCapturedPieceType = NO_PIECE;
    newPosition.colors[us] =
      (newPosition.colors[us] & ~bitAtIndex[castlingKingFrom[us]]) | bitAtIndex[castlingKingQueensideTo[us]];
    newPosition.pieces[KING] =
      (newPosition.pieces[KING] & ~bitAtIndex[castlingKingFrom[us]]) | bitAtIndex[castlingKingQueensideTo[us]];
    newPosition.colors[us] =
      (newPosition.colors[us] & ~bitAtIndex[castlingRookQueensideFrom[us]]) | bitAtIndex[castlingRookQueensideTo[us]];
    newPosition.pieces[ROOK] =
      (newPosition.pieces[ROOK] & ~bitAtIndex[castlingRookQueensideFrom[us]]) | bitAtIndex[castlingRookQueensideTo[us]];
    newPosition.lastMovedPieceType = KING;
    newPosition.lastPieceMovedToIndex = castlingKingQueensideTo[us];
    newPosition.lastPieceMovedFromIndex = castlingKingFrom[us];
    newPosition.castling[us] = 0;
    newPositions[numberNewPositions] = newPosition;
    numberNewPositions++;
  }
}
