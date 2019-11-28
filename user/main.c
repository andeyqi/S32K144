#include <stdlib.h>
#include <stdio.h>
#include "clock_manager.h"

int main(int argc,char ** argv)
{
	CLOCK_SYS_Init(NULL,0,NULL,0);
	CLOCK_SYS_UpdateConfiguration(0,CLOCK_MANAGER_POLICY_FORCIBLE);
	return 0;
}