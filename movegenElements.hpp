#pragma once

#include "bitMasks.hpp"
#include "bitOperations.hpp"
#include "utils.hpp"
#include <vector>

using namespace BitOperations;
using namespace BitMasks;


inline bool isKingInCheck(const COLOR_TYPE & us, const COLOR_TYPE & enemy, const Position & position, int kingsIndex)
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
inline bool isKingInCheck(const COLOR_TYPE &  us, const COLOR_TYPE &  enemy, const Position & position)
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
  newPosition.lastMove.lastCaptured = NO_PIECE;
  newPosition.lastMove.promotedTo = NO_PIECE;
  newPosition.colors[us] = (newPosition.colors[us] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.pieces[Pt] = (newPosition.pieces[Pt] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.lastMove.lastMoved = Pt;
  newPosition.lastMove.to = toFieldIndex;
  newPosition.lastMove.from = fromFieldIndex;
  newPosition.castling &= ~bitAtIndex[fromFieldIndex];
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
    newPosition.lastMove.promotedTo = NO_PIECE;
    if(newPosition.pieces[i] & bitAtIndex[toFieldIndex])
    {
      newPosition.lastMove.lastCaptured = PIECE_TYPE(i);
      newPosition.colors[enemy] = newPosition.colors[enemy] & ~bitAtIndex[toFieldIndex];
      newPosition.pieces[PIECE_TYPE(i)] = newPosition.pieces[PIECE_TYPE(i)] & ~bitAtIndex[toFieldIndex];
    }
  }
  newPosition.colors[us] = (newPosition.colors[us] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.pieces[Pt] = (newPosition.pieces[Pt] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.lastMove.lastMoved = Pt;
  newPosition.lastMove.to = toFieldIndex;
  newPosition.lastMove.from = fromFieldIndex;
  newPosition.castling &= ~bitAtIndex[fromFieldIndex];
  newPosition.castling &= ~bitAtIndex[toFieldIndex];
}

inline void applyCaptureEnPassantMove(
  const COLOR_TYPE & us, const COLOR_TYPE & enemy,
  Position & newPosition,
  const int & toFieldIndex, const int & fromFieldIndex,
  const Bitboard & enPassant,
  const int & moveDirection
)
{
  newPosition.lastMove.lastCaptured = PAWN;
  newPosition.colors[enemy] = newPosition.colors[enemy] & ~bitAtIndex[toFieldIndex - moveDirection];
  newPosition.pieces[PAWN] = newPosition.pieces[PAWN] & ~bitAtIndex[toFieldIndex - moveDirection];
  newPosition.colors[us] = (newPosition.colors[us] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.pieces[PAWN] = (newPosition.pieces[PAWN] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.lastMove.lastMoved = PAWN;
  newPosition.lastMove.to = toFieldIndex;
  newPosition.lastMove.from = fromFieldIndex;
}

template<PIECE_TYPE Pt>
inline void generateMoves(
  const COLOR_TYPE & us, const COLOR_TYPE & enemy,
  const Position & origPosition,
  const Bitboard & occupancy,
  PositionVector & newPositions
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
          newPositions.array[newPositions.size] = newPosition;
          newPositions.size++;
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
          newPositions.array[newPositions.size] = newPosition;
          newPositions.size++;
          captureAttackMask &= ~bitAtIndex[toFieldIndex];
        } while(captureAttackMask);
      }
      pieceOccupancy &= ~bitAtIndex[fromFieldIndex];
    } while(pieceOccupancy);
  }
}


#define ADD_PROMOTION \
newPosition.pieces[PAWN] = newPosition.pieces[PAWN] & ~bitAtIndex[toFieldIndex];\
\
newPosition.pieces[KNIGHT] = newPosition.pieces[KNIGHT] | bitAtIndex[toFieldIndex];\
newPosition.lastMove.lastMoved = KNIGHT;\
newPosition.lastMove.promotedTo = KNIGHT;\
newPositions.array[newPositions.size] = newPosition;\
newPositions.size++;\
newPosition.pieces[KNIGHT] = newPosition.pieces[KNIGHT] &  ~bitAtIndex[toFieldIndex];\
\
newPosition.pieces[BISHOP] = newPosition.pieces[BISHOP] | bitAtIndex[toFieldIndex];\
newPosition.lastMove.lastMoved = BISHOP;\
newPosition.lastMove.promotedTo = BISHOP;\
newPositions.array[newPositions.size] = newPosition;\
newPositions.size++;\
newPosition.pieces[BISHOP] = newPosition.pieces[BISHOP] &  ~bitAtIndex[toFieldIndex];\
\
newPosition.pieces[ROOK] = newPosition.pieces[ROOK] | bitAtIndex[toFieldIndex];\
newPosition.lastMove.lastMoved = ROOK;\
newPosition.lastMove.promotedTo = ROOK;\
newPositions.array[newPositions.size] = newPosition;\
newPositions.size++;\
newPosition.pieces[ROOK] = newPosition.pieces[ROOK] &  ~bitAtIndex[toFieldIndex];\
\
newPosition.pieces[QUEEN] = newPosition.pieces[QUEEN] | bitAtIndex[toFieldIndex];\
newPosition.lastMove.lastMoved = QUEEN;\
newPosition.lastMove.promotedTo = QUEEN;
inline void generatePawnMoves(
  const COLOR_TYPE & us, const COLOR_TYPE & enemy,
  const Position & origPosition,
  PositionVector & newPositions,
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
        ADD_PROMOTION
      }
      newPositions.array[newPositions.size] = newPosition;
      newPositions.size++;

      toFieldIndex += moveDirection;
      if(
        !((origPosition.colors[BLACK] | origPosition.colors[WHITE]) & bitAtIndex[toFieldIndex]) &&
        (bitAtIndex[fromFieldIndex] & pawnDoubleMove)
      )
      {
        newPosition = origPosition;
        newPosition.enPassant = bitAtIndex[fromFieldIndex - moveDirection];
        applyQuietMove<PAWN>(us, enemy, newPosition, toFieldIndex, fromFieldIndex);
        newPositions.array[newPositions.size] = newPosition;
        newPositions.size++;
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
        newPositions.array[newPositions.size] = newPosition;
        newPositions.size++;
      }
      else
      {
        newPosition = origPosition;
        applyCaptureMove<PAWN>(us, enemy, newPosition, toFieldIndex, fromFieldIndex);
        if(bitAtIndex[toFieldIndex] & promotionMove)
        {
          ADD_PROMOTION
        }
        newPositions.array[newPositions.size] = newPosition;
        newPositions.size++;
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
        newPositions.array[newPositions.size] = newPosition;
        newPositions.size++;
      }
      else
      {
        newPosition = origPosition;
        applyCaptureMove<PAWN>(us, enemy, newPosition, toFieldIndex, fromFieldIndex);
        if(bitAtIndex[toFieldIndex] & promotionMove)
        {
          ADD_PROMOTION
        }
        newPositions.array[newPositions.size] = newPosition;
        newPositions.size++;
      }
    }
    pawnOccupancy &= ~bitAtIndex[fromFieldIndex];
  } while(pawnOccupancy);
}


inline void generateCastlingMoves(
    const COLOR_TYPE & us, const COLOR_TYPE & enemy,
    const Position & origPosition,
    PositionVector & newPositions,
    const Bitboard & occupancy
)
{
  static Position newPosition;
  //kingsside
  if(
    !(occupancy & castlingKingsideOccupancyRelevant[us]) &&
    (origPosition.castling & bitAtIndex[castlingRookKingsideFrom[us]]) &&
    (origPosition.castling & bitAtIndex[castlingKingFrom[us]])
  )
  {
    newPosition = origPosition;
    newPosition.lastMove.lastCaptured = NO_PIECE;
    newPosition.colors[us] =
      (newPosition.colors[us] & ~bitAtIndex[castlingKingFrom[us]]) | bitAtIndex[castlingKingKingsideTo[us]];
    newPosition.pieces[KING] =
      (newPosition.pieces[KING] & ~bitAtIndex[castlingKingFrom[us]]) | bitAtIndex[castlingKingKingsideTo[us]];
    newPosition.colors[us] =
      (newPosition.colors[us] & ~bitAtIndex[castlingRookKingsideFrom[us]]) | bitAtIndex[castlingRookKingsideTo[us]];
    newPosition.pieces[ROOK] =
      (newPosition.pieces[ROOK] & ~bitAtIndex[castlingRookKingsideFrom[us]]) | bitAtIndex[castlingRookKingsideTo[us]];
    newPosition.lastMove.lastMoved = KING;
    newPosition.lastMove.to = castlingKingKingsideTo[us];
    newPosition.lastMove.from = castlingKingFrom[us];
    newPosition.castling &= ~bitAtIndex[castlingKingFrom[us]];
    newPositions.array[newPositions.size] = newPosition;
    newPositions.size++;
  }
  //queenside
  if(
    !(occupancy & castlingQueensideOccupancyRelevant[us]) &&
    (origPosition.castling & bitAtIndex[castlingRookQueensideFrom[us]]) &&
    (origPosition.castling & bitAtIndex[castlingKingFrom[us]])
  )
  {
    newPosition = origPosition;
    newPosition.lastMove.lastCaptured = NO_PIECE;
    newPosition.colors[us] =
      (newPosition.colors[us] & ~bitAtIndex[castlingKingFrom[us]]) | bitAtIndex[castlingKingQueensideTo[us]];
    newPosition.pieces[KING] =
      (newPosition.pieces[KING] & ~bitAtIndex[castlingKingFrom[us]]) | bitAtIndex[castlingKingQueensideTo[us]];
    newPosition.colors[us] =
      (newPosition.colors[us] & ~bitAtIndex[castlingRookQueensideFrom[us]]) | bitAtIndex[castlingRookQueensideTo[us]];
    newPosition.pieces[ROOK] =
      (newPosition.pieces[ROOK] & ~bitAtIndex[castlingRookQueensideFrom[us]]) | bitAtIndex[castlingRookQueensideTo[us]];
    newPosition.lastMove.lastMoved = KING;
    newPosition.lastMove.to = castlingKingQueensideTo[us];
    newPosition.lastMove.from = castlingKingFrom[us];
    newPosition.castling &= ~bitAtIndex[castlingKingFrom[us]];
    newPositions.array[newPositions.size] = newPosition;
    newPositions.size++;
  }
}
