#include "stm32f0xx.h"

#define SANDBOX 
/* sandbox-режим для тестирования фич */

#ifdef SANDBOX 
#include "test_sandbox.h"
#endif

/* точка входа */
int main(void)
{
#ifdef SANDBOX
	SBX_main(); 
	return 0;
#else
	
	
	
	return 0;	
#endif
}

