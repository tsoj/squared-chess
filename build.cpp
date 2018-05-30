#include "types.hpp"
#include "bitOperations.hpp"

#include <fstream>
#include <string>
#include <iostream>

const uint64_t UPPER_LEFT_SIDE_ZERO =  0b1000000011000000111000001111000011111000111111001111111011111111;
const uint64_t LOWER_RIGHT_SIDE_ZERO = 0b1111111101111111001111110001111100001111000001110000001100000001;
const uint64_t LOWER_LEFT_SIDE_ZERO = 0b1111111011111100111110001111000011100000110000001000000000000000;
const uint64_t UPPER_RIGHT_SIDE_ZERO = 0b0000000000000001000000110000011100001111000111110011111101111111;
const uint64_t MAIN_DIAGONAL = 0b1000000001000000001000000001000000001000000001000000001000000001; //A1 to H8
const uint64_t ANTI_DIAGONAL = 0b0000000100000010000001000000100000010000001000000100000010000000; //H1 to A8

uint64_t rankToFile(uint64_t rank, uint64_t files[8])
{
  return (((rank & 0b11111111)*MAIN_DIAGONAL) & files[7]) >> 7;
}
size_t getHashkeyRank(size_t index, uint64_t occupancy)
{
  return ((occupancy >> ((index / 8)*8)) >> 1) & 0b111111;
}
size_t getHashkeyFile(size_t index, uint64_t occupancy, uint64_t files[8])
{
  return (((((occupancy >> (index % 8)) & files[0] )*MAIN_DIAGONAL) >> 56) >> 1) & 0b111111;
}
size_t getHashkeyDiagonal(size_t index, uint64_t occupancy, uint64_t files[8], uint64_t diagonals64[64])
{
  return ((((occupancy & diagonals64[index])*files[0]) >> 56) >> 1) & 0b111111;
}
size_t getHashkeyAntiDiagonal(size_t index, uint64_t occupancy, uint64_t files[8], uint64_t antiDiagonals64[64])
{
    return ((((occupancy & antiDiagonals64[index])*files[0]) >> 56) >> 1) & 0b111111;
}

void printBitboard(uint64_t b)
{
  for(int i = 0; i<64; i++)
  {
    if(i%8==0)
    {
      std::cout << std::endl;
    }
    if((((uint64_t)(0b1) << i) & b) != 0)
    {
      std::cout << "1";
    }
    else
    {
      std::cout << "0";
    }
  }
  std::cout << std::endl;
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
    ret.append("(uint64_t)");
    ret.append(std::to_string(a[i]));
    if(i<size-1)
    {
      ret.append(", ");
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
      ret.append(", ");
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
      ret.append(", ");
    }
    else
    {
      ret.append("}");
    }
  }
  return ret;
}

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
      size_t index = BitOperations::trailingZeros(tmp);
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
      size_t index = BitOperations::trailingZeros(tmp);
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
      size_t index = BitOperations::trailingZeros(tmp);
      antiDiagonals64[index] = currentAntiDiagonal;
      tmp = tmp & ~bitAtIndex[index];
    }
  }
  {
    uint64_t currentAntiDiagonal = ANTI_DIAGONAL;
    uint64_t tmp = currentAntiDiagonal;
    while(tmp!=0)
    {
      size_t index = BitOperations::trailingZeros(tmp);
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
      size_t index = BitOperations::trailingZeros(tmp);
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
          tmpAttackMask |= bitAtIndex[index];
        }
      }
      antiDiagonalAttackTable[index][getHashkeyAntiDiagonal(index, occupancy, files, antiDiagonals64)] = tmpAttackMask;
    }
  }
  stringToFile("./chessData/antiDiagonalAttackTable.in", arrayOfArrayToString<64, 64>(antiDiagonalAttackTable));

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
          tmpAttackMask |= bitAtIndex[index];
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
          tmpAttackMask |= bitAtIndex[index];
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
          tmpAttackMask |= bitAtIndex[index];
        }
      }
      rankAttackTable[index][getHashkeyRank(index, occupancy)] = tmpAttackMask;
    }
  }
  stringToFile("./chessData/rankAttackTable.in", arrayOfArrayToString<64, 64>(rankAttackTable));


  /*

    let mut knight_attack_table: [u64; 64] = [0; 64];
    for k in 0..knight_attack_table.len()
    {
        let i = k as isize;
        knight_attack_table[i as usize] = 0;
        if i + NORTH + NORTH_WEST <= 63 && i % 8 != 0
        {
            knight_attack_table[i as usize] |= bit_at_index[(i + NORTH + NORTH_WEST) as usize];
        }
        if i + NORTH + NORTH_EAST <= 63 && (i + 1) % 8 != 0
        {
            knight_attack_table[i as usize] |= bit_at_index[(i + NORTH + NORTH_EAST) as usize];
        }
        if i + NORTH_WEST + WEST <= 63 && i % 8 > 1
        {
            knight_attack_table[i as usize] |= bit_at_index[(i + NORTH_WEST + WEST) as usize];
        }
        if i + NORTH_EAST + EAST <= 63 && (i + 1) % 8 < 7 && (i + 1) % 8 != 0
        {
            knight_attack_table[i as usize] |= bit_at_index[(i + NORTH_EAST + EAST) as usize];
        }
        if i + SOUTH + SOUTH_WEST >= 0 && i % 8 != 0
        {
            knight_attack_table[i as usize] |= bit_at_index[(i + SOUTH + SOUTH_WEST) as usize];
        }
        if i + SOUTH + SOUTH_EAST >= 0 && (i + 1) % 8 != 0
        {
            knight_attack_table[i as usize] |= bit_at_index[(i + SOUTH + SOUTH_EAST) as usize];
        }
        if i + SOUTH_WEST + WEST >= 0 && i % 8 > 1
        {
            knight_attack_table[i as usize] |= bit_at_index[(i + SOUTH_WEST + WEST) as usize];
        }
        if i + SOUTH_EAST + EAST >= 0 && (i + 1) % 8 < 7 && (i + 1) % 8 != 0
        {
            knight_attack_table[i as usize] |= bit_at_index[(i + SOUTH_EAST + EAST) as usize];
        }
        //print_bitboard(knight_attack_table[i as usize], &bit_at_index);
    }
    write_string_to_file("./src/chess_data_in/knight_attack_table.in", &write_64_array_to_string(&knight_attack_table));

    let mut king_attack_table: [u64; 64] = [0; 64];
    for k in 0..king_attack_table.len()
    {
        let i = k as isize;
        king_attack_table[i as usize] = 0;
        if i + NORTH <= 63
        {
            king_attack_table[i as usize] |= bit_at_index[(i + NORTH) as usize];
            if i % 8 != 0
            {
                king_attack_table[i as usize] |= bit_at_index[(i + NORTH_WEST) as usize];
            }
            if (i + EAST) % 8 != 0
            {
                king_attack_table[i as usize] |= bit_at_index[(i + NORTH_EAST) as usize];
            }
        }
        if i + SOUTH >= 0
        {
            king_attack_table[i as usize] |= bit_at_index[(i + SOUTH) as usize];
            if i % 8 != 0
            {
                king_attack_table[i as usize] |= bit_at_index[(i + SOUTH_WEST) as usize];
            }
            if (i + EAST) % 8 != 0
            {
                king_attack_table[i as usize] |= bit_at_index[(i + SOUTH_EAST) as usize];
            }
        }
        if i % 8 != 0
        {
            king_attack_table[i as usize] |= bit_at_index[(i + WEST) as usize];
        }
        if (i + EAST) % 8 != 0
        {
            king_attack_table[i as usize] |= bit_at_index[(i + EAST) as usize];
        }
        //print_bitboard(king_attack_table[i as usize], &bit_at_index);
    }
    write_string_to_file("./src/chess_data_in/king_attack_table.in", &write_64_array_to_string(&king_attack_table));

    let mut pawn_capture_attack_table: [[u64; 64]; 2] = [[0; 64]; 2];
    for i in 0..64
    {
        //white
        if i + NORTH < 64
        {
            if i%8!=0
            {
                pawn_capture_attack_table[0][i as usize] |= bit_at_index[(i +NORTH_WEST)as usize];
            }
            if i%8!=7
            {
                pawn_capture_attack_table[0][i as usize] |= bit_at_index[(i +NORTH_EAST)as usize];
            }
        }
        //black
        if i + SOUTH >= 0
        {
            if i%8!=0
            {
                pawn_capture_attack_table[1][i as usize] |= bit_at_index[(i +SOUTH_WEST)as usize];
            }
            if i%8!=7
            {
                pawn_capture_attack_table[1][i as usize] |= bit_at_index[(i +SOUTH_EAST)as usize];
            }
        }
    }
    write_string_to_file("./src/chess_data_in/pawn_capture_attack_table.in", &write_2_64_array_to_string(&pawn_capture_attack_table));

    let mut pawn_quiet_attack_table: [[u64; 64]; 2] = [[0; 64]; 2];
    for i in 0..64
    {
        //white
        if i + NORTH < 64
        {
            pawn_quiet_attack_table[0][i as usize] |= bit_at_index[(i +NORTH)as usize];
        }
        //black
        if i + SOUTH >= 0
        {
            pawn_quiet_attack_table[1][i as usize] |= bit_at_index[(i +SOUTH)as usize];
        }
    }
    write_string_to_file("./src/chess_data_in/pawn_quiet_attack_table.in", &write_2_64_array_to_string(&pawn_quiet_attack_table));

    let mut is_passed: [[u64; 64]; 2] = [[0; 64]; 2];
    for i in 0..64
    {
        is_passed[0][i] |= files_64[i];
        if i % 8 != 0
        {
            is_passed[0][i] |= files_64[i - 1];
        }
        if i % 8 != 7
        {
            is_passed[0][i] |= files_64[i + 1];
        }
        is_passed[1][i] = is_passed[0][i];

        //WHITE
        for j in 0..((i/8)+1)
        {
            is_passed[0][i] &= !ranks[j];
        }
        //BLACK
        for j in (i/8)..8
        {
            is_passed[1][i] &= !ranks[j];
        }
    }
    write_string_to_file("./src/chess_data_in/is_passed.in", &write_2_64_array_to_string(&is_passed));
    //panic!();
    */
  return 0;
}
/*
fn print64_fields_to_chessboard(field_content: &Vec<String>)
{
    println!("");
    for _ in 0..33
    {
        print!("{}", chess_data::HORIZONTAL_LINE_UNICODE);
    }
    println!("");
    for h in 0..8
    {
        let i = 7 - h;
        for j in 0..8
        {
            print!("{} {} ", chess_data::VERTICAL_LINE_UNICODE, field_content[8*i + j]);
        }
        print!("{} {}", chess_data::VERTICAL_LINE_UNICODE, (i+1) as u32);
        println!("");
        for _ in 0..33
        {
            print!("{}", chess_data::HORIZONTAL_LINE_UNICODE);
        }
        println!("");
    }
  	println!("  A   B   C   D   E   F   G   H");
}

fn print_bitboard(bitboard: u64, bit_at_index: &[u64; 64])
{
  let mut temp: Vec<String> = vec![String::new(); 64];
  for  i in 0..bit_at_index.len()
  {

    temp[i] = format!("{}", chess_data::ZERO_UNICODE);
    if (bitboard & bit_at_index[i]) != 0
    {
        temp[i] = format!("{}", chess_data::ONE_UNICODE);
    }
  }
  print64_fields_to_chessboard(&temp);
}

fn rank_to_file(rank: u64, files: &[u64; 8]) -> u64
{
    (((rank & 0b11111111).wrapping_mul(MAIN_DIAGONAL)) & files[7]) >> 7
}

fn get_hashkey_rank(index: usize, occupancy: u64) -> usize
{
    (((occupancy >> ((index / 8)*8)) >> 1) & 0b111111) as usize
}

fn get_hashkey_file(index: usize , occupancy: u64, files: &[u64; 8]) -> usize
{
    ((((((occupancy >> (index % 8)) & files[0] ).wrapping_mul(MAIN_DIAGONAL)) >> 56) >> 1) & 0b111111) as usize
}

fn get_hashkey_diagonal(index: usize, occupancy: u64, files: &[u64; 8], diagonals_64: &[u64; 64]) -> usize
{
    (((((occupancy & diagonals_64[index]).wrapping_mul(files[0])) >> 56) >> 1) & 0b111111) as usize
    //return ((((occupancy & diagonals64[fieldIndex])* files[0]) >> 56) >> 1) & 0b111111;
}

fn get_hashkey_anti_diagonal(index: usize, occupancy: u64, files: &[u64; 8], anti_diagonals_64: &[u64; 64]) -> usize
{
    (((((occupancy & anti_diagonals_64[index]).wrapping_mul(files[0])) >> 56) >> 1) & 0b111111) as usize
}
*/
