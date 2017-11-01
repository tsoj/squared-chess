#pragma once

#include <string>
#include "position.hpp"
#include "types.hpp"

void printBitboard(Bitboard bitboard);
void printPosition(Position position);
Position setPositionFromFen(std::string);
void printPositionBitboards(Position position);
