#include <stdint.h>

#define GPIOAEN (1U << 0)
#define UART2EN (1U << 17)
#define SYS_FREQ 16000000
#define APB1_CLK SYS_FREQ
#define UART_BAUDRATE 115200

uint32_t *portaAHB1ENR = (uint32_t*) 0x40023830;
uint32_t *APB1ENR      = (uint32_t*) 0x40023840;
uint32_t *GPIOA_MODER  = (uint32_t*) 0x40020000;
uint32_t *GPIOA_AFRL   = (uint32_t*) 0x40020020;
uint32_t *USART2_SR    = (uint32_t*) 0x40004400;
uint32_t *USART2_DR    = (uint32_t*) 0x40004404;
uint32_t *USART2_BRR   = (uint32_t*) 0x40004408;
uint32_t *USART2_CR1   = (uint32_t*) 0x4000440C;

void uart2_write(int ch);
void uart2_tx_init(void);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t Baudrate);

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

int main(void)
{
	//Enable clocK access to GPIOA
	//SET PA2 MOODE TO Alternate function Mode
	//SET PA2 Alternative function type to UART_TX (AF07)
	//CONFIGURE UART MODULE
	//ENABLE CLOCK ACCESS TO UART2
	//CONFIGURE BAUDRATE
	//CONFIGURE TRANSFER DIRECTION
	//ENABLE UART MODULE
	uart2_tx_init();
	while(1){
		//uart2_write('A');
		printf("Hello World! \n\r");
	}
}

void uart2_tx_init(void)
{
	//enable clock access to GPIOA
	*portaAHB1ENR |= GPIOAEN;

	//set PA2 MODE to alternate function mode
	*GPIOA_MODER &= ~(1U << 4);
	*GPIOA_MODER |= (1U << 5);

	//set PA2 alternative function type to UART_TX (AF07)
	*GPIOA_AFRL |= (1U << 8);
	*GPIOA_AFRL |= (1U << 9);
	*GPIOA_AFRL |= (1U << 10);
	*GPIOA_AFRL &= ~(1U << 11);

	//CONFIGURE UART MODULE
	*APB1ENR |= UART2EN;
	*USART2_BRR = compute_uart_bd(APB1_CLK, UART_BAUDRATE);

	//CONFIGURE TRANSFER DIRECTION
	*USART2_CR1 = 0x0008;
	*USART2_CR1 |= 0x2000;

}


static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t Baudrate)
{
	return ((PeriphClk+(Baudrate/2U))/Baudrate);
}

void uart2_write(int ch)
{
	//make sure the transmit data register is empty
	while(!(*USART2_SR & 0x0080)){}
	//write to transmit data register
	*USART2_DR = (ch & 0xFF);
}
