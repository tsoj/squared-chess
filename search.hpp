#pragma once

#include "types.hpp"
#include "position.hpp"

PositionValue negaMax(COLOR_TYPE us, COLOR_TYPE enemy, Position origPosition, int depth);
