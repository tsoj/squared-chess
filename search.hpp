#pragma once

#include "types.hpp"
#include "position.hpp"

void startSearch(Position origPosition, int depth);
Position startSearchMinMax(Position origPosition, int depth);
