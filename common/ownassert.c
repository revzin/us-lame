#include "ownassert.h"
#include "stm32f0xx.h"

void __assert_handler(const char *p)
{
	__disable_irq();
	while (1);
}
