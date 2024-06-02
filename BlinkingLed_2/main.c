

#include <stdint.h>
#include "main.h"

void delay(void)
{
	for(uint32_t i=0;i<300000;i++);
}


int main(void)
{

	RCC_AHB1ENR_t *AHB1ENR = (RCC_AHB1ENR_t*) 0x40023830;
	//GPIOx_MODER_t *GPIOA_MODER = 0x40020000;
	GPIOx_MODER_t *GPIOC_MODER = (GPIOx_MODER_t*) 0x40020800;
	GPIOx_ODR_t *GPIOC_ODR = (GPIOx_ODR_t*)0x40020814;

	AHB1ENR -> gpioc_en = 1; //enable CLK for GPIOC
	GPIOC_MODER -> moder_0 = 1; //output mode 01

	while(1){

		GPIOC_ODR -> ODR_0 = 1;
		delay();
		GPIOC_ODR -> ODR_0 = 0;
		delay();
	}



	for(;;);
}
