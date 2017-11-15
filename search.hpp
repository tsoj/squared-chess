#pragma once

#include "types.hpp"
#include "position.hpp"

int Perft(COLOR_TYPE us, COLOR_TYPE enemy, Position origPosition, int depth);
Position startSearch(Position origPosition, int depth);
Position startSearchMinMax(Position origPosition, int depth);
