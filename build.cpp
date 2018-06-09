#include "countZeros.hpp"

#include <cstdint>
#include <fstream>
#include <string>
#include <iostream>
#include <bitset>

#define NORTH 8
#define SOUTH -8
#define EAST 1
#define WEST -1
#define NORTH_EAST 9
#define NORTH_WEST 7
#define SOUTH_WEST -9
#define SOUTH_EAST -7

const uint64_t UPPER_LEFT_SIDE_ZERO =  0b1000000011000000111000001111000011111000111111001111111011111111;
const uint64_t LOWER_RIGHT_SIDE_ZERO = 0b1111111101111111001111110001111100001111000001110000001100000001;
const uint64_t LOWER_LEFT_SIDE_ZERO = 0b1111111011111100111110001111000011100000110000001000000000000000;
const uint64_t UPPER_RIGHT_SIDE_ZERO = 0b0000000000000001000000110000011100001111000111110011111101111111;
const uint64_t MAIN_DIAGONAL = 0b1000000001000000001000000001000000001000000001000000001000000001; //A1 to H8
const uint64_t ANTI_DIAGONAL = 0b0000000100000010000001000000100000010000001000000100000010000000; //H1 to A8

uint64_t rankToFile(uint64_t rank, uint64_t files[8])
{
  return (((rank & 0b11111111U)*MAIN_DIAGONAL) & files[7]) >> 7;
}
size_t getHashkeyRank(size_t index, uint64_t occupancy)
{
  return ((occupancy >> ((index / 8)*8)) >> 1) & 0b111111U;
}
size_t getHashkeyFile(size_t index, uint64_t occupancy, uint64_t files[8])
{
  return (((((occupancy >> (index % 8)) & files[0] )*MAIN_DIAGONAL) >> 56) >> 1) & 0b111111U;
}
size_t getHashkeyDiagonal(size_t index, uint64_t occupancy, uint64_t files[8], uint64_t diagonals64[64])
{
  return ((((occupancy & diagonals64[index])*files[0]) >> 56) >> 1) & 0b111111U;
}
size_t getHashkeyAntiDiagonal(size_t index, uint64_t occupancy, uint64_t files[8], uint64_t antiDiagonals64[64])
{
  return ((((occupancy & antiDiagonals64[index])*files[0]) >> 56) >> 1) & 0b111111U;
}

inline std::string getBitboardString(const uint64_t a)
{
  std::string ret = "";
  for(size_t rank = 7; rank<8; rank--)
  {
    for(size_t file = 0; file<8; file++)
    {
      if(((uint64_t)(0b1) << (8*rank + file) & a) != 0)
      {
        ret+="1";
      }
      else
      {
        ret+="0";
      }
    }
    ret+="\n";
  }
  return ret;
}
void printBitboard(uint64_t b)
{

  std::cout << getBitboardString(b) <<  std::endl;
}

void stringToFile(std::string path, std::string s)
{
  std::ofstream file(path);
  file << s;
  file.close();
}
template<size_t size>
std::string arrayToString(uint64_t a[size])
{
  std::string ret = "{";
  for(size_t i = 0; i< size; i++)
  {
    ret.append(std::to_string(a[i]));
    ret.append("U");
    if(i<size-1)
    {
      ret.append(",");
    }
    else
    {
      ret.append("}");
    }
  }
  return ret;
}
template<size_t size1, size_t size2>
std::string arrayOfArrayToString(uint64_t a[size1][size2])
{
  std::string ret = "{";
  for(size_t i = 0; i<size1; i++)
  {
    ret.append(arrayToString<size2>(a[i]));
    if(i<size1-1)
    {
      ret.append(",");
    }
    else
    {
      ret.append("}");
    }
  }
  return ret;
}
template<size_t size1, size_t size2, size_t size3>
std::string arrayOfArrayOfArrayToString(uint64_t a[size1][size2][size3])
{
  std::string ret = "{";
  for(size_t i = 0; i<size1; i++)
  {
    ret.append(arrayOfArrayToString(a[i]));
    if(i<size1-1)
    {
      ret.append(",");
    }
    else
    {
      ret.append("}");
    }
  }
  return ret;
}

uint64_t testOccupancy =
  0b0000000000000000000000000001100000010000001001011111111010010001U |
  0b1001000101111101011100110000000000000010100000000000000000000000U;

int main()
{
  uint64_t bitAtIndex[64] = {0};
  for(size_t i = 0; i<64; i++)
  {
    bitAtIndex[i] = (uint64_t)(0b1) << i;
  }
  stringToFile("./chessData/bitAtIndex.in", arrayToString<64>(bitAtIndex));

  uint64_t ranks[8] = {0};
  for(size_t i = 0; i<8; i++)
  {
    ranks[i] = (uint64_t)(0b11111111) << (i*8);
  }
  stringToFile("./chessData/ranks.in", arrayToString<8>(ranks));

  uint64_t files[8] = {0};
  for(size_t i = 0; i<8; i++)
  {
    files[i] = (uint64_t)(0b0000000100000001000000010000000100000001000000010000000100000001) << i;
  }
  stringToFile("./chessData/files.in", arrayToString<8>(files));

  uint64_t ranks64[64] = {0};
  for(size_t i = 0; i<64; i++)
  {
    ranks64[i] = ranks[i/8];
  }
  stringToFile("./chessData/ranks64.in", arrayToString<64>(ranks64));

  uint64_t files64[64] = {0};
  for(size_t i = 0; i<64; i++)
  {
    files64[i] = files[i%8];
  }
  stringToFile("./chessData/files64.in", arrayToString<64>(files64));

  uint64_t diagonals64[64] = {0};
  for(size_t i = 0; i<8; i++)
  {
    uint64_t currentDiagonal = (MAIN_DIAGONAL << i) & UPPER_LEFT_SIDE_ZERO;
    uint64_t tmp = currentDiagonal;
    while(tmp!=0)
    {
      size_t index = CountZeros::trailingZeros(tmp);
      diagonals64[index] = currentDiagonal;
      tmp = tmp & (~bitAtIndex[index]);
    }
  }
  for(size_t i = 1; i<8; i++)
  {
    uint64_t currentDiagonal = (MAIN_DIAGONAL >> i) & LOWER_RIGHT_SIDE_ZERO;
    uint64_t tmp = currentDiagonal;
    while(tmp!=0)
    {
      size_t index = CountZeros::trailingZeros(tmp);
      diagonals64[index] = currentDiagonal;
      tmp = tmp & ~bitAtIndex[index];
    }
  }
  stringToFile("./chessData/diagonals64.in", arrayToString<64>(diagonals64));

  uint64_t antiDiagonals64[64] = {0};
  for(int i = 1; i<8; i++)
  {
    uint64_t currentAntiDiagonal = (ANTI_DIAGONAL << i) & LOWER_LEFT_SIDE_ZERO;
    uint64_t tmp = currentAntiDiagonal;
    while(tmp!=0)
    {
      size_t index = CountZeros::trailingZeros(tmp);
      antiDiagonals64[index] = currentAntiDiagonal;
      tmp = tmp & ~bitAtIndex[index];
    }
  }
  {
    uint64_t currentAntiDiagonal = ANTI_DIAGONAL;
    uint64_t tmp = currentAntiDiagonal;
    while(tmp!=0)
    {
      size_t index = CountZeros::trailingZeros(tmp);
      antiDiagonals64[index] = currentAntiDiagonal;
      tmp = tmp & ~bitAtIndex[index];
    }
  }
  for(int i = 1; i<8; i++)
  {
    uint64_t currentAntiDiagonal = (ANTI_DIAGONAL >> i) & UPPER_RIGHT_SIDE_ZERO;
    uint64_t tmp = currentAntiDiagonal;
    while(tmp!=0)
    {
      size_t index = CountZeros::trailingZeros(tmp);
      antiDiagonals64[index] = currentAntiDiagonal;
      tmp = tmp & ~bitAtIndex[index];
    }
  }
  stringToFile("./chessData/antiDiagonals64.in", arrayToString<64>(antiDiagonals64));

  uint64_t possibleRanks[64] = {0};
  for(size_t i = 0; i<64; i++)
  {
    uint64_t tmp = (uint64_t)(0b10000001) | (((uint64_t)(i))<<1);
    for(int j = 0; j<8; j++)
    {
      possibleRanks[i] |= tmp << (j*8);
    }
  }

  uint64_t possibleFiles[64] = {0};
  for(size_t i = 0; i<64; i++)
  {
    uint64_t tmp = rankToFile((uint64_t)(0b10000001) | (((uint64_t)(i)) << 1), files);
    for(int j = 0; j<8; j++)
    {
      possibleFiles[i] |= tmp << j;
    }
  }

  uint64_t antiDiagonalAttackTable[64][64] = {0};
  for(size_t index = 0; index<64; index++)
  {
    for(size_t possibleRanksIndex = 0; possibleRanksIndex<64; possibleRanksIndex++)
    {
      uint64_t tmpAttackMask = 0;
      uint64_t occupancy = possibleRanks[possibleRanksIndex];
      size_t i = index;
      while(true)
      {
        i += NORTH_WEST;
        if(i >= 64)
        {
          break;
        }
        if(index%8 == 0)
        {
          break;
        }
        if((occupancy & bitAtIndex[i]) != 0)
        {
          tmpAttackMask |= bitAtIndex[i];
          break;
        }
        else
        {
            tmpAttackMask |= bitAtIndex[i];
        }
      }
      i = index;
      while(true)
      {
        i += SOUTH_EAST;
        if(i>=64/*equals i < 0*/)
        {
          break;
        }
        if(index%8 == 7)
        {
          break;
        }
        if((occupancy & bitAtIndex[i]) != 0)
        {
          tmpAttackMask |= bitAtIndex[i];
          break;
        }
        else
        {
          tmpAttackMask |= bitAtIndex[i];
        }
      }
      antiDiagonalAttackTable[index][getHashkeyAntiDiagonal(index, occupancy, files, antiDiagonals64)] = tmpAttackMask;
    }
  }
  stringToFile("./chessData/antiDiagonalAttackTable.in", arrayOfArrayToString<64, 64>(antiDiagonalAttackTable));

  uint64_t t = 0;

  uint64_t diagonalAttackTable[64][64] = {0};
  for(size_t index = 0; index<64; index++)
  {
    for(size_t possibleRanksIndex = 0; possibleRanksIndex<64; possibleRanksIndex++)
    {
      uint64_t tmpAttackMask = 0;
      uint64_t occupancy = possibleRanks[possibleRanksIndex];
      size_t i = index;
      while(true)
      {
        i += NORTH_EAST;
        if(i >= 64)
        {
          break;
        }
        if(index%8 == 7)
        {
          break;
        }
        if((occupancy & bitAtIndex[i]) != 0)
        {
          tmpAttackMask |= bitAtIndex[i];
          break;
        }
        else
        {
            tmpAttackMask |= bitAtIndex[i];
        }
      }
      i = index;
      while(true)
      {
        i += SOUTH_WEST;

        if(i>=64/*equals i < 0*/)
        {
          break;
        }
        if(index%8 == 0)
        {
          break;
        }
        if((occupancy & bitAtIndex[i]) != 0)
        {
          tmpAttackMask |= bitAtIndex[i];
          break;
        }
        else
        {
          tmpAttackMask |= bitAtIndex[i];
        }
      }
      diagonalAttackTable[index][getHashkeyDiagonal(index, occupancy, files, diagonals64)] = tmpAttackMask;
    }
  }
  stringToFile("./chessData/diagonalAttackTable.in", arrayOfArrayToString<64, 64>(diagonalAttackTable));

  uint64_t fileAttackTable[64][64] = {0};
  for(size_t index = 0; index<64; index++)
  {
    for(size_t possibleFilesIndex = 0; possibleFilesIndex<64; possibleFilesIndex++)
    {
      uint64_t tmpAttackMask = 0;
      uint64_t occupancy = possibleFiles[possibleFilesIndex];
      size_t i = index;
      while(true)
      {
        i += NORTH;
        if(i >= 64)
        {
          break;
        }
        if((occupancy & bitAtIndex[i]) != 0)
        {
          tmpAttackMask |= bitAtIndex[i];
          break;
        }
        else
        {
            tmpAttackMask |= bitAtIndex[i];
        }
      }
      i = index;
      while(true)
      {
        i += SOUTH;
        if(i>=64/*equals i < 0*/)
        {
          break;
        }
        if((occupancy & bitAtIndex[i]) != 0)
        {
          tmpAttackMask |= bitAtIndex[i];
          break;
        }
        else
        {
          tmpAttackMask |= bitAtIndex[i];
        }
      }
      fileAttackTable[index][getHashkeyFile(index, occupancy, files)] = tmpAttackMask;
    }
  }
  stringToFile("./chessData/fileAttackTable.in", arrayOfArrayToString<64, 64>(fileAttackTable));

  uint64_t rankAttackTable[64][64] = {0};
  for(size_t index = 0; index<64; index++)
  {
    for(size_t possibleRanksIndex = 0; possibleRanksIndex<64; possibleRanksIndex++)
    {
      uint64_t tmpAttackMask = 0;
      uint64_t occupancy = possibleRanks[possibleRanksIndex];
      size_t i = index;
      while(true)
      {
        i += EAST;
        if(i >= 64)
        {
          break;
        }
        if(index%8 == 7)
        {
          break;
        }
        if((occupancy & bitAtIndex[i]) != 0)
        {
          tmpAttackMask |= bitAtIndex[i];
          break;
        }
        else
        {
            tmpAttackMask |= bitAtIndex[i];
        }
      }
      i = index;
      while(true)
      {
        i += WEST;
        if(i>=64/*equals i < 0*/)
        {
          break;
        }
        if(index%8 == 0)
        {
          break;
        }
        if((occupancy & bitAtIndex[i]) != 0)
        {
          tmpAttackMask |= bitAtIndex[i];
          break;
        }
        else
        {
          tmpAttackMask |= bitAtIndex[i];
        }
      }
      rankAttackTable[index][getHashkeyRank(index, occupancy)] = tmpAttackMask;
    }
  }
  stringToFile("./chessData/rankAttackTable.in", arrayOfArrayToString<64, 64>(rankAttackTable));

  uint64_t knightAttackTable[64] = {0};
  for(size_t i = 0; i<64; i++)
  {
    knightAttackTable[i] = 0;
    if(i+NORTH+NORTH_WEST < 64 && i % 8 != 0)
    {
      knightAttackTable[i] |= bitAtIndex[i + NORTH + NORTH_WEST];
    }
    if(i + NORTH + NORTH_EAST < 64 && (i + 1) % 8 != 0)
    {
      knightAttackTable[i] |= bitAtIndex[i + NORTH + NORTH_EAST];
    }
    if(i + NORTH_WEST + WEST < 64 && i % 8 > 1)
    {
      knightAttackTable[i] |= bitAtIndex[i + NORTH_WEST + WEST];
    }
    if(i + NORTH_EAST + EAST < 64 && (i + 1) % 8 < 7 && (i + 1) % 8 != 0)
    {
      knightAttackTable[i] |= bitAtIndex[i + NORTH_EAST + EAST];
    }
    if(i + SOUTH + SOUTH_WEST < 64 && i % 8 != 0)
    {
      knightAttackTable[i] |= bitAtIndex[i + SOUTH + SOUTH_WEST];
    }
    if(i + SOUTH + SOUTH_EAST < 64 && (i + 1) % 8 != 0)
    {
      knightAttackTable[i] |= bitAtIndex[i + SOUTH + SOUTH_EAST];
    }
    if(i + SOUTH_WEST + WEST < 64 && i % 8 > 1)
    {
      knightAttackTable[i] |= bitAtIndex[i + SOUTH_WEST + WEST];
    }
    if(i + SOUTH_EAST + EAST < 64 && (i + 1) % 8 < 7 && (i + 1) % 8 != 0)
    {
      knightAttackTable[i] |= bitAtIndex[i + SOUTH_EAST + EAST];
    }
  }
  stringToFile("./chessData/knightAttackTable.in", arrayToString<64>(knightAttackTable));

  uint64_t kingAttackTable[64] = {0};
  for(size_t i = 0; i<64; i++)
  {
    kingAttackTable[i] = 0;

    if(i + NORTH < 64)
    {
      kingAttackTable[i] |= bitAtIndex[i + NORTH];
      if(i % 8 != 0)
      {
        kingAttackTable[i] |= bitAtIndex[i + NORTH_WEST];
      }
      if((i + EAST) % 8 != 0)
      {
        kingAttackTable[i] |= bitAtIndex[i + NORTH_EAST];
      }
    }
    if(i + SOUTH < 64)
    {
      kingAttackTable[i] |= bitAtIndex[i + SOUTH];
      if(i % 8 != 0)
      {
        kingAttackTable[i] |= bitAtIndex[i + SOUTH_WEST];
      }
      if((i + EAST) % 8 != 0)
      {
        kingAttackTable[i] |= bitAtIndex[i + SOUTH_EAST];
      }
    }
    if(i % 8 != 0)
    {
      kingAttackTable[i] |= bitAtIndex[i + WEST];
    }
    if((i + EAST) % 8 != 0)
    {
      kingAttackTable[i] |= bitAtIndex[i + EAST];
    }
  }
  stringToFile("./chessData/kingAttackTable.in", arrayToString<64>(kingAttackTable));

  uint64_t pawnCaptureAttackTable[2][64] = {0};
  for(size_t i = 0; i<64; i++)
  {
    //white
    if(i + NORTH < 64)
    {
      if(i%8!=0)
      {
        pawnCaptureAttackTable[0][i] |= bitAtIndex[i +NORTH_WEST];
      }
      if(i%8!=7)
      {
        pawnCaptureAttackTable[0][i] |= bitAtIndex[i +NORTH_EAST];
      }
    }
    //black
    if(i + SOUTH < 64)
    {
      if(i%8!=0)
      {
        pawnCaptureAttackTable[1][i] |= bitAtIndex[i +SOUTH_WEST];
      }
      if(i%8!=7)
      {
        pawnCaptureAttackTable[1][i] |= bitAtIndex[i +SOUTH_EAST];
      }
    }
  }
  stringToFile("./chessData/pawnCaptureAttackTable.in", arrayOfArrayToString<2, 64>(pawnCaptureAttackTable));

  uint64_t pawnQuietAttackTable[2][64] = {0};
  for(size_t i = 0; i<64; i++)
  {
    //white
    if(i + NORTH < 64)
    {
      pawnQuietAttackTable[0][i] |= bitAtIndex[i +NORTH];
    }
    //black
    if(i + SOUTH < 64)
    {
      pawnQuietAttackTable[1][i] |= bitAtIndex[i +SOUTH];
    }
  }
  stringToFile("./chessData/pawnQuietAttackTable.in", arrayOfArrayToString<2, 64>(pawnQuietAttackTable));

  uint64_t isPassed[2][64] = {0};
  for(size_t i = 0; i<64; i++)
  {
    isPassed[0][i] |= files64[i];
    if(i % 8 != 0)
    {
      isPassed[0][i] |= files64[i - 1];
    }
    if(i % 8 != 7)
    {
      isPassed[0][i] |= files64[i + 1];
    }
    isPassed[1][i] = isPassed[0][i];

    //WHITE
    for(size_t j = 0; j<(i/8)+1; j++)
    {
      isPassed[0][i] &= ~ranks[j];
    }
    //BLACK
    for(size_t j = (i/8); j<8; j++)
    {
      isPassed[1][i] &= ~ranks[j];
    }
  }
  stringToFile("./chessData/isPassed.in", arrayOfArrayToString<2, 64>(isPassed));
  return 0;
}
