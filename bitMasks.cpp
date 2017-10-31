#include "bitMasks.hpp"
#include <iostream>
#include <bitset>
#include "utils.hpp"
#include "bitOperations.hpp"

#define UPPER_LEFT_SIDE_ZERO 0b1000000011000000111000001111000011111000111111001111111011111111
#define LOWER_RIGHT_SIDE_ZERO 0b1111111101111111001111110001111100001111000001110000001100000001
#define LOWER_LEFT_SIDE_ZERO 0b1111111011111100111110001111000011100000110000001000000000000000
#define UPPER_RIGHT_SIDE_ZERO 0b0000000000000001000000110000011100001111000111110011111101111111

namespace BitMasks
{
  std::array<Bitboard, 64> generateBitAtIndex();
  std::array<Bitboard, 8> generateRanks();
  int getHashKeyForRank(int fieldIndex, Bitboard occupancy)
  {
    return ((occupancy >> ((fieldIndex / 8)*8)) >> 1) & 0b111111;
  }
  int getHashKeyForFile(int fieldIndex, Bitboard occupancy)
  {
    return (((((occupancy >> (fieldIndex % 8)) & files[0] )*MAIN_DIAGONAL) >> 56) >> 1) & 0b111111;
  }
  int getHashKeyForDiagonal(int fieldIndex, Bitboard occupancy)
  {
    return ((((occupancy & diagonals64[fieldIndex])* files[0]) >> 56) >> 1) & 0b111111;
  }
  int getHashKeyForAntiDiagonal(int fieldIndex, Bitboard occupancy)
  {
    return ((((occupancy & antiDiagonals64[fieldIndex])* files[0]) >> 56) >> 1) & 0b111111;
  }
  std::array<Bitboard, 8> generateFiles();
  std::array<Bitboard, 64> generateRanks64();
  std::array<Bitboard, 64> generateFiles64();
  std::array<Bitboard, 64> generateDiagonals64();
  std::array<Bitboard, 64> generateAntiDiagonals64();
  std::array<Bitboard, 64> generatePossibleRanks();
  std::array<Bitboard, 64> generatePossibleFiles();

  const std::array<Bitboard, 64> bitAtIndex = generateBitAtIndex();
  const std::array<Bitboard, 8> ranks = generateRanks();
  const std::array<Bitboard, 8> files = generateFiles();
  const std::array<Bitboard, 64> ranks64 = generateRanks64();
  const std::array<Bitboard, 64> files64 = generateFiles64();
  const std::array<Bitboard, 64> diagonals64 = generateDiagonals64();
  const std::array<Bitboard, 64> antiDiagonals64 = generateAntiDiagonals64();
  const std::array<Bitboard, 64> possibleRanks = generatePossibleRanks();
  const std::array<Bitboard, 64> possibleFiles = generatePossibleFiles();

  std::array<std::array<Bitboard, 64>, 64> generateAntiDiagonalAttackTable();
  std::array<std::array<Bitboard, 64>, 64> generateDiagonalAttackTable();
  std::array<std::array<Bitboard, 64>, 64> generateFileAttackTable();
  std::array<std::array<Bitboard, 64>, 64> generateRankAttackTable();
  std::array<Bitboard, 64> generateKnightAttackTable();
  std::array<Bitboard, 64> generateKingAttackTable();

  const std::array<std::array<Bitboard, 64>, 64> antiDiagonalAttackTable = generateAntiDiagonalAttackTable();
  const std::array<std::array<Bitboard, 64>, 64> diagonalAttackTable = generateDiagonalAttackTable();
  const std::array<std::array<Bitboard, 64>, 64> fileAttackTable = generateFileAttackTable();
  const std::array<std::array<Bitboard, 64>, 64> rankAttackTable = generateRankAttackTable();
  const std::array<Bitboard, 64> knightAttackTable = generateKnightAttackTable();
  const std::array<Bitboard, 64> kingAttackTable = generateKingAttackTable();

  Bitboard rankToFile(Bitboard rank)
  {
    Bitboard ret = (((rank & 0b11111111) * MAIN_DIAGONAL) & files[7]) >> 7;
    return ret;
  }
  std::array<Bitboard, 64> generateBitAtIndex()
  {
    std::array<Bitboard, 64> ret;
    for (int i = 0; i < 64; i++)
    {
      ret[i] = Bitboard(0b1)<<i;
    }
    return ret;
  }
  std::array<Bitboard, 8> generateRanks()
  {
    std::array<Bitboard, 8> ret;
    for(int i = 0; i<8; i++)
    {
      ret[i] = (Bitboard)0b11111111 << (i*8);
    }
    return ret;
  }
  std::array<Bitboard, 8> generateFiles()
  {
    std::array<Bitboard, 8> ret;
    for(int i = 0; i<8; i++)
    {
      ret[i] = (Bitboard)0b0000000100000001000000010000000100000001000000010000000100000001 << i;
    }
    return ret;
  }
  std::array<Bitboard, 64> generateRanks64()
  {
    std::array<Bitboard, 64> ret;
    for(int i = 0; i<64; i++)
    {
      ret[i] = ranks[i/8];
    }
    return ret;
  }
  std::array<Bitboard, 64> generateFiles64()
  {
      std::array<Bitboard, 64> ret;
      for(int i = 0; i<64; i++)
      {
        ret[i] = files[i%8];
      }
      return ret;
  }
  std::array<Bitboard, 64> generateDiagonals64()
  {
  	std::array<Bitboard, 64> ret;
    for(int i = 0; i<8; i++)
    {
      Bitboard currentDiagonal = (MAIN_DIAGONAL << i) & UPPER_LEFT_SIDE_ZERO;
      Bitboard temp = currentDiagonal;
      do {
        unsigned long index;
        BitOperations::bitScanForward(index, temp);
        ret[index] = currentDiagonal;
        temp = temp & ~(bitAtIndex[index]);
      } while(temp);
    }
    for(int i = 1; i<8; i++)
    {
      Bitboard currentDiagonal = (MAIN_DIAGONAL >> i) & LOWER_RIGHT_SIDE_ZERO;
      Bitboard temp = currentDiagonal;
      do {
        unsigned long index;
        BitOperations::bitScanForward(index, temp);
        ret[index] = currentDiagonal;
        temp = temp & ~(bitAtIndex[index]);
      } while(temp);
    }
  	return ret;
  }
  std::array<Bitboard, 64> generateAntiDiagonals64()
  {
  	std::array<Bitboard, 64> ret;
    for(int i = 1; i<8; i++)
    {
      Bitboard currentAntiDiagonal = (ANTI_DIAGONAL << i) & LOWER_LEFT_SIDE_ZERO;
      Bitboard temp = currentAntiDiagonal;
      do {
        unsigned long index;
        BitOperations::bitScanForward(index, temp);
        ret[index] = currentAntiDiagonal;
        temp = temp & ~(bitAtIndex[index]);
      } while(temp);
    }
    Bitboard currentAntiDiagonal = ANTI_DIAGONAL;
    Bitboard temp = currentAntiDiagonal;
    do {
      unsigned long index;
      BitOperations::bitScanForward(index, temp);
      ret[index] = currentAntiDiagonal;
      temp = temp & ~(bitAtIndex[index]);
    } while(temp);
    for(int i = 1; i<8; i++)
    {
      Bitboard currentAntiDiagonal = (ANTI_DIAGONAL >> i) & UPPER_RIGHT_SIDE_ZERO;
      Bitboard temp = currentAntiDiagonal;
      do {
        unsigned long index;
        BitOperations::bitScanForward(index, temp);
        ret[index] = currentAntiDiagonal;
        temp = temp & ~(bitAtIndex[index]);
      } while(temp);
    }
  	return ret;
  }
  std::array<Bitboard, 64> generatePossibleRanks()
  {
    std::array<Bitboard, 64> ret;
    for (int i = 0; i < 64; i++)
    {
      ret[i] = 0;
      Bitboard temp = 0b10000001 | (i<<1);
      for(int j = 0; j<8; j++)
      {
        ret[i] |= temp<<(j*8);
      }
    }
    return ret;
  }
  std::array<Bitboard, 64> generatePossibleFiles()
  {
    std::array<Bitboard, 64> ret;
    for (int i = 0; i < 64; i++)
    {
      ret[i] = 0;
      Bitboard temp = rankToFile(0b10000001 | (i<<1));
      for(int j = 0; j<8; j++)
      {
        ret[i] |= temp<<(j);
      }
    }
    return ret;
  }

  std::array<std::array<Bitboard, 64>, 64> generateAntiDiagonalAttackTable()
  {
    std::array<std::array<Bitboard, 64>, 64> ret;
    for(int fieldIndex = 0; fieldIndex < 64; fieldIndex++)
    {
      for(int possibleRanksIndex = 0; possibleRanksIndex < 64; possibleRanksIndex++)
      {
        Bitboard tempAttackmask = 0;
        Bitboard occupancy = possibleRanks[possibleRanksIndex];
        for(int i = fieldIndex + NORTH_WEST; i<64; i+=NORTH_WEST)
        {
          if(fieldIndex%8 == 0)
          {
            break;
          }
          if(occupancy & bitAtIndex[i])
          {
            tempAttackmask |= bitAtIndex[i];
            break;
          }
          else
          {
            tempAttackmask |= bitAtIndex[i];
          }
        }
        for(int i = fieldIndex + SOUTH_EAST; i>=0; i+=SOUTH_EAST)
        {
          if(fieldIndex%8 == 7)
          {
            break;
          }
          if(occupancy & bitAtIndex[i])
          {
            tempAttackmask |= bitAtIndex[i];
            break;
          }
          else
          {
            tempAttackmask |= bitAtIndex[i];
          }
        }
        ret[fieldIndex][getHashKeyForAntiDiagonal(fieldIndex, occupancy)] = tempAttackmask;
        /*if(fieldIndex == 44)
        {
          printBitboard(possibleRanks[possibleRanksIndex]);
          printBitboard(tempAttackmask);
        }*/
      }
    }
    return ret;
  }
  std::array<std::array<Bitboard, 64>, 64> generateDiagonalAttackTable()
  {
    std::array<std::array<Bitboard, 64>, 64> ret;
    for(int fieldIndex = 0; fieldIndex < 64; fieldIndex++)
    {
      for(int possibleRanksIndex = 0; possibleRanksIndex < 64; possibleRanksIndex++)
      {
        Bitboard tempAttackmask = 0;
        Bitboard occupancy = possibleRanks[possibleRanksIndex];
        for(int i = fieldIndex + NORTH_EAST; i<64; i+=NORTH_EAST)
        {
          if(fieldIndex%8 == 7)
          {
            break;
          }
          if(occupancy & bitAtIndex[i])
          {
            tempAttackmask |= bitAtIndex[i];
            break;
          }
          else
          {
            tempAttackmask |= bitAtIndex[i];
          }
        }
        for(int i = fieldIndex + SOUTH_WEST; i>=0; i+=SOUTH_WEST)
        {
          if(fieldIndex%8 == 0)
          {
            break;
          }
          if(occupancy & bitAtIndex[i])
          {
            tempAttackmask |= bitAtIndex[i];
            break;
          }
          else
          {
            tempAttackmask |= bitAtIndex[i];
          }
        }
        ret[fieldIndex][getHashKeyForDiagonal(fieldIndex, occupancy)] = tempAttackmask;
        /*if(fieldIndex == 44)
        {
          printBitboard(possibleRanks[possibleRanksIndex]);
          printBitboard(tempAttackmask);
        }*/
      }
    }
    return ret;
  }
  std::array<std::array<Bitboard, 64>, 64> generateFileAttackTable()
  {
    std::array<std::array<Bitboard, 64>, 64> ret;
    for(int fieldIndex = 0; fieldIndex < 64; fieldIndex++)
    {
      for(int possibleFilesIndex = 0; possibleFilesIndex < 64; possibleFilesIndex++)
      {
        Bitboard tempAttackmask = 0;
        Bitboard occupancy = possibleFiles[possibleFilesIndex];
        for(int i = fieldIndex + NORTH; i<64; i+=NORTH)
        {
          if(occupancy & bitAtIndex[i])
          {
            tempAttackmask |= bitAtIndex[i];
            break;
          }
          else
          {
            tempAttackmask |= bitAtIndex[i];
          }
        }
        for(int i = fieldIndex + SOUTH; i>=0; i+=SOUTH)
        {
          if(occupancy & bitAtIndex[i])
          {
            tempAttackmask |= bitAtIndex[i];
            break;
          }
          else
          {
            tempAttackmask |= bitAtIndex[i];
          }
        }
        ret[fieldIndex][getHashKeyForFile(fieldIndex, occupancy)] = tempAttackmask;
        /*if(fieldIndex == 44)
        {
          printBitboard(possibleFiles[possibleFilesIndex]);
          printBitboard(tempAttackmask);
        }*/
      }
    }
    return ret;
  }
  std::array<std::array<Bitboard, 64>, 64> generateRankAttackTable()
  {
    std::array<std::array<Bitboard, 64>, 64> ret;
    for(int fieldIndex = 0; fieldIndex < 64; fieldIndex++)
    {
      for(int possibleRanksIndex = 0; possibleRanksIndex < 64; possibleRanksIndex++)
      {
        Bitboard tempAttackmask = 0;
        Bitboard occupancy = possibleRanks[possibleRanksIndex];
        for(int i = fieldIndex + EAST;i<64; i+=EAST)
        {
          if(fieldIndex%8 == 7)
          {
            break;
          }
          if(occupancy & bitAtIndex[i])
          {
            tempAttackmask |= bitAtIndex[i];
            break;
          }
          else
          {
            tempAttackmask |= bitAtIndex[i];
          }
        }
        for(int i = fieldIndex + WEST;i>=0; i+=WEST)
        {
          if(fieldIndex%8 == 0)
          {
            break;
          }
          if(occupancy & bitAtIndex[i])
          {
            tempAttackmask |= bitAtIndex[i];
            break;
          }
          else
          {
            tempAttackmask |= bitAtIndex[i];
          }
        }
        ret[fieldIndex][getHashKeyForRank(fieldIndex, occupancy)] = tempAttackmask;
        /*if(fieldIndex == 44)
        {
          printBitboard(possibleRanks[possibleRanksIndex]);
          printBitboard(tempAttackmask);
        }*/
      }
    }
    return ret;
  }
  std::array<Bitboard, 64> generateKnightAttackTable()
  {
    std::array<Bitboard, 64> ret;

    for (int i = 0; i < 64; i++) {
      ret[i] = 0;
			if (i + NORTH + NORTH_WEST <= 63 && i % 8 != 0) {
				ret[i] |= bitAtIndex[i + NORTH + NORTH_WEST];
			}
			if (i + NORTH + NORTH_EAST <= 63 && (i + 1) % 8 != 0) {
				ret[i] |= bitAtIndex[i + NORTH + NORTH_EAST];
			}
			if (i + NORTH_WEST + WEST <= 63 && i % 8 > 1) {
				ret[i] |= bitAtIndex[i + NORTH_WEST + WEST];
			}
			if (i + NORTH_EAST + EAST <= 63 && (i + 1) % 8 < 7 && (i + 1) % 8 != 0) {
				ret[i] |= bitAtIndex[i + NORTH_EAST + EAST];
			}
			if (i + SOUTH + SOUTH_WEST >= 0 && i % 8 != 0) {
				ret[i] |= bitAtIndex[i + SOUTH + SOUTH_WEST];
			}
			if (i + SOUTH + SOUTH_EAST >= 0 && (i + 1) % 8 != 0) {
				ret[i] |= bitAtIndex[i + SOUTH + SOUTH_EAST];
			}
			if (i + SOUTH_WEST + WEST >= 0 && i % 8 > 1) {
				ret[i] |= bitAtIndex[i + SOUTH_WEST + WEST];
			}
			if (i + SOUTH_EAST + EAST >= 0 && (i + 1) % 8 < 7 && (i + 1) % 8 != 0) {
				ret[i] |= bitAtIndex[i + SOUTH_EAST + EAST];
			}
		}
    return ret;
  }
  std::array<Bitboard, 64> generateKingAttackTable()
  {
    std::array<Bitboard, 64> ret;
    for (int i = 0; i < 64; i++) {
			ret[i] = 0;
			if (i + NORTH <= 63) {
				ret[i] |= bitAtIndex[i + NORTH];
				if (i % 8 != 0) {
					ret[i] |= bitAtIndex[i + NORTH_WEST];
				}
				if ((i + EAST) % 8 != 0) {
					ret[i] |= bitAtIndex[i + NORTH_EAST];
				}
			}
			if (i + SOUTH >= 0) {
				ret[i] |= bitAtIndex[i + SOUTH];
				if (i % 8 != 0) {
					ret[i] |= bitAtIndex[i + SOUTH_WEST];
				}
				if ((i + EAST) % 8 != 0) {
					ret[i] |= bitAtIndex[i + SOUTH_EAST];
				}
			}
			if (i % 8 != 0) {
				ret[i] |= bitAtIndex[i WEST];
			}
			if ((i + EAST) % 8 != 0) {
				ret[i] |= bitAtIndex[i + EAST];
			}
		}
    return ret;
  }
}
