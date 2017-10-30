#pragma once

#include "types.hpp"

namespace BitOperations
{
  inline bool bitScanForward(unsigned long & Index, const uint64 & Mask);
  inline bool bitScanReverse(unsigned long & Index, const uint64 & Mask);

  #ifdef _MSC_VER
    #include <intrin.h>
    #ifdef ARCH_X86
      inline bool bitScanForward(unsigned long & Index, const uint64 & Mask)
      {
        return (bool)_BitScanForward(Index, Mask);
      }
      inline bool bitScanReverse(unsigned long & Index, const uint64 & Mask)
      {
        return (bool)_BitScanReverse(Index, Mask);
      }
    #endif
    #ifdef ARCH_X86_64
      inline bool bitScanForward(unsigned long & Index, const uint64 & Mask)
      {
        return (bool)_BitScanForward64(Index, Mask);
      }
      inline bool bitScanReverse(unsigned long & Index, const uint64 & Mask)
      {
        return (bool)_BitScanReverse64(Index, Mask);
      }
    #endif
  #endif


  #ifdef __INTEL_COMPILER
    #ifdef ARCH_X86
      inline bool bitScanForward(unsigned long & Index, const uint64 & Mask)
      {
        bool ret;
        uint32 lowerBits = Mask;
        ret = (bool)_BitScanForward(Index, lowerBits);
        if(ret)
        {
          return ret;
        }
        uint32 upperBits = Mask >> 32;
        ret = (bool)_BitScanForward(Index, upperBits);
        Index+=32;
        return ret;
      }
      inline bool bitScanReverse(unsigned long & Index, const uint64 & Mask)
      {
        bool ret;
        uint32 upperBits = Mask >> 32;
        ret = (bool)_BitScanReverse(Index, upperBits);
        Index+=32;
        if(ret)
        {
          return ret;
        }
        uint32 lowerBits = Mask;
        ret = (bool)_BitScanReverse(Index, lowerBits);
        return ret;
      }
    #endif
    #ifdef ARCH_X86_64
      inline bool bitScanForward(unsigned long & Index, const uint64 & Mask)
      {
        return (bool)_BitScanForward64(Index, Mask);
      }
      inline bool bitScanReverse64(unsigned long & Index, const uint64 & Mask)
      {
        return (bool)_BitScanReverse64(Index, Mask);
      }
    #endif
  #endif

  #if defined(__GNUC__) || defined(__clang__)
    #ifdef ARCH_X86
      inline bool bitScanForward(unsigned long & Index, const uint64 & Mask)
      {
        bool ret;
        uint32 lowerBits = Mask;
        uint64 Ret;
        __asm__
        (
            "bsf %[lowerBits], %[Ret]"
            :[Ret] "=r" (Ret)
            :[Mask] "mr" (Mask)
        );
        Index = (unsigned long)Ret;
        if(ret)
        {
          return ret;
        }
        uint32 upperBits = Mask >> 32;
        uint64 Ret;
        __asm__
        (
            "bsf %[upperBits], %[Ret]"
            :[Ret] "=r" (Ret)
            :[Mask] "mr" (Mask)
        );
        Index = (unsigned long)Ret;
        Index+=32;
        return ret;
      }
      inline bool bitScanReverse(unsigned long & Index, const uint64 & Mask)
      {
        bool ret;
        uint32 upperBits = Mask >> 32;
        uint64 Ret;
        __asm__
        (
            "bsr %[upperBits], %[Ret]"
            :[Ret] "=r" (Ret)
            :[Mask] "mr" (Mask)
        );
        Index = (unsigned long)Ret;
        Index+=32;
        ret = (bool)Mask?1:0;
        if(ret)
        {
          return ret;
        }
        uint32 lowerBits = Mask;
        uint64 Ret;
        __asm__
        (
            "bsr %[lowerBits], %[Ret]"
            :[Ret] "=r" (Ret)
            :[Mask] "mr" (Mask)
        );
        Index = (unsigned long)Ret;
        ret = (bool)Mask?1:0;
        return ret;
      }
    #endif
    #ifdef ARCH_X86_64
      inline bool bitScanForward(unsigned long & Index, const uint64 & Mask)
      {
        //see https://chessprogramming.wikispaces.com/BitScan
        uint64 Ret;
        __asm__
        (
            "bsfq %[Mask], %[Ret]"
            :[Ret] "=r" (Ret)
            :[Mask] "mr" (Mask)
        );
        Index = (unsigned long)Ret;
        return (bool)Mask?1:0;
      }
      inline bool bitScanReverse(unsigned long & Index, const uint64 & Mask)
      {
        uint64 Ret;
        __asm__
        (
            "bsrq %[Mask], %[Ret]"
            :[Ret] "=r" (Ret)
            :[Mask] "mr" (Mask)
        );
        Index = (unsigned long)Ret;
        return (bool)Mask?1:0;
      }
    #endif
  #endif


}
