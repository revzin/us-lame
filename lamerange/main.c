#include "stm32f0xx.h"

#define SANDBOX

#ifdef SANDBOX
#include "test_sandbox.h"
#endif

int main(void)
{
#ifdef SANDBOX
	SBX_main();
	return 0;
#else
	
	
	
	return 0;	
#endif
}

