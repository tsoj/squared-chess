#pragma once

#include <iostream>

#define DEBUG

#ifdef DEBUG
#define ASSERT(_statement) \
{\
  if(not (_statement))\
  {\
    std::cout << "In file " << __FILE__ << ", line " <<  __LINE__ << ": statement '" << #_statement << "' is not true." << std::endl;\
    std::cout << "Aborting." << std::endl;\
    exit(0);\
  }\
}
#else
#define ASSERT(statement)
#endif
