#include <limits.h>

#include "stm32f0xx.h"

#include "misc.h"


#define BITHACK_FAST

/* подсчёт нулей от начала числа до первой 1 
   http://embeddedgurus.com/state-space/2014/09/fast-deterministic-and-portable-counting-leading-zeros/ */
uint32_t count_leading_zeros(uint32_t value)
{
#ifdef BITHACK_FAST
	static uint8_t const clz_lkup[] = {
        32U, 31U, 30U, 30U, 29U, 29U, 29U, 29U,
        28U, 28U, 28U, 28U, 28U, 28U, 28U, 28U
    };
	
    uint32_t n;

    if (value >= (1U << 16)) {
        if (value >= (1U << 24)) {
            if (value >= (1 << 28)) {
                n = 28U;
            }
            else {
                n = 24U;
            }
        }
        else {
            if (value >= (1U << 20)) {
                n = 20U;
            }
            else {
                n = 16U;
            }
        }
    }
    else {
        if (value >= (1U << 8)) {
            if (value >= (1U << 12)) {
                n = 12U;
            }
            else {
                n = 8U;
            }
        }
        else {
            if (value >= (1U << 4)) {
                n = 4U;
            }
            else {
                n = 0U;
            }
        }
    }
    return (uint32_t)clz_lkup[value >> n] - n;
	
#elif (defined BITHACK_OBVIOUS)
	
#else
#error "Please define at least something here..."
#endif	
	
}

/* http://graphics.stanford.edu/~seander/bithacks.html#BitReverseObvious */

uint32_t reverse_bits(uint32_t value)
{
	unsigned int c; // c will get v reversed
	
#ifdef BITHACK_FAST
	static const unsigned char BitReverseTable256[256] = 
	{
	#   define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
	#   define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
	#   define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )
		R6(0), R6(2), R6(1), R6(3)
	};


	// Option 1:
	c = (BitReverseTable256[value & 0xff] << 24) | 
		(BitReverseTable256[(value >> 8) & 0xff] << 16) | 
		(BitReverseTable256[(value >> 16) & 0xff] << 8) |
		(BitReverseTable256[(value >> 24) & 0xff]);
	
#elif (defined BITHACK_OBVIOUS)

	c = value; // r will be reversed bits of v; first get LSB of v
	int s = sizeof(c) * CHAR_BIT - 1; // extra shift needed at end

	for (v >>= 1; v; v >>= 1)
	{   
	  c <<= 1;
	  c |= v & 1;
	  s--;
	}
	c <<= s; // shift when v's highest bits are zero
	
#else
#error "Please define at least something here..."
#endif	
	
	return c;
}

