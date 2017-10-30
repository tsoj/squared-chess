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
        for(int i = fieldIndex + 7; i<64; i+=7)
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
        for(int i = fieldIndex - 7; i>=0; i-=7)
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
        /*if(fieldIndex == 7)
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
        for(int i = fieldIndex + 9; i<64; i+=9)
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
        for(int i = fieldIndex - 9; i>=0; i-=9)
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
        /*if(fieldIndex == 47)
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
        for(int i = fieldIndex + 8; i<64; i+=8)
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
        for(int i = fieldIndex - 8; i>=0; i-=8)
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
        /*if(fieldIndex == 0)
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
        for(int i = fieldIndex + 1;i<64; i++)
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
        for(int i = fieldIndex - 1;i>=0; i--)
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
        /*if(fieldIndex == 47)
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
			if (i + 15 <= 63 && i % 8 != 0) {
				ret[i] |= bitAtIndex[i + 15];
			}
			if (i + 17 <= 63 && (i + 1) % 8 != 0) {
				ret[i] |= bitAtIndex[i + 17];
			}
			if (i + 6 <= 63 && i % 8 > 1) {
				ret[i] |= bitAtIndex[i + 6];
			}
			if (i + 10 <= 63 && (i + 1) % 8 < 7 && (i + 1) % 8 != 0) {
				ret[i] |= bitAtIndex[i + 10];
			}
			if (i - 17 >= 0 && i % 8 != 0) {
				ret[i] |= bitAtIndex[i - 17];
			}
			if (i - 15 >= 0 && (i + 1) % 8 != 0) {
				ret[i] |= bitAtIndex[i - 15];
			}
			if (i - 10 >= 0 && i % 8 > 1) {
				ret[i] |= bitAtIndex[i - 10];
			}
			if (i - 6 >= 0 && (i + 1) % 8 < 7 && (i + 1) % 8 != 0) {
				ret[i] |= bitAtIndex[i - 6];
			}
		}

    return ret;
  }
  std::array<Bitboard, 64> generateKingAttackTable()
  {
    std::array<Bitboard, 64> ret;
    for (int i = 0; i < 64; i++) {
			ret[i] = 0;
			if (i + 8 <= 63) {
				ret[i] |= bitAtIndex[i + 8];
				if (i % 8 != 0) {
					ret[i] |= bitAtIndex[i + 7];
				}
				if ((i + 1) % 8 != 0) {
					ret[i] |= bitAtIndex[i + 9];
				}
			}
			if (i - 8 >= 0) {
				ret[i] |= bitAtIndex[i - 8];
				if (i % 8 != 0) {
					ret[i] |= bitAtIndex[i - 9];
				}
				if ((i + 1) % 8 != 0) {
					ret[i] |= bitAtIndex[i - 7];
				}
			}
			if (i % 8 != 0) {
				ret[i] |= bitAtIndex[i - 1];
			}
			if ((i + 1) % 8 != 0) {
				ret[i] |= bitAtIndex[i + 1];
			}
		}
    return ret;
  }
}
