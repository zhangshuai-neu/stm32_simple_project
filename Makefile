# 添加包含路径
vpath %.S stlib
vpath %.c stlib stlib/src 
vpath %.h stlib stlib/cminc stlib/inc



#定义条件编译需要的宏
DEFS += -D USE_STDPERIPH_DRIVER

#添加头文件目录列表
INCS += -I stlib -I stlib/cminc -I stlib/inc

# 使用其他外设在这里添加
OBJS += main.o
OBJS += stlib/startup_stm32f40xx.o 
OBJS += stlib/system_stm32f4xx.o
OBJS += stm32_mpu.o
OBJS += stlib/stm32f4xx_it.o


#指定cpu/fpu以及优化参数
CFLAGS += -mcpu=cortex-m4 -mthumb -Wall 		#处理器,执行在thumb状态
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 	#浮点计算硬件,硬件型号
CFLAGS += -Os 									#禁用多指令发射
CFLAGS += -g 									#GDB调试
#CFLAGS += -ffunction-sections -fdata-sections #将函数和数据放到自己的section
											   #优化,与-g同时使用时可能会有问题
LFLAGS += -mcpu=cortex-m4 -mthumb
LFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
LFLAGS += -Wl,--gc-sections 

# 最后生成的bin文件
all:blink.bin

clean:
	@rm -f $(OBJS) $(OBJ_FLASH) $(OBJ_SRAM)  
	@rm -f blink.bin blink.elf blinks.bin blinks.elf
    
blink.bin:blink.elf
	@arm-none-eabi-objcopy -O binary -S $< $@
    
blink.elf:$(OBJS) $(OBJ_FLASH)
	@arm-none-eabi-gcc $(LFLAGS) $^ -T stlib/STM32F407ZET6_FLASH.ld -o $@
	@arm-none-eabi-size $@ 			#列出各section大小
    
burn:blink.bin
	@st-flash write $< 0x08000000 	#将二进制文件写入SMT32设备
    
%.o:%.S
	@echo cc: $<
	@arm-none-eabi-gcc $(CFLAGS) -c $< -o $@
%.o:%.c
	@echo cc: $<
	@arm-none-eabi-gcc $(CFLAGS) $(DEFS) $(INCS) -c $< -o $@
