#ifndef __MISC__H__
#define __MISC__H__

uint32_t count_leading_zeros(uint32_t value);

uint32_t reverse_bits(uint32_t value);

#if (__CORTEX_M == 0x00)

#define __CLZ count_leading_zeros
#define __RBIT reverse_bits

#endif /* __CORTEX_M = 0x00 */

#endif /* __MISC__H__ */
