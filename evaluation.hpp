#pragma once

#include "types.hpp"
#include "position.hpp"
#include "data.hpp"
#include "bitOperations.hpp"

using namespace Data;
using namespace BitOperations;

inline Score evaluation(COLOR_TYPE us, COLOR_TYPE enemy, Position position)
{
  static Score ret;
  ret = 0;

  static unsigned long fieldIndex;
  static Bitboard tempOccupancy;

  if(position.pieces[PAWN])
  {
    tempOccupancy = position.pieces[PAWN];
    do
    {
      bitScanForward(fieldIndex, tempOccupancy);
      if(bitAtIndex[fieldIndex] & position.colors[us])
      {
        ret += pieceValues[PAWN];
      }
      else{
        ret -= pieceValues[PAWN];
      }
      ret+=(fieldIndex / 8);
      tempOccupancy &= ~bitAtIndex[fieldIndex];
    } while(tempOccupancy);
  }
  if(position.pieces[KNIGHT])
  {
    tempOccupancy = position.pieces[KNIGHT];
    do
    {
      bitScanForward(fieldIndex, tempOccupancy);
      if(bitAtIndex[fieldIndex] & position.colors[us])
      {
        ret += pieceValues[KNIGHT];
      }
      else{
        ret -= pieceValues[KNIGHT];
      }
      tempOccupancy &= ~bitAtIndex[fieldIndex];
    } while(tempOccupancy);
  }
  if(position.pieces[BISHOP])
  {
    tempOccupancy = position.pieces[BISHOP];
    do
    {
      bitScanForward(fieldIndex, tempOccupancy);
      if(bitAtIndex[fieldIndex] & position.colors[us])
      {
        ret += pieceValues[BISHOP];
      }
      else{
        ret -= pieceValues[BISHOP];
      }
      tempOccupancy &= ~bitAtIndex[fieldIndex];
    } while(tempOccupancy);
  }
  if(position.pieces[ROOK])
  {
    tempOccupancy = position.pieces[ROOK];
    do
    {
      bitScanForward(fieldIndex, tempOccupancy);
      if(bitAtIndex[fieldIndex] & position.colors[us])
      {
        ret += pieceValues[ROOK];
      }
      else{
        ret -= pieceValues[ROOK];
      }
      tempOccupancy &= ~bitAtIndex[fieldIndex];
    } while(tempOccupancy);
  }
  if(position.pieces[QUEEN])
  {
    tempOccupancy = position.pieces[QUEEN];
    do
    {
      bitScanForward(fieldIndex, tempOccupancy);
      if(bitAtIndex[fieldIndex] & position.colors[us])
      {
        ret += pieceValues[QUEEN];
      }
      else{
        ret -= pieceValues[QUEEN];
      }
      tempOccupancy &= ~bitAtIndex[fieldIndex];
    } while(tempOccupancy);
  }
  if(position.pieces[KING])
  {
    tempOccupancy = position.pieces[KING];
    do
    {
      bitScanForward(fieldIndex, tempOccupancy);
      if(bitAtIndex[fieldIndex] & position.colors[us])
      {
        ret += pieceValues[KING];
      }
      else{
        ret -= pieceValues[KING];
      }
      tempOccupancy &= ~bitAtIndex[fieldIndex];
    } while(tempOccupancy);
  }
  return ret;
}
