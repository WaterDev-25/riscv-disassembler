#include "le_be_swap.h"
#include <stdint.h>

/// Byte swap unsigned short
uint16_t
SwapUInt16(uint16_t val) 
{
    return (val << 8) | (val >> 8 );
}

/// Byte swap short
int16_t
SwapInt16(int16_t val) 
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

/// Byte swap unsigned int
uint32_t
SwapUInt32(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}

/// Byte swap int
int32_t
SwapInt32(int32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | ((val >> 16) & 0xFFFF);
}
