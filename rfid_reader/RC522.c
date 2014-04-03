#include "RC522.h"


uint8_t SPI_send(uint8_t data)
{
	/*


	SPI3->DR = data; // write data to be transmitted to the SPI data register
	while( !(SPI3->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPI3->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI3->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
	return SPI3->DR; // return received data from SPI data register

	*/



	    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
	    SPI_I2S_SendData(SPI3,data);
	    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);

	    return SPI_I2S_ReceiveData(SPI3);

}

void delay(uint32_t time)
{
	while(time--);
}

void nss_low()
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void nss_high()
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}


void reset_dev()
{

	nss_low();
	delay(5000);
	SPI_send(0x0F);
	delay(5000);
	nss_high();
}

uint8_t get_register(uint8_t address)
{
	uint8_t data;

	address <<= 1;
	address |= 0x80;

	nss_low();
	//delay(5000);
	SPI_send(address);
	data = SPI_send(0x00);

	//delay(5000);
	nss_high();
	return data;
}


uint8_t get_version()
{
	uint8_t data;

	nss_low();
	//delay(5000);
	SPI_send(0xee);
	data = SPI_send(0x00);

	//delay(5000);
	nss_high();
	return data;

}
