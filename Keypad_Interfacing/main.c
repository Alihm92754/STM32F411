
#include <stdio.h>
#include <stdint.h>


void delay(void)
{
	for(volatile uint32_t i=0;i<400000;i++);
}

int main(void)
{

    uint32_t *portcAHB1ENR = (uint32_t*) 0x40023830;
    uint32_t *portcMODER = (uint32_t*) 0x40020800;
    uint32_t *portcODR = (uint32_t*) 0x40020814;
    uint32_t *portcIDR = (uint32_t*) 0x40020810;
    uint32_t *portcPUPDR = (uint32_t*) 0x4002080C;

    //Enable the peripheral clock for GPIOC

    *portcAHB1ENR |= (1 << 2);
    //*portcAHB1ENR |= 0x04;
    //Enable the PC0-PC3 as output PC4-PC7 as input

    *portcMODER &= 0xFFFF0000; //clearing the bit
    *portcMODER |= 0x00000055; //set the mode register

    *portcPUPDR &= 0xFFFF0000; //clearing the bit
    *portcPUPDR |= 0x00005500; //set the mode register

    while(1){

    	//make R1 LOW
    	*portcODR |= 0x0F;
    	*portcODR &= ~(1 << 0);

    	if(!(*portcIDR &(1 << 4))){
    		delay();
    		printf("1 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 5))){
    	    delay();
    	    printf("2 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 6))){
    	    delay();
    	    printf("3 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 7))){
    	    delay();
    	    printf("4 is Pressed \n");
    	}


    	//make R2 LOW
    	*portcODR |= 0x0F;
    	*portcODR &= ~(1 << 1);

    	if(!(*portcIDR &(1 << 4))){
    	    delay();
    	    printf("5 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 5))){
    	    delay();
    	    printf("6 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 6))){
    	    delay();
    	    printf("7 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 7))){
    	    delay();
    	    printf("8 is Pressed \n");
    	}

    	//make R3 LOW
    	*portcODR |= 0x0F;
    	*portcODR &= ~(1 << 2);

    	if(!(*portcIDR &(1 << 4))){
    	    delay();
    	    printf("9 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 5))){
    	    delay();
    	    printf("10 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 6))){
    	    delay();
    	    printf("11 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 7))){
    	    delay();
    	    printf("12 is Pressed \n");
    	}

    	//make R4 LOW
    	*portcODR |= 0x0F;
    	*portcODR &= ~(1 << 3);

    	if(!(*portcIDR &(1 << 4))){
    	    delay();
    	    printf("13 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 5))){
    	    delay();
    	    printf("14 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 6))){
    	    delay();
    	    printf("15 is Pressed \n");
    	}

    	if(!(*portcIDR &(1 << 7))){
    	    delay();
    	    printf("16 is Pressed \n");
    	}


    }//end of while loop


	for(;;);
}
