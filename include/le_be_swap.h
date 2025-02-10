#ifndef LE_BE_SWAP_H_
    #define LE_BE_SWAP_H_

#include <stdint.h>


/// Byte swap unsigned short
uint16_t SwapUInt16(uint16_t val);
/// Byte swap short
int16_t SwapInt16(int16_t val);
/// Byte swap unsigned int
uint32_t SwapUInt32(uint32_t val);
/// Byte swap int
int32_t SwapInt32(int32_t val);

#endif /* !LE_BE_SWAP_H_ */
