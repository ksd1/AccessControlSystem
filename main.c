
#define HSE_VALUE ((uint32_t)8000000)

#include "stm32f4xx_can.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_spi.h"
#include "interrupts.h"
#include "view.h"
#include "mifare.h"
#include "ACS_protocol.h"
#include <stdio.h>


void _delay(uint32_t time)
{
	while(time--);
}



#define USE_CAN1
/* #define USE_CAN2 */

#ifdef  USE_CAN1
  #define CANx                       CAN1
  #define CAN_CLK                    RCC_APB1Periph_CAN1
#else /*USE_CAN2*/
  #define CANx                       CAN2
  #define CAN_CLK                    (RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2)
#endif  /* USE_CAN1 */



void delay(uint32_t val)
{
	while(val--);
}



uint8_t CAN_Polling(void)
{

  uint8_t test = 0;
  GPIO_InitTypeDef GPIO_InitStructure;
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CanTxMsg TxMessage;
  CanRxMsg RxMessage;
  uint32_t uwCounter = 0;
  uint8_t TransmitMailbox = 0;

  /*Port init*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

  /* CAN register init */
  CAN_DeInit(CANx);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

  /* CAN Baudrate = 175kbps (CAN clocked at 42 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
  CAN_InitStructure.CAN_Prescaler = 12;
  if( CAN_Init(CANx, &CAN_InitStructure) == CAN_InitStatus_Success )
	  test = 1;

  /* CAN filter init */
#ifdef  USE_CAN1
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
#else /* USE_CAN2 */
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
#endif  /* USE_CAN1 */
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;

  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);


  /*

  //transmit

  TxMessage.StdId = 0x11;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 2;
  TxMessage.Data[0] = 0xCA;
  TxMessage.Data[1] = 0xFE;

  TransmitMailbox = CAN_Transmit(CANx, &TxMessage);
  uwCounter = 0;
  while((CAN_TransmitStatus(CANx, TransmitMailbox)  !=  CANTXOK) && (uwCounter  !=  0xFFFF))
  {
    uwCounter++;
  }

  if(CAN_TransmitStatus(CANx, TransmitMailbox)  ==  CANTXOK)
	  test = 1;


  uwCounter = 0;
  while((CAN_MessagePending(CANx, CAN_FIFO0) < 1) && (uwCounter  !=  0xFFFF))
  {
    uwCounter++;
  }

  //receive
  RxMessage.StdId = 0x00;
  RxMessage.IDE = CAN_ID_STD;
  RxMessage.DLC = 0;
  RxMessage.Data[0] = 0x00;
  RxMessage.Data[1] = 0x00;
  CAN_Receive(CANx, CAN_FIFO0, &RxMessage);

  if (RxMessage.StdId != 0x11)
  {
    return 0;
  }

  if (RxMessage.IDE != CAN_ID_STD)
  {
    return 0;
  }

  if (RxMessage.DLC != 2)
  {
    return 0;
  }

  if ((RxMessage.Data[0]<<8|RxMessage.Data[1]) != 0xCAFE)
  {
    return 0;
  }


  if ((CANx->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
      {
        test = CAN_InitStatus_Failed;
      }
      else
      {
        test = CAN_InitStatus_Success;
      }

  */

  return 1; //Test Passed
}


void SPIInit()
{
	SPI_InitTypeDef SPI_Init_St;
	GPIO_InitTypeDef SPI_Port_Init;



	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	SPI_Port_Init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	SPI_Port_Init.GPIO_Mode = GPIO_Mode_AF;
	SPI_Port_Init.GPIO_OType = GPIO_OType_PP;
	SPI_Port_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	SPI_Port_Init.GPIO_Speed = GPIO_Speed_25MHz;

	GPIO_Init(GPIOC, &SPI_Port_Init);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

	SPI_Port_Init.GPIO_Pin = GPIO_Pin_13;
	SPI_Port_Init.GPIO_Mode = GPIO_Mode_OUT;
	SPI_Port_Init.GPIO_OType = GPIO_OType_PP;
	SPI_Port_Init.GPIO_PuPd = GPIO_PuPd_UP;
	SPI_Port_Init.GPIO_Speed = GPIO_Speed_25MHz;

	GPIO_Init(GPIOC, &SPI_Port_Init);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	SPI_Init_St.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_Init_St.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Init_St.SPI_Mode = SPI_Mode_Master;
	SPI_Init_St.SPI_DataSize = SPI_DataSize_8b;
	SPI_Init_St.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init_St.SPI_CPOL - SPI_CPOL_Low;
	SPI_Init_St.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_Init_St.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;

	SPI_Init(SPI3, &SPI_Init_St);
	SPI_Cmd(SPI3, ENABLE);

}

/*
uint8_t SPI3_send(uint8_t data){

	SPI3->DR = data; // write data to be transmitted to the SPI data register
	while( !(SPI3->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPI3->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI3->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
	return SPI3->DR; // return received data from SPI data register
}

*/

uint8_t i,tmp;
uint8_t status;
uint8_t str[MAX_LEN];
uint8_t RC_size;
uint8_t blockAddr;
char buffer[64];
char znak;

int main()
{
	SystemInit();
	//SPIInit();
	//MFRC522_Init();
	Init_View();



	CAN_Polling();

	CanTxMsg TxMessage;
	CanRxMsg RxMessage;

	TxMessage.StdId = 0x11;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 2;
	TxMessage.Data[0] = 0xCA;
	TxMessage.Data[1] = 0xFE;


	VCP_put_string("Hello");


	//Sample Devices;
	ACS_AddDevice(1, 0xD1);
	ACS_AddDevice(2, 0xB2);
	ACS_AddDevice(3, 0xc2);

	while(1)
	{

		if(CAN_MessagePending(CANx, CAN_FIFO0) > 0)
		{
			CAN_Receive(CANx,CAN_FIFO0, &RxMessage);
			Display_CAN_Rx_Frame(RxMessage);

			switch(RxMessage.Data[0])	//Data[0] -> Command
			{
				case CONNECT_DEVICE:	ACS_ConnectDevice(RxMessage);
					break;
			}

		}


		if(VCP_get_string(buffer))
		{
			VCP_put_string("Odebrano");
			VCP_put_string("\r\n");
			VCP_put_string(buffer);
			VCP_put_string("\r\n");
			DisplayDevicesList(ACS_GetDeviceListPointer());

		}

		/*
		tmp = Read_MFRC522(0x37);
		status = MFRC522_Request(PICC_REQIDL, str);
		if (status == MI_OK)
		{
			VCP_put_string("Detected card");
			VCP_put_string("\r\n");
			status = MFRC522_Anticoll(str);

			if(status == MI_OK)
			{
				sprintf(mynum,"%X %X %X %X %X", str[0],str[1],str[2],str[3],str[4]);
				VCP_put_string(mynum);
				VCP_put_string("\r\n");
			}

			delay(800000);
		}
		else
		{
			//VCP_put_string("No card");
			//VCP_put_string("\r\n");
		}

		delay(800000);
	*/

	}

	return 0;
}
