/* Includes ------------------------------------------------------------------*/
#include "stm32_mpu.h"

//外部函数
void drop_privileges(void);

/* Private define ------------------------------------------------------------*/
#define ARRAY_ADDRESS_START    (0x20002000UL)
#define ARRAY_SIZE             (0x09UL << 0UL)
#define ARRAY_REGION_NUMBER    (0x03UL << MPU_RNR_REGION_Pos) 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

void MPU_Config(void)
{
  /* Disable MPU */
  MPU->CTRL &= ~MPU_CTRL_ENABLE_Msk;
  
  /* Configure RAM region as Region N°0, 8kB of size and R/W region */
  MPU->RNR  = RAM_REGION_NUMBER;
  MPU->RBAR = RAM_ADDRESS_START;
  MPU->RASR = RAM_SIZE | portMPU_REGION_READ_WRITE;
  
  /* Configure FLASH region as REGION N°1, 1MB of size and R/W region */
  MPU->RNR  = FLASH_REGION_NUMBER;
  MPU->RBAR = FLASH_ADDRESS_START;
  MPU->RASR = FLASH_SIZE | portMPU_REGION_READ_WRITE;
  
  /* Configure Peripheral region as REGION N°2, 0.5GB of size, R/W and Execute
  Never region */
  MPU->RNR  = PERIPH_REGION_NUMBER;  
  MPU->RBAR = PERIPH_ADDRESS_START;
  MPU->RASR = PERIPH_SIZE |portMPU_REGION_READ_WRITE | MPU_RASR_XN_Msk;
  
  /* Enable the memory fault exception */
  SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
   
  /* Enable MPU */
  MPU->CTRL |= MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_ENABLE_Msk;
}

void MPU_AccessPermConfig(void)
{ 

	char *PrivilegedReadOnlyAddr=(char *)ARRAY_ADDRESS_START;
	
	/* Configure region for PrivilegedReadOnlyArray as REGION N°3, 32byte and R 
     only in privileged mode */
	MPU->RNR  = ARRAY_REGION_NUMBER;
	MPU->RBAR |= ARRAY_ADDRESS_START;
	MPU->RASR |= ARRAY_SIZE | portMPU_REGION_PRIVILEGED_READ_ONLY;
	
  /* 
   * 测试1： 全为特权模式下，写 特权读
   * 
   */
//	*PrivilegedReadOnlyAddr=1;
  
  /* 
   * 测试2： 非特权模式下，写 特权区
   * 
   */
   	drop_privileges();
	char a=*PrivilegedReadOnlyAddr;
}

