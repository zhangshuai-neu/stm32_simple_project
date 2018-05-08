#include "stm32f4xx.h"
#include "stm32f4xx_syscfg.h"
#include "stm32_mpu.h"

int main(void)
{
	MPU_Config();				//MPU配置
	MPU_AccessPermConfig();		//触发访问错误
	
	
	
	*(char*)0x20008003=0x1f;
	
    return 0;
}
