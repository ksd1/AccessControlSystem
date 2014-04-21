#include "view.h"

volatile uint32_t ticker, downTicker;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;


uint8_t Init_View(void)
{
	Init_USB();


}


void Init_USB()
{
	GPIO_InitTypeDef LED_Config;
	/* Always remember to turn on the peripheral clock...  If not, you may be up till 3am debugging... */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	LED_Config.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	LED_Config.GPIO_Mode = GPIO_Mode_OUT;
	LED_Config.GPIO_OType = GPIO_OType_PP;
	LED_Config.GPIO_Speed = GPIO_Speed_25MHz;
	LED_Config.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &LED_Config);



	/* Setup SysTick or CROD! */
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		//ColorfulRingOfDeath();
	}


	/* Setup USB */
	USBD_Init(&USB_OTG_dev,
	            USB_OTG_FS_CORE_ID,
	            &USR_desc,
	            &USBD_CDC_cb,
	            &USR_cb);

	return;
}


void VCP_put_string(char* string)
 {
 	 while(*string)
 	 {
 		VCP_put_char(*string++);
 	 }
 }

void Display_CAN_Rx_Frame(CanRxMsg RxMessage)
{
	char buffer[32];
	uint8_t i;

	VCP_put_string("------ RxFrame Begin ------");
	VCP_put_string("\r\n");

	sprintf(buffer,"Std. Ident: %X", RxMessage.StdId);
	VCP_put_string(buffer);
	VCP_put_string("\r\n");

	if(RxMessage.RTR == CAN_RTR_DATA)
	{
		VCP_put_string("Remote Trans: NO");
		VCP_put_string("\r\n");
	}
	else
	{
		VCP_put_string("Remote Trans: YES");
		VCP_put_string("\r\n");
	}

	sprintf(buffer,"Data Length: %d", RxMessage.DLC);
	VCP_put_string(buffer);
	VCP_put_string("\r\n");

	for(i=0; i<RxMessage.DLC; i++)
	{
		sprintf(buffer,"Data %d: %X", i, RxMessage.Data[i]);
		VCP_put_string(buffer);
		VCP_put_string("\r\n");
	}

	VCP_put_string("------- RxFrame End -------");
	VCP_put_string("\r\n");
}


void DisplayDevicesList(DeviceStruct* DV_Head)
{
	DeviceStruct* DV_Temp1 = DV_Head;
	uint8_t i;
	char buffer[32];

	VCP_put_string("------- Devices List --------");
	VCP_put_string("\r\n");
	if(DV_Temp1 == NULL)
	{
		VCP_put_string("No devices connected");
		VCP_put_string("\r\n");
		return;
	}


	while(DV_Temp1 != NULL)
	{
		sprintf(buffer,"Device ID: %X", DV_Temp1->DeviceID);
		VCP_put_string(buffer);
		VCP_put_string("\r\n");


		if( (DV_Temp1->DeviceType & 0xF0)==0xB0 || (DV_Temp1->DeviceType & 0xF0)==0xC0 ) //Requester = 0xBX - 0xCX
		{
			sprintf(buffer,"Device Type: %X (Requester)", DV_Temp1->DeviceType);
			VCP_put_string(buffer);
			VCP_put_string("\r\n");

			VCP_put_string("Attached Devices: ");


			for(i=0; i<5; i++)
			{
				sprintf(buffer,"%X, ", DV_Temp1->AttachDevices[i]);
				VCP_put_string(buffer);
			}
		}
		else //Executor - no attached deviced
		{
			sprintf(buffer,"Device Type: %X (Executor)", DV_Temp1->DeviceType);
			VCP_put_string(buffer);
			VCP_put_string("\r\n");

			VCP_put_string("Attached Devices: ");

			VCP_put_string("N/A");
		}
		VCP_put_string("\r\n");

		VCP_put_string("-----------------------------");
		VCP_put_string("\r\n");

		DV_Temp1 = (DeviceStruct*)DV_Temp1->next;
	}



}




