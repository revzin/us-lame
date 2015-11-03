#ifndef __MISC__H__
#define __MISC__H__

/* возвращает число лидирующих нулей в value */
uint32_t count_leading_zeros(uint32_t value);

/* возвращает "перевёрнутое" value */
uint32_t reverse_bits(uint32_t value);

#if (__CORTEX_M == 0x00)
/* для Cortex-M0 эти инструкции эмулируются софтово */
#define __CLZ count_leading_zeros
#define __RBIT reverse_bits

#endif /* __CORTEX_M = 0x00 */

#endif /* __MISC__H__ */
