#include <stdint.h>
#include "stm32F4xx.h"

//MISO -> PA6
//MOSI -> PA7
//CLK  -> PA5
//CS   -> PA9
//AF05

#define GPIOAEN (1U << 0)
#define SPI1EN (1U << 12)

#define DEVID_R (0x00)
#define DATA_FORMAT_R (0x31)
#define POWER_CTL_R (0x2D)
#define DATA_START_ADDR (0x32)
#define DEVICE_ADDR (0x53)
#define FOUR_G (0x01)
#define RESET (0x00)
#define SET_MEASURE_B (0x08)

#define SR_TXE (1U << 1)
#define SR_RXNE (1U << 0)
#define SR_BUSY (1U << 7)
#define MULTI_BYTE_EN 0x40
#define READ_OPERATION 0x80


void spi_transmit(uint8_t *data, uint32_t size);
void spi_receive(uint8_t *data, uint32_t size);
void spi_config(void);
void cs_enable(void);
void cs_disable(void);
void spi_gpio_init(void);
void adxl_read_values(uint8_t reg);
void adxl_write(uint8_t reg, char value);
void adxl_read_address(uint8_t reg);
void adxl_init(void);

char data;
uint8_t data_rec[6];
int16_t x,y,z;
float xg,yg,zg;


//PB6 - SCL
//PB7 - SDA

int main(void)
{
	adxl_init();
	while(1){
		adxl_read(DATA_START_ADDR, data_rec);
		x = ((data_rec[1] << 8) | data_rec[0]);
		y = ((data_rec[3] << 8) | data_rec[2]);
		z = ((data_rec[5] << 8) | data_rec[4]);
		xg = (x * 0.0078);
		yg = (y * 0.0078);
		zg = (z * 0.0078);
	}
}

void adxl_init(void)
{
	spi_gpio_init();
	spi_config();
	adxl_write(DATA_FORMAT_R, FOUR_G);
	//Rest all the bits
	adxl_write(POWER_CTL_R, RESET);
	adxl_write(POWER_CTL_R, SET_MEASURE_B);
}

void adxl_read_values(uint8_t reg)
{


}

void adxl_write(uint8_t address, char value)
{
	uint8_t data[2];
	//Enable MULTI-BYTE
	data[0] = address | MULTI_BYTE_EN;
	data[1] = value;
	cs_enable();
	spi_transmit(data,2);
	cs_disable();

}

void adxl_read(uint8_t address, uint8_t *rxdata)
{
	//Set the read operation
	address |= READ_OPERATION;
    //Enable MULTIBYTE
	address |= MULTI_BYTE_EN;
	cs_enable();
	spi_transmit(&address,1);
	spi_receive(rxdata,6);
	cs_disable();

}

void spi_gpio_init(void)
{
	RCC -> AHB1ENR |= GPIOAEN;
	//Set PA5 for SPI -> ALTERNATE
	GPIOA -> MODER &=~ (1U << 10); //Set 0
	GPIOA -> MODER |= (1U << 11); //Set 1

	//Set PA6 for SPI -> ALTERNATE
	GPIOA -> MODER &=~ (1U << 12); //Set 0
	GPIOA -> MODER |= (1U << 13); //Set 1

	//Set PA7 for SPI -> ALTERNATE
	GPIOA -> MODER &=~ (1U << 14); //Set 0
	GPIOA -> MODER |= (1U << 15); //Set 1

	//Set PA9 for Slave Select -> OUTPUT
	GPIOA -> MODER |= (1U << 18); //Set 1
	GPIOA -> MODER &=~ (1U << 19); //Set 0

	//Set PA5
	GPIOA -> AFR[0] |= (1U << 20);
	GPIOA -> AFR[0] &=~ (1U << 21);
	GPIOA -> AFR[0] |= (1U << 22);
	GPIOA -> AFR[0] &=~ (1U << 23);

	//Set PA6
	GPIOA -> AFR[0] |= (1U << 24);
	GPIOA -> AFR[0] &=~ (1U << 25);
	GPIOA -> AFR[0] |= (1U << 26);
	GPIOA -> AFR[0] &=~ (1U << 27);

	//Set PA7
	GPIOA -> AFR[0] |= (1U << 28);
	GPIOA -> AFR[0] &=~ (1U << 29);
	GPIOA -> AFR[0] |= (1U << 30);
	GPIOA -> AFR[0] &=~ (1U << 31);
}


void spi_config(void)
{
	//Enable the clock access for SPI1
	RCC -> APB2ENR |= SPI1EN;

	//Set clock FPCLK/4
	SPI1 -> CR1 |= (1U << 3);
	SPI1 -> CR1 &=~ (1U << 4);
	SPI1 -> CR1 &=~ (1U << 5);

	//Set CPOL to 1 CPHA to 1
	SPI1 -> CR1 |= (1U << 0);
	SPI1 -> CR1 |= (1U << 1);
	//Enable FULL DUPLEX
	SPI1 -> CR1 &=~ (1U << 10);
	//Set MSB FIRST
	SPI1 -> CR1 &=~ (1U << 7);
	SPI1 -> CR1 |= (1U << 2); //Set as MASTER




	//To set 8 bit data mode
	SPI1 -> CR1 &=~ (1U << 11);

	//Select software slave management
	SPI1 -> CR1 |= (1U << 8);
	SPI1 -> CR1 |= (1U << 9);

	//SPI ENABLE
	SPI1 -> CR1 |= (1U << 6);

}

void spi_transmit(uint8_t *data, uint32_t size)
{
	uint32_t i=0;
	uint8_t temp;

	while(i < size){
		while(!(SPI1 -> SR * (SR_TXE))){} //wait until TXE is set

		//Write data to the data register
		SPI1 -> DR = data[i];
		i++;
	}
	while(!(SPI1 -> SR * (SR_TXE))){} //wait until TXE is set

	while(!(SPI1 -> SR * (SR_BUSY))){} //wait until TXE is set


	//Clear overflow flag
	temp = SPI1 -> DR;
	temp = SPI1 -> SR;

}

void spi_receive(uint8_t *data, uint32_t size)
{
	while(size){
		SPI1 -> DR = 0;
		//
		while(!(SPI1 -> SR * (SR_RXNE))){} //wait until TXE is set
		*data++ = SPI1 -> DR;
		size--;
	}
}

void cs_enable(void)
{
	GPIOA -> ODR &=~ (1U << 9);

}

void cs_disable(void)
{
	GPIOA -> ODR |= (1U << 9);

}







