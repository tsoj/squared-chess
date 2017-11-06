#include "position.hpp"
#include "bitMasks.hpp"
using namespace BitMasks;
//0b 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
Position::Position()
{
  setToStartposition();
}

void Position::setToStartposition()
{
  colors[WHITE] = 0b0000000000000000000000000000000000000000000000001111111111111111;
  colors[BLACK] = 0b1111111111111111000000000000000000000000000000000000000000000000;
  pieces[PAWN] = 0b0000000011111111000000000000000000000000000000001111111100000000;
  colors[WHITE] = 0b0000000000000000000000000000000000000000000000000000000011111111;
  colors[BLACK] = 0b1111111100000000000000000000000000000000000000000000000000000000;
  pieces[PAWN] = 0b0;
  pieces[KNIGHT] = 0b0100001000000000000000000000000000000000000000000000000001000010;
  pieces[BISHOP] = 0b0010010000000000000000000000000000000000000000000000000000100100;
  pieces[ROOK] = 0b1000000100000000000000000000000000000000000000000000000010000001;
  pieces[QUEEN] = 0b0000100000000000000000000000000000000000000000000000000000001000;
  pieces[KING] = 0b0001000000000000000000000000000000000000000000000000000000010000;
  enPassant = 0b0;
  castling[BLACK] = 0b1001000100000000000000000000000000000000000000000000000000000000;
  castling[WHITE] = 0b0000000000000000000000000000000000000000000000000000000010010001;
  whoIsToMove = WHITE_TO_MOVE;
  numberHalfmovesPlayed = 0;
  lastMove.to = 0;
  lastMove.from = 0;
  lastMove.lastMoved = NO_PIECE;
  lastMove.lastCaptured = NO_PIECE;
  lastMove.promotedTo = NO_PIECE;
}

void Position::clearBoard()
{
  colors[WHITE] = 0b0;
  colors[BLACK] = 0b0;
  pieces[PAWN] = 0b0;
  pieces[KNIGHT] = 0b0;
  pieces[BISHOP] = 0b0;
  pieces[ROOK] = 0b0;
  pieces[QUEEN] = 0b0;
  pieces[KING] = 0b00;
  enPassant = 0b0;
  castling[WHITE] = 0b0;
  castling[BLACK] = 0b0;
  whoIsToMove = WHITE_TO_MOVE;
  numberHalfmovesPlayed = 0;
  lastMove.to = 0;
  lastMove.from = 0;
  lastMove.lastMoved = NO_PIECE;
  lastMove.lastCaptured = NO_PIECE;
  lastMove.promotedTo = NO_PIECE;
}


int Position::getNumberHalfmovesPlayed()
{
  return numberHalfmovesPlayed;
}
int Position::getNumberMovesPlayed()
{
  return numberHalfmovesPlayed / 2;
}
bool Position::whiteToMove()
{
  if(whoIsToMove == WHITE_TO_MOVE)
  {
    return true;
  }
  return false;
}
bool Position::blackToMove()
{
  if(whoIsToMove == BLACK_TO_MOVE)
  {
    return true;
  }
  return false;
}
